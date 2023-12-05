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

void EINT3_init ( void )
{

}

 void EINT3_IRQHandler (void)
{

}

void lcdTouchscreenGetCoords ( int* x, int* y )
{
	char temp[10];
	while ( 1 )
	{
		if ( !GPIO_PinRead ( 0, 19 ) )
		{
			touchpanelGetXY ( x, y );
			sprintf ( temp, "%d, %d \r\n", *x, *y );
			send ( temp );
			break;
		}
	}
}

void lcdTouchscreenCallibrate ( void )
{
    int x_arr[10*3];
    int y_arr[10*3];
		int last_element = 1;
		for(int i = 0; i<30;i++){
			x_arr[i] = -1;
			y_arr[i] = -1;
		}
    static int callibrated = 0;
    if ( callibrated > 2 )
    {
        return;
    }
    lcdDrawCross ( 20, 20, 10, LCDRed );
		for(unsigned int i = 0; i < 10; ++i)
		{
			send("Jestem w for 1!\n\r");
			lcdTouchscreenGetCoords ( &x_arr[i], &y_arr[i] );
			send("1 jestem po\n\r");
			touchpanelDelayUS ( 2000 );
			if(x_arr[i] < 0 || y_arr[i] <0){
				i = 0;
			}
		}
    touchpanelDelayUS ( 2000000 );
		lcdDrawCross ( 20, 20, 10, LCDBlack );
		touchpanelDelayUS ( 2000000 );
    lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDRed );
		for(unsigned int i = 10; i < 20; ++i)
    {
			send("Jestem w for 2!\n\r");
			lcdTouchscreenGetCoords ( &x_arr[i], &y_arr[i] );
			send("2 jestem po\n\r");
			touchpanelDelayUS ( 2000 );
			if(x_arr[i] < 0 || y_arr[i] <0){
				i = 10;
			}
		}
		touchpanelDelayUS ( 2000000 );
    lcdDrawCross ( LCD_MAX_X - 20, 20, 10, LCDBlack );
		touchpanelDelayUS ( 2000000 );
    lcdDrawCross ( 20, LCD_MAX_Y - 20, 10, LCDRed );
		for(unsigned int i = 20; i < 30; ++i)
		{
			send("Jestem w for 3!\n\r");
			lcdTouchscreenGetCoords ( &x_arr[i], &y_arr[i] );
			send("3 jestem po\n\r");
			touchpanelDelayUS ( 2000 );
			if(x_arr[i] < 0 || y_arr[i] <0){
				i = 20;
			}
		}
		touchpanelDelayUS ( 2000000 );
    lcdDrawCross ( 20, LCD_MAX_Y - 20, 10, LCDBlack );
}
