#include "interface_LCD.h"
#include "touchscreen_LCD.h"
#include "timer.h"
#include "PIN_LPC17xx.h"
#include "Board_Buttons.h"
#include "fm24clxx.h"

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

static void touchpanelDelayUS ( uint32_t cnt )
{
	volatile uint32_t i;
	for ( i = 0; i < cnt; i++ )
	{
		volatile uint8_t us = 12; /*  */
		while ( us-- ) /*  */
		{
		}
	}
}
int main ( void )
{
	int x = 0;
	int	y = 0;
	int insertpass = 0;
	int changepass = 0; //Dla 1 zmieniamy haslo, dla 2 zmieniamy kod serwisowy
	uint32_t keys, key1, key2;
	int res = 0;
	uint8_t buff[5] = { '\0' };

#ifdef __DEBUG__
	UART_init ();
#endif

// tak wiec tak, ponizej masz write i read jak sie obslugje, init_pass dajemy raz na flash-a, pozniej flashujemy juz bez tego w definach
// jak chcesz wyslac na uart to prosze rob te ifdefy zeby bylo ladnie ze jest wersja bez debugu, ja to jeszcze jutro troche posprzatam po swojej stronie i zrobie docsy :D	o i mozesz powymieniach touchpanelDelaysUS na delayUS
#ifdef INIT_PASS
	res = fm24clxx_fram_write ( USER_PASS_ADDR, ( uint8_t* )USER_PASS, sizeof ( USER_PASS ) - 1 );
	if ( res )
	{
#ifdef __DEBUG__
		send ( "fm24clxx: user password init failed." );
#endif
	}

	res = fm24clxx_fram_write ( SERVICE_PASS_ADDR, ( uint8_t* )SERVICE_PASS, sizeof ( SERVICE_PASS ) - 1 );
	if ( res )
	{
#ifdef __DEBUG__
		send ( "fm24clxx: service password init failed." );
#endif
	}
#endif
		Buttons_Initialize ();
		PIN_Configure ( 0, 26, 0, 2, 0 );
		touchpanelInit ();
		lcdConfiguration ();
		init_ILI9325 ();
		lcdWriteReg ( ADRX_RAM, 0 );
		lcdWriteReg ( ADRY_RAM, 0 );
		lcdWriteIndex ( DATA_RAM );
		touchpanelDelayUS ( 40 );
		touchpanelDelayUS ( 40 );
		for ( int register i = 0; i < LCD_MAX_X * LCD_MAX_Y; i++ )
		{
			lcdWriteData ( LCDBlueSea );
		}
		lcdDrawConfigTouchpanel ();
		lcdTouchscreenCallibrate ();
		lcdDrawInterface ();
		RTC_Configuration ();
		//	LPC_RTC->DOM = 7;   // Update date value 
		//	LPC_RTC->MONTH = 1;   // Update month value
		//	LPC_RTC->YEAR = 2024; // Update year value
		//	LPC_RTC->HOUR = 21;   // Update hour value 
		//	LPC_RTC->MIN = 01;   // Update min value
		//	LPC_RTC->SEC = 00;   // Update sec value 
	#ifdef INIT_PASS
		fm24clxx_fram_read ( USER_PASS_ADDR, buff, 4 );
#ifdef __DEBUG__
		send ( buff );
#endif
		fm24clxx_fram_read ( SERVICE_PASS_ADDR, buff, 4 );
#ifdef __DEBUG__
		send ( buff );
#endif
#endif
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
			touchpanelDelayUS ( 500000 );
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
		touchpanelDelayUS ( 200000 );
	}
}
