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
    ips114_showint16(10,8,Car.mileage);
}

void GUI_icm20602(){
	ips114_showint16(80,2,icm_data.gyro_x);
	ips114_showint16(80,3,icm_data.gyro_x);
	ips114_showint16(80,4,icm_data.gyro_x);

	ips114_showint16(80,6,icm_data.acc_x);
	ips114_showint16(80,7,icm_data.acc_y);
	ips114_showint16(80,8,icm_data.acc_z);
}

void GUI_duty(){
	ips114_showint16(160,2,duty1);
	ips114_showint16(160,3,duty2);
	ips114_showint16(160,4,duty3);
	ips114_showint16(160,5,duty4);
	
	ips114_showint16(160,6,speed_tar_1);
	ips114_showint16(160,7,speed_tar_2);
	ips114_showint16(160,8,speed_tar_3);
	ips114_showint16(160,9,speed_tar_4);
}

void GUI_TargetPoint(){
    ips114_drawRectangle(240, 30, 315, 80,GREEN);
}