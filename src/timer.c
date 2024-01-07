#include "timer.h"
#include "LPC17xx.h"
#include "interface_LCD.h"
#include "UART.h"
#include <stdio.h>

static uint8_t hour, min, date, month;
static uint16_t year;

void RTC_Configuration(void)
{
    LPC_RTC->CCR = 1;
    LPC_RTC->ILR = 1;
    LPC_RTC->CIIR = 2; // przerwanie generowane co minute

    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_IRQHandler(void)
{
    year = LPC_RTC->YEAR;
    hour = LPC_RTC->HOUR;
    min = LPC_RTC->MIN;
    date = LPC_RTC->DOM;
    month = LPC_RTC->MONTH;
    char napis[20];
    sprintf(napis, "%02u:%02u %02u/%02u/%02u", hour, min, date, month, year);
    send(napis);
    lcdDisplayDate(napis);
    LPC_RTC->ILR = 1;
}
void RTC_ShowOpenDate(void)
{
    year = LPC_RTC->YEAR;
    hour = LPC_RTC->HOUR;
    min = LPC_RTC->MIN;
    date = LPC_RTC->DOM;
    month = LPC_RTC->MONTH;
    char napis[20];
    sprintf(napis, "%02u:%02u %02u/%02u/%02u", hour, min, date, month, year%100);
    send(napis);
    lcdOpenDate(napis);
}
void RTC_ShowAlarmDate(void)
{
    year = LPC_RTC->YEAR;
    hour = LPC_RTC->HOUR;
    min = LPC_RTC->MIN;
    date = LPC_RTC->DOM;
    month = LPC_RTC->MONTH;
    char napis[20];
    sprintf(napis, "%02u:%02u %02u/%02u/%02u", hour, min, date, month, year%100);
    send(napis);
    lcdAlarmDate(napis);
}