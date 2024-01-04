#include "delay.h"

void delayUS ( uint32_t cnt )
{
    volatile uint32_t i;
    for ( i = 0; i < cnt; i++ )
    {
        volatile uint8_t us = 12; /*  */
        while ( us-- );
    }
}
