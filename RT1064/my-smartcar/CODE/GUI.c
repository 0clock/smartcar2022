#include "GUI.h"

uint8 GUI_areaflag=1;

void GUI_init(){
	ips114_init();     	//≥ı ºªØ1.47¥ÁIPS∆¡ƒª
	ips114_showstr(10,0,"everything is okk!!             AHSTU");
    ips114_drawline(0,20,320,20,WHITE);
    GUI_TargetPoint();
}

void GUI_speed(){
    ips114_showint16(10,3,RC_encoder1);
	ips114_showint16(10,4,RC_encoder2);
	ips114_showint16(10,5,RC_encoder3);
	ips114_showint16(10,6,RC_encoder4);
}

void GUI_icm20602(){
    ips114_showfloat(80,2,ahrs_angle.z,3,4);
    ips114_showfloat(80,3,cpmangle_z,3,4);

    ips114_showint16(80,5,Car.Angel_Target);
    ips114_showint16(80,7,((float)speed_tar * sin(Car.Angel_Target/180 *PI)));
    ips114_showint16(80,8,((float)speed_tar * cos(Car.Angel_Target/180 *PI)));
}

void GUI_duty(){
	ips114_showint16(160,2,Car.MileageX);
	ips114_showint16(160,3,Car.MileageY);

    ips114_showint16(160,5,Car.DistanceX);
    ips114_showint16(160,6,Car.DistanceY);
}

void GUI_TargetPoint(){
    ips114_drawRectangle(240, 30, 315, 80,GREEN);
    for(int i=0;i<=locate_sz;i++){
        ips114_drawCircle(240+Car_Location[i][0]*2,80-Car_Location[i][1]*2,1,WHITE);
    }
    ips114_drawpoint(Car.x,Car.y,RED);
}