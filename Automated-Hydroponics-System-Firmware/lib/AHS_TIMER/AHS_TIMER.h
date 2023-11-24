#ifndef AHS_TIMER_H
#define AHS_TIMER_H

#include "avr/io.h"
#include "avr/interrupt.h"

typedef enum{
    NORMAL = 0,
    PWM_PHASE_FREQ = 1,
    CTC = 2,
    FAST_PWM = 3
}timer_mode_def_t;


void timer0_init(timer_mode_def_t timer_mode);

void timer1_init(timer_mode_def_t timer_mode);

void timer2_init(timer_mode_def_t timer_mode);

#endif