#include "timer.h"
#include "LPC17xx.h"
#include "interface_LCD.h"

#include <stdio.h>

#ifdef __DEBUG__
#include "UART.h"
#endif

static uint8_t hour, min, date, month;
static uint16_t year;

void RTC_Configuration ( void )
{
  LPC_RTC->CCR = 1;
  LPC_RTC->ILR = 1;
  LPC_RTC->CIIR = 2; // przerwanie generowane co minute

  NVIC_EnableIRQ ( RTC_IRQn );
}

void RTC_IRQHandler ( void )
{
  char napis[20];

  year = LPC_RTC->YEAR;
  hour = LPC_RTC->HOUR;
  min = LPC_RTC->MIN;
  date = LPC_RTC->DOM;
  month = LPC_RTC->MONTH;

#ifdef __DEBUG__
  sprintf ( napis, "%02u:%02u %02u/%02u/%02u", hour, min, date, month, year );
  send ( napis );
#endif
  lcdDisplayDate ( napis );
  LPC_RTC->ILR = 1;
}
void RTC_ShowOpenDate ( void )
{
  char napis[20];

  year = LPC_RTC->YEAR;
  hour = LPC_RTC->HOUR;
  min = LPC_RTC->MIN;
  date = LPC_RTC->DOM;
  month = LPC_RTC->MONTH;

#ifdef __DEBUG__
  sprintf ( napis, "%02u:%02u %02u/%02u/%02u", hour, min, date, month, year % 100 );
  send ( napis );
#endif
  lcdOpenDate ( napis );
}
void RTC_ShowAlarmDate ( void )
{
  char napis[20];

  year = LPC_RTC->YEAR;
  hour = LPC_RTC->HOUR;
  min = LPC_RTC->MIN;
  date = LPC_RTC->DOM;
  month = LPC_RTC->MONTH;

#ifdef __DEBUG__
  sprintf ( napis, "%02u:%02u %02u/%02u/%02u", hour, min, date, month, year % 100 );
  send ( napis );
#endif
  lcdAlarmDate ( napis );
}
