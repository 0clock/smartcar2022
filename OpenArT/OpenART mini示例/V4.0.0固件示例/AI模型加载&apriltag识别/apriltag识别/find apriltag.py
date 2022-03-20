import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 设置摄像头像素格式
sensor.set_framesize(sensor.QQVGA)  # 设置摄像头分辨率
sensor.set_brightness(1000)         # 设置摄像头亮度
sensor.skip_frames(time = 200)

clock = time.clock()



def family_name(tag):
    if(tag.family() == image.TAG16H5):
        return "TAG16H5"
    if(tag.family() == image.TAG25H7):
        return "TAG25H7"
    if(tag.family() == image.TAG25H9):
        return "TAG25H9"
    if(tag.family() == image.TAG36H10):
        return "TAG36H10"
    if(tag.family() == image.TAG36H11):
        return "TAG36H11"
    if(tag.family() == image.ARTOOLKIT):
        return "ARTOOLKIT"

while(True):
    clock.tick()
    
    img = sensor.snapshot()         # 获取一幅图像
    for tag in img.find_apriltags(families=image.TAG25H9):      # 指定apriltag类型为TAG25H9
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))     # 找到apriltag后绘制矩形框，将apriltag框起来
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0)) # 在apriltag中间绘制十字
        print_args = (family_name(tag), tag.id(), (180 * tag.rotation()) / math.pi)
        # 打印apriltag的类型、ID、旋转角度
        print("Tag Family %s, Tag ID %d, rotation %f (degrees)" % print_args)
    print(clock.fps())
