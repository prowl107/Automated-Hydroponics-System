#ifndef AHS_ADC_H
#define AHS_ADC_H

#include "avr/io.h"

void adc_init();

uint16_t adc_read_value(uint8_t pin);

#endif