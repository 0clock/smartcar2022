#ifndef _GUI_H_
#define _GUI_H_

#include "motor.h"
#include "key.h"

enum Gui_Page{
    InfoPage,
    ConfigPage,
    TargetPointPage,
    AboutPage
};

void GUI_init(void);
void GUI_speed(void);
void GUI_icm20602(void);
void GUI_duty(void);
void GUI_TargetPoint(void);

#endif