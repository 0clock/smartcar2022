#ifndef _display_h
#define _display_h

#include "headfile.h"

enum Gui_Page{
    InfoPage,
    ConfigPage,
    CameraPage,
    TargetPointPage,
    AboutPage
};

extern enum Gui_Page Page_Number;
extern bool Gui_Page_Active;
void display_init(void);
    
#endif