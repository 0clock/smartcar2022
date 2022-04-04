#include "vofa.h"	//vofa上位机接口

//Vofa专用缓冲区
float vofa_ch_data[ch_sz]    = {1.2,2.4,3.6,4.8};  //测试用通道数据		
uint8_t vofa_image[image_sz]  = {0};	 						 //测试用图像数据
uint8_t custom_buf[custom_sz]  = {0};    				   //下位机发送状态数据缓冲区
uint8_t cmd_rxbuf[cmd_sz] = {0};  					 		   //下位机接收命令缓冲区


void VOFA_Send(){
    //VOFA_pt->sendzip(VOFA_pt,VOFA_PROTOCOL_JUSTFLOAT,VOFA_CH_FRAME);
    //VOFA_pt->sendzip(VOFA_pt,VOFA_PROTOCOL_JUSTFLOAT,VOFA_CH_FRAME);
    //myprintf("%f,%f,%f,%f,%d,%f\n",RC_Encoder1.value,RC_Encoder2.value,RC_Encoder3.value,RC_Encoder4.value,1000,Car.mileage);
    myprintf("%d,%d,%d,%d,%d\n",RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4,speed_tar_1);
    vofa_ch_data[0]=RC_encoder1;
    vofa_ch_data[1]=RC_encoder2;
    vofa_ch_data[2]=RC_encoder3;
    vofa_ch_data[3]=RC_encoder4;
}

/**********************************************************************
 *	@fcn	初始化VOFA协议对象、释放VOFA对象占用的资源
 *	@para vofa_pt:vofa对象
 *	 			ch_data_pt:通道型数据缓冲区
 **********************************************************************/ 	
//初始化 
VOFA* vofa_create(void)
{
		//创建VOFA对象
		VOFA* vofa_pt = (VOFA*)malloc(sizeof(VOFA));
		//可以通过链表来管理多个同类的对象
		//---
		return vofa_pt;
}

VOFA_STATE vofa_init(VOFA* vofa_pt,
											void* 	 ch_data_pt	  	,uint32_t ch_size,\
											uint8_t* image_pt  	  	,uint32_t image_size,\
											uint8_t* custom_buf_pt  ,uint32_t custom_size,\
											uint8_t* cmd_rxbuf_pt   ,uint32_t cmd_size,\
											uint8_t  VOFA_UART 		  ,uint8_t  BLE_UART,uint8_t SCOPE_UART)
{
		//vofa数据缓冲区
			//1.普通数据缓冲区入口(主要用于把下位机的数据发送到上位机)
		vofa_pt->ch_data_pt = ch_data_pt;
		vofa_pt->ch_size 	  = ch_size;
			//2.图像数据缓冲区入口
		vofa_pt->image_pt   = image_pt;		
		vofa_pt->image_size = image_size;
			//3.自定义型数据缓冲区入口
		vofa_pt->custom_buf_pt  = custom_buf_pt;		
		vofa_pt->custom_buf_size = custom_size;
			//4.控制指令型数据缓冲区入口(主要用于接收上位机的控制指令)
		vofa_pt->cmd_rxbuf  = cmd_rxbuf_pt;
		vofa_pt->cmd_size		= cmd_size;
		
		//物理实现端口		
			//1.vofa上位机的端口
		vofa_pt->VOFA_UART  = VOFA_UART;
			//2.蓝牙调试器的端口
		vofa_pt->BLE_UART   = BLE_UART;
			//3.虚拟示波器的端口
		vofa_pt->SCOPE_UART = SCOPE_UART;
		
		//协议规定的引导部分
			//1.JF、FW通用的图像数据前导帧(用户可调用image_property_set来vofa_preframe数组的前5个元素，这些元素用来描述待传输图片的信息)
		vofa_pt->vofa_preframe[5] = 0x7F800000;
		vofa_pt->vofa_preframe[6] = 0x7F800000;   
			//2.JF通道型数据的固定帧尾
		vofa_pt->vofa_JF_tail[0] = 0x00;
		vofa_pt->vofa_JF_tail[1] = 0x00;
		vofa_pt->vofa_JF_tail[2] = 0x80;
		vofa_pt->vofa_JF_tail[3] = 0x7F;
		
		//vofa对象包含的方法	
		vofa_pt->vofa_release  = vofa_release;
		vofa_pt->sendzip = vofa_sendzip;
		vofa_pt->unzip 	 = vofa_unzip;
		vofa_pt->image_property_set = image_property_set;
		vofa_pt->image_property_get = image_property_get;
		return VOFA_OK;
}

