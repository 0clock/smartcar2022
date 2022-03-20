import pyb
from pyb import LED #导入LED

print('Start test LED\r\n') # 通过串口3输出提示信息 C8为TX C9为RX
red = LED(1)    # 定义一个LED1   红灯
green = LED(2)  # 定义一个LED2   绿灯
blue = LED(3)   # 定义一个LED3   蓝灯
white = LED(4)  # 定义一个LED4   照明灯

while(True):
    red.on()        # 打开红灯
    pyb.mdelay(500) # 延时500ms
    red.off()       # 关闭红灯
    pyb.mdelay(500)

    green.on()
    pyb.mdelay(500)
    green.off()
    pyb.mdelay(500)

    blue.on()
    pyb.mdelay(500)
    blue.off()
    pyb.mdelay(500)

    white.on()
    pyb.mdelay(500)
    white.off()
    pyb.mdelay(500)



