#include "AHS_I2C.h"

void i2c_init()
{
    /************************************************************
     * Emulate internal pullups resistors on GPIO pins 
     * 
     * PC5 = SCL
     * PC4 = SDA
     * 
     * NOTE: Alternative is to place a pull up resistor (10k) on 
     * SCL and SDA lines
     ***********************************************************/
    DDRC &= ~((1 << PC4) | (1 << PC5));
    PORTC |= (1 << PC4) | (1 << PC5);

    /************************************************************
     * Set SCL frequency
     * 
     * SCL_freq = CPU_CLK/(16+2(TWBR)*Prescaler_value)
     * 
     * NOTE: Currently set to 400khz. AHT20 and BH1750 do not 
     * have specifed scl frequency and should allow a value 
     * within the i2c spec
     * 
     * NOTE: Refer to 21.5.2 Bit Rate generator Unit in the
     * Atmega328p datasheet for more information on SCL frequency
     * calculation
     ***********************************************************/
    TWSR &= ~((1 << TWPS1) | (1 << TWPS1));
    TWBR |= (1<<TWBR3) | (1<<TWBR2);

    /************************************************************
     * Enable ACK pulse
     *
     * NOTE: According to the datasheet, an ACK is generated
     * if the following conditions are met:
     * 
     * 1. The device's own slave address has been received
     * 2. A genral call has been received, while the TWGCE bit 
     *    in the TWAR is set
     * 3. A data byte has been received in master receiver or 
     *   slave receiver mode
     ***********************************************************/
    TWCR |= (1 << TWEA);
}

void i2c_start()
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Send START condition
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag set. This indicates that the START condition has been transmitted
}

void i2c_stop()
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // Transmit STOP condition
}

void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read()
{
    uint8_t data;
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    data = TWDR;
    return data;
}