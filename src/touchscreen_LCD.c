#include "touchscreen_LCD.h"

static void touchpanelDelayUS ( uint32_t cnt )
{
	volatile uint32_t i;
	for ( i = 0; i < cnt; i++ )
	{
		volatile uint8_t us = 12; /*  */
		while ( us-- );
	}
}

void lcdTouchscreenGetCoords ( int* x, int* y )
{
	char temp[20];
	while ( 1 )
	{
		if ( !GPIO_PinRead ( 0, 19 ) )
		{
			touchpanelGetXY ( x, y );
			sprintf ( temp, "%d, %d", *x, *y );
			send ( temp );
			break;
		}
	}
}

void lcdTouchscreenCallibrate ( void )
{
	int x_arr[3 * 8];
	int y_arr[3 * 8];

	static int callibrated = 0;
	if ( callibrated > 2 )
	{
		return;
	}
	lcdDrawCross ( 20, 20, 10, LCDRed );
	for ( unsigned int i = 0; i < 8; ++i )
	{
		lcdTouchscreenGetCoords ( &x_arr[i], &y_arr[i] );
		touchpanelDelayUS ( 2000 );
	}
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( 20, 20, 10, LCDBlack );
	lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDRed );
	for ( unsigned int i = 8; i < 16; ++i )
	{
		lcdTouchscreenGetCoords ( &x_arr[1], &y_arr[1] );
		touchpanelDelayUS ( 2000 );
	}
	touchpanelDelayUS ( 2000000 );
	lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDBlack );
	lcdDrawCross ( LCD_MAX_X / 2, LCD_MAX_Y - 20, 10, LCDRed );
	for ( unsigned int i = 16; i < 24; ++i )
	{
		lcdTouchscreenGetCoords ( &x_arr[i], &y_arr[i] );
		touchpanelDelayUS ( 2000 );
	}
	lcdDrawCross ( LCD_MAX_X / 2, LCD_MAX_Y - 20, 10, LCDBlack );
}
