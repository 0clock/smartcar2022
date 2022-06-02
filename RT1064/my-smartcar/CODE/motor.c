#include "motor.h"
#include <math.h>

#define PWM_LIMIT 25000

int32 duty1=0,duty2=0,duty3=0,duty4=0;//电机PWM值
int16 encoder1=0,encoder2=0,encoder3=0,encoder4=0;//编码器的值
int16 RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4;//滤波之后encoder的值

//---------------------结构体---------------------//
struct RC_Para Encoder1_Para = {0,0,10};
struct RC_Para Encoder2_Para = {0,0,10};
struct RC_Para Encoder3_Para = {0,0,10};
struct RC_Para Encoder4_Para = {0,0,10};



RC_Filter_pt RC_Encoder1 = &Encoder1_Para;
RC_Filter_pt RC_Encoder2 = &Encoder2_Para;
RC_Filter_pt RC_Encoder3 = &Encoder3_Para;
RC_Filter_pt RC_Encoder4 = &Encoder4_Para;


//记录PID系数
float Position_KP =150;
float Position_KI =20;
float Position_KD =0;

float Angel_KP = 5;
float Angel_KI = 0;
float Angel_KD = 15;

//电机目标速度
int speed_tar_1 = 0;
int speed_tar_2 = 0;
int speed_tar_3 = 0;
int speed_tar_4 = 0;

int speed_tar = 0;//目标速度
float deta_mileage=0;

/**
 * @name: 全向解算
 **/

void Car_SpeedGet(){
    Car.Speed_X=(int16)(speed_tar * sin(Car.Angel_Target/180 *PI));//((float)speed_tar * sin(Car.Angel_Target/180 *PI)),((float)speed_tar * cos(Car.Angel_Target/180 *PI)),0);
    Car.Speed_Y=(int16)(speed_tar * cos(Car.Angel_Target/180 *PI));
    Car.Speed_Z=-Angel_PID(Car.Angel,0);
}
void Car_Omni(int16 x, int16 y, int16 z){
    speed_tar_1= y + x + z;
    speed_tar_2= y - x + z;
    speed_tar_3= y + x - z;
    speed_tar_4= y - x - z;
}


void Car_Ahead(){
	speed_tar_1 = speed_tar;
	speed_tar_2 = speed_tar;
	speed_tar_3 = speed_tar;
	speed_tar_4 = speed_tar;
}

void Car_Back(){
	speed_tar_1 = -speed_tar;
	speed_tar_2 = -speed_tar;
	speed_tar_3 = -speed_tar;
	speed_tar_4 = -speed_tar;
}

void Car_SideWay(){
	speed_tar_1 = speed_tar;
	speed_tar_2 = -speed_tar;
	speed_tar_3 = speed_tar;
	speed_tar_4 = -speed_tar;
}

void Car_RSideWay(){
	speed_tar_1 = -speed_tar;
	speed_tar_2 = speed_tar;
	speed_tar_3 = -speed_tar;
	speed_tar_4 = speed_tar;
}

void Car_Diagonal(){
	speed_tar_1 = speed_tar;
	speed_tar_2 = 0;
	speed_tar_3 = speed_tar;
	speed_tar_4 = 0;
}

void Car_Turnround(){
	speed_tar_1 = speed_tar;
	speed_tar_2 = speed_tar;
	speed_tar_3 = -speed_tar;
	speed_tar_4 = -speed_tar;
}

void Car_Anticlockwise() {
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void Car_Concerning(){
	speed_tar_1 = speed_tar;
	speed_tar_2 = speed_tar;
	speed_tar_3 = 0;
	speed_tar_4 = 0;
}

void Car_Stop(){
    speed_tar_1 = 0;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}


//增量式PID
int Position_PID1(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=(float)(Target - Encoder);//当前误差
	Integral_bias+=Bias;//误差的积累
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);//当前误差-上次误差
	Last_Bias=Bias;
	return (int)Pwm;
}

int Position_PID2(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=(float)(Target - Encoder);
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return (int)Pwm;
}

int Position_PID3(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=(float)(Target - Encoder);
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return (int)Pwm;
}


int Position_PID4(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=(float)(Target - Encoder);
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return (int)Pwm;
}

