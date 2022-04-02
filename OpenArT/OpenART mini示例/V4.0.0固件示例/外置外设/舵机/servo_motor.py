
import sensor, image, time, pyb
import seekfree
from seekfree import Servo # 从seekfree导入舵机模块


# 需要特别注意
# 需要特别注意
# 需要特别注意
# seekfree下的Servo模块 可以与 machine下的PWM模块同时使用但是需要注意一些问题
# Servo占用了C6 或者 C7的时候 PWM就只能使用C8 C9两个引脚
# Servo占用了C8 或者 C9的时候 PWM就只能使用C6 C7两个引脚




# 在创建舵机对象前务必调用此函数进行初始化
seekfree.init()

# 新建一个舵机对象
# 2：选择二通道，信号从C7输出（可设置范围是1-4，分别对应引脚C6 C7 C8 C9）
# 50：频率设置为50hz（可设置范围是50-300）
# 45：初始化角度设置为45度（可设置范围0-180）
servo_1 = Servo(2,50,45)


# servo_1.help()           # 可以调用help来查看舵机模块有哪些函数接口以及函数参数介绍
temp = 0                   # 控制舵机转动的变量
dir = 0                    # 控制舵机转动的方向
while(True):
    pass
    # 1.使用角度函数控制舵机转动
    if dir == 1:           # temp变量逐渐从0度增加到180度
        temp += 1
        if temp >= 180:
            temp = 180
            dir = 0
    else:                  # temp变量逐渐从180度递减到0度
        temp -= 1
        if temp <= 0:
            temp = 0
            dir = 1
    servo_1.angle(temp)    # 最终输出角度给舵机，然后舵机转动到指定位置
    print(servo_1.angle()) # 获取舵机角度信息并打印出来
    pyb.mdelay(10)        # 做一个短暂的延时，便于观察现象

    # 2.使用设置脉宽的方式控制舵机
    #if dir == 1:          # temp变量逐渐从500us增加到2500us
        #temp += 1
        #if temp >= 2500:
            #temp = 2500
            #dir = 0
    #else:
        #temp -= 1         # temp变量逐渐从2500us递减到500us
        #if temp <= 500:
            #temp = 500
            #dir = 1
    #servo_1.pulse_width(temp) # 最终输出脉宽给舵机，然后舵机转动到指定位置
    #print(servo_1.pulse_width()) # 获取舵机脉宽信息并打印出来
    #pyb.mdelay(100)       # 做一个短暂的延时，便于观察现象

    # 3.舵机输出的关闭与开启方法
    #servo_1.disable()     # 关闭输出，引脚就不会输出信号了
    #pyb.mdelay(1000)
    #servo_1.enable()      # 使能输出，引脚继续输出信号
