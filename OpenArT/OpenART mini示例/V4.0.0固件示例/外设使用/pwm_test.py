import pyb
from machine import PWM # 导入PWM


pwm1=PWM(2,1,2000,50)  # 初始化pwm 使用pwm2 通道1 频率2khz 占空比数值设置为50(设置范围是0-255) PWM通过C6输出
pwm2=PWM(2,2,2000,100) # 使用pwm2通道2与使用pwm2通道1使用的是同一个定时器，因此两个通道频率必须一致，占空比可以不同

pwm3=PWM(2,3,3000,150) # 初始化pwm 使用pwm2 通道3 频率3khz 占空比数值设置为150(设置范围是0-255)  PWM通过C8输出
pwm4=PWM(2,4,3000,200) # 使用pwm2通道3与使用pwm2通道4使用的是同一个定时器，因此两个通道频率必须一致，占空比可以不同

n = 255
while(n>0):
    pwm1.duty(n)    # 修改占空比 占空比设置范围0-255
    pwm2.duty(n)    # 修改占空比 占空比设置范围0-255
    pwm3.duty(n)
    pwm4.duty(n)
    n = n-1         # 重新计算占空比 下次设置
    pyb.mdelay(10)   # 延时2ms
    if n == 0:
        while(n < 255):
           pwm1.duty(n)    # 修改占空比 占空比设置范围0-255
           pwm2.duty(n)    # 修改占空比 占空比设置范围0-255
           pwm3.duty(n)
           pwm4.duty(n)
           n = n+1
           pyb.mdelay(10)   # 延时2ms

pwm1.deinit()       # PWM2通道1反初始化 释放资源
pwm2.deinit()
pwm3.deinit()
pwm4.deinit()
