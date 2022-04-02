import pyb
from machine import PWM # 导入PWM


# 需要特别注意
# 需要特别注意
# 需要特别注意
# seekfree下的Servo模块 可以与 machine下的PWM模块同时使用但是需要注意一些问题
# Servo占用了C6 或者 C7的时候 PWM就只能使用C8 C9两个引脚
# Servo占用了C8 或者 C9的时候 PWM就只能使用C6 C7两个引脚




pwm1=PWM(2,1,100,50)   # 初始化pwm 使用pwm2 通道1 频率100hz 占空比数值设置为101(设置范围是0-255) PWM通过C6输出
pwm1.freq(2000)        # 修改频率为2khz
pwm2=PWM(2,2,2000,100) # 使用pwm2通道2与使用pwm2通道1使用的是同一个定时器，因此两个通道频率必须一致，占空比可以不同 PWM通过C7输出

pwm3=PWM(2,3,3000,150) # 初始化pwm 使用pwm2 通道3 频率3khz 占空比数值设置为101(设置范围是0-255)  PWM通过C8输出
pwm4=PWM(2,4,3000,200) # 使用pwm2通道3与使用pwm2通道4使用的是同一个定时器，因此两个通道频率必须一致，占空比可以不同 PWM通过C9输出

n = 255
while(n>0):

    pwm1.duty(n)    # 修改占空比 占空比设置范围0-255
    pwm2.duty(n)    # 修改占空比 占空比设置范围0-255
    pwm3.duty(n)
    pwm4.duty(n)
    n = n-1         # 重新计算占空比 下次设置
    pyb.mdelay(2)   # 延时2ms

pwm1.deinit()       # PWM2通道1反初始化
pwm2.deinit()
pwm3.deinit()
pwm4.deinit()
