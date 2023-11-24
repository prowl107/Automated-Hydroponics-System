#ifndef AHS_GPIO_H
#define AHS_GPIO_H

#include "avr/io.h"

void gpio_init();

void set_pin(uint8_t* port, uint8_t pin);

void clear_pin(uint8_t *port, uint8_t pin);

void set_port(uint8_t *port);

void clear_port(uint8_t *port);

#endif