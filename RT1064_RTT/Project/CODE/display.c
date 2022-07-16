#include "headfile.h"
#include "encoder.h"
#include "display.h"
#include "location.h"
#include "button.h"
#include "attitude_solution.h"
#include "MahonyAHRS.h"
#include "imgproc.h"
#include "flash_param.h"
#include "carmove.h"

enum Gui_Page Page_Number=InfoPage;
bool Gui_Page_Active=true;

extern image_t img_raw;
extern image_t img_thres;
extern image_t img_line;
extern float ahrs_kp;
extern int tempTest;


extern int openartBuffer;

void GUI_Motor(){
    ips114_showstr(10,0,"Encoder:");
    ips114_showint16(15,1,RC_encoder1);
    ips114_showint16(15,2,RC_encoder2);
    ips114_showint16(15,3,RC_encoder3);
    ips114_showint16(15,4,RC_encoder4);
    ips114_showstr(100,0,"Mileage:");
    ips114_showint16(105,1,Car.MileageX);
    ips114_showint16(105,2,Car.MileageY);
    ips114_showstr(100,4,"Distance:");
    ips114_showint16(105,5,Car.DistanceX);
    ips114_showint16(105,6,Car.DistanceY);
    ips114_showstr(100,8,"Speed:");
    ips114_showint16(105,9,Car.Speed_Z);

    ips114_showstr(190,0,"NP:");
    //ips114_showint8(220,0,Car.Position_Pointer);
	ips114_showint32(220,0,tempTest,2);

    ips114_showstr(190,2,"SpeedX,Y,Z");
    ips114_showint8(195,3,Car.Speed_X);
    ips114_showint8(230,3,Car.Speed_Y);
    ips114_showint8(265,3,Car.Speed_Z);

    ips114_showstr(190,5,"NextPoint:");
    ips114_showint8(195,6,originMap[nextpoint][0]);
    ips114_showint8(230,6,originMap[nextpoint][1]);

    ips114_showstr(190,7,"NowPoint:");
    ips114_showint8(195,8,Car.x);
    ips114_showint8(230,8,Car.y);

}

void GUI_icm20602(){
    ips114_showstr(10,5,"Angel:");
    ips114_showfloat(15,6,Car.Angel,3,4);
    ips114_showstr(10,8,"An_Tar:");
    ips114_showint16(15,9,Car.Angel_Target);
}


void GUI_TargetPoint(){
    ips114_drawRectangle(55, 11, 265, 161,GREEN);
/*    for(int i=0;i<=locate_sz;i++){
        ips114_drawCircle(55+Car_Location[i][0]*6,161-Car_Location[i][1]*6,1,WHITE);
        //ips114_drawline(55+Car_Location_Route[i][0]*6,161-Car_Location_Route[i][1]*6,240+Car_Location_Route[i+1][0]*6,80-Car_Location_Route[i+1][1]*6,WHITE);
    }*/
    //ips114_drawpoint(Car.x,Car.y,RED);
}

