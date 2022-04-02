#include "GUI.h"

void GUI_init(){
	ips114_init();     	//≥ı ºªØ1.47¥ÁIPS∆¡ƒª
	ips114_showstr(10,0,"everything is okk!");
}

void GUI_speed(){
	ips114_showint16(10,2,encoder1); 
	ips114_showint16(10,3,encoder2);
	ips114_showint16(10,4,encoder3);
	ips114_showint16(10,5,encoder4);
}

void GUI_icm20602(){
	ips114_showint16(100,2,icm_acc_x);
	ips114_showint16(100,3,icm_acc_y);
	ips114_showint16(100,4,icm_acc_z);
	
	ips114_showint16(180,2,icm_gyro_x);
	ips114_showint16(180,3,icm_gyro_y);
	ips114_showint16(180,4,icm_gyro_z);
}

void GUI_duty(){
	ips114_showint16(255,2,duty1);
	ips114_showint16(255,3,duty2);
	ips114_showint16(255,4,duty3);
	ips114_showint16(255,5,duty4);
	
	ips114_showint16(255,6,speed_tar_1);
	ips114_showint16(255,7,speed_tar_2);
	ips114_showint16(255,8,speed_tar_3);
	ips114_showint16(255,9,speed_tar_4);
}