int Angel_PID(int NowAngel,int TargetAngel){
    static float Bias,Speed_Z,Integral_bias,Last_Bias;
    Bias=(float)(TargetAngel - NowAngel);
    Integral_bias+=Bias;
    Speed_Z=Angel_KP*Bias+Angel_KI*Integral_bias+Angel_KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    if(Speed_Z>=70)
        Speed_Z=70;
    if(Speed_Z<=-70)
        Speed_Z=-70;
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

void PID_Calculate(void){
	duty1 = Position_PID1(RC_encoder1,speed_tar_1);
	duty2 = Position_PID2(RC_encoder2,speed_tar_2);
	duty3 = Position_PID3(RC_encoder3,speed_tar_3);
	duty4 = Position_PID4(RC_encoder4,speed_tar_4);
	
	duty1 = limit_pwm(duty1);
	duty2 = limit_pwm(duty2);
	duty3 = limit_pwm(duty3);
	duty4 = limit_pwm(duty4);
}

void Motor_Init(){
	gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D0 初始化DIR_1			GPIO
	gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D1 初始化DIR_2			GPIO
	pwm_init(PWM_1, 17000, 0);      							//单片机端口D2 初始化PWM_1周期10K 占空比0
	pwm_init(PWM_2, 17000, 0);     								//单片机端口D3 初始化PWM_2周期10K 占空比0
	gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);     //单片机端口D0 初始化DIR_1          GPIO
	gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);     //单片机端口D1 初始化DIR_2          GPIO
	pwm_init(PWM_3, 17000, 0);                     //单片机端口D2 初始化PWM_1周期10K 占空比0
	pwm_init(PWM_4, 17000, 0);                     //单片机端口D3 初始化PWM_2周期10K 占空比0
}


void Motor_Ctrl(){
	PID_Calculate();
		
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

/**
 * @name 全向移动位移
 * bug:无法算出负值？
 **/
void Omni_Mileage(){
    float detax=0,detay=0;
    detax=(float)(RC_encoder1 - RC_encoder2 + RC_encoder3 - RC_encoder4)/4;
    detay=(float)(RC_encoder1 + RC_encoder2 + RC_encoder3 + RC_encoder4)/4;
    Car.MileageX+=(float)(detax*0.0095);
    Car.MileageY+=(float)(detay*0.0086);
}


void RCEncoder_Init(void)
{
    RC_Encoder1->RC = 0.6;
    RC_Encoder2->RC = 0.6;
    RC_Encoder3->RC = 0.6;
    RC_Encoder4->RC = 0.6;

    RC_Encoder1->value = 0;
    RC_Encoder2->value = 0;
    RC_Encoder3->value = 0;
    RC_Encoder4->value = 0;

    RC_Encoder1->temp = 0;
    RC_Encoder2->temp = 0;
    RC_Encoder3->temp = 0;
    RC_Encoder4->temp = 0;
}

void Encoder_Init(){
	//一个QTIMER可以 创建两个正交解码
	qtimer_quad_init(QTIMER_3,QTIMER3_TIMER2_B18,QTIMER3_TIMER3_B19);
	qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
	//初始化 QTIMER_1 A相使用QTIMER1_TIMER0_C0 B相使用QTIMER1_TIMER1_C1
	qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
	//初始化 QTIMER_1 A相使用QTIMER2_TIMER0_C3 B相使用QTIMER2_TIMER3_C25
	qtimer_quad_init(QTIMER_2,QTIMER2_TIMER0_C3,QTIMER2_TIMER3_C25);
}

void Get_Encoder(){
	encoder1 = qtimer_quad_get(QTIMER_3,QTIMER3_TIMER2_B18 ); //这里需要注意第二个参数务必填写A相引脚
	encoder2 = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 ); //这里需要注意第二个参数务必填写A相引脚
	encoder3 = -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2); //这里需要注意第二个参数务必填写A相引脚
	encoder4 = -qtimer_quad_get(QTIMER_2,QTIMER2_TIMER0_C3); //这里需要注意第二个参数务必填写A相引脚


    //计算位移(单位：m)
    //Car.mileage=(Encoder/1024)*(45/104)*2*PI*0.03;
    Omni_Mileage();

    RC_encoder1 = (int16_t)RCFilter(encoder1,RC_Encoder1);
    RC_encoder2 = (int16_t)RCFilter(encoder2,RC_Encoder2);
    RC_encoder3 = (int16_t)RCFilter(encoder3,RC_Encoder3);
    RC_encoder4 = (int16_t) RCFilter(encoder4,RC_Encoder4);
	
	qtimer_quad_clear(QTIMER_3,QTIMER3_TIMER2_B18 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	qtimer_quad_clear(QTIMER_2,QTIMER2_TIMER0_C3 );
}
