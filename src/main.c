#include "interface_LCD.h"
#include "touchscreen_LCD.h"
#include "timer.h"
#include "PIN_LPC17xx.h"
#include "Board_Buttons.h"
#include "fm24clxx.h"
#include "delay.h"

#ifdef __DEBUG__
#include "UART.h"
#endif

#ifdef INIT_PASS
#define USER_PASS ("1234")
#define USER_PASS_ADDR (0x0000)
#define SERVICE_PASS ("1111")
#define SERVICE_PASS_ADDR (0x0004)
#endif
extern int passpos;

int main ( void )
{
	int x = 0;
	int	y = 0;
	int insertpass = 0;
	int changepass = 0; //Dla 1 zmieniamy haslo, dla 2 zmieniamy kod serwisowy
	uint32_t keys, key1, key2;
#ifdef INIT_PASS
	int res = 0;
#endif
#if defined(__DEBUG__) && defined(INIT_PASS)
	uint8_t buff[5] = { '\0' };
#endif

#ifdef __DEBUG__
	UART_init ();
#endif

#ifdef INIT_PASS
	res = fm24clxx_fram_write ( SERVICE_PASS_ADDR, ( uint8_t* )SERVICE_PASS, sizeof ( SERVICE_PASS ) - 1 );
#ifdef __DEBUG__
	if ( res )
	{
		send ( "fm24clxx: service password init failed." );
	}
	else
	{
		fm24clxx_fram_read ( SERVICE_PASS_ADDR, buff, 4 );
		send ( ( const char* )buff );
	}
#endif
	res = fm24clxx_fram_write ( USER_PASS_ADDR, ( uint8_t* )USER_PASS, sizeof ( USER_PASS ) - 1 );
#ifdef __DEBUG__
	if ( res )
	{
		send ( "fm24clxx: user password init failed." );
	}
	else
	{
		fm24clxx_fram_read ( USER_PASS_ADDR, buff, 4 );
		send ( ( const char* )buff );
	}
#endif
#endif
	RTC_Configuration ();
#ifdef INIT_RTC
	LPC_RTC->DOM = 23;   // Update date value 
	LPC_RTC->MONTH = 1;   // Update month value
	LPC_RTC->YEAR = 2024; // Update year value
	LPC_RTC->HOUR = 9;   // Update hour value 
	LPC_RTC->MIN = 30;   // Update min value
	LPC_RTC->SEC = 00;   // Update sec value 
#ifdef __DEBUG__
	send ( "RTC time initialized" );
#endif
#endif
	Buttons_Initialize ();
	PIN_Configure ( 0, 26, 0, 2, 0 );
	touchpanelInit ();
	lcdConfiguration ();
	init_ILI9325 ();
	lcdWriteReg ( ADRX_RAM, 0 );
	lcdWriteReg ( ADRY_RAM, 0 );
	lcdWriteIndex ( DATA_RAM );
	delayUS ( 50 );
	for ( int register i = 0; i < LCD_MAX_X * LCD_MAX_Y; i++ )
	{
		lcdWriteData ( LCDBlueSea );
		delayUS(1);
	}
	delayUS ( 50 );
	lcdDrawConfigTouchpanel ();
	lcdTouchscreenCallibrate ();
	delayUS ( 50 );
	lcdDrawInterface ();
	while ( 1 )
	{
		keys = Buttons_GetState ();
		key1 = keys & 0x01;
		key2 = keys & 0x02;
		if ( key1 )
		{
			insertpass = 1;
			changepass = 0;
		}
		if ( key2 )
		{
			insertpass = 0;
			changepass++;
			if ( changepass > 2 )
			{
				changepass = 0;
			}
		}
		if ( insertpass )
		{
			lcdInsertPassword ();
			lcdTouchscreenGetCoords ( &x, &y );
			delayUS ( 500000 );
		}
		else if ( changepass == 1 )
		{
#ifdef __DEBUG__
			send ( "zmiana hasla" );
#endif
			lcdNewCode ();
			changepass = 0;
			insertpass = 1;
			passpos = 0;

		}
		else if ( changepass == 2 )
		{
#ifdef __DEBUG__
			send ( "zmiana hasla serwisowego" );
#endif
			lcdNewServiceCode ();
			changepass = 0;
			insertpass = 1;
			passpos = 0;
		}
		delayUS ( 200000 );
	}
}
