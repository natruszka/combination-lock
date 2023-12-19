#include "interface_LCD.h"
#include "asciiLib.h"
#include <string.h>
#include <stdlib.h>

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

void lcdWriteString ( const char* text )
{
    static uint16_t y_start = 0;
    static uint16_t x_start = 0;
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
    int dx, sx;
    int dy, sy;
    int err, e2;

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

    for ( ;; )
    {  /* loop */
        lcdWriteReg ( ADRX_RAM, x0 );
        lcdWriteReg ( ADRY_RAM, y0 );
        lcdWriteReg ( DATA_RAM, color );
        if ( x0 == x1 && y0 == y1 ) break;
        e2 = 2 * err;
        if ( e2 >= dy ) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if ( e2 <= dx ) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
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
    //lewy gorny rog
    lcdDrawCross ( 20, 20, 10, LCDBlack );

    //prawy gorny rog
    lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDBlack );

    //srodek
    lcdDrawCross ( 20, LCD_MAX_Y - 20, 10, LCDBlack );
}

/*rysuje interface*/
void lcdDrawInterface ( void )
{
    uint16_t x = LCD_MAX_X / 3;
    uint16_t y = LCD_MAX_Y / 3;
    uint16_t y_key = y / 2;
		uint16_t y_numbers = y_key / 2;

    lcdDrawLine ( 0, y, LCD_MAX_X - 1, y, LCDBlack ); //gorna pozioma 
    lcdDrawLine ( 0, y + y_key, LCD_MAX_X - 1, y + y_key, LCDBlack ); //druga pozioma
    lcdDrawLine ( 0, y + y_key * 2, LCD_MAX_X - 1, y + y_key * 2, LCDBlack ); //trzecia pozioma
		lcdDrawLine ( 0, y + y_key * 3, LCD_MAX_X - 1, y + y_key * 3, LCDBlack ); //ostatnia
    lcdDrawLine ( x, y, x, LCD_MAX_Y - 1, LCDBlack );
    lcdDrawLine ( x + x, y, x + x, LCD_MAX_Y - 1, LCDBlack );

    GetASCIICode ( 0, letter, '1' );
    lcdWriteChar ( LCD_MAX_X / 2 - x - 4, y + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '2' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, y + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '3' );
    lcdWriteChar ( LCD_MAX_X / 2 + x - 4, y + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '4' );
    lcdWriteChar ( LCD_MAX_X / 2 - x - 4, y * 2 - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '5' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, y * 2 - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '6' );
    lcdWriteChar ( LCD_MAX_X / 2 + x - 4, y * 2 - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '7' );
    lcdWriteChar ( LCD_MAX_X / 2 - x - 4, y * 2 + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '8' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, y * 2 + y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '9' );
    lcdWriteChar ( LCD_MAX_X / 2 + x - 4, y * 2 + y_numbers - 8, letter );
		
		GetASCIICode ( 0, letter, '*' );
    lcdWriteChar ( LCD_MAX_X / 2 - x - 4, LCD_MAX_Y - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '0' );
    lcdWriteChar ( LCD_MAX_X / 2 - 4, LCD_MAX_Y - y_numbers - 8, letter );

    GetASCIICode ( 0, letter, '#' );
    lcdWriteChar ( LCD_MAX_X / 2 + x - 4, LCD_MAX_Y - y_numbers - 8, letter );
}
