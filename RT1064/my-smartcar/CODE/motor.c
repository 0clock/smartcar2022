#include "motor.h"
#include "headfile.h"

uint8 dir1=0,dir2=0,dir3=0,dir4=0;
int32 duty1=0,duty2=5000,duty3=5000,duty4=0;
int16 encoder1,encoder2,encoder3,encoder4;

void motorinit(){
	gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D0 初始化DIR_1			GPIO
	gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D1 初始化DIR_2			GPIO
	pwm_init(PWM_1, 17000, 0);      				//单片机端口D2 初始化PWM_1周期10K 占空比0
	pwm_init(PWM_2, 17000, 0);     					//单片机端口D3 初始化PWM_2周期10K 占空比0
	gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D0 初始化DIR_1          GPIO
	gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D1 初始化DIR_2          GPIO
	pwm_init(PWM_3, 17000, 0);                      //单片机端口D2 初始化PWM_1周期10K 占空比0
	pwm_init(PWM_4, 17000, 0);                      //单片机端口D3 初始化PWM_2周期10K 占空比0
}


void motorctrl(){
	if(duty1>=0){
		gpio_set(DIR_1,0);
		pwm_duty(PWM_1,duty1);
	} else {
		gpio_set(DIR_1,1);
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
		gpio_set(DIR_4,0);
		pwm_duty(PWM_4,duty4);
	} else {
		gpio_set(DIR_4,1);
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

void getencoder(){
	encoder1 = qtimer_quad_get(QTIMER_3,QTIMER3_TIMER2_B18 ); //这里需要注意第二个参数务必填写A相引脚
	encoder2 = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 ); //这里需要注意第二个参数务必填写A相引脚
	encoder3 = -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2); //这里需要注意第二个参数务必填写A相引脚
	encoder4 = -qtimer_quad_get(QTIMER_2,QTIMER2_TIMER0_C3); //这里需要注意第二个参数务必填写A相引脚
	
	ips114_showint16(10,2,encoder1); 
	ips114_showint16(10,3,encoder2);
	ips114_showint16(10,4,encoder3);
	ips114_showint16(10,5,encoder4);	
	
	qtimer_quad_clear(QTIMER_3,QTIMER3_TIMER2_B18 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	qtimer_quad_clear(QTIMER_2,QTIMER2_TIMER0_C3 );
	
	systick_delay_ms(5);
}

