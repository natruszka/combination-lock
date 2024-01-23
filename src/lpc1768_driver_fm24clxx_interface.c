/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_fm24clxx_interface_template.c
 * @brief     driver fm24clxx interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-06-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/06/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "lpc1768_driver_fm24clxx_interface.h"
#include "i2c.h"
#include "delay.h"

#ifdef __DEBUG__
#include "UART.h"
#endif

#include <stdarg.h>

 /**
  * @brief  interface iic bus init
  * @return status code
  *         - 0 success
  *         - 1 iic init failed
  * @note   none
  */
uint8_t fm24clxx_interface_iic_init ( void )
{
	i2c0_init ();
	return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t fm24clxx_interface_iic_deinit ( void )
{
	i2c0_deinit ();
	return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t fm24clxx_interface_iic_read ( uint8_t addr, uint8_t reg, uint8_t* buf, uint16_t len )
{
	i2c0_read ( addr, reg, buf, len );
	return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t fm24clxx_interface_iic_write ( uint8_t addr, uint8_t reg, uint8_t* buf, uint16_t len )
{
	i2c0_write ( addr, reg, buf, len );
	return 0;
}

/**
 * @brief      interface iic bus read with 16 bits register address
 * @param[in]  addr is the iic device write address
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t fm24clxx_interface_iic_read_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint16_t len )
{
	i2c0_read_address16 ( addr, reg, buf, len );
	return 0;
}

/**
 * @brief     interface iic bus write with 16 bits register address
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t fm24clxx_interface_iic_write_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint16_t len )
{
	i2c0_write_address16 ( addr, reg, buf, len );
	return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void fm24clxx_interface_delay_ms ( uint32_t ms )
{
	delayUS ( ms * 1000 );
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
#ifdef __GNUC__
__attribute__ ( ( __format__ ( __printf__, 1, 2 ) ) )
#endif
void fm24clxx_interface_debug_print ( const char* const fmt, ... )
{
	char buf[256];
	va_list args;

	memset ( buf, 0, sizeof ( buf ) );
	va_start ( args, fmt );
	vsnprintf ( buf, 256, fmt, args );
	va_end ( args );
	buf[255] = '\0';

	send ( buf );
}
