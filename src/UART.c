#include "UART.h"
#include "string.h"

void send ( const char* str )
{
    int i = 0;
    while ( str[i] != '\0' )
    {
        if ( LPC_UART0->LSR & ( 1 << 5 ) )
        {
            LPC_UART0->THR = str[i++];
        }
    }
    while ( ( LPC_UART0->LSR & ( 1 << 5 ) ) ) {}
    LPC_UART0->THR = '\n';
    while ( ( LPC_UART0->LSR & ( 1 << 5 ) ) ) {}
    LPC_UART0->THR = '\r';
}

void UART_init ( void )
{
    //    LPC_SC->PCONP |= (1 << 25);
    PIN_Configure ( 0, 2, 1, 2, 0 );
    PIN_Configure ( 0, 3, 1, 0, 0 );

    LPC_UART0->LCR = 3 | ( 1 << 7 );
    LPC_UART0->DLL = 27;
    LPC_UART0->DLM = 0;
    LPC_UART0->LCR = 3;
    LPC_UART0->FCR = 1;
}
