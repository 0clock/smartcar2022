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
    pit_interrupt_ms(PIT_CH2,20);  //初始化pit通道2 周期10ms	PID控制中断
    pit_interrupt_ms(PIT_CH3,5);  //初始化pit通道3 周期10ms	icm中断
    NVIC_SetPriority(PIT_IRQn,1);


    Motor_Init();
    Encoder_Init();
    RCEncoder_Init();
    Key_Init();

    icm20602_init_spi();
    icmOffsetInit();//icm零漂消除
    GUI_init();

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

    Location_Route(); //路径规划

    while(1)
    {
        GUI_FirstPage();
    }
}




