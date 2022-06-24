#include "GUI.h"

enum Gui_Page Page_Number=CameraPage;
int8 Gui_Page_Active=TRUE;
void GUI_init(){
	ips114_init();     	//初始化1.47寸IPS屏幕
/*	ips114_showstr(10,0,"everything is okk!!             AHSTU");
    ips114_drawline(0,20,320,20,WHITE);*/
    //GUI_TargetPoint();
}

void GUI_FirstPage(){
    if(Gui_Page_Active==TRUE){
        ips114_clear(BLACK);
        Gui_Page_Active=FALSE;
    }
    switch(Page_Number){
        case InfoPage:
            GUI_icm20602();
            GUI_Motor();
            break;
        case ConfigPage:
            ips114_showstr(10,0,"Config:");
            break;
        case TargetPointPage:
            ips114_showstr(10,0,"Route");
            GUI_TargetPoint();
            break;
        case CameraPage:
            //ips114_showstr(10,0,"Camera:");
            if(mt9v03x_csi_finish_flag)			//图像采集完成
            {
                mt9v03x_csi_finish_flag = 0;	//清除采集完成标志位
                ImageHandel();
                ips114_displayimage032_zoom(img_thres.data, MT9V03X_CSI_W, MT9V03X_CSI_H, 240, 135);	//显示摄像头图像
            }
            break;
        case AboutPage:
            ips114_showstr(10,0,"About:");
            break;
    }
}


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
    ips114_showint8(220,0,Car.Position_Pointer);
}

void GUI_icm20602(){
    ips114_showstr(10,5,"Angel:");
    ips114_showfloat(15,6,Car.Angel,3,4);
    ips114_showstr(10,8,"An_Tar:");
    ips114_showint16(15,9,Car.Angel_Target);
}


void GUI_TargetPoint(){
    ips114_drawRectangle(55, 11, 265, 161,GREEN);
    for(int i=0;i<=locate_sz;i++){
        ips114_drawCircle(55+Car_Location[i][0]*6,161-Car_Location[i][1]*6,1,WHITE);
        //ips114_drawline(55+Car_Location_Route[i][0]*6,161-Car_Location_Route[i][1]*6,240+Car_Location_Route[i+1][0]*6,80-Car_Location_Route[i+1][1]*6,WHITE);
    }
    //ips114_drawpoint(Car.x,Car.y,RED);
}