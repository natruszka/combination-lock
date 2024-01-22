#ifndef UART_H
#define UART_H

#ifdef __DEBUG__

#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

void send ( const char* str );
void UART_init ( void );

#endif

#endif // UART_H
