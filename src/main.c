#include "interface_LCD.h"
#include "touchscreen_LCD.h"
#include "UART.h"

int main ( void )
{
	UART_init ();
	//	EINT3_init();
	touchpanelInit ();
	send ( "START\r\n" );
	lcdConfiguration ();
	init_ILI9325 ();
	lcdWriteReg ( ADRX_RAM, 0 );
	lcdWriteReg ( ADRY_RAM, 0 );
	lcdWriteIndex ( DATA_RAM );
	for ( int register x = 0; x < LCD_MAX_X * LCD_MAX_Y; x++ )
	{
		lcdWriteData ( LCDBlueSea );
	}
	lcdDrawConfigTouchpanel ();
	lcdDrawInterface ();
	lcdTouchscreenCallibrate ();

	//	char temp[10];
	//	sprintf(temp, "%d", LCD_MAX_Y);
	//	send(temp);

	while ( 1 )
	{

	}
}
