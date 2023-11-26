/*************************************************************************
 *
 * File: main.cpp
 *
 * Purpose: Main module for automated hydroponics system firmware
 *
 *************************************************************************/

#include <avr/io.h>
#include <stdio.h>
#include "AHS_CORE.h"

#define F_CPU

int main()
{
  uint8_t state_id = 0;
  /* Insert code here :) */
  char buffer[32];
  sprintf(buffer, "Hello World");

  while (1)
  {
    switch (state_id)
    {
    case 0: /* Sys init */
      gpio_init();
      adc_init();
      timer0_init(NORMAL);
      i2c_init();
      lcd_init();
      uart_init(9600);

      /*********************************************************************
       * TODO: Initialize sensors here:
       *********************************************************************/
      // light_sensor_init();
      // temp_humidity_sensor_init();
      // pH_sensor_init();

      /*********************************************************************
       * TODO: Initialize peripherals here:
       *********************************************************************/
      // light_array_init()
      // pump_ctrl_init();

      uart_putstring(buffer);
      break;

    case 1:
      /*********************************************************************
       * TODO: Read pH sensor, update pH value
       *********************************************************************/
      // ph_val = ph_sensor_read();

      /*********************************************************************
       * TODO: Evaluate current pH value --> Adjust pump speed accordingly
       *********************************************************************/

      // if(ph_val <= lower_bound)
      // {
      //   /* Enable pump */
      // }else if(ph_val >= upper_bound && pump == enabled)
      // {
      //   /* Disable/turn-off pump */
      // }

      state_id = 2;
      break;

    case 2: /* Update light exposure */
      /*********************************************************************
       * TODO: Read light sensor, update current light exposure value
       *********************************************************************/
      // lux_cnt = light_sensor_read();

      /*********************************************************************
       * TODO: Evaluate current lux cnt --> Adjust pump speed accordingly
       *********************************************************************/
      // adjust_light_intensity(lux_cnt);

      state_id = 3;

      break;

    case 3: /* Display values */
      /*********************************************************************
       * TODO: Update temp & humidity sensor values
       *********************************************************************/
      // temp_farenheit = AHT20_read_temp(farenheit);
      // temp_celsius = AHT20_read_temp(celsisus);
      // humidity = AHT20_read_humidity();

      /*********************************************************************
       * TODO: Update display
       *********************************************************************/
#if 0
    if (scene_1)
    {
      /* Write data to display */
    }else if(scene_2){
      /* Display graph of pH value over time */
    }
#endif

      state_id = 1;
      break;

    default:
      break;
    }
  }

  return 0;
}