#include "interface_LCD.h"
#include "touchscreen_LCD.h"
#include "UART.h"

static void touchpanelDelayUS(uint32_t cnt) {
	volatile uint32_t i;
	for (i = 0; i < cnt; i++) {
		volatile uint8_t us = 12; /*  */
		while (us--) /*  */
		{
			;
		}
	}
}
int main ( void )
{
	UART_init ();
	send ( "ACHTUNG\r\n" );
	touchpanelInit ();
	send ( "START\r\n" );
	lcdConfiguration ();
	init_ILI9325 ();
	lcdWriteReg ( ADRX_RAM, 0 );
	lcdWriteReg ( ADRY_RAM, 0 );
	lcdWriteIndex ( DATA_RAM );
	touchpanelDelayUS(40);
	for ( int register x = 0; x < LCD_MAX_X * LCD_MAX_Y; x++ )
	{
		lcdWriteData ( LCDBlueSea );
	}
	lcdDrawConfigTouchpanel ();
	lcdTouchscreenCallibrate ();
	lcdDrawInterface ();
	lcdTest();

	//	char temp[10];
	//	sprintf(temp, "%d", LCD_MAX_Y);
	//	send(temp);

	send("I'm free\r\n");
	int x = 0;
	int	y = 0;
	while ( 1 )
	{
		lcdTouchscreenGetCoords(&x, &y);
		touchpanelDelayUS(20000);
	}
}
