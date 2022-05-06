#include "headfile.h"



int main(void)
{
	DisableGlobalIRQ();
 
	board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	/*******************************************/
	pit_init();//
	pit_interrupt_ms(PIT_CH0,5);  //初始化pit通道0 周期5ms 编码器中断
	pit_interrupt_ms(PIT_CH1,5);  //初始化pit通道1 周期10ms	按键、串口发送中断
	pit_interrupt_ms(PIT_CH2,10);  //初始化pit通道2 周期10ms	PID控制中断
  pit_interrupt_ms(PIT_CH3,5);  //初始化pit通道3 周期10ms	icm中断
	NVIC_SetPriority(PIT_IRQn,1);


	
	Motor_Init();
	Encoder_Init();
	Key_Init();

  icm20602_init_spi();
  icmOffsetInit();//icm零漂消除
	GUI_init();
	//mt9v03x_csi_init();	//初始化摄像头 使用CSI接口
	//如果屏幕一直显示初始化信息，请检查摄像头接线
	//如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?
	uart_init (USART_8, 115200,UART8_TX_D16,UART8_RX_D17); //初始化串口
	
	// VOFA+ 
    VOFA* VOFA_pt = vofa_create();       //创建VOFA对象
    vofa_init(VOFA_pt,                   //初始化当前的vofa对象
                vofa_ch_data,ch_sz,
       vofa_image,image_sz,
       custom_buf,custom_sz,
       cmd_rxbuf,cmd_sz,
       USART_8,USART_8,USART_8);

    mt9v03x_csi_init();		//初始化摄像头	使用CSI接口
	//如果图像只采集一次，请检查场信号(VSY)是否连接OK?
	systick_delay_ms(500);
	systick_start();
	EnableGlobalIRQ(0);

	Beep_flag=1;
	Location_Route();
	while(1)
	{
        if(mt9v03x_csi_finish_flag)			//图像采集完成
        {
            mt9v03x_csi_finish_flag = 0;	//清除采集完成标志位

            //使用缩放显示函数，根�9                                                                                                                                                                   菰纪枷翊笮� 以及设置需要显示的大小自动进行缩放或者放大显示
            //总钻风采集到的图像分辨率为 188*120 ，2.0寸IPS屏显示分辨率为 320*240 ，图像拉伸全屏显示。
            //ips114_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 240, 135);	//显示摄像头图像
        }

        Car.Angel=-(int)cpmangle_z;
        //Car_Move();

        //Car_Omni(Car.Speed_X,Car.Speed_Y,Car.Speed_Z);
        Car_OmniMove();
        //屏幕显示
/*        GUI_icm20602();
        GUI_speed();
        GUI_duty();*/
    }
}




