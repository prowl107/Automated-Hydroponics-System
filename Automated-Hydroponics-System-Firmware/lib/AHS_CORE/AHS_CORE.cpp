#include "AHS_CORE.h"

void Initialize()
{
  lcd_init();          // Intialise LCD
  uart_init(9600);     // Initialise serial communication
  adc_init();          // Initialise Analog to Digital Converter (A0)
  i2c_init();          // Initialise I2C communication
  bh1750_init();       // Initialise light sensor
  DDRC |= (1 << PC2);  // Pin D5 set to output for pump
  DDRD &= ~(1 << PD2); // Pin D2 set to input for switch
}

/* ---------- Function to Initialise pin change interrupt on Pin 2 ---------- */
void Initialize_interrupt() // Initialise pin change interrupt on Pin 2
{
  cli();                    // Disable global interrupts
  DDRD &= ~(1 << DDD2);     // Pin D2 for the switch
  PORTD |= (1 << DDD2);     // Enable pull up resistor for PD2
  PCICR &= ~(0X7);               /* Reset pin change inerrupt enable */
  PCICR |= (1 << PCIE2);    // Enable PCINT18 pin change interrupt
  PCICR &= ~(1 << PCIE1);
  PCICR &= ~(1 << PCIE0);
  PCMSK2 = 0;
  PCMSK2 |= (1 << PCINT18); // Enable trigger for PCINT18
  PCMSK1 = 0;
  PCMSK0 = 0;
  sei();                    // Enable global interrupts
}

/* ---------- Timer 2 Initialisation ---------- */
void InitializePWM() // Timer 2 Initialisation
{
  DDRD |= (1 << DDD3); // Set PD3 (Pin 3) as an output for PWM

  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // Configure Timer/Counter 2 for Fast PWM mode, non-inverting output
  TCCR2B |= (1 << WGM22) | (1 << CS21);                  // Timer Prescaler 8

  OCR2A = 0xFF; // Set the TOP value for 8-bit PWM resolution (0xFF)
}

/* ---------- Function to control LED brightness ---------- */
void SetPWM(uint8_t dutyCycle) // Setting OCR2B to control the brightness of LED.
{
  OCR2B = dutyCycle; // Set the OCR2B register to control the duty cycle
}

/* ---------- Function to map pH value to LCD pixel to be plotted ---------- */
uint8_t pH_map(float pH_value)
{
  float upper_pH = 8.5;                                                       // Max pH value displayed on graph's y-axis
  float pH_pixel_upper = 3.5;                                                 // Min pH value displayed on graph's y-axis
  float segment = 11;                                                         // Pixels allotted per 0.5 pH
  uint8_t delta = (((upper_pH - pH_value) / 0.5) * segment) + pH_pixel_upper; // Linear relationship to find mapped pixel value

  return delta;
}

/* ---------- Function to get pH values from SEN0161---------- */
float adc_read_pH()
{
  uint16_t voltage_value = adc_read_value(0);                 // Read pH sensor reading from A0 pin
  float ph_value = ((float)voltage_value - 286.452) / 15.637; // Linear relationship between voltage and pH

  return ph_value;
}

/* ---------- Function to set allowable pH range on graphical display---------- */
void setpH_desired_region(char *String)
{
  uint16_t pH_pixel_lower = pH_map(5.5); // Find y-coordinate of pixel corresponding to 5.5 pH

  sprintf(String, "5.5");                                                        // Store in buffer for printing
  LCD_drawString(0.007 * LCD_WIDTH, pH_pixel_lower - 3, String, 0x6e43, 0xffff); // Label for 5.5pH on y-axis

  uint16_t pH_pixel_upper = pH_map(6.0); // Find y-coordinate of pixel corresponding to 6.0 pH

  sprintf(String, "6.0");                                                        // Store in buffer for printing
  LCD_drawString(0.007 * LCD_WIDTH, pH_pixel_upper - 3, String, 0x6e43, 0xffff); // Label for 6.0 pH on y-axis

  LCD_drawBlock(21, pH_pixel_upper, LCD_WIDTH, pH_pixel_lower, 0x6e43); // Drawing a coloured band of allowable pH values
}

/* ---------- Function to set y limit axis limits on graphical display ---------- */
void setpH_axis_bounds(uint8_t pH_pixel, char *String)
{
  pH_pixel = pH_map(3.0);                                         // Find y-coordinate of pixel corresponding to 3.0 pH
  LCD_drawLine(16, pH_pixel, 0.15 * LCD_WIDTH, pH_pixel, 0x0000); // Display marking for 3pH on y axis

  sprintf(String, "3.0");                                                  // Store in buffer for printing
  LCD_drawString(0.007 * LCD_WIDTH, pH_pixel - 4, String, 0x0000, 0xffff); // Label for 3.0 pH on y-axis

  pH_pixel = pH_map(8.5);                                         // Find y-coordinate of pixel corresponding to 3.0 pH
  LCD_drawLine(16, pH_pixel, 0.15 * LCD_WIDTH, pH_pixel, 0x0000); // Display marking for 8.5pH on y axis

  sprintf(String, "8.5");                                                  // Store in buffer for printing
  LCD_drawString(0.007 * LCD_WIDTH, pH_pixel - 2, String, 0x0000, 0xffff); // Label for 8.5 pH on y-axis
}

/* ---------- Function to control pump based on pH values of solution---------- */
void activate_pump(float pH_value)
{
  if (pH_value < 5.5)
  {
    PORTC |= (1 << PC2); // Turn on pump if pH less than 5.5
  }
  else
  {
    PORTC &= ~(1 << PC2); // Turn off pump otherwise
  }
}

uint8_t get_dutyCycle_lux(uint64_t lux_value)
{
  uint8_t duty_cycle = (30000 - 4 * lux_value) / 2000; // Calculating duty_cycle as per the linear relationship with lux value
  if (duty_cycle < 0)
  {
    duty_cycle = 0;
  }
  else if (duty_cycle > 100)
  {
    duty_cycle = 0;
  }
  return duty_cycle;
}