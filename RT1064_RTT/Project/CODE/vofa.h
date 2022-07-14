#ifndef _VOFA_H
#define _VOFA_H

/*********************************************************************************************************************
 * @file       		main
 * @author     		HHF
 * @version    		2022/02/25
 * @Software 		  MDK 5.32
 * @brief 				通过vofa的justfloat或者firewater协议传输数据
 * @note					上位机的字节接收区选项要勾选十六进制，以十六进制方式打印字符，否则只能打印乱码。
 * @note					arm的单片机多是小端形式的（little Eidian）
 ********************************************************************************************************************/
#include "common.h"						//逐飞的common.h中包含uint8_t类型的定义等必要内容
#include "Vofa_imageformat.h"	//图片格式枚举
#include "zf_uart.h"					//vofa的物理层实现
#include "SEEKFREE_PRINTF.h"	//FW所需的printf函数实现
#include "motor.h"

//Vofa专用缓冲区接口
#define ch_sz     8
#define image_sz  100
#define custom_sz 4*7
#define cmd_sz    3+2*5
extern float 	vofa_ch_data[ch_sz];
extern uint8_t vofa_image[image_sz];
extern uint8_t custom_buf[custom_sz];      //下位机发送缓冲区																		
//tx:float yaw、accl(preprocessed)x,y,z、gyro,x,y,z
extern uint8_t cmd_rxbuf[cmd_sz];          //下位机接收缓冲区
//rx:char start/stop,LED,reserved,short forward/backward+leftshift/rightshift、yaw_adj、cam_rotate、rod_rotate



//Vofa专用的状态量	
typedef uint8_t VOFA_STATE;								
enum VOFA_STATE{
		VOFA_ERR = 0,
		VOFA_OK,
};


//vofa支持的协议（标有EXT的表示非vofa的协议）
enum vofa_protocol{
		VOFA_PROTOCOL_FIREWATER = 0,
		VOFA_PROTOCOL_JUSTFLOAT,		//本协议是小端浮点数组形式的字节流协议，纯十六进制浮点传输，节省带宽。\
																//--此协议非常适合用在通道数量多、发送频率高的时候。
		VOFA_PROTOCOL_RAWDATA,			//纯字节流
		EXT_PROTOCOL_BLEDEBUGGER,		//兼容手机端的蓝牙调试器的简单协议
		EXT_PROTOCOL_DATASCOPE,			//电脑端十通道示波器协议
};		

		

enum vofa_frame_category{									//数据帧的类型//
		VOFA_CH_FRAME = 0,										//通道类型（所有通道的数据类型需一致）
		VOFA_IMAGE_FRAME,											//图像类型
		VOFA_RAMDOM_FRAME,										//通用类型
};

enum vofa_frame_leader{										//不同类型数据帧的引导部分//
		JF_TAIL = 0,													//JF协议的通道类型
		JF_PREFRAME,													//JF协议的图像类型
		EXT_BLE
};

#define VOFA_SINGLE_IMAGE_TRANSMIT 0			//传输图片时默认为单通道
//#define VOFA_MULTI_IMAGE_TRANSMIT 1			//也可以设置为多通道
#ifdef VOFA_SINGLE_IMAGE_TRANSMIT
	#define IMG_ID  0												//默认用通道0传输图片数据
#endif



