#ifndef UART_H
#define UART_H

#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

void send ( const char* str );
void UART_init ( void );

#endif // UART_H
