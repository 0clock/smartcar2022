# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)

threshold=(181, 255)


sensor.set_auto_gain(True)          #自动增益
sensor.set_auto_whitebal(False)      #自动白平衡

sensor.skip_frames(time = 10)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.

BlackPoint = (0, 181)



while(True):
    clock.tick()                    # Update the FPS clock.

    img = sensor.snapshot()         # Take a picture and return the image.

    img.get_statistics(roi=(0,0,10,20))
    img.binary([threshold])

    #找矩形
    Points = img.find_rects(threshold = 20000)
    for r in Points:
        img.draw_rectangle(r.rect(), color = (255, 0, 0))
        for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))

    area=[r[0],r[1],r[2],r[3]]
    img.draw_rectangle(area,123)
    print(Points)  #[{"x":24, "y":12, "w":280, "h":191, "magnitude":235714}]
    print("--------------------")

    #透视变换（未完成）


    #找黑点
    blobs = img.find_blobs([BlackPoint],area_threshold=12,roi=area)

    for blob in blobs:
        #img.draw_rectangle(blob[0:4],123)
        img.draw_cross(blob[5], blob[6])
    print(blobs)  #[{"x":16, "y":6, "w":288, "h":203, "pixels":4637, "cx":155, "cy":111, "rotation":0.073228, "code":1, "count":1, "perimeter":4111, "roundness":0.613540},
                  #{"x":104, "y":65, "w":6, "h":6, "pixels":32, "cx":106, "cy":68, "rotation":0.000000, "code":1, "count":1, "perimeter":18, "roundness":0.913043},
                  #{"x":174, "y":67, "w":6, "h":6, "pixels":32, "cx":176, "cy":70, "rotation":0.000000, "code":1, "count":1, "perimeter":18, "roundness":0.913043},
                  #{"x":103, "y":120, "w":7, "h":7, "pixels":33, "cx":106, "cy":123, "rotation":0.000000, "code":1, "count":1, "perimeter":20, "roundness":0.692308},
                  #{"x":202, "y":122, "w":7, "h":6, "pixels":29, "cx":205, "cy":124, "rotation":0.169646, "code":1, "count":1, "perimeter":20, "roundness":0.776191},
                  #{"x":61, "y":159, "w":7, "h":7, "pixels":39, "cx":64, "cy":162, "rotation":0.000000, "code":1, "count":1, "perimeter":19, "roundness":0.905512},
                  #{"x":102, "y":159, "w":7, "h":7, "pixels":36, "cx":105, "cy":162, "rotation":0.000000, "code":1, "count":1, "perimeter":19, "roundness":0.953271},
                  #{"x":241, "y":160, "w":6, "h":6, "pixels":30, "cx":244, "cy":162, "rotation":0.000000, "code":1, "count":1, "perimeter":17, "roundness":0.948052}]
    print("--------------------")
    #输出点的坐标



#(69, 212)
#(181, 255)
