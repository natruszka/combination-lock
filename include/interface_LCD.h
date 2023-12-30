#ifndef INTERFACE_LCD_H
#define INTERFACE_LCD_H

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"

#include "UART.h"

void lcdWriteChar ( uint16_t x_start, uint16_t y_start, unsigned char letter[16] );
void lcdWriteString(const char *text, uint16_t x_start, uint16_t y_start);
void lcdDrawLine ( uint16_t x0, uint16_t y0,
                   uint16_t x1, uint16_t y1, uint16_t color );
void lcdClearScreen( void );
void lcdClearScreen( void );
void lcdDrawCross ( uint16_t x, uint16_t y, uint16_t len, uint16_t color );
void lcdDrawConfigTouchpanel ( void );
void lcdDrawInterface ( void );
void lcdDisplayCode(char letter, bool isBackspace, bool isEnter);
void lcdDisplayInfo(bool isOpen);
void checkCode(const char *code);
void lcdDisplayDate(void);
void lcdHandler(int x, int y);
void lcdTest(void);
#endif // INTERFACE_LCD_H
