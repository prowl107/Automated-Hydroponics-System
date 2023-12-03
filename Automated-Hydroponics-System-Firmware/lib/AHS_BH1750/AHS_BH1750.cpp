#include "AHS_BH1750.h"

void bh1750_init()
{
  i2c_start();
  i2c_setAddress(0x23, 0);
  i2c_write((0x10));
  i2c_stop();
}

uint16_t bh1750_get_light()
{
    i2c_start();
    i2c_setAddress(0x23, 1);
    uint16_t high_byte = i2c_read();
    uint16_t low_byte = i2c_read();
    int data = (high_byte << 8) | low_byte;
    i2c_stop();
    return data;
}