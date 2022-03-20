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


    #透视变换（未完成）

    #找黑点
    blobs = img.find_blobs([BlackPoint],area_threshold=12,roi=area)

    for blob in blobs:
        #img.draw_rectangle(blob[0:4],123)
        img.draw_cross(blob[5], blob[6])


#(69, 212)
#(181, 255)
