#include "buzzer.h"
#include "button.h"
#include "display.h"
#include "motor.h"
#include "flash_param.h"
#include "ahrs.h"
#include "location.h"



#define KEY_1   C27	// 定义主板上按键对应引脚
#define KEY_2   C26	// 定义主板上按键对应引脚
#define KEY_3   C31	// 定义主板上按键对应引脚
#define KEY_4   C4	// 定义主板上按键对应引脚

enum Config_Action config_action;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//开关信号量
rt_sem_t key1_sem;
rt_sem_t key2_sem;
rt_sem_t key3_sem;
rt_sem_t key4_sem;



void button_entry(void *parameter)
{
    //保存按键状态
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    
    //读取当前按键状态
    key1_status = gpio_get(KEY_1);
    key2_status = gpio_get(KEY_2);
    key3_status = gpio_get(KEY_3);
    key4_status = gpio_get(KEY_4);
    
    //检测到按键按下之后并放开 释放一次信号量
    if(key1_status && !key1_last_status)    
    {
        if(Page_Number==InfoPage){
            speed_tar=5;//小车走起
        }
        if(Page_Number==ConfigPage){
            switch (config_action) {
                case MileageX : MileageKx+=0.0002f;break;
                case MileageY : MileageKy+=0.0002f;break;
                case AngelP : Angel_KP+=0.1f;break;
                case AngelI: Angel_KI+=0.1f;break;
                case AngelD:Angel_KD+=0.1f;break;
                case MotorP:Position_KP+=1;break;
                case MotorI:Position_KI+=1;break;
                case MotorD:Position_KD+=1;break;
                default:break;
            }
        }
        rt_sem_release(key1_sem);
        rt_mb_send(buzzer_mailbox, 23);
    }
    if(key2_status && !key2_last_status)    
    {
        if(Page_Number==InfoPage){
            speed_tar=0;//小车刹车
        }
        if(Page_Number==ConfigPage){
            switch (config_action) {
                case MileageX : MileageKx-=0.0002f;break;
                case MileageY : MileageKy-=0.0002f;break;
                case AngelP : Angel_KP-=0.1f;break;
                case AngelI: Angel_KI-=0.1f;break;
                case AngelD:Angel_KD-=0.1f;break;
                case MotorP:Position_KP-=1;break;
                case MotorI:Position_KI-=1;break;
                case MotorD:Position_KD-=1;break;
                default:break;
            }
        }
        rt_sem_release(key2_sem);
        rt_mb_send(buzzer_mailbox, 23);
    }
    if(key3_status && !key3_last_status)    
    {
        if(Page_Number==ConfigPage){
            config_action++;
            if(config_action>7)
                config_action=0;
            Gui_Page_Active=true;
        }
        rt_sem_release(key3_sem);
        rt_mb_send(buzzer_mailbox, 23);
    }
    if(key4_status && !key4_last_status)    
    {
        Page_Number++;
        Gui_Page_Active=true;
        if(Page_Number>4)
            Page_Number=0;
        rt_sem_release(key4_sem);
        rt_mb_send(buzzer_mailbox, 23);
    }
}

void button_init(void)
{
    rt_timer_t timer1;
    
    gpio_init(KEY_1, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);			// 初始化为GPIO浮空输入 默认上拉高电平
	gpio_init(KEY_2, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
	gpio_init(KEY_3, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
	gpio_init(KEY_4, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);


    
    key1_sem = rt_sem_create("key1", 0, RT_IPC_FLAG_FIFO);		//创建按键的信号量，当按键按下就释放信号量，在需要使用按键的地方获取信号量即可
    key2_sem = rt_sem_create("key2", 0, RT_IPC_FLAG_FIFO);  
    key3_sem = rt_sem_create("key3", 0, RT_IPC_FLAG_FIFO);  
    key4_sem = rt_sem_create("key4", 0, RT_IPC_FLAG_FIFO);  
    
    timer1 = rt_timer_create("button", button_entry, RT_NULL, 20, RT_TIMER_FLAG_PERIODIC);

    if(RT_NULL != timer1) 
    {
        rt_timer_start(timer1);
    }
}