#include "openart_mini.h"

enum picture_type{
    apple,
    banana,
    sedan,//小轿车
    cat,
    cow,
    bus,//大巴
    dog,
    durian,
    grape,
    horse,
    orange,
    pig,
    plane,
    ship,
    trains
}picture_type;



uint8               openart_rx_buffer;
lpuart_transfer_t   openart_receivexfer;
lpuart_handle_t     openart_g_lpuartHandle;

uint8 openartBuffer;

void openart_uart1_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_LPUART_RxIdle == status)
    {
        //openart_rx_buffer;    //串口收到数据后会自动进入到这里，然后读取openart_rx_buffer变量即可读取串口收到的数据
        openartBuffer=openart_rx_buffer;
    }
    
    handle->rxDataSize = openart_receivexfer.dataSize;  //还原缓冲区长度
    handle->rxData = openart_receivexfer.data;          //还原缓冲区地址
}



void openart_mini(void)
{
    uart_init(USART_1, 115200, UART1_TX_B12, UART1_RX_B13);
    
    //配置串口接收的缓冲区及缓冲区长度
    openart_receivexfer.dataSize = 1;
    openart_receivexfer.data = &openart_rx_buffer;
    
    //设置中断函数及其参数
    uart_set_handle(USART_1, &openart_g_lpuartHandle, openart_uart1_callback, NULL, 0, openart_receivexfer.data, 1);
    
    NVIC_SetPriority(LPUART4_IRQn, 14);         //设置串口中断优先级 范围0-15 越小优先级越高
    uart_rx_irq(USART_1, 1);
}