//反初始化
VOFA_STATE vofa_release(VOFA* vofa_pt)
{
		if(!vofa_pt)	free(vofa_pt);
		else	return VOFA_ERR;
		return VOFA_OK;
}



/**********************************************************************
 *	@fcn	数据发送函数(私有函数)
 *	@para vofa_pt:vofa对象
 * 				vofa_frame:待传输的数据帧
 **********************************************************************/ 		
//发送数据帧头或帧尾（引导部分）
static VOFA_STATE vofa_transmit_leader(VOFA* vofa_pt,uint8_t vofa_frame_leader)
{		
		if(vofa_pt == NULL)	return VOFA_ERR;
	
		if(vofa_frame_leader == JF_TAIL)
		{		
				//向上位机发送JF通道型数据的帧尾：4byte
				uart_putbuff(vofa_pt->VOFA_UART,vofa_pt->vofa_JF_tail,4);									
				return VOFA_OK;
		}
		
		else if(vofa_frame_leader == JF_PREFRAME)
		{			
				//向上位机发送JF图像前导帧：28byte
				uart_putbuff(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->vofa_preframe),28);		
				return VOFA_OK;
		}
		
		return VOFA_ERR;
}


//发送用户数据
static VOFA_STATE vofa_transmit(VOFA* vofa_pt,uint8_t vofa_frame_category)
{
		if(vofa_pt == NULL)	return VOFA_ERR;
	
		if(vofa_frame_category == VOFA_CH_FRAME)
		{	
				uart_putbuff(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->ch_data_pt),vofa_pt->ch_size);
				return VOFA_OK;
		}

		else if(vofa_frame_category == VOFA_IMAGE_FRAME)
		{
				uart_putbuff(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->image_pt),vofa_pt->vofa_preframe[2]);
				return VOFA_OK;
		}
		
//		else if(vofa_frame_category == VOFA_RAMDOM_FRAME)	
//		{
//				uart_putbuff(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->image_pt),vofa_pt->vofa_preframe[2]);
//				return VOFA_OK;
//		}
		
		return VOFA_ERR;
}



/**********************************************************************
 *	@fcn	数据打包成用户指定的类型并发送
 *	@para vofa_pt:VOFA对象;
 *				vofa_protocal:数据传输协议
 *				vofa_frame_category:待传输数据的类型
 *				rawdata为原始数据
 **********************************************************************/ 	
