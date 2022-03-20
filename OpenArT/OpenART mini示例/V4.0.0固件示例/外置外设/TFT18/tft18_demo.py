import sensor, image, time
import seekfree, pyb

# IO接线方法
# 屏         openart
# GND       ---> GND
# VCC       ---> 3.3V
# SCL       ---> B0(SCLK)
# SDA/MISO  ---> B1(MISO)
# RESET     ---> B12
# DC        ---> B13
# CS        ---> B3
# BL        ---> B16 (背光控制)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)

# 3：屏幕显示方向 参数范围0-3
lcd = seekfree.LCD180(3)
lcd.full()  # 将背景颜色显示到整个屏幕

# lcd.help() 使用此help可以查看所有的函数以及函数参数介绍
# lcd.info()


while(True):
    img=sensor.snapshot()

    # 显示图像
    # img：需要显示的图像
    # 160：图像的宽度
    # 120：图像的高度
    # zoom：图像缩放 参数范围0、2、4、8
    lcd.show_image(img, 160, 120, zoom=2)

    # 显示图像并指定图像显示的起点
    # img：需要显示的图像
    # 160：图像的宽度
    # 120：图像的高度
    # 0：x起点为0
    # 0：y起点为0
    # zoom：图像缩放 参数范围0、2、4、8
    # lcd.show_image(img, 160, 120, 0, 0, zoom=2)

    # 显示字符串，背景色不修改
    # 'SEEKFREE'：需要显示的字符串
    # 0：x坐标（范围是0-159）
    # 44：y坐标（0-119）
    # lcd.RED：字体颜色
    # 1：是否保留背景颜色
    lcd.show_str('SEEKFREE',0, 44, lcd.RED, 1)


    # 显示字符串，背景色为白色
    # 'SEEKFREE'：需要显示的字符串
    # 0：x坐标（范围是0-159）
    # 44：y坐标（0-119）
    lcd.show_str('SEEKFREE', 0, 60)

