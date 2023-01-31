#include "sendmsg.h"
#define DATA_SEND_UART USART_8



uint8 msgs[18] = {0};


/*int main(void)
{
    DisableGlobalIRQ();
    board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
    uart_init (DATA_SEND_UART, 115200, UART8_TX_D16, UART8_RX_D17);           //无线设备初始化
    //这里使用的是主板上无线模块的串口
    //总中断最后开启
    EnableGlobalIRQ(0);
    while (1)
    {
        //按照比赛规则，传入时间，坐标，类别向上位机发送数据

        //发送无目标信息
        send_msg(DATA_SEND_UART, 123, 456, 1, 2, CATEGORY_NULL);
        //发送正在搬运信息
        send_msg(DATA_SEND_UART, 123, 456, 9, 1, CARRY);
        systick_delay_ms(2000);
        //发送动物类别信息
        send_msg(DATA_SEND_UART, 123, 456, 1, 2, DOG);
        send_msg(DATA_SEND_UART, 123, 456, 1, 2, HORSE);
        send_msg(DATA_SEND_UART, 234, 567, 3, 4, CAT);
        send_msg(DATA_SEND_UART, 123, 456, 5, 6, CATTLE);
        send_msg(DATA_SEND_UART, 234, 567, 7, 8, PIG);
        systick_delay_ms(2000);
        //发送水果类别信息
        send_msg(DATA_SEND_UART, 123, 456, 9, 1, ORANGE);
        send_msg(DATA_SEND_UART, 123, 456, 1, 2, APPLE);
        send_msg(DATA_SEND_UART, 234, 567, 3, 4, DURIAN);
        send_msg(DATA_SEND_UART, 123, 456, 5, 6, GRAPE);
        send_msg(DATA_SEND_UART, 234, 567, 7, 8, BANANA);
        systick_delay_ms(2000);
        //发送交通工具类别信息
        send_msg(DATA_SEND_UART, 123, 456, 9, 1, TRAINS);
        send_msg(DATA_SEND_UART, 123, 456, 1, 2, STEAMSHIP);
        send_msg(DATA_SEND_UART, 234, 567, 3, 4, PLANE);
        send_msg(DATA_SEND_UART, 123, 456, 5, 6, CAR);
        send_msg(DATA_SEND_UART, 234, 567, 7, 8, COACH);
        systick_delay_ms(2000);
    }
}*/

//-------------------------------------------------------------------------------------------------------------------
//  @brief      智能视觉组数据转发模块 发送函数
//  @param      uartn               串口枚举
//  @param      second              运行时长（秒）
//  @param      msecond             运行时长（毫秒）
//  @param      pos_x               X坐标
//  @param      pos_y               Y坐标
//  @param      cg                  类别
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void send_msg(UARTN_enum uartn, uint16 second, uint16 msecond, uint8 pos_x, uint8 pos_y, category_enum cg)
{
    //时间信息
    msgs[0] = second/100 + '0';
    msgs[1] = second%100/10 + '0';
    msgs[2] = second%10 + '0';
    msgs[3] = '.';
    msgs[4] = msecond/100 + '0';
    msgs[5] = msecond%100/10 + '0';
    msgs[6] = msecond%10 + '0';

    //坐标X信息
    msgs[7] = ' ';
    msgs[8] = pos_x/10 + '0';
    msgs[9] = pos_x%10 + '0';

    //坐标Y信息
    msgs[10] = ' ';
    msgs[11] = pos_y/10 + '0';
    msgs[12] = pos_y%10 + '0';

    //类别 大类信息
    msgs[13] = ' ';
    msgs[14] = (uint8)cg/6 + '0';

    //类别 小类信息
    msgs[15] = ' ';
    msgs[16] = (uint8)cg%6 + '0';

    msgs[17] = '\n';
    uart_putbuff(uartn, msgs, 18);
}


