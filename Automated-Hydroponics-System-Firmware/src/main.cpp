/*************************************************************************
 *
 * File: main.cpp
 *
 * Purpose: Main module for automated hydroponics system firmware
 *
 * Authors: Oryza Tarigan, Shreya Maggo, Miles Osborne
 *
 *************************************************************************/
#include "AHS_CORE.h"

char String[25];           // buffer to store pH value
uint8_t pH_pixel;          // value for pixel position of pH value in the graph
volatile int scenario = 0; // variable corresponding to type of display

int main()
{
  Initialize();
  InitializePWM(); // Initilaise PWM for LED brightness settings
  // Initialize_interrupt(); // Initialise Interrupt to control display change

  uint64_t lux_value; // To store the lux value from the light sensor

  while (1)
  {
    if (scenario == 0) // Display the Numeric display
    {
      LCD_setScreen(0xffff); // Reset the LCD screen

      /*---------- Formatting the numerical display screen begins here----------*/
      /*Header title*/
      LCD_drawBlock(0, 0, LCD_WIDTH, 15, 0x1b15);
      LCD_drawString(12, 4, "AUTOMATIC HYDROPONIC SYSTEM", 0xffff, 0x1b15);

      /*Block below for display only*/
      LCD_drawBlock(0, LCD_HEIGHT - 10, LCD_WIDTH, LCD_HEIGHT, 0x1b15);

      /*Sub-header*/
      LCD_drawString(28, 20, "Parameter Monitoring", 0x1b15, 0xffff);

      /*Y-Axis Position of the various parameters*/
      uint8_t y_temp_C = 35;
      uint8_t y_temp_F = 50;
      uint8_t y_hum = 65;
      uint8_t y_light = 80;
      uint8_t y_pH = 95;

      /*X-Axis Position of the display*/
      uint8_t x_sensor_title = 5;
      uint8_t x_sensor_value = 120;

      /* Writing the text for temperature, humidity, light, and pH*/
      LCD_drawString(x_sensor_title, y_temp_C, "Temperature (deg C)  : ", 0x1b15, 0xffff);
      LCD_drawString(x_sensor_title, y_temp_F, "Temperature (deg F)  : ", 0x1b15, 0xffff);
      LCD_drawString(x_sensor_title, y_hum, "Relative humidity (%): ", 0x1b15, 0xffff);
      LCD_drawString(x_sensor_title, y_light, "Light value (lux)    : ", 0x1b15, 0xffff);
      LCD_drawString(x_sensor_title, y_pH, "pH value             : ", 0x1b15, 0xffff);
      /*---------- Formatting the numerical display screen ends here----------*/

      /* Creating buffers and variables to store and print parameter values */
      char buffer[64];
      char temp_buf_1[16];
      char temp_buf_2[16];
      char humidity_buf[16];
      char light_buf[16];
      char pH_buf[10];
      float pH_value;
      AHS_AHT20 sensor;

      while (1)
      {
        if (PIND & (1 << PIND2))
        {
          scenario = 1; // Toggle/switch scene being displayed on the graph
        }

        aht20_init(&sensor); // Initialize Tempertature, Humidity sensor

        aht20_update(&sensor); // Update AHT20 sensor

        lux_value = bh1750_get_light();                    // Getting light_value
        uint8_t duty_cycle = get_dutyCycle_lux(lux_value); // Calculating duty cylce of LED
        SetPWM(duty_cycle);                                // Setting PWM on Pin 2

        pH_value = adc_read_pH(); // Update reading pH sensor
        activate_pump(pH_value);  // Checking the pH value to run pump

        /* -------- Store sensor data in buffers in string format --------- */
        (sensor.temperature_celsius, 6, 2, temp_buf_1);
        dtostrf(sensor.temperature_fahrenheit, 6, 2, temp_buf_2);
        dtostrf(sensor.relative_humidity, 6, 2, humidity_buf);
        sprintf(light_buf, "%d", lux_value);
        dtostrf(pH_value, 6, 2, pH_buf);

        // /* ------ Print sensor values in Serial Terminal (debugging purpose only ------*/

        // sprintf(buffer, "Temperature (C°): %s | ", temp_buf_1);
        // uart_putstring(buffer);
        // sprintf(buffer, "Temperature (F°): %s | ", temp_buf_2);
        // uart_putstring(buffer);
        // sprintf(buffer, "Relative humidity: %s | ", humidity_buf);
        // uart_putstring(buffer);
        // sprintf(buffer, "Light value: %d | ", lux_value);
        // uart_putstring(buffer);
        // sprintf(buffer, "pH value: %s\n", pH_buf);
        // uart_putstring(buffer);

        /* ------------ Print sensor values to LCD ------------ */

        // Temperature (C°)
        LCD_drawBlock(x_sensor_value, y_temp_C, (x_sensor_value + 30), (y_temp_C + 10), 0xffff); // Deleting the previous value
        LCD_drawString(x_sensor_value, y_temp_C, temp_buf_1, 0x1b15, 0xffff);                    // Printing temp sensor value in C

        // Temperature (F°)
        LCD_drawBlock(x_sensor_value, y_temp_F, (x_sensor_value + 30), (y_temp_F + 10), 0xffff); // Deleting the previous value
        LCD_drawString(x_sensor_value, y_temp_F, temp_buf_2, 0x1b15, 0xffff);                    // Printing temp sensor value in F

        // Relative humidity
        LCD_drawBlock(x_sensor_value, y_hum, (x_sensor_value + 30), (y_hum + 10), 0xffff); // Deleting the previous value
        LCD_drawString(x_sensor_value, y_hum, humidity_buf, 0x1b15, 0xffff);               // Printing humidity sensor value in %

        // Light value
        LCD_drawBlock(x_sensor_value + 5, y_light, (x_sensor_value + 30), (y_light + 10), 0xffff); // Deleting the previous value (the digit of the previous value may be larger)
        LCD_drawString(x_sensor_value + 5, y_light, light_buf, 0x1b15, 0xffff);                    // Printing light sensor value in lux

        // pH value
        LCD_drawBlock(x_sensor_value - 5, y_pH, (x_sensor_value + 30), (y_pH + 10), 0xffff); // Deleting the previous value
        LCD_drawString(x_sensor_value - 5, y_pH, pH_buf, 0x1b15, 0xffff);                    // Printing pH sensor value

        /* If you want to see the raw data, uncomment the 2 lines below: */
        // sprintf(buffer, "Raw temperature data: %d\n\n", sensor.temperature_data_raw);
        // uart_putstring(buffer);

        /* Break out of current scene if interrupt is detected */
        if (scenario == 1 && !(PIND & (1 << PIND2)))
        {
          break;
        }
      }
    }

    else if (scenario == 1)
    {
      LCD_setScreen(0xffff); // Reset the LCD screen

      LCD_drawLine(20.8, 0, 20.8, LCD_HEIGHT, 0x0000); // Draw y axis line

      setpH_desired_region(String); // Draw allowable range of pH

      setpH_axis_bounds(pH_pixel, String); // Draw upper and lower y axis bounds

      sprintf(String, "Real time pH monitoring");                                  // Storing in buffer for printing
      LCD_drawString(0.25 * LCD_WIDTH, 0.03 * LCD_HEIGHT, String, 0x0000, 0xffff); // Printing the x over y graph header

      while (1)
      {
        sprintf(String, "scenario 1\n");
        // uart_putstring(String);
        /* Break out of current scene if interrupt is detected s*/
        if (PIND & (1 << PIND2))
        {
          scenario = 0; // Toggle/switch scene being displayed on the graph
        }

        if (scenario == 0 && !(PIND & (1 << PIND2)))
        {
          break;
        }

        // Create array to store all y-axis (ph_pixel) values
        int pH_pixel_array[139];

        // Iterate over all x values. Print pH_pixel for each
        for (int x = 21; x <= LCD_WIDTH; x++)
        {
          /* LED Brightness Part */
          lux_value = bh1750_get_light();                    // Getting light_value
          uint8_t duty_cycle = get_dutyCycle_lux(lux_value); // Calculating duty cylce of LED
          SetPWM(duty_cycle);                                // Setting PWM on Pin 2
          float pH_value = adc_read_pH();                    // pH_value that will be taken from pH sensor

          activate_pump(pH_value); // Checking the pH value to run pump

          pH_pixel = pH_map(pH_value);        // Finding the corresponding pH_pixel for the pH value obtained by sensor
          LCD_drawPixel(x, pH_pixel, 0x0000); // Draw the pH vs reading

          // Update array with pixel value.
          pH_pixel_array[x] = pH_pixel;

          // Re-start from the left edge of graph when it occupies entire screen width
          if (x == LCD_WIDTH)
          {
            // Re-set x value to left edge of graph
            x = 21;

            /****************************************************************
             * Break out of current scene if inerrupt is detected
             *
             * NOTE: We need another break condition here since the for
             * loop's condition variable is reset to the initial state
             * effectively creating an infinite loop
             ****************************************************************/
            if (PIND & (1 << PIND2))
            {
              scenario = 0; // Toggle/switch scene being displayed on the graph
            }

            if (scenario == 0 && !(PIND & (1 << PIND2)))
            {
              break;
            }

            // Clear each pixel of the previous graph
            for (int i = 21; i < LCD_WIDTH; i++)
            {
              // Consider edge case when previous graph overlaps with upper and lower bounds
              if (pH_pixel_array[i] >= pH_map(6) && pH_pixel_array[i] <= pH_map(5.5))
              {
                LCD_drawPixel(i, pH_pixel_array[i], 0x6e43);
              }
              else
              {
                LCD_drawPixel(i, pH_pixel_array[i], 0xffff);
              }

              if (PIND & (1 << PIND2))
              {
                scenario = 0; // Toggle/switch scene being displayed on the graph
              }
            }
          }
        }
      }
    }
  }
}