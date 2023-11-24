#include "AHS_GPIO.h"

void gpio_init()
{

}

void set_pin(uint8_t* port, uint8_t pin)
{
    *port |= (1 << pin);
}

void clear_pin(uint8_t *port, uint8_t pin)
{
    *port &= (1 << pin);
}

void set_port(uint8_t *port)
{
    *port |= (0xFF);
}

void clear_port(uint8_t *port)
{
    *port &= ~(0xFF);
}