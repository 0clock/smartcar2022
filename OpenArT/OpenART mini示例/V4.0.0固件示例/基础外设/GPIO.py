from machine import Pin # 从machine导入Pin
import time

pin10=Pin(("B0",0)) # 定义引脚 B端口序号从0开始
pin11=Pin(("B1",1))
pin12=Pin(("B2",2))
pin13=Pin(("B3",3))

pin112=Pin(("B12",12))
pin113=Pin(("B13",13))
pin116=Pin(("B16",16))


pin26=Pin(("C6",32+6)) # 定义引脚 C端口序号从32开始
pin27=Pin(("C7",32+7))
pin28=Pin(("C8",32+8))
pin29=Pin(("C9",32+9))


pin10.init(Pin.IN, Pin.PULL_UP) # 初始化引脚 设置为输入并设置上拉电阻
pin11.init(Pin.OUT_PP, None)    # 初始化引脚 设置为输出没有上下拉电阻
pin12.init(Pin.OUT_PP, None)
pin13.init(Pin.OUT_PP, None)

pin112.init(Pin.OUT_PP, None)
pin113.init(Pin.OUT_PP, None)
pin116.init(Pin.OUT_PP, None)

pin26.init(Pin.OUT_PP, None)
pin27.init(Pin.OUT_PP, None)
pin28.init(Pin.OUT_PP, None)
pin29.init(Pin.OUT_PP, None)

while(True):

    pin11.value(pin10.value())      # 读取B0引脚状态然后用于设置B1的引脚状态

    pin12.value(not pin12.value())  # 引脚电平翻转
    pin13.value(not pin13.value())

    pin112.value(not pin112.value())
    pin113.value(not pin113.value())
    pin116.value(not pin116.value())

    pin26.value(not pin26.value())
    pin27.value(not pin27.value())
    pin28.value(not pin28.value())
    pin29.value(not pin29.value())
