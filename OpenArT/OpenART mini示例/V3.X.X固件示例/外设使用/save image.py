from machine import UART
import pyb
import struct
from pyb import LED
import sensor, image, time, math, tf
import os, nncu


red = LED(1)
green = LED(2)
blue = LED(3)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)     # 设置摄像头像素格式
sensor.set_framesize(sensor.QQVGA)      # 设置摄像头分辨率
sensor.set_brightness(1000)             # 设置摄像头亮度 越大越亮
sensor.set_auto_whitebal(False,(0,0x80,0))
sensor.skip_frames(time = 20)

clock = time.clock()

save_img_num = 0;
while(True):
    img = sensor.snapshot()             # 获取一幅图像
    blue.toggle()                       # 蓝灯翻转

    # 搜索矩形
    for r in img.find_rects(threshold = 50000):
        # 绘制矩形框 便于查看识别到的矩形位置
        img.draw_rectangle(r.rect(), color = (255, 0, 0))

        # 拷贝识别到的矩形
        img1 = img.copy(r.rect())

        # 修改文件名称，准备保存
        save_img_num += 1;
        image_pat = "/num"+str(save_img_num)+".jpg"

        # 将拷贝之后的图像保存到sd卡
        img1.save(image_pat,quality=99)


