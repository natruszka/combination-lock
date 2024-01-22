#ifndef INTERFACE_LCD_H
#define INTERFACE_LCD_H

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"

#include <stdbool.h>

#ifdef __DEBUG__
#include "UART.h"
#endif

void lcdWriteChar ( uint16_t x_start, uint16_t y_start, unsigned char letter[16] );
void lcdWriteString ( const char* text, uint16_t x_start, uint16_t y_start );
void lcdDrawLine ( uint16_t x0, uint16_t y0,
                   uint16_t x1, uint16_t y1, uint16_t color );
void lcdClearScreen ( void );
void lcdDrawCross ( uint16_t x, uint16_t y, uint16_t len, uint16_t color );
void lcdDrawConfigTouchpanel ( void );
void lcdDrawInterface ( void );
void lcdDisplayCode ( char letter, bool isBackspace, bool isEnter );
void lcdDisplayInfo ( bool isOpen );
void lcdServisCode ( void );
void lcdNewCode ( void );
void lcdNewServiceCode ( void );
void lcdInsertPassword ( void );
bool checkCode ( const char* code, int len, const char correctCode[], int correctLen );
void lcdClearInput ( void );
void lcdDisplayDate ( const char* date );
void lcdHandler ( int x, int y );
char lcdGetChar ( int x, int y, bool showChar, int pos );
void lcdOpenDate ( const char* date );
void lcdAlarmDate ( const char* date );

#endif // INTERFACE_LCD_H
