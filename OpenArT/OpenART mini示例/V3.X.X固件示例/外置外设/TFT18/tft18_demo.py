import sensor, image, time
import SEEKFREE_18TFT as tft18

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
tft18.tft_init()

#在运行前务必将SEEKFREE_IPS114_SPI.py放入 SD卡根目录并复位OpenART
#在运行前务必将SEEKFREE_IPS114_SPI.py放入 SD卡根目录并复位OpenART
#在运行前务必将SEEKFREE_IPS114_SPI.py放入 SD卡根目录并复位OpenART

while(True):
    img=sensor.snapshot()

#图像上显示字符
    tft18.tft_display_string_in_image(img,20,40,"openART",255,0,0)

#图像上显示数字,使用和字符一个函数，需要用str()格式转换
    tft18.tft_display_string_in_image(img,20,60,str(1.23),255,0,0)

#将图像显示到屏幕
    tft18.tft_display(img,160,120)

    #以下三个显示函数是直接操作屏幕，会导致屏幕刷新很慢，建议使用tft_display_string_in_image来进行数字或者字符显示。
    #tft18.ips_shownum(50,2,1.23)
    #tft18.ips_showchar(50,3,"-")
    #tft18.ips_showstr(50,4,"ABCDijkl")
