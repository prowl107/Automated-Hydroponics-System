/*
 * LCD_GFX.h
 *
 * Created: 9/20/2021 6:54:37 PM
 *  Author: You
 */ 

#include <avr/io.h>
#include "ASCII_LUT.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AHS_UART.h"
#include "math.h"

#ifndef LCD_GFX_H_
#define LCD_GFX_H_

// colors
#define	BLACK     0x0000
#define WHITE     0xFFFF
#define	BLUE      0x001F
#define	RED       0xF800
#define	GREEN     0x07E0
#define CYAN      0x07FF
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0

uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue);
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color);
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor);
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color, uint16_t bg_color);
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t color);
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color);
void LCD_setScreen(uint16_t color);
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg);
void LCD_setColor(uint16_t color_data, int len);
void *memset16(void *str, uint16_t data, int len, int scale);
#endif /* LCD_GFX_H_ */