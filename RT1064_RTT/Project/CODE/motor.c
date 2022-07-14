#include "motor.h"
#include "encoder.h"
#include "flash_param.h"

#define DIR_1 D0
#define DIR_2 D14
#define DIR_3 D15
#define DIR_4 D1
#define PWM_LIMIT 25000
#define PWM_1 PWM2_MODULE3_CHA_D2
#define PWM_2 PWM1_MODULE0_CHA_D12
#define PWM_3 PWM1_MODULE0_CHB_D13
#define PWM_4 PWM2_MODULE3_CHB_D3

int32 duty1=0,duty2=0,duty3=0,duty4=0;//电机PWM值






//积分法计算位移参数

//电机目标速度
int speed_tar_1 = 0;
int speed_tar_2 = 0;
int speed_tar_3 = 0;
int speed_tar_4 = 0;

int speed_tar = 0;//目标速度


void motor_init(void)
{
    gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D0 初始化DIR_1			GPIO
    gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D1 初始化DIR_2			GPIO
    pwm_init(PWM_1, 17000, 0);      					//单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_2, 17000, 0);     						//单片机端口D3 初始化PWM_2周期10K 占空比0
    gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);       //单片机端口D0 初始化DIR_1          GPIO
    gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);       //单片机端口D1 初始化DIR_2          GPIO
    pwm_init(PWM_3, 17000, 0);                          //单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_4, 17000, 0);                          //单片机端口D3 初始化PWM_2周期10K 占空比0											// PWM 通道4 初始化频率10KHz 占空比初始为0
}


void car_omni(int16 x, int16 y, int16 z){
    speed_tar_1= y + x + z;
    speed_tar_2= y - x + z;
    speed_tar_3= y + x - z;
    speed_tar_4= y - x - z;
}



void car_ahead(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_back(){
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_sideWay(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_rsideWay(){
    speed_tar_1 = -speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = speed_tar;
}

void car_diagonal(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = 0;
    speed_tar_3 = speed_tar;
    speed_tar_4 = 0;
}

void car_turnround(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_anticlockwise() {
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_concerning(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}

void car_stop(){
    speed_tar_1 = 0;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}

/**
 * @name: position_pid
 * @msg: 位置式pid
 * @param {int Encoder,int Target}
 * @return {int pwm}
 */
int position_pid1(int Encoder,int Target){
    static float Bias,Pwm,Integral_bias,Last_Bias;
    Bias=(float)(Target - Encoder);//当前误差
    Integral_bias+=Bias;//误差的积累
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);//当前误差-上次误差
    Last_Bias=Bias;
    return (int)Pwm;
}

int position_pid2(int Encoder,int Target){
    static float Bias,Pwm,Integral_bias,Last_Bias;
    Bias=(float)(Target - Encoder);
    Integral_bias+=Bias;
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    return (int)Pwm;
}

int position_pid3(int Encoder,int Target){
    static float Bias,Pwm,Integral_bias,Last_Bias;
    Bias=(float)(Target - Encoder);
    Integral_bias+=Bias;
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    return (int)Pwm;
}


int position_pid4(int Encoder,int Target){
    static float Bias,Pwm,Integral_bias,Last_Bias;
    Bias=(float)(Target - Encoder);
    Integral_bias+=Bias;
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    return (int)Pwm;
}

int angel_pid(int NowAngel,int TargetAngel){
    if (NowAngel<=0){
        if(NowAngel-TargetAngel<=-180){
            NowAngel+=180;
            TargetAngel-=180;
        }
    }else if(NowAngel>0){
        if(NowAngel-TargetAngel>=180){
            NowAngel-=180;
            TargetAngel+=180;
        }
    }
    static float Bias,Speed_Z,Integral_bias,Last_Bias;
    Bias=(float)(TargetAngel - NowAngel);
    Integral_bias+=Bias;
    Speed_Z=Angel_KP*Bias+Angel_KI*Integral_bias+Angel_KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    if(Speed_Z>=20)
        Speed_Z=20;
    if(Speed_Z<=-20)
        Speed_Z=-20;
    return (int)Speed_Z;
}

/**
 * @name: limit_pwm
 * @msg: pwm限幅
 * @param {int pwm}
 * @return {int pwm}
 */

int limit_pwm(int pwm){
    if (pwm>=0)
    {
        if (pwm>=PWM_LIMIT)
        {
            pwm = PWM_LIMIT;
        }

    }
    else if (pwm<=0)
    {
        if (pwm<-PWM_LIMIT)
        {
            pwm = -PWM_LIMIT;
        }
    }
    return pwm;
}


/**
 * @name: PID_Calculate
 * @msg: 调用各个轮子的pid函数，得出运算后的PWM值
 * @param {void}
 * @return {void}
 */

void pid_calculate(void){
    duty1 = position_pid1(RC_encoder1,speed_tar_1);
    duty2 = position_pid2(RC_encoder2,speed_tar_2);
    duty3 = position_pid3(RC_encoder3,speed_tar_3);
    duty4 = position_pid4(RC_encoder4,speed_tar_4);

    duty1 = limit_pwm(duty1);
    duty2 = limit_pwm(duty2);
    duty3 = limit_pwm(duty3);
    duty4 = limit_pwm(duty4);
}


void motor_control(bool run)
{
    if(run) {
        pid_calculate();
    }else{
        duty1 = 0;
        duty2 = 0;
        duty3 = 0;
        duty4 = 0;
    }

    if(duty1>=0){
        gpio_set(DIR_1,1);
        pwm_duty(PWM_1,duty1);
    } else {
        gpio_set(DIR_1,0);
        pwm_duty(PWM_1,-duty1);
    }
    if(duty2>=0){
        gpio_set(DIR_2,0);
        pwm_duty(PWM_2,duty2);
    } else {
        gpio_set(DIR_2,1);
        pwm_duty(PWM_2,-duty2);
    }

    if(duty3>=0){
        gpio_set(DIR_3,0);
        pwm_duty(PWM_3,duty3);
    } else {
        gpio_set(DIR_3,1);
        pwm_duty(PWM_3,-duty3);
    }

    if(duty4>=0){
        gpio_set(DIR_4,1);
        pwm_duty(PWM_4,duty4);
    } else {
        gpio_set(DIR_4,0);
        pwm_duty(PWM_4,-duty4);
    }
}