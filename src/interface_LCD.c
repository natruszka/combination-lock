#include "interface_LCD.h"
#include "asciiLib.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

const static uint16_t xFirstRow = LCD_MAX_X / 3;
const static uint16_t xSecondRow = 2 * LCD_MAX_X / 3;
const static uint16_t yFirstRow = LCD_MAX_Y / 3;
const static uint16_t ySecondRow = LCD_MAX_Y / 2;
const static uint16_t yThirdRow = 2 * LCD_MAX_Y / 3;
const static uint16_t yForthRow = 5 * LCD_MAX_Y / 6;
extern int A;
extern int B;
extern int C;
extern int D;

void lcdWriteChar(uint16_t x_start, uint16_t y_start, unsigned char letter[16])
{
    lcdWriteReg(ADRX_RAM, x_start);
    for (uint16_t register y = 0; y < 16; y++)
    {
        lcdWriteReg(ADRY_RAM, y + y_start);
        lcdWriteIndex(DATA_RAM);
        for (uint16_t register x = 0; x < 8; x++)
        {
            (letter[y] & (1 << (8 - x))) ? lcdWriteData(LCDBlack) : lcdWriteData(LCDBlueSea);
        }
    }
}

void lcdWriteString(const char *text, uint16_t x_start, uint16_t y_start)
{
    unsigned char letter[16] = {'\0'};
    for (unsigned int i = 0; i < strlen(text); i++)
    {
        GetASCIICode(0, letter, text[i]);
        lcdWriteChar(x_start, y_start, letter);
        x_start = x_start + 8;
        if (x_start >= LCD_MAX_X)
        {
            x_start = 0;
            y_start += 16;
        }
    }
}