typedef struct vofa{
		//协议规定的引导部分
		uint32_t vofa_preframe[7];  //JF、FW协议通用的图片前导帧
		uint8_t vofa_JF_tail[4];		//JF协议特有的帧尾
		
		//vofa数据缓冲区
		void* 		ch_data_pt;				//通道型数据专用
		uint32_t 	ch_size;					//通道型数据的字节量
		uint8_t*	image_pt;					//图像型数据专用
		uint32_t 	image_size;				//图像型数据的字节量
		uint8_t*  custom_buf_pt;		//自定义型数据专用
		uint32_t	custom_buf_size; //自定义型数据的字节量
		uint8_t* 	cmd_rxbuf;				//控制指令型数据专用
		uint32_t	cmd_size;					//控制指令型数据的字节量
			
		//物理实现端口		
		uint8_t VOFA_UART; 					//与Vofa上位机通信的端口
		uint8_t BLE_UART;						//与蓝牙/WIFI模块通信的端口
		uint8_t SCOPE_UART;					//与Datascope上位机通信的端口
		
		//vofa对象包含的方法												
		VOFA_STATE 	 (*vofa_release)						(struct vofa* vofa_pt);
		VOFA_STATE   (*sendzip)						(struct vofa* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category);		//发送已打包的数据
		VOFA_STATE   (*unzip)  						(struct vofa* vofa_pt,uint8_t vofa_protocol,uint8_t msg_catagory);																//解码上位机发送的数据
		VOFA_STATE 	 (*image_property_set)(struct vofa* vofa_pt,uint32_t id,uint32_t size,uint32_t width,uint32_t height,uint32_t format);	//设置图片属性
		VOFA_STATE   (*image_property_get)(struct vofa* vofa_pt,uint32_t id);																																//获取图片属性
} VOFA;
 


/**********************************************************************
 *	@fcn	初始化VOFA协议对象、释放VOFA对象占用的资源
 *	@para vofa_pt:vofa对象
 *	 			ch_data_pt:通道型数据缓冲区
 **********************************************************************/ 	 
VOFA* vofa_create(void);
VOFA_STATE vofa_release(VOFA* vofa_pt);
VOFA_STATE vofa_init(VOFA* vofa_pt,
											void* 	 ch_data_pt	  	,uint32_t ch_size,\
											uint8_t* image_pt  	  	,uint32_t image_size,\
											uint8_t* custom_buf_pt  ,uint32_t custom_buf_size,\
											uint8_t* cmd_rxbuf_pt   ,uint32_t cmd_size,\
											uint8_t  VOFA_UART 		  ,uint8_t  BLE_UART,uint8_t SCOPE_UART);
//后面打算优化一下上面这个函数，它的输入参数太多了，用起来还是不方便，看看有没有可变长度的函数输入方案



/**********************************************************************
 *	@fcn	数据发送函数(私有函数)
 *	@para vofa_pt							:vofa对象
 * 				vofa_frame_leader		:当前协议规定的数据引导部分
 *				vofa_frame_category :待传输数据的类型
 **********************************************************************/ 	
//发送数据引导块
static VOFA_STATE vofa_transmit_leader(VOFA* vofa_pt,uint8_t vofa_frame_leader);
//发送用户数据
static VOFA_STATE vofa_transmit(VOFA* vofa_pt,uint8_t vofa_frame_category);
	


/**********************************************************************
 *	@fcn	数据打包成用户指定的类型并发送
 *	@para vofa_pt						  :VOFA对象
 *				vofa_protocal				:上下位机通信协议
 *				vofa_frame_category :待传输数据的类型
 *				rawdata为原始数据
 **********************************************************************/ 	
VOFA_STATE vofa_sendzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category);



/**********************************************************************
 *	@fcn	数据自动解包，以便接收上位机的指令
 *	@para vofa_pt			 :vofa对象
 * 				vofa_protocol:上下位机通信协议
 * 				msg_catagory :上位机信息的类型
 **********************************************************************/ 	
VOFA_STATE vofa_unzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t msg_catagory);//暂未实现
	


/**********************************************************************
 *	@fcn	设置、查看待传图片属性函数
 *	@para vofa_pt:vofa对象
 *				图像的五个属性
 *	@note 以下格式发送时，IMG_WIDTH和IMG_HEIGHT不需要强制指定，设置为-1即可：
 *						Format_BMP,Format_GIF,Format_JPG,Format_PNG,Format_PBM,Format_PGM,
 *						Format_PPM,Format_PNG,Format_XBM,Format_XPM,Format_SVG
 **********************************************************************/ 	
VOFA_STATE image_property_set(VOFA* vofa_pt,uint32_t id,uint32_t size,uint32_t width,uint32_t height,uint32_t format);
VOFA_STATE image_property_get(VOFA* vofa_pt,uint32_t id); //查看某个图片通道的信息

void vofa_Send(void);
 
 

#endif
