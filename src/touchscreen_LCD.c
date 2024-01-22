#include "touchscreen_LCD.h"
#include "fm24clxx.h"

#define SERVICE_PASS_ADDR (0x0004)
#define USER_PASS_ADDR (0x0000)

float A;
float B;
float C;
float D;
float E;
float F;

static bool isConf;

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

void lcdTouchscreenGetCoords ( int* x, int* y )
{
#ifdef __DEBUG__
	char temp[20];
#endif
	while ( 1 )
		if ( !GPIO_PinRead ( 0, 19 ) )
		{
			touchpanelGetXY ( x, y );
#ifdef __DEBUG__
			sprintf ( temp, "%d, %d \r\n", *x, *y );
			send ( temp );
#endif
			if ( isConf )
			{
				lcdHandler ( *x, *y );
			}
			break;
		}
}

char lcdTouchscreenGetPassword ( int* x, int* y, bool showChar, int pos )
{
	char password;
#ifdef __DEBUG__
	char temp[20];
#endif
	while ( 1 )
		if ( !GPIO_PinRead ( 0, 19 ) )
		{
#ifdef __DEBUG__
			sprintf ( temp, "%d, %d", *x, *y );
			send ( temp );
#endif
			touchpanelGetXY ( x, y );
			password = lcdGetChar ( *x, *y, showChar, pos );
			break;
		}
	return password;
}

int getMin ( int cords[], int len )
{
	int min = cords[0];
	for ( int i = 0; i < len; ++i )
	{
		if ( cords[i] < min )
			min = cords[i];
	}
	return min;
}

int getMax ( int cords[], int len )
{
	int max = cords[0];
	for ( int i = 0; i < len; ++i )
	{
		if ( cords[i] > max )
			max = cords[i];
	}
	return max;
}

int average ( int cords[], int len )
{
	int maxVal = getMax ( cords, len );
	int minVal = getMin ( cords, len );
	int sum = 0;
	for ( int i = 0; i < 10; ++i )
	{
		if ( cords[i] == minVal || cords[i] == maxVal )
			continue;
		sum += cords[i];
	}
	return sum / 8;
}

void calculateConstants ( int x[], int y[], int len )
{
	int x1 = average ( x, len / 3 );
	int x2 = average ( x + len / 3, len / 3 );
	int x3 = average ( x + 2 * len / 3, len / 3 );

	int y1 = average ( y, len / 3 );
	int y2 = average ( y + len / 3, len / 3 );
	int y3 = average ( y + 2 * len / 3, len / 3 );

#ifdef __DEBUG__
	char srednie[40];
	sprintf ( srednie, "Srednia 1: %d, %d", x1, y1 );
	send ( srednie );
	sprintf ( srednie, "Srednia 2: %d, %d", x2, y2 );
	send ( srednie );
	sprintf ( srednie, "Srednia 3: %d, %d", x3, y3 );
	send ( srednie );
#endif

	float dzielnik = ( float )( y1 * ( x2 - x3 ) - y2 * ( x1 - x3 ) + y3 * ( x1 - x2 ) );
	int mozeA = 200 * y1 - 200 * y3;
	int mozeB = -200 * x1 + 200 * x3;
	int mozeC = 20 * x2 * y1 - 220 * x3 * y1 - 20 * x1 * y2 + 20 * x3 * y2 + 220 * x1 * y3 - 20 * x2 * y3;
	int mozeD = -280 * y1 + 280 * y2;
	int mozeE = 280 * x1 - 280 * x2;
	int mozeF = 300 * x2 * y1 - 20 * x3 * y1 - 300 * x1 * y2 + 20 * x3 * y2 + 20 * x1 * y3 - 20 * x2 * y3;
	A = ( float )mozeA / dzielnik;
	B = ( float )mozeB / dzielnik;
	C = ( float )mozeC / dzielnik;
	D = ( float )mozeD / dzielnik;
	E = ( float )mozeE / dzielnik;
	F = ( float )mozeF / dzielnik;

#ifdef __DEBUG__
	char temp[70];
	sprintf ( temp, "%f, %f, %f, %f, %f, %f", A, B, C, D, E, F );
	send ( temp );
#endif
	isConf = true;
}

