#ifndef AHS_UART_H
#define AHS_UART_H

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(int baudrate);
void uart_send(unsigned char data);
void uart_put_string(char* StringPtr);

#endif