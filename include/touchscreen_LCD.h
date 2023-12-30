#ifndef LCD_TOUCHSCREEN_H
#define LCD_TOUCHSCREEN_H

#include "asciiLib.h"
#include "GPIO_LPC17xx.h"
#include "TP_Open1768.h"
#include "interface_LCD.h"

#include "UART.h"

void lcdTouchscreenGetCoords ( int* x, int* y );
void lcdTouchscreenCallibrate ( void );
int getMin(int cords[], int len);
int getMax(int cords[], int len);
int average(int cords[], int len);
void calculateConstants(int x[], int y[], int len);
#endif // LCD_TOUCHSCREEN_H
