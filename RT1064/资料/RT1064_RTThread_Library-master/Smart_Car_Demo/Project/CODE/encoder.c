#include "encoder.h"

#define ENCODER1_QTIMER		QTIMER_1
#define ENCODER1_A			QTIMER1_TIMER0_C0
#define ENCODER1_B			QTIMER1_TIMER1_C1

#define ENCODER2_QTIMER		QTIMER_1
#define ENCODER2_A			QTIMER1_TIMER2_C2
#define ENCODER2_B			QTIMER1_TIMER3_C24

uint16 speed_l, speed_r;

void encoder_init(void)
{
    qtimer_quad_init(ENCODER1_QTIMER, ENCODER1_A, ENCODER1_B);
	qtimer_quad_init(ENCODER2_QTIMER, ENCODER2_A, ENCODER2_B);
}


void encoder_get(void)
{
    speed_l = qtimer_quad_get(ENCODER1_QTIMER, ENCODER1_A);
    qtimer_quad_clear(ENCODER1_QTIMER, ENCODER1_A);
    speed_r = qtimer_quad_get(ENCODER2_QTIMER, ENCODER2_A);
    qtimer_quad_clear(ENCODER2_QTIMER, ENCODER2_A);
}