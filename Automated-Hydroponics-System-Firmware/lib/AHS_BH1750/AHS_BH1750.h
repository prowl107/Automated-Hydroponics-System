#ifndef AHS_BH1750_H
#define AHS_BH1750_H

#include "avr/io.h"
#include "avr/interrupt.h"
#include "AHS_I2C.h"
#include "stdio.h"

#define BH1750_SLAVE_ADDRESS 0x23
#define BH1750_CONTINUOUS_RES_INSTRUCTION 0x10
 
void bh1750_init();

uint16_t bh1750_get_light();
uint16_t bh1750_get_light_raw();

#endif