void config_page(){
    uint8_t line=1;
    switch (config_action) {
        case MileageX:
            ips114_showstr(10,line,"#");
            break;
        case MileageY:
            ips114_showstr(10,line+1,"#");
            break;
        case AngelP:
            ips114_showstr(10,line+2,"#");
            break;
        case AngelI:
            ips114_showstr(10,line+3,"#");
            break;
        case AngelD:
            ips114_showstr(10,line+4,"#");
            break;
        case MotorP:
            ips114_showstr(10,line+5,"#");
            break;
        case MotorI:
            ips114_showstr(10,line+6,"#");
            break;
        case MotorD:
            ips114_showstr(10,line+7,"#");
            break;
        default:break;
    }
    ips114_showstr(30,line,"MileKX:");
    ips114_showfloat(100,line,MileageKx,1,5);
    ips114_showfloat(200,line,Car.MileageX,3,2);
    ips114_showstr(30,line+1,"MileKY:");
    ips114_showfloat(100,line+1,MileageKy,1,5);
    ips114_showfloat(200,line+1,Car.MileageY,3,2);
    ips114_showstr(30,line+2,"AngelP:");
    ips114_showfloat(100,line+2,Angel_KP,2,3);
    ips114_showstr(30,line+3,"AngelI:");
    ips114_showfloat(100,line+3,Angel_KI,2,3);
    ips114_showstr(30,line+4,"AngelD:");
    ips114_showfloat(100,line+4,Angel_KD,2,3);
    ips114_showstr(30,line+5,"MotorP:");
    ips114_showfloat(100,line+5,Position_KP,3,2);
    ips114_showstr(30,line+6,"MotorI:");
    ips114_showfloat(100,line+6,Position_KI,3,2);
    ips114_showstr(30,line+7,"MotorD:");
    ips114_showfloat(100,line+7,Position_KD,3,2);
}
void display_page(){
    if(Gui_Page_Active){
        ips114_clear(BLACK);
        Gui_Page_Active=false;
    }
    switch(Page_Number){
        case InfoPage:
            GUI_icm20602();
            GUI_Motor();
            break;
        case ConfigPage:
            ips114_showstr(10,0,"Config:");
            config_page();
            // 调试模式下定时写入flash参数
            static int flash_cnt = 0;
            if (++flash_cnt % 100 == 0) {
                flash_param_write();
            }
            break;
        case TargetPointPage:
            ips114_showstr(10,0,"Route");
            GUI_TargetPoint();
            break;
        case CameraPage:
            ips114_showstr(10,0,"Camera:");
            ips114_showint8(10,2,openartBuffer);
            //ips114_displayimage032_zoom1(mt9v03x_csi_image[0], MT9V03X_CSI_W  , MT9V03X_CSI_H, 66, 26, MT9V03X_CSI_W, MT9V03X_CSI_H);
            //ips114_showfloat(80,0,thres,3,2);
            //mt9v03x_csi_finish_flag = 0;	//清除采集完成标志位
            //ImageHandel();
            //ips114_displayimage032_zoom1(img_thres.data, MT9V03X_CSI_W, MT9V03X_CSI_H, 66, 26, MT9V03X_CSI_W, MT9V03X_CSI_H);
            //ips114_displayimage032_zoom(img_thres.data, MT9V03X_CSI_W, MT9V03X_CSI_H, 240, 135);	//显示摄像头图像
            //ips114_displayimage032_zoom(img_raw.data, MT9V03X_CSI_W, MT9V03X_CSI_H, 240, 135);	//显示摄像头图像
//            ips114_drawline(66+Xt,26+Yl,66+Xt,26+Yl,BLUE);
//            ips114_drawline(66+Xt,26+Yr,66+Xb,26+Yr,BLUE);
//            ips114_drawline(66+Xb,26+Yr,66+Xb,26+Yl,BLUE);
//            ips114_drawline(66+Xb,26+Yl,66+Xt,26+Yl,BLUE);
            break;
        case AboutPage:
            ips114_showstr(10,0,"About:");
            break;
    }
}


void display_entry(void *parameter)
{
    //角度环测试
//	ips114_clear(BLACK);
//    rt_thread_delay(5000);
//    Car.Angel_Target=10;
//    rt_thread_delay(1000);
//    Car.Angel_Target=100;
//    rt_thread_delay(1500);
//    Car.Angel_Target=-179;
//    rt_thread_delay(1500);
//    Car.Angel_Target=170;
//    rt_thread_delay(1500);
//    Car.Angel_Target=0;
//    rt_thread_delay(1500);
//    Car.Angel_Target=-179;
//    rt_thread_delay(1500);
//    Car.Angel_Target=0;
//    rt_thread_delay(1500);
//    Car.Angel_Target=90;
//    rt_thread_delay(1500);
//    Car.Angel_Target=-90;
//    rt_thread_delay(1500);
//    Car.Angel_Target=180;
//    rt_thread_delay(1500);
//    Car.Angel_Target=0;
    while(1){
        display_page();
        //ips114_showstr(10,0,"camera");
        //ips114_displayimage032_zoom1(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 66, 26, MT9V03X_CSI_W, MT9V03X_CSI_H);
    }


}




void display_init(void)
{
    rt_thread_t tid;
    
    //初始化屏幕
    //ips114_init();
    ips114_clear(BLACK);

    //创建显示线程 优先级设置为31
    tid = rt_thread_create("display", display_entry, RT_NULL, 1024, 11, 16);
    
    //启动显示线程
    if(RT_NULL != tid)
    {
        rt_thread_startup(tid);
    }
}