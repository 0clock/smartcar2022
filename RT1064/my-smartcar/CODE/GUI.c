#include "GUI.h"

enum Gui_Page Page_Number=InfoPage;
int8 Gui_Page_Active=TRUE;
void GUI_init(){
	ips114_init();     	//≥ı ºªØ1.47¥ÁIPS∆¡ƒª
/*	ips114_showstr(10,0,"everything is okk!!             AHSTU");
    ips114_drawline(0,20,320,20,WHITE);*/
    GUI_TargetPoint();
}

void GUI_FirstPage(){
    if(Gui_Page_Active==TRUE){
        ips114_clear(BLACK);
        Gui_Page_Active=FALSE;
    }
    switch(Page_Number){
        case InfoPage:
            break;
        case ConfigPage:
            break;
        case TargetPointPage:
            break;
        case AboutPage:
            break;
    }
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
}

void GUI_duty(){
	ips114_showint16(160,2,Car.MileageX);
	ips114_showint16(160,3,Car.MileageY);

    ips114_showint16(160,5,Car.DistanceX);
    ips114_showint16(160,6,Car.DistanceY);

    ips114_showint16(160,8,Car.Speed_Z);
}

void GUI_TargetPoint(){
    ips114_drawRectangle(55, 11, 265, 161,GREEN);
    for(int i=0;i<=locate_sz;i++){
        ips114_drawCircle(55+Car_Location[i][0]*6,161-Car_Location[i][1]*6,1,WHITE);
        ips114_drawline(55+Car_Location_Route[i][0]*6,161-Car_Location_Route[i][1]*6,240+Car_Location_Route[i+1][0]*6,80-Car_Location_Route[i+1][1]*6,WHITE);
    }
    ips114_drawpoint(Car.x,Car.y,RED);
}