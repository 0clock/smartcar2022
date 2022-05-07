#ifndef _GUI_H_
#define _GUI_H_

#include "motor.h"
#include "key.h"

enum Gui_Page{
    InfoPage,
    ConfigPage,
    CameraPage,
    TargetPointPage,
    AboutPage
};

extern enum Gui_Page Page_Number;
extern int8 Gui_Page_Active;
void GUI_init(void);
void GUI_FirstPage(void);
void GUI_Motor(void);
void GUI_icm20602(void);
void GUI_TargetPoint(void);

#endif