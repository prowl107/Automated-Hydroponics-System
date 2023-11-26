#ifndef AHS_I2C_LIB
#define AHS_I2C_LIB

#include "avr/io.h"
#include "avr/interrupt.h"

void i2c_init();

void i2c_start();

void i2c_stop();

void i2c_write(uint8_t data);

uint8_t i2c_read();
#endif