#include "interface_LCD.h"
#include "touchscreen_LCD.h"
#include "UART.h"
#include <stdio.h>
#include "fm24clxx.h"


int main ( void )
{
	fm24clxx_address_t addr = FM24CLXX_ADDRESS_A000;
	fm24clxx_t chip_type = FM24CL16B;
	fm24clxx_info_t info;
	uint8_t res = 0;

	UART_init ();

	res = fm24clxx_fram_init ( chip_type, addr );
	if ( res )
	{
		send ( "FM24CLXX init failed" );
	}
	else
	{
		send ( "FM24CLXX initialized" );
	}

	res = fm24clxx_info ( &info );
	if ( res )
	{
		send ( "FM24CLXX get info failed" );
	}
	else
	{
		fm24clxx_interface_debug_print ( "fm24clxx: chip is %s.", info.chip_name );
		fm24clxx_interface_debug_print ( "fm24clxx: manufacturer is %s.", info.manufacturer_name );
		fm24clxx_interface_debug_print ( "fm24clxx: interface is %s.", info.interface );
		fm24clxx_interface_debug_print ( "fm24clxx: driver version is %d.%d.", info.driver_version / 1000, ( info.driver_version % 1000 ) / 100 );
		fm24clxx_interface_debug_print ( "fm24clxx: min supply voltage is %0.1fV.", ( double )info.supply_voltage_min_v );
		fm24clxx_interface_debug_print ( "fm24clxx: max supply voltage is %0.1fV.", ( double )info.supply_voltage_max_v );
		fm24clxx_interface_debug_print ( "fm24clxx: max current is %0.2fmA.", ( double )info.max_current_ma );
		fm24clxx_interface_debug_print ( "fm24clxx: max temperature is %0.1fC.", ( double )info.temperature_max );
		fm24clxx_interface_debug_print ( "fm24clxx: min temperature is %0.fC.", ( double )info.temperature_min );
	}

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

	send ( "Hello World!" );
	fm24clxx_read_test ();
	while ( 1 )
	{
	}
}
