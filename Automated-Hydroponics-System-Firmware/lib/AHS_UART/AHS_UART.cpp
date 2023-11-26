#include "AHS_UART.h"

void uart_init(int baudrate)
{
  /*******************************************************************
   * Local variables
   ******************************************************************/
  int UBRRn_val;

  uint8_t USART_mode_select;
  uint8_t USART_parity_mode;
  uint8_t USART_data_bit_setting; 
  uint8_t USART_clk_polarity; 
  uint8_t USART_stop_bit_setting; 

  uint8_t USART_TX_RX_mode; 

  /*******************************************************************
   * Configure UART/USART process
   * 
   * 1. Set desired baudrate
   * 2. Set frame format
   * 3. Enable transmit/receiver/both
   ******************************************************************/

  /*******************************************************************
   * 1. Set desired baudrate
   * 
   * NOTE: UBRR0H bits 15:12 are reserved and should not be modified
   * 
   * Use UBRR0H for 4 MSB of desired baudrate
   * Use UBRR0L for 8 LSB of desired baudrate
   * 
   * NOTE: Assume asynchronous normal mode (U2Xn = 0)
   ******************************************************************/
  UBRRn_val = (F_CPU / (baudrate * 16UL)) - 1; /* NOTE: This equation is found in ATmega328p datasheet p.146 */
  UBRR0H = (uint8_t) (UBRRn_val >> 8);
  UBRR0L = (uint8_t) (UBRRn_val);

  /*******************************************************************
   * 2. Set Frame format
   * 
   * Configure:
   * - Parity mode
   * - # of stop bits
   * - # of data bits
   * - Clock polarity
   ******************************************************************/
  /* Assuming normal for USART0 */
  USART_mode_select = 0 << UMSEL00;
  
  /* Parity mode - Disabled */
  USART_parity_mode = 0 << UPM00;

  /* # of data bits - 8 */
  USART_data_bit_setting = (1 << UCSZ01) | (1 << UCSZ00);

  /* # of stop bits - 1 */
  USART_stop_bit_setting = (0 << USBS0); 

  /* Set clock polarity */
  USART_clk_polarity = 0;

 /*******************************************************************
   * 3. Enable tx,rx, or both
   * 
   * Enable both
   ******************************************************************/
  USART_TX_RX_mode = (1 << TXEN0) | (1 << RXEN0);

  /* Save config to registers */
  UCSR0B = ( USART_TX_RX_mode);

  UCSR0C = (
    USART_mode_select |
    USART_parity_mode |
    USART_data_bit_setting |
    USART_stop_bit_setting | 
    USART_clk_polarity
  );
}

void uart_send(unsigned char data)
{
  // Wait for empty transmit buffer
  while(!(UCSR0A & (1<<UDRE0)));
  // Put data into buffer and send data
  UDR0 = data;
  
}

void uart_putstring(char* StringPtr)
{
  while(*StringPtr != 0x00)
  {
    uart_send(*StringPtr);
  StringPtr++;
  }
}
