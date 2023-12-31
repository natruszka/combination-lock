#include "interface_LCD.h"
#include "touchscreen_LCD.h"
#include "UART.h"
#include "I2C_LPC17xx.h"
#include <stdio.h>
#include "driver_fm24clxx.h"


int main ( void )
{
	UART_init ();
	
	send("FRAM initialized\r\n");
	
	touchpanelInit ();
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
//	lcdTouchscreenCallibrate ();
	
	send("Hello World!\n\r");
	while ( 1 )
	{
	}
}
