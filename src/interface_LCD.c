#include "interface_LCD.h"
#include "timer.h"
#include "asciiLib.h"
#include "GPIO_LPC17xx.h"
#include "touchscreen_LCD.h"
#include "fm24clxx.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define USER_PASS_ADDR (0x0000)
#define SERVICE_PASS_ADDR (0x0004)

#ifdef __DEBUG__
#include "UART.h"
#endif

const static uint16_t xFirstRow = LCD_MAX_X / 3;
const static uint16_t xSecondRow = 2 * LCD_MAX_X / 3;
const static uint16_t yFirstRow = LCD_MAX_Y / 3;
const static uint16_t ySecondRow = LCD_MAX_Y / 2;
const static uint16_t yThirdRow = 2 * LCD_MAX_Y / 3;
const static uint16_t yForthRow = 5 * LCD_MAX_Y / 6;

extern float A;
extern float B;
extern float C;
extern float D;
extern float E;
extern float F;

int passpos;

static bool isService;

static void touchpanelDelayUS ( uint32_t cnt )
{
	volatile uint32_t i;
	for ( i = 0; i < cnt; i++ )
	{
		volatile uint8_t us = 12; /*  */
		while ( us-- )
			;
	}
}

void lcdWriteChar ( uint16_t x_start, uint16_t y_start, unsigned char letter[16] )
{
    lcdWriteReg ( ADRX_RAM, x_start );
    for ( uint16_t register y = 0; y < 16; y++ )
    {
        lcdWriteReg ( ADRY_RAM, y + y_start );
        lcdWriteIndex ( DATA_RAM );
        for ( uint16_t register x = 0; x < 8; x++ )
        {
            ( letter[y] & ( 1 << ( 8 - x ) ) ) ? lcdWriteData ( LCDBlack ) : lcdWriteData ( LCDBlueSea );
        }
    }
}

void lcdWriteString ( const char* text, uint16_t x_start, uint16_t y_start )
{
    unsigned char letter[16] = { '\0' };
    for ( unsigned int i = 0; i < strlen ( text ); i++ )
    {
        GetASCIICode ( 0, letter, text[i] );
        lcdWriteChar ( x_start, y_start, letter );
        x_start = x_start + 8;
        if ( x_start >= LCD_MAX_X )
        {
            x_start = 0;
            y_start += 16;
        }
    }
}

