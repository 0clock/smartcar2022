# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time
from machine import UART



threshold=(181, 255)
uart = UART(1, baudrate=115200)


clock = time.clock()                # Create a clock object to track the FPS.

BlackPoint = (0, 181)

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_auto_gain(True)          #自动增益
sensor.set_auto_whitebal(True)      #自动白平衡
sensor.skip_frames(time = 10)     # Wait for settings take effect.

isgetmap=0


#uart.any()
while(True):
    while(True):
        temp = uart.readline().decode()
        buff=temp
        cc=[0xFD]
        if buff=='1':
            img = sensor.snapshot().lens_corr(strength = 0.3, zoom = 1)         # Take a picture and return the image.
            img.get_statistics(roi=(0,0,10,20))
            #img.binary([threshold])
            #找矩形
            Points = img.find_rects(threshold = 20000)
            for r in Points:
                if r.w() > 100 and r.w() < 200 and r.h() > 50 and r.h() < 150 :
                    print(r.corners()[0][0]-r.corners()[3][0])
                    print(r.corners()[0][1]-r.corners()[1][1])
                    if (r.corners()[0][1]-r.corners()[1][1])==0 and (r.corners()[0][0]-r.corners()[3][0])==0:
                        img.draw_rectangle(r.rect(), color = (255, 0, 0))
                        print(r.corners()[0])
                        for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
                        area=[r[0],r[1],r[2],r[3]]
                        img.draw_rectangle(area,123)
                        #print(Points)  #[{"x":24, "y":12, "w":280, "h":191, "magnitude":235714}]
                        print("--------------------")
                        #print(r)
                        Xwidth = 35 / (r[2])
                        Yheigt = 25 / (r[3])
                        Xmiddle=r[0]+r[2]/2
                        Ymiddle=r[1]+r[3]/2
                        #print("Middle:" ,Xmiddle,Ymiddle)
                        #透视变换（未完成）
                        #找黑点
                        blobs = img.find_blobs([(0,200)],roi=area)
                        for blob in blobs:
                            #print(blob)
                            #img.draw_rectangle(blob[0:4],123)
                            img.draw_cross(blob[5], blob[6])
                            if (blob[4] <= 100):
                                print ("Find true blob : ",blob[0:2])
                                blobstrue = [(blob.cxf()-r[0])+3 ,r[3]-(blob.cyf()-r[1])+2]
                                coordinate = [round (blobstrue[0] * Xwidth) ,round (blobstrue[1] * Yheigt)]
                                cc.append(coordinate[0])
                                cc.append(coordinate[1])

                                isgetmap=1;
                                #print (blobstrue)
                                img.draw_string(blob.x() - 5, blob.y() - 10, "(" + str(int(coordinate[0])) + "," + str(int(coordinate[1])) + ")", color = (0, 0, 255), scale = 1, mono_space = False)
                               #print (coordinate)
                                #blobstrue[0]/Xwidth,blobstrue[1]/Yheigt
                                #输出点的坐标
                    else:
                        uart_array = bytearray([0XFC,int(r.corners()[0][0]-r.corners()[3][0]),int(r.corners()[0][1]-r.corners()[1][1])])
                        uart.write(uart_array)

                    if isgetmap:
                        cc.append(int(0xDD))
                        uart_array = bytearray(cc)
                        uart.write(uart_array)
                        print(cc)

        elif buff == '2':
            img = sensor.snapshot()         # Take a picture and return the image.
            isgetpicture=1
            blobscount=0
            img.get_statistics(roi=(0,0,10,20))
            img.binary([threshold])

            blobs = img.find_blobs([(222,255)],merge=True)
            print(blobs)
            for b in blobs:
                blobscount+=1
                img.draw_rectangle(b.rect())
                if b.pixels()>200 and b.pixels()<10000:
                    uart_array = bytearray([0XFF,int(b.cx()/2),int(b.cy()/2)])
                    uart.write(uart_array)
                    isgetpicture=0
                    print("blobsx=",b.cx())
                    print("blobsy=",b.cy())

                    '''
            if isgetpicture:
                rects = img.find_rects(threshold =100000)            #找矩形
                if(rects):
                    for r in rects:
                        img.draw_rectangle(r.rect(), color = (255, 0, 0))
                        for p in r.corners():
                            img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
                            img.draw_string(p[0]-5,p[1]-10,"(" + str(int(p[0])) + "," + str(int(p[1])) + ")", color = (100, 255, 0), scale = 1, mono_space = False)
                            print(p)
                        uart_array = bytearray([0XFF,int((r.rect()[0] + r.rect()[2]/2)),int((r.rect()[1] + r.rect()[3]/2))])
                        uart.write(uart_array)
                        isgetpicture=1
                        #print(int((r.rect()[0] + r.rect()[2])/2))
                        print(r.rect()[0] + r.rect()[2]/2)
                        print("-------")
                        #uart_array = bytearray([0XFF,int(4),int(0),int((r.rect()[0] + r.rect()[2]/2)/2),int((r.rect()[1] + r.rect()[3]/2)/2)])
                        #uart_array = bytearray([int(r.rect()[0])])
                        #uart.write(uart_array)
'''
            if isgetpicture:
                uart_array = bytearray([0XFE,0XEE])
                uart.write(uart_array)
                print("No Picture")