bool lcdGetServiceCode ( void )
{
	bool res = false;
	static int pos;
	int x;
	int y;
	uint8_t code[4];
	uint8_t correctCode[4];
	fm24clxx_fram_read ( SERVICE_PASS_ADDR, correctCode, 4 );
	for ( int i = 0; i < 4; i++ )
	{
#ifdef __DEBUG__
		send ( "czekam na haslo" );
#endif
		code[i] = (uint8_t)lcdTouchscreenGetPassword ( &x, &y, true, pos );
		touchpanelDelayUS ( 500000 );
		pos++;
	}
	res = checkCode ( code, 4, correctCode, 4 );
	if ( res )
	{
		lcdWriteString ( "OK    ", 160, 8 );
		lcdClearInput ();
	}
	else
	{
		lcdWriteString ( "WRONG ", 160, 8 );
		lcdClearInput ();
	}
	pos = 0;
	return res;
}

void lcdNewPassword(bool isService)
{
	static int pos;
	int x;
	int y;
	int res;
	uint8_t code[4];
	#ifdef __DEBUG__
		send ( "czekam na nowe haslo" );
#endif
	for ( int i = 0; i < 4; i++ )
	{
		code[i] = (uint8_t)lcdTouchscreenGetPassword ( &x, &y, true, pos );
		touchpanelDelayUS ( 500000 );
		pos++;
	}
	if(isService)
	{
		res = fm24clxx_fram_write ( SERVICE_PASS_ADDR, code, 4 );
	}
	else
	{
		res = fm24clxx_fram_write ( USER_PASS_ADDR, code, 4 );
	}
		pos = 0;
	#ifdef __DEBUG__
	if(res)
	{
		send ( "Nie ustawiono nowego hasla" );
	}
	else
		{
		send ( "Ustawiono nowe haslo" );
	}
#endif
	lcdClearInput();
}
void lcdTouchscreenCallibrate ( void )
{
	int x_arr[30];
	int y_arr[30];
	unsigned int i;
	int* x_ptr = x_arr;
	int* y_ptr = y_arr;

	isConf = false;

	for ( i = 0; i < 30; ++i )
	{
		x_arr[i] = -1;
		y_arr[i] = -1;
	}
	lcdDrawCross ( 20, 20, 10, LCDRed );
	for ( i = 0; i < 10; ++i )
	{
#ifdef __DEBUG__
		send ( "Jestem w for 1!" );
#endif
		lcdTouchscreenGetCoords ( x_ptr++, y_ptr++ );
#ifdef __DEBUG__
		send ( "1 jestem po" );
#endif
		touchpanelDelayUS ( 2000 );
	}
#ifdef __DEBUG__
	send ( "czekam po for 1" );
#endif
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( 20, 20, 10, LCDBlack );
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDRed );
	for ( i = 10; i < 20; ++i )
	{
#ifdef __DEBUG__
		send ( "Jestem w for 2!" );
#endif
		lcdTouchscreenGetCoords ( x_ptr++, y_ptr++ );
#ifdef __DEBUG__
		send ( "2 jestem po" );
#endif
		touchpanelDelayUS ( 2000 );
	}
#ifdef __DEBUG__
	send ( "czekam po for 2" );
#endif
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDBlack );
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( 20, LCD_MAX_Y - 20, 10, LCDRed );
	for ( i = 20; i < 30; ++i )
	{
#ifdef __DEBUG__
		send ( "Jestem w for 3!" );
#endif
		lcdTouchscreenGetCoords ( x_ptr++, y_ptr++ );
#ifdef __DEBUG__
		send ( "3 jestem po" );
#endif
		touchpanelDelayUS ( 2000 );
	}
#ifdef __DEBUG__
	send ( "czekam po for 3" );
#endif
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( 20, LCD_MAX_Y - 20, 10, LCDBlack );
#ifdef __DEBUG__
	send ( "Done" );
#endif
	touchpanelDelayUS ( 2000 );
	lcdClearScreen ();
	calculateConstants ( x_arr, y_arr, 30 );
}