void lcdDrawLine ( uint16_t x0, uint16_t y0,
                   uint16_t x1, uint16_t y1, uint16_t color )
{
    int dx, sx, dy, sy, err, e2;
    if ( x1 >= LCD_MAX_X || x0 >= LCD_MAX_X ||
         y1 >= LCD_MAX_Y || y0 >= LCD_MAX_Y )
    {
        return;
    }
    dx = abs ( x1 - x0 );
    sx = x0 < x1 ? 1 : -1;
    dy = -abs ( y1 - y0 );
    sy = y0 < y1 ? 1 : -1;
    err = dx + dy; /* error value e_xy */

    for ( ;;)
    { /* loop */
        lcdWriteReg ( ADRX_RAM, x0 );
        lcdWriteReg ( ADRY_RAM, y0 );
        lcdWriteReg ( DATA_RAM, color );
        if ( x0 == x1 && y0 == y1 )
            break;
        e2 = 2 * err;
        if ( e2 >= dy )
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if ( e2 <= dx )
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

void lcdClearScreen ( void )
{
    for ( int register x = 0; x < LCD_MAX_X * LCD_MAX_Y; x++ )
    {
        lcdWriteData ( LCDBlueSea );
    }
}
void lcdDrawCross ( uint16_t x, uint16_t y, uint16_t len, uint16_t color )
{
    lcdDrawLine ( x - len, y, x + len, y, color );
    lcdDrawLine ( x, y - len, x, y + len, color );
}

/*rysuje krzyzyki, ktorych punkty przeciecia sa
w punktach oddalonych o 20 pikseli od rogow*/
void lcdDrawConfigTouchpanel ( void )
{
    // lewy gorny rog
    lcdDrawCross ( 20, 20, 10, LCDBlack );

    // prawy gorny rog
    lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDBlack );

    // srodek
    lcdDrawCross ( 20, LCD_MAX_Y - 20, 10, LCDBlack );
}

/*rysuje interface*/
void lcdDrawInterface ( void )
{
    unsigned char letter[16];

    uint16_t y_numbers = LCD_MAX_Y / 12;

    lcdDrawLine ( 0, yFirstRow, LCD_MAX_X - 1, yFirstRow, LCDBlack );   // gorna pozioma
    lcdDrawLine ( 0, ySecondRow, LCD_MAX_X - 1, ySecondRow, LCDBlack ); // druga pozioma
    lcdDrawLine ( 0, yThirdRow, LCD_MAX_X - 1, yThirdRow, LCDBlack );   // trzecia pozioma
    lcdDrawLine ( 0, yForthRow, LCD_MAX_X - 1, yForthRow, LCDBlack );   // ostatnia
    lcdDrawLine ( xFirstRow, yFirstRow, xFirstRow, LCD_MAX_Y - 1, LCDBlack );
    lcdDrawLine ( xSecondRow, yFirstRow, xSecondRow, LCD_MAX_Y - 1, LCDBlack );

    lcdWriteString ( "____", xFirstRow, yFirstRow / 2 - 4 );

    GetASCIICode ( 0, letter, '1' );
    lcdWriteChar ( LCD_MAX_X / 2 - xFirstRow - 4, yFirstRow + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '2' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, yFirstRow + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '3' );
    lcdWriteChar ( LCD_MAX_X / 2 + xFirstRow - 4, yFirstRow + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '4' );
    lcdWriteChar ( LCD_MAX_X / 2 - xFirstRow - 4, yFirstRow * 2 - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '5' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, yFirstRow * 2 - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '6' );
    lcdWriteChar ( LCD_MAX_X / 2 + xFirstRow - 4, yFirstRow * 2 - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '7' );
    lcdWriteChar ( LCD_MAX_X / 2 - xFirstRow - 4, yFirstRow * 2 + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '8' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, yFirstRow * 2 + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '9' );
    lcdWriteChar ( LCD_MAX_X / 2 + xFirstRow - 4, yFirstRow * 2 + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '*' );
    lcdWriteChar ( LCD_MAX_X / 2 - xFirstRow - 4, LCD_MAX_Y - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '0' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, LCD_MAX_Y - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '#' );
    lcdWriteChar ( LCD_MAX_X / 2 + xFirstRow - 4, LCD_MAX_Y - y_numbers - 8, letter );
    lcdDisplayInfo ( false );
}

void lcdClearInput ( void )
{
    unsigned char buffer[16] = { '\0' };
    GetASCIICode ( 0, buffer, '_' );
    for ( int i = 0; i < 4; i++ )
        lcdWriteChar ( xFirstRow + ( uint16_t )i * 8, yFirstRow / 2 - 4, buffer );
}

void lcdDisplayCode ( char letter, bool isBackspace, bool isEnter )
{
    static uint8_t code[4];
    unsigned char buffer[16] = { '\0' };
    uint8_t correctCode[4];
		fm24clxx_fram_read ( USER_PASS_ADDR, correctCode, 4 );
#ifdef __DEBUG__
    char buf_uart[2];
    buf_uart[0] = letter;
    buf_uart[1] = '\0';
    send ( buf_uart );
#endif
		if(isService)
		{
			fm24clxx_fram_read ( SERVICE_PASS_ADDR, correctCode, 4 );
		}
    if ( isBackspace )
    {
        if ( passpos == 0 )
            return;
        passpos--;
        GetASCIICode ( 0, buffer, '_' );
        lcdWriteChar ( xFirstRow + ( uint16_t )passpos * 8, yFirstRow / 2 - 4, buffer );
        return;
    }

    if ( isEnter )
    {
        checkCode ( code, passpos, correctCode, 4 );
        passpos = 0;
        lcdClearInput ();
        return;
    }

    if ( passpos < 4 )
    {
        code[passpos] = letter;
        GetASCIICode ( 0, buffer, '*' );
        lcdWriteChar ( xFirstRow + ( uint16_t )passpos * 8, yFirstRow / 2 - 4, buffer );
    }

    if ( ++passpos >= 4 )
    {
        passpos = 4;
        return;
    }
}

void lcdDisplayInfo ( bool isOpen )
{
    if ( isOpen )
    {
        lcdWriteString ( "OPEN    ", xSecondRow, 8 );
        GPIO_PinWrite ( 0, 26, 1 );
				touchpanelDelayUS(20000);
        RTC_ShowOpenDate ();
    }
    else
    {
        GPIO_PinWrite ( 0, 26, 0 );
				touchpanelDelayUS(20000);
        lcdWriteString ( "CLOSED  ", xSecondRow, 8 );
    }
}

void lcdServisCode ( void )
{
    lcdWriteString ( "SERVICE", xSecondRow, 8 );
    isService = true;
    RTC_ShowAlarmDate ();
}

void lcdNewCode ( void )
{
    lcdWriteString ( "Podaj kod serwisowy     ", 0, 32 );
    while(!lcdGetServiceCode ());
		lcdWriteString ( "Podaj nowe haslo        ", 0, 32 );
		lcdNewPassword(false);
}

void lcdNewServiceCode ( void )
{
    lcdWriteString ( "Podaj stary kod serwisowy", 0, 32 );
    while(!lcdGetServiceCode ());
		lcdWriteString ( "Podaj nowy kod serwisowy ", 0, 32 );
		lcdNewPassword(true);
}
void lcdInsertPassword ( void )
{
    lcdWriteString ( "Podaj haslo              ", 0, 32 );
}

bool checkCode ( const uint8_t* code, int len, const uint8_t correctCode[], int correctLen )
{
    static int failureCount;
    bool isOpen = true;
    if ( len < 3 )
    {
        isOpen = false;
        lcdDisplayInfo ( isOpen );
    }
    for ( int i = 0; i < correctLen; i++ )
    {
        if ( code[i] != correctCode[i] )
        {
            isOpen = false;
        }
    }
    if ( !isOpen )
    {
        failureCount++;
    }
    else
    {
        failureCount = 0;
    }
    if ( failureCount >= 3 )
    {
        lcdServisCode ();
        return false;
    }
    isService = false;
    lcdDisplayInfo ( isOpen );
    return isOpen;
}

void lcdDisplayDate ( const char* date )
{
    lcdWriteString ( date, 0, 8 ); 
}

void lcdHandler ( int x, int y )
{
    float xd = ( float )x * A + ( float )y * B + C;
    float yd = ( float )x * D + ( float )y * E + F;

    if ( yd > yFirstRow )
    {
        if ( xd < xFirstRow )
        {
            if ( yd < ySecondRow )
            {
                lcdDisplayCode ( '1', false, false );
            }
            else if ( yd < yThirdRow )
            {
                lcdDisplayCode ( '4', false, false );
            }
            else if ( yd < yForthRow )
            {
                lcdDisplayCode ( '7', false, false );
            }
            else
            {
                lcdDisplayCode ( ' ', true, false );
            }
        }
        else if ( xd < xSecondRow )
        {
            if ( yd < ySecondRow )
            {
                lcdDisplayCode ( '2', false, false );
            }
            else if ( yd < yThirdRow )
            {
                lcdDisplayCode ( '5', false, false );
            }
            else if ( yd < yForthRow )
            {
                lcdDisplayCode ( '8', false, false );
            }
            else
            {
                lcdDisplayCode ( '0', false, false );
            }
        }
        else
        {
            if ( yd < ySecondRow )
            {
                lcdDisplayCode ( '3', false, false );
            }
            else if ( yd < yThirdRow )
            {
                lcdDisplayCode ( '6', false, false );
            }
            else if ( yd < yForthRow )
            {
                lcdDisplayCode ( '9', false, false );
            }
            else
            {
                lcdDisplayCode ( ' ', false, true );
            }
        }
    }
}

char lcdGetChar ( int x, int y, bool showChar, int pos )
{
    float xd = ( float )x * A + ( float )y * B + C;
    float yd = ( float )x * D + ( float )y * E + F;
    unsigned char buffer[16] = { '\0' };
    if ( yd > yFirstRow )
    {
        if ( xd < xFirstRow )
        {
            if ( yd < ySecondRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '1' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '1';
            }
            else if ( yd < yThirdRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '4' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '4';
            }
            else if ( yd < yForthRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '7' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '7';
            }
            else
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '7' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '7';
            }
        }
        else if ( xd < xSecondRow )
        {
            if ( yd < ySecondRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '2' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '2';
            }
            else if ( yd < yThirdRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '5' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '5';
            }
            else if ( yd < yForthRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '8' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '8';
            }
            else
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '0' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '0';
            }
        }
        else
        {
            if ( yd < ySecondRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '3' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '3';
            }
            else if ( yd < yThirdRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '6' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '6';
            }
            else if ( yd < yForthRow )
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '9' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '9';
            }
            else
            {
                if ( showChar ) { GetASCIICode ( 0, buffer, '9' ); }
                else { GetASCIICode ( 0, buffer, '*' ); }
                lcdWriteChar ( xFirstRow + ( uint16_t )pos * 8, yFirstRow / 2 - 4, buffer );
                return '9';
            }
        }
    }
    return '0';
}

void lcdOpenDate ( const char* date )
{
    lcdWriteString ( date, 0, yFirstRow - 16 );
}

void lcdAlarmDate ( const char* date )
{
    lcdWriteString ( date, LCD_MAX_X / 2, yFirstRow - 16 );
}
