import sensor, image, time

sensor.reset()                      # 摄像头复位
sensor.set_pixformat(sensor.RGB565) # 设置摄像头像素格式  可选 RGB565 与 GRAYSCALE
sensor.set_framesize(sensor.QVGA)   # 设置分辨率大小 QVGA(320x240) QQVGA(160x120)(AI分类的时候推荐QQVGA，可以提高运算速度)


# 设置自动曝光
# 参数一：True 开启自动曝光 False关闭自动曝光
# 参数二：开启自动曝光时可设置范围是1到255  关闭自动曝光时可设置范围是1-65535
sensor.set_auto_exposure(True, 100)


# 设置亮度
# 参数一：开启自动曝光时可设置范围是1到255  关闭自动曝光时可设置范围是1-65535
sensor.set_brightness(100)

# 需要特别注意 水平镜像 与 垂直翻转 需要在配置完sensor其他参数后进行设置

# sensor参数配置完毕，接下来配置水平镜像 与 垂直翻转
# 设置垂直翻转
sensor.set_vflip(True)

# 设置水平镜像
sensor.set_hmirror(True)


# 如果 设置垂直翻转与设置水平镜像没有效果并且你需要使用到这个功能则联系技术协助判断问题

sensor.skip_frames(time = 200)      # 跳过一些帧


while(True):
    img = sensor.snapshot()         # 获取一幅图像

