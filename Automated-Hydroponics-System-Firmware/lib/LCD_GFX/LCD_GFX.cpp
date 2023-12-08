/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */

#include "LCD_GFX.h"
#include "ST7735.h"
#include "avr/delay.h"

/******************************************************************************
 * Local Functions
 ******************************************************************************/

/******************************************************************************
 * Global Functions
 ******************************************************************************/

/******************************************************************************
 * @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
 * @brief		Convert RGB888 value to RGB565 16-bit color data
 * @note
 ******************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31 * (red + 4)) / 255) << 11) | (((63 * (green + 2)) / 255) << 5) | ((31 * (blue + 4)) / 255));
}

/*****************************************************************************
 * @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
 * @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
 * @note
 *****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
{
	LCD_setAddr(x, y, x, y);
	SPI_ControllerTx_16bit(color);
}

/****************************************************************************
 * @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
 * @brief		Draw a character starting at the point with foreground and background colors
 * @note
 *****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
{
	/********************************************
	 * Local Variables
	 *******************************************/
	uint16_t width_displacement = 5;
	uint16_t height_displacement = 8;
	uint16_t pixel_color_buf[height_displacement][width_displacement];
	uint16_t row = character - 0x20; // Determine row of ASCII table starting at space
	int i, j;
	char buffer[50];

	/********************************************
	 * Create pixel data
	 *******************************************/
	if ((LCD_WIDTH - x > 7) && (LCD_HEIGHT - y > 7))
	{
		for (i = 0; i < 5; i++)
		{
			uint8_t pixels = ASCII[row][i]; // Go through the list of pixels
			for (j = 0; j < 8; j++)
			{
				if ((pixels >> j) & 1 == 1)
				{
					pixel_color_buf[j][i] = fColor;
				}
				else
				{
					pixel_color_buf[j][i] = bColor;
				}
			}
			// sprintf(buffer, "Pixels: %d | i: %d\n", pixels, i);
			// uart_putstring(buffer);
		}
	}

	/********************************************
	 * Write pixel data to screen
	 *******************************************/
	LCD_setAddr(x, y, x + width_displacement - 1, y + height_displacement - 1);
	for (int i = 0; i < height_displacement; i++)
	{
		for (int j = 0; j < width_displacement; j++)
		{
			uint8_t temp = pixel_color_buf[i][j] >> 8;
			clear(LCD_PORT, LCD_TFT_CS); // CS pulled low to start communication

			SPDR = temp; // Place data to be sent on rgisters
			while (!(SPSR & (1 << SPIF)))
				; // wait for end of transmission

			temp = (0xFF) & pixel_color_buf[i][j];
			SPDR = temp; // Place data to be sent on registers
			while (!(SPSR & (1 << SPIF)))
				; // wait for end of transmission

			set(LCD_PORT, LCD_TFT_CS); // set CS to high
		}
	}
}