void lcdDrawLine(uint16_t x0, uint16_t y0,
                 uint16_t x1, uint16_t y1, uint16_t color)
{
    if (x1 >= LCD_MAX_X || x0 >= LCD_MAX_X ||
        y1 >= LCD_MAX_Y || y0 >= LCD_MAX_Y)
    {
        return;
    }
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        lcdWriteReg(ADRX_RAM, x0);
        lcdWriteReg(ADRY_RAM, y0);
        lcdWriteReg(DATA_RAM, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

void lcdClearScreen( void )
{
    for (int register x = 0; x < LCD_MAX_X * LCD_MAX_Y; x++)
    {
        lcdWriteData(LCDBlueSea);
    }
}
void lcdDrawCross(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    lcdDrawLine(x - len, y, x + len, y, color);
    lcdDrawLine(x, y - len, x, y + len, color);
}

/*rysuje krzyzyki, ktorych punkty przeciecia sa
w punktach oddalonych o 20 pikseli od rogow*/
void lcdDrawConfigTouchpanel(void)
{
    // lewy gorny rog
    lcdDrawCross(20, 20, 10, LCDBlack);

    // prawy gorny rog
    lcdDrawCross(LCD_MAX_X - 20, 20, 10, LCDBlack);

    // srodek
    lcdDrawCross(20, LCD_MAX_Y - 20, 10, LCDBlack);
}

/*rysuje interface*/
void lcdDrawInterface(void)
{
    uint16_t y_numbers = LCD_MAX_Y / 12;

    lcdDrawLine(0, yFirstRow, LCD_MAX_X - 1, yFirstRow, LCDBlack);   // gorna pozioma
    lcdDrawLine(0, ySecondRow, LCD_MAX_X - 1, ySecondRow, LCDBlack); // druga pozioma
    lcdDrawLine(0, yThirdRow, LCD_MAX_X - 1, yThirdRow, LCDBlack);   // trzecia pozioma
    lcdDrawLine(0, yForthRow, LCD_MAX_X - 1, yForthRow, LCDBlack);   // ostatnia
    lcdDrawLine(xFirstRow, yFirstRow, xFirstRow, LCD_MAX_Y - 1, LCDBlack);
    lcdDrawLine(xSecondRow, yFirstRow, xSecondRow, LCD_MAX_Y - 1, LCDBlack);

    unsigned char letter[16];

    GetASCIICode(0, letter, '1');
    lcdWriteChar(LCD_MAX_X / 2 - xFirstRow - 4, yFirstRow + y_numbers - 8, letter);

    GetASCIICode(0, letter, '2');
    lcdWriteChar(LCD_MAX_X / 2 - 4, yFirstRow + y_numbers - 8, letter);

    GetASCIICode(0, letter, '3');
    lcdWriteChar(LCD_MAX_X / 2 + xFirstRow - 4, yFirstRow + y_numbers - 8, letter);

    GetASCIICode(0, letter, '4');
    lcdWriteChar(LCD_MAX_X / 2 - xFirstRow - 4, yFirstRow * 2 - y_numbers - 8, letter);

    GetASCIICode(0, letter, '5');
    lcdWriteChar(LCD_MAX_X / 2 - 4, yFirstRow * 2 - y_numbers - 8, letter);

    GetASCIICode(0, letter, '6');
    lcdWriteChar(LCD_MAX_X / 2 + xFirstRow - 4, yFirstRow * 2 - y_numbers - 8, letter);

    GetASCIICode(0, letter, '7');
    lcdWriteChar(LCD_MAX_X / 2 - xFirstRow - 4, yFirstRow * 2 + y_numbers - 8, letter);

    GetASCIICode(0, letter, '8');
    lcdWriteChar(LCD_MAX_X / 2 - 4, yFirstRow * 2 + y_numbers - 8, letter);

    GetASCIICode(0, letter, '9');
    lcdWriteChar(LCD_MAX_X / 2 + xFirstRow - 4, yFirstRow * 2 + y_numbers - 8, letter);

    GetASCIICode(0, letter, '*');
    lcdWriteChar(LCD_MAX_X / 2 - xFirstRow - 4, LCD_MAX_Y - y_numbers - 8, letter);

    GetASCIICode(0, letter, '0');
    lcdWriteChar(LCD_MAX_X / 2 - 4, LCD_MAX_Y - y_numbers - 8, letter);

    GetASCIICode(0, letter, '#');
    lcdWriteChar(LCD_MAX_X / 2 + xFirstRow - 4, LCD_MAX_Y - y_numbers - 8, letter);
    lcdDisplayDate();
    lcdDisplayInfo(false);
}

void lcdDisplayCode(char letter, bool isBackspace, bool isEnter)
{
    static int pos;
		unsigned char buffer[16] = {'\0'};
		send(&letter);
    if (isBackspace)
    {
        if (pos == 0)
            return;
        pos--;
        GetASCIICode(0, buffer, ' ');
        lcdWriteChar(xFirstRow + pos * 8, yFirstRow / 2 - 4, buffer);
        return;
    }
    GetASCIICode(0, buffer, '*');
    lcdWriteChar(xFirstRow + pos * 8, yFirstRow / 2 - 4, buffer);
    if (++pos == 4)
    {
        checkCode(" ");
        pos = 0;
    }
}
void lcdDisplayInfo(bool isOpen)
{
    if (isOpen)
        lcdWriteString("OPEN  ", xFirstRow, 8);
    else
        lcdWriteString("CLOSED", xFirstRow, 8);
}

void checkCode(const char *code)
{
    static int failureCount;
    //if ()
}

void lcdDisplayDate(void)
{
    lcdWriteString("date", 0, 8); // test pozycji
}

void lcdHandler(int x, int y)
{
    x = x * A + y * B;
    y = x * C + y * D;
    if (y > yFirstRow)
    {
        if (x < xFirstRow)
        {
            if (y < ySecondRow)
            {
                lcdDisplayCode('1', false, false);
            }
            else if (y < yThirdRow)
            {
                lcdDisplayCode('4', false, false);
            }
            else if (y < yForthRow)
            {
                lcdDisplayCode('7', false, false);
            }
            else
            {
                lcdDisplayCode(' ', true, false);
            }
        }
        else if (x < xSecondRow)
        {
            if (y < ySecondRow)
            {
                lcdDisplayCode('2', false, false);
            }
            else if (y < yThirdRow)
            {
                lcdDisplayCode('5', false, false);
            }
            else if (y < yForthRow)
            {
                lcdDisplayCode('8', false, false);
            }
            else
            {
                lcdDisplayCode('0', false, false);
            }
        }
        else
        {
            if (y < ySecondRow)
            {
                lcdDisplayCode('3', false, false);
            }
            else if (y < yThirdRow)
            {
                lcdDisplayCode('6', false, false);
            }
            else if (y < yForthRow)
            {
                lcdDisplayCode('9', false, false);
            }
            else
            {
                lcdDisplayCode(' ', false, true);
            }
        }
    }
}
void lcdTest(void)
{
    lcdWriteString("date", 0, yFirstRow); // test pozycji
    lcdWriteString("date", xSecondRow, yFirstRow); // test pozycji
    lcdWriteString("KODD", xFirstRow,  yFirstRow / 2 - 4);
}