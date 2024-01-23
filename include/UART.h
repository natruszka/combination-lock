#ifdef __DEBUG__

#ifndef UART_H
#define UART_H

#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

/**
 * @brief Send a string to the UART
 * @param str
 * @note The string must be null terminated
 * @return none
 */
void send ( const char* str );

/**
 * @brief Initialize the UART
 * @note Requires pin P0.2 and P0.3
 * @return none
 */
void UART_init ( void );

#endif // UART_H

#endif
