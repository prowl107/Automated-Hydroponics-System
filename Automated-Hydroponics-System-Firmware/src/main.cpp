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
  /* Insert code here :) */
  uart_init(9600);
  char buffer[32];
  sprintf(buffer, "Hello World");
  uart_put_string(buffer);

  return 0;
}