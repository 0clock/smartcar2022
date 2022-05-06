#include "key.h"

//-----------------------宏-----------------------//
/*BUTTON*/
#define KEY1 C31 
#define KEY2 C27 
#define KEY3 C26 
#define KEY4 C4
#define BEEP_PIN   B11       //蜂鸣器引脚

/*Dial_switch*/
#define Dial_switch1 D27
#define Dial_switch2 D4

struct Beep beep;

void Beep_Set(int time,int frequency){
    beep.time=time;
    beep.frequency=frequency;
}

void Beep_Ctrl() {
    static int count = 0, flag = 1;
    if (flag == 1) {
        if (beep.frequency > 0) {
            gpio_set(BEEP_PIN, 1);
            count++;
        }
        if (count >= beep.time) {
            count = 0;
            beep.frequency--;
            flag = !flag;
        }
    } else {
        gpio_set(BEEP_PIN, 0);
        count++;
        if (count >= beep.time) {
            count = 0;
            flag = !flag;
        }
    }
}

void Key_Init(void){
    //按键引脚初始化
	gpio_init(C31, GPI, 1, GPIO_PIN_CONFIG);
	gpio_init(C27, GPI, 1, GPIO_PIN_CONFIG);
	gpio_init(C26, GPI, 1, GPIO_PIN_CONFIG);
	gpio_init(C4, GPI, 1, GPIO_PIN_CONFIG);
	//拨码开关引脚初始化
	gpio_init(D27, GPI, 1, GPIO_PIN_CONFIG);
	gpio_init(D4, GPI, 1, GPIO_PIN_CONFIG);
    //蜂鸣器引脚初始化
    gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);
}


//按键初始状态为0
//如果状态0或状态1的某按键低电平，计数加一
//如果状态0或状态1的某按键一旦高电平，则回到状态0，计数清零
//如果处于状态0的某按键计数达到FIRST_COUNTER_MAX，则进入状态1，并执行相应动作，计数清零
//如果处于状态1的某按键计数达到SECOND_COUNTER_MAX，则留在状态1，并执行相应动作，计数清零
//该函数要求10ms执行一次
void Key_Scans(void)
{
    static uint8 counter[4], status[4], trigger[4];
    //检查Key1
    if (gpio_get(KEY1)==0)
    {
        counter[Key1]++;
        if (counter[Key1]>=(status[Key1]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key1] = 1;
            trigger[Key1] = 1;
            counter[Key1] = 0;
        }
    }
    else
    {
        status[Key1] = 0;
        counter[Key1] = 0;
    }

    //检查Key2
    if (gpio_get(KEY2)==0)
    {
        counter[Key2]++;
        if (counter[Key2]>=(status[Key2]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key2] = 1;
            trigger[Key2] = 1;
            counter[Key2] = 0;
        }
    }
    else
    {
        status[Key2] = 0;
        counter[Key2] = 0;
    }

    //检查Key3
    if (gpio_get(KEY3)==0)
    {
        counter[Key3]++;
        if (counter[Key3]>=(status[Key3]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key3] = 1;
            trigger[Key3] = 1;
            counter[Key3] = 0;
        }
    }
    else
    {
        status[Key3] = 0;
        counter[Key3] = 0;
    }

    //检查Key4
    if (gpio_get(KEY4)==0)
    {
        counter[Key4]++;
        if (counter[Key4]>=(status[Key4]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key4] = 1;
            trigger[Key4] = 1;
            counter[Key4] = 0;
        }
    }
    else
    {
        status[Key4] = 0;
        counter[Key4] = 0;
    }

    //执行动作
    if (trigger[Key1])
    {
        Key1_Action();
        trigger[Key1] = 0;
    }
    if (trigger[Key2])
    {
        Key2_Action();
        trigger[Key2] = 0;
    }
    if (trigger[Key3])
    {
        Key3_Action();
        trigger[Key3] = 0;
    }
    if (trigger[Key4])
    {
        Key4_Action();
        trigger[Key4] = 0;
    }
}

void Key1_Action(void)
{
    speed_tar=10;
    key1number=0;
}

void Key2_Action(void)
{
    speed_tar=40;
}


void Key3_Action(void)
{
    speed_tar=60;
}

void Key4_Action(void)
{
    ips114_clear(GREEN);
    GUI_TargetPoint();
    speed_tar=0;
}