/****************************************************************************
 * @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
 * @brief		Draw a colored circle of set radius at coordinates
 * @note
 *****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color, uint16_t bg_color)
{
	uint16_t theta = 0;
	uint8_t center_x = x0;
	uint8_t center_y = y0;
	uint8_t x; /* Point along circle's edge (x-coordinate) */
	uint8_t y; /* Point along circle's edge (y-coordinate) */
	char buffer[50];
	int diameter = radius * 2;

	/* Note: Based implementation on bounding circle approach described here: https://www.redblobgames.com/grids/circle-drawing/#bounding-circle */
	uint8_t top = center_y - radius;
	uint8_t bottom = center_y + radius;
	int pixel_idx = 0;

	uint16_t color_buffer[diameter * diameter];
	sprintf(buffer, " ");
	uart_putstring(buffer);

	for (int i = center_y - radius; i < center_y + radius; i++)
	{

		int dy = i - center_y;
		int dx = (int)round(sqrt(radius * radius - dy * dy));
		int left = ceil(center_x - dx);
		int right = floor(center_x + dx);
		// sprintf(buffer, "LEFT: %d RIGHT: %d dY: %d  dx: %d Center_x : %d\n", left, right, dy, dx, center_x);
		// uart_putstring(buffer);

		for (int j = center_x - radius; j <= center_x + radius; j++)
		{
			if ((j <= left && j >= center_x - radius) || (j >= right && j <= center_x + radius))
			{
				color_buffer[pixel_idx] = bg_color;
				// sprintf(buffer, "background color\n");
				// uart_putstring(buffer);
			}
			else
			{
				color_buffer[pixel_idx] = color;
				// sprintf(buffer, "white\n");
				// uart_putstring(buffer);
			}
			pixel_idx += 1;
		}
	}

	LCD_setAddr(
		center_x - radius,
		top,
		center_x + radius,
		bottom);

	for (int i = 0; i < (diameter * diameter); i++)
	{

		uint8_t temp = (uint8_t)(color_buffer[i] >> 8);
		clear(LCD_PORT, LCD_TFT_CS); // CS pulled low to start communication

		SPDR = temp; // Place data to be sent on registers
		while (!(SPSR & (1 << SPIF)))
			; // wait for end of transmission

		temp = (0xFF) & color_buffer[i];
		SPDR = temp; // Place data to be sent on registers
		while (!(SPSR & (1 << SPIF)))
			; // wait for end of transmission

		// color_data+=1;
		set(LCD_PORT, LCD_TFT_CS); // set CS to high
	}
}

/******************************************************************************
 * @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
 * @brief		Draw a line from and to a point with a color
 * @note
 *****************************************************************************/
void LCD_drawLine(short x0, short y0, short x1, short y1, uint16_t color)
{
	/*********************************************************************
	 * Handle if line is vertical or horizontal
	 ********************************************************************/
	/* Vertical line */
	if (x0 == x1 && y0 != y1)
	{
		LCD_setAddr(x0, y0, x1, y1);

		/* Determine area of block */
		uint16_t pixel_count = abs(y1 - y0);

		/* Create pixel color buffer */
		// uint16_t *pixel_color_buffer = (uint16_t *)malloc(pixel_count * 16);
		// memset(pixel_color_buffer, color, pixel_count * 16);

		/* Assign color to region */
		for (int i = 0; i < pixel_count; i++)
		{
			LCD_setColor(color, pixel_count);
		}

		return;
	}

	/* Horizontal line */
	if (x0 != x1 && y0 == y1)
	{
		// uart_putstring("Entered draw horizontal line\n");
		LCD_setAddr(x0, y0, x1, y1);

		/* Determine area of block */
		uint16_t pixel_count = abs(x1 - x0);

		/* Create pixel color buffer */
		// uint16_t *pixel_color_buffer = (uint16_t *)malloc(pixel_count * 16);
		// memset(pixel_color_buffer, color, pixel_count * 16);

		/* Assign color to region */
		for (int i = 0; i < pixel_count; i++)
		{
			LCD_setColor(color, pixel_count);
			// SPI_ControllerTx_16bit(color);
		}

		return;
	}

	/*********************************************************************
	 * Diagonal Line
	 *
	 * NOTE: Calculating line based on Bresenham's algorithm
	 ********************************************************************/
	uint16_t A = 2 * (y1 - y0);
	uint16_t B = A - 2 * (x1 - x0);
	uint16_t P = A - (x1 - x0);
	short current_pixel_x = x0;
	short current_pixel_y = y0;

	/* 1. Fill the starting pixel (x0, y0) */
	LCD_drawPixel(x0, y0, color);

	/* 2, Execute for every point/position along the x axis*/
	while (current_pixel_x < x1)
	{
		/******************************************************************
		 * If P is less than 0, draw new pixel on the same line as previous
		 * pixel otherwise draw next pixel one row higher than the previous
		 *****************************************************************/
		LCD_drawPixel(++current_pixel_x, current_pixel_y, color);
		P += A;

		if (P >= 0)
		{
			LCD_drawPixel(current_pixel_x, ++current_pixel_y, color);

			/* Add B to P */
			P -= B;
		}

		A = 2 * (y1 - current_pixel_y);
		B = A - 2 * (x1 - current_pixel_x);
		P = A - (x1 - current_pixel_x);
	}
}