//下位机数据打包
VOFA_STATE vofa_sendzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category)
{
		
		if(vofa_pt == NULL)	return VOFA_ERR;

		//JF协议//
		if(vofa_protocol == VOFA_PROTOCOL_JUSTFLOAT)
		{
				if(vofa_frame_category == VOFA_CH_FRAME)				//小数据通道模式
				{
						if(vofa_transmit(vofa_pt,VOFA_CH_FRAME))		//发送用户数据(通道类型)
						{
								vofa_transmit_leader(vofa_pt,JF_TAIL);	//发送帧尾
								return VOFA_OK;
						}
						else return VOFA_ERR;
				}
				
				else if(vofa_frame_category == VOFA_IMAGE_FRAME)//图像模式							
				{
						vofa_transmit_leader(vofa_pt,JF_PREFRAME);	//发送前导帧
						if(vofa_transmit(vofa_pt,VOFA_IMAGE_FRAME)) //发送用户数据（图像类型）
						{
								return VOFA_OK;
						}
				}
		}
		
		//FW协议//
		else if(vofa_protocol == VOFA_PROTOCOL_FIREWATER)
		{
				if(vofa_frame_category == VOFA_CH_FRAME)
				{
						uint32_t ch_size = vofa_pt->ch_size/sizeof(float);
						uint8_t i = 0;
						for(;i < ch_size;i++)
						{
								myprintf("%f",((float*)(vofa_pt->ch_data_pt))[i]);//小数据通道模式（无前导帧）
								if(i < ch_size-1)
								{
										myprintf(",");
								}	
						}							
						myprintf("\n");																  //换行符（FW协议的帧尾）
						return VOFA_OK;
				}
		}
		
		//RW协议//
//		else if(vofa_protocol == VOFA_PROTOCOL_RAWDATA)
//		{
//				vofa_transmit(vofa_pt,VOFA_RAMDOM_FRAME);
//		}
		
		//手机蓝牙调试器协议(v1.0)
		//备注：上位机协议仅接收byte类型的变量，也就是说，使用前需要先float_to_byte之类的操作(在fast_algorithm中)
		else if(vofa_protocol == EXT_PROTOCOL_BLEDEBUGGER)
		{
				uint16_t checksum = 0; 					
				uart_putchar(vofa_pt->BLE_UART,0xA5); //发送帧头	
				uart_putbuff(vofa_pt->BLE_UART,custom_buf,sizeof(custom_buf));	//发送用户数据				
        for (int i = 1;i < sizeof(custom_buf)-2;i++)
        { 
            checksum += custom_buf[i];
        }
				uart_putchar(vofa_pt->BLE_UART,(uint8_t)(checksum&0xff));		//发送校验位(ps.强制转换将保留低位)	
        uart_putchar(vofa_pt->BLE_UART,0x5A); //发送帧尾
				return VOFA_OK;
		}  
		
		//电脑虚拟示波器协议(v1.0)
		//备注：该协议仅接收byte类型的变量，也就是说，使用前需要先float_to_byte之类的操作(在fast_algorithm中)
		else if(vofa_protocol == EXT_PROTOCOL_DATASCOPE)
		{
				uart_putchar(vofa_pt->SCOPE_UART,'$');//发送帧头	
				uart_putbuff(vofa_pt->SCOPE_UART,custom_buf,28);//发送7个通道的数据（要求下位机发送的数据类型为float）
				return VOFA_OK;
		}	
		
		return VOFA_ERR;
}



//上位机数据解包(目前仅有一类数据)
VOFA_STATE vofa_unzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t msg_catagory)//暂未实现
{
		//上位机的指令列表：
		//1.
		
		return VOFA_ERR;
}



/**********************************************************************
 *	@fcn	设置、查看待传图片属性函数
 *	@para vofa_pt:vofa对象
 *				other  :图像的五个属性
 *	@note 以下格式发送时，IMG_WIDTH和IMG_HEIGHT不需要强制指定，设置为-1即可：
 *						Format_BMP,Format_GIF,Format_JPG,Format_PNG,Format_PBM,Format_PGM,
 *						Format_PPM,Format_PNG,Format_XBM,Format_XPM,Format_SVG
 **********************************************************************/ 	
VOFA_STATE image_property_set(VOFA* vofa_pt,uint32_t id,uint32_t size,uint32_t width,uint32_t height,uint32_t format)
{
		if(vofa_pt == NULL)	return VOFA_ERR;
	
		vofa_pt->vofa_preframe[0] = id;
		vofa_pt->vofa_preframe[1] = size;
		vofa_pt->vofa_preframe[2] = width;
		vofa_pt->vofa_preframe[3] = height;
		vofa_pt->vofa_preframe[4] = format;
	
		return VOFA_ERR;
}

VOFA_STATE image_property_get(VOFA* vofa_pt,uint32_t id) //查看某个图片通道的信息(目前仅有通道0)
{
		if(vofa_pt == NULL)	return VOFA_ERR;
		
		if(vofa_pt->vofa_preframe[0] == id)
		{
				myprintf("IMG_ID:%d",vofa_pt->vofa_preframe[0]);
				myprintf("IMG_SIZE:%d",vofa_pt->vofa_preframe[1]);
				myprintf("IMG_WIDTH:%d",vofa_pt->vofa_preframe[2]);
				myprintf("IMG_HEIGHT:%d",vofa_pt->vofa_preframe[3]);
				myprintf("IMG_FORMAT:%d",vofa_pt->vofa_preframe[4]);
				return VOFA_OK;
		}

		return VOFA_ERR;
}



























