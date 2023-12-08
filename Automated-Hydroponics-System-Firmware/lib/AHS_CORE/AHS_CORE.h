#ifndef AHS_CORE_H
#define AHS_CORE_H

#include "AHS_I2C.h"
#include "AHS_GPIO.h"
#include "AHS_UART.h"
#include "AHS_ADC.h"
#include "AHS_TIMER.h"
#include "ST7735.h"
#include "LCD_GFX.h"
#include "AHS_AHT20.h"
#include "AHS_BH1750.h"

void Initialize();
void Initialize_interrupt(); // Initialise pin change interrupt on Pin 2
void InitializePWM(); // Timer 2 Initialisation
void SetPWM(uint8_t dutyCycle); // Setting OCR2B to control the brightness of LED.
uint8_t pH_map(float pH_value);
float adc_read_pH();
void setpH_desired_region(char *String);
void setpH_axis_bounds(uint8_t pH_pixel, char *String);
void activate_pump(float pH_value);
uint8_t get_dutyCycle_lux(uint64_t lux_value);

#endif