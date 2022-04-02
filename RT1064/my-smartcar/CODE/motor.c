#include "motor.h"
#include <math.h>

int32 duty1=0,duty2=0,duty3=0,duty4=0;//电机PWM值
int16 encoder1=0,encoder2=0,encoder3=0,encoder4=0;//编码器的值

//记录PID系数
float Position_KP = 3;
float Position_KI = 2;
float Position_KD =2;

//电机目标速度
int speed_tar_1 = 500;
int speed_tar_2 = 500;
int speed_tar_3 = 500;
int speed_tar_4 = 500;


int speed = 0;

void Car_Ahead()
{
	speed_tar_1 = 500;
	speed_tar_2 = 500;
	speed_tar_3 = 500;
	speed_tar_4 = 500;
}

void Car_Back()
{
	speed_tar_1 = -500;
	speed_tar_2 = -500;
	speed_tar_3 = -500;
	speed_tar_4 = -500;
}

void Car_SideWay()
{
	speed_tar_1 = 500;
	speed_tar_2 = -500;
	speed_tar_3 = 500;
	speed_tar_4 = -500;
}

void Car_RSideWay()
{
	speed_tar_1 = -500;
	speed_tar_2 = 500;
	speed_tar_3 = -500;
	speed_tar_4 = 500;
}

void Car_Diagonal()
{
	speed_tar_1 = 500;
	speed_tar_2 = 0;
	speed_tar_3 = 500;
	speed_tar_4 = 0;
}

void Car_Turnround()
{
	speed_tar_1 = 500;
	speed_tar_2 = 500;
	speed_tar_3 = -500;
	speed_tar_4 = -500;
}

void Car_Concerning()
{
	speed_tar_1 = 500;
	speed_tar_2 = 500;
	speed_tar_3 = 0;
	speed_tar_4 = 0;
}


//增量式PID

int Position_PID1(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Target - Encoder;
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return Pwm;
}

int Position_PID2(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Target - Encoder;
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return Pwm;
}

int Position_PID3(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Target - Encoder;
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return Pwm;
}


int Position_PID4(int Encoder,int Target){ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Target - Encoder;
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return Pwm;
}
/**
 * @name: limit_pwm 
 * @msg: pwm限幅，这里的幅值是+-10000
 * @param {int pwm} 
 * @return {int pwm} 
 */

int limit_pwm(int pwm){
  if (pwm>=0)
  {
    if (pwm>=10000)
    {
      pwm = 10000;
    }
    
  }
  else if (pwm<=0)
  {
    if (pwm<-10000)
    {
      pwm = -10000;
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
	duty1 = Position_PID1(encoder1,speed_tar_1);
	duty2 = Position_PID2(encoder2,speed_tar_2);
	duty3 = Position_PID3(encoder3,speed_tar_3);
	duty4 = Position_PID4(encoder4,speed_tar_4);
	
	duty1 = limit_pwm(duty1);
	duty2 = limit_pwm(duty2);
	duty3 = limit_pwm(duty3);
	duty4 = limit_pwm(duty4);
}

void motorinit(){
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


void encoderinit(){
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
	
	qtimer_quad_clear(QTIMER_3,QTIMER3_TIMER2_B18 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	qtimer_quad_clear(QTIMER_2,QTIMER2_TIMER0_C3 );
}

