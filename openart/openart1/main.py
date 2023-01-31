# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time, math
from machine import UART

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)

threshold=(181, 255)
uart = UART(1, baudrate=115200)

sensor.set_auto_gain(True)          #自动增益
sensor.set_auto_whitebal(True)      #自动白平衡

sensor.skip_frames(time = 10)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.

BlackPoint = (0, 181)




while(True):
    clock.tick()                    # Update the FPS clock.

    img = sensor.snapshot()         # Take a picture and return the image.

    img.get_statistics(roi=(0,0,10,20))
    img.binary([threshold])

    #找矩形
    Points = img.find_rects(threshold =100000)
    if(Points):
        for r in Points:
            img.draw_rectangle(r.rect(), color = (255, 0, 0))
            for p in r.corners():
                img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
                img.draw_string(p[0]-5,p[1]-10,"(" + str(int(p[0])) + "," + str(int(p[1])) + ")", color = (100, 255, 0), scale = 1, mono_space = False)
                print(p)
            uart_array = bytearray([0XFF,int((r.rect()[0] + r.rect()[2]/2)),int((r.rect()[1] + r.rect()[3]/2))])
            uart.write(uart_array)

            #print(int((r.rect()[0] + r.rect()[2])/2))

            print(r.rect()[0] + r.rect()[2]/2)
            print("-------")
            #uart_array = bytearray([0XFF,int(4),int(0),int((r.rect()[0] + r.rect()[2]/2)/2),int((r.rect()[1] + r.rect()[3]/2)/2)])
            #uart_array = bytearray([int(r.rect()[0])])
            #uart.write(uart_array)

    else:
        uart_array = bytearray([0XFE,0XEE])
        uart.write(uart_array)
        print("No Picture")



    #print(Points)  #[{"x":24, "y":12, "w":280, "h":191, "magnitude":235714}]
    #print("--------------------")



    #透视变换（未完成）





#(69, 212)
#(181, 255)
