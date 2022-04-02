import pyb
from machine import Pin # 从machine导入Pin
from pyb import LED #导入LED

PB0 = Pin(("B0",0)) # 定义引脚 B端口序号从0开始
PB0.init(Pin.OUT_PP, Pin.PULL_NONE)

# 函数功能：根据引脚电平，点亮或者关闭LED(4)
def led(pin):
    if(pin.value()):
        LED(4).on()
    if(not pin.value()):
        LED(4).off()

while(True):
    PB0.value(1)    # 设置PB0引脚为高电平
    led(PB0)
    pyb.mdelay(500) # 延时500ms
    print(PB0.value())

    PB0.value(0)    # 设置PB0引脚为低电平
    led(PB0)
    pyb.mdelay(1500) # 延时1500ms
    print(PB0.value())
