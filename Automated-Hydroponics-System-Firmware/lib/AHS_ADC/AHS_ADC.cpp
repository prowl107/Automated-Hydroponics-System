#include "AHS_ADC.h"

/****************************************************************
 * NOTE: The current implementation is only designed to enable
 * the ADC on pin A0
 * 
 * TODO: If necessary enable/disable the ADC for the other 
 * analog pins
 ****************************************************************/

void adc_init()
{
	/************************************************************************
	 * 1. Disable power reduction ADC bit (PRADC)
	 *
	 * NOTE: By clearing the bit, the ADC can be enabled
	 *************************************************************************/
	PRR &= ~(1 << PRADC);

	/************************************************************************
	 * 2. Set voltage reference selection
	 *
	 * NOTE: Current selection is mode 1 --> AVcc with external capacitor at
	 * AREF pin
	 *************************************************************************/
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);

	/************************************************************************
	 * 3. Set ADC prescaler
	 *
	 * NOTE: According to datasheet, a successive approximation circuitry
	 * requires an input clock frequency between 50kHz and 200kHz to get
	 * maximum resolution.
	 *
	 * NOTE: This is achievable by setting the prescaler to 128
	 * 16Mhz clock / 128 --> 125Khz
	 *************************************************************************/
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	/************************************************************************
	 * 4. Set ADC input channel/pins
	 *
	 * NOTE: For PA0, select channel 0 - MUX3:0 = 0000
	 *************************************************************************/
	ADMUX &= ~(0xF);

	/************************************************************************
	 * 4b. Disable digital input for channel/pins
	 *************************************************************************/
	DIDR0 |= (1 << ADC0D);

	/************************************************************************
	 * 5. Set ADC auto enable trigger
	 *************************************************************************/
	ADCSRA |= (1 << ADATE);

	/************************************************************************
	 * 6. Set ADC auto trigger source selection
	 *
	 * NOTE: Current source to set to freerunning mode ADTS2:0 = 000
	 *************************************************************************/
	ADCSRB &= ~(0X7);

	/************************************************************************
	 * 7. Enable ADC
	 *************************************************************************/
	ADCSRA |= (1 << ADEN);

	/************************************************************************
	 * 8. Start coversation
	 *************************************************************************/
	ADCSRA |= (1 << ADSC);
}


uint16_t adc_read_value(uint8_t pin)
{
	/*********************************************************************
	 * Local Variables
	*********************************************************************/
	uint8_t adcl;
	uint8_t adch;
	uint16_t adc_full_value;

	/* Change ADMUX channel if new pin is rquested  */
	if (ADMUX & 0xF != pin)
	{
		ADMUX &= ~(0xF); /* Clear ADMUX reg MUX [3..0] */
		ADMUX |= pin;
	}

	adcl = ADCL;
	adch = ADCH & 0x3;
	adc_full_value = (adch << 8) | adcl;
	return adc_full_value;
	

}
