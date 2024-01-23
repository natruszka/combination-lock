#ifndef LCD_TOUCHSCREEN_H
#define LCD_TOUCHSCREEN_H

#include "asciiLib.h"
#include "GPIO_LPC17xx.h"
#include "TP_Open1768.h"
#include "interface_LCD.h"

#include <stdbool.h>

#ifdef __DEBUG__
#include "UART.h"
#endif

extern float A;
extern float B;
extern float C;
extern float D;
extern float E;
extern float F;

void lcdTouchscreenGetCoords ( int* x, int* y );
char lcdTouchscreenGetPassword ( int* x, int* y, bool showChar, int pos );
void lcdTouchscreenCallibrate ( void );
bool lcdGetServiceCode ( void );
void lcdNewPassword ( bool isService );
int getMin ( int cords[], int len );
int getMax ( int cords[], int len );
int average ( int cords[], int len );
void calculateConstants ( int x[], int y[], int len );

#endif // LCD_TOUCHSCREEN_H