/*****************************************************************************
 * @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
 * @brief		Draw a colored block at coordinates
 * @note
 *****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{
	uint16_t pixel_count;
	/* Determine area of block */
	if (x1 == x0)
	{
		pixel_count = abs(y1 - y0);
	}
	else if (y1 == y0)
	{
		pixel_count = abs(x1 - x0);
	}
	else
	{
		pixel_count = (x1 - x0) * (y1 - y0);
	}

	// uint16_t pixel_count = (x1 - x0) * (y1 - y0);

	/* Create pixel color buffer */
	// uint16_t *pixel_color_buffer = (uint16_t *)malloc(pixel_count * 16);
	// memset(pixel_color_buffer, color, pixel_count * 16);

	/* Set address - display window for block */
	LCD_setAddr(x0, y0, x1, y1);

	/* Assign color to region */
	LCD_setColor(color, pixel_count);
	// SPI_ControllerTx_16bit(color);

	// for (int i = 0; i < pixel_count; i++)
	// {
	// 	LCD_setColor(color);
	// }
}

/*****************************************************************************
 * @fn			void LCD_setScreen(uint16_t color)
 * @brief		Draw the entire screen to a color
 * @note
 *****************************************************************************/
void LCD_setScreen(uint16_t color)
{
	uint16_t pixel_count = LCD_WIDTH * LCD_HEIGHT;
	LCD_setAddr(0, 0, LCD_WIDTH, LCD_HEIGHT);

	/* Create pixel color buffer */
	uint16_t *pixel_color_buffer = (uint16_t *)malloc(pixel_count * 16);
	memset(pixel_color_buffer, color, pixel_count * 16);

	LCD_setColor(color, pixel_count);
}

/*****************************************************************************
 * @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
 * @brief		Draw a string starting at the point with foreground and background colors
 * @note
 *****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char *str, uint16_t fg, uint16_t bg)
{
	uint16_t col_offset = 5;
	uint16_t row_offset = 0;
	uint16_t char_count = 0;
	while (*str != 0)
	{
		LCD_drawChar(x + (char_count * col_offset), y + row_offset, *str, fg, bg);
		str++;
		char_count += 1;
		if (char_count >= 31)
		{
			row_offset += 9;
			char_count = 0;
		}
	}
}

/******************************************************************************
 * @fn			void LCD_setColor(uint16_t color)
 * @brief		Sets the color of the currently selected display window
 * @note			Expects 16bit color values
 *****************************************************************************/
void LCD_setColor(uint16_t color_data, int len)
{
	// SPI_ControllerTx_16bit(color);
	for (int i = 0; i < len; i++)
	{
		uint8_t temp = color_data >> 8;
		clear(LCD_PORT, LCD_TFT_CS); // CS pulled low to start communication

		SPDR = temp; // Place data to be sent on registers
		while (!(SPSR & (1 << SPIF)))
			; // wait for end of transmission

		temp = (0xFF) & color_data;
		SPDR = temp; // Place data to be sent on registers
		while (!(SPSR & (1 << SPIF)))
			; // wait for end of transmission

		// color_data+=1;
		set(LCD_PORT, LCD_TFT_CS); // set CS to high
	}
}

/******************************************************************************
 * @fn			void memset16(uint16_t color)
 * @brief		16 bit variant of memset which will set 2 bytes of memory
 *				at the desired location for the desired length
 * @note
 *****************************************************************************/
void *memset16(void *str, int data, size_t len)
{
	uint16_t *dest_ptr = (uint16_t *)str;
	while (len-- > 0)
	{
		*dest_ptr++ = data;
	}

	return dest_ptr;
}