#ifndef _I2C_H_
#define _I2C_H_

/* includes */
#include "stdlib.h"
#include "stdint.h"
#include "LPC17xx.h"


/**
 * @brief I2C speed modes
 */
#define MODE_100kbps 100000
#define MODE_400kbps 400000
#define MODE_1Mbps 1000000

 /**
  * @brief Initializes the I2C0 peripheral
  * @param i2c_freq I2C frequency in Hz
  * @param int_pri Interrupt priority
  */
void i2c0_init ( uint32_t i2c_freq, uint8_t int_pri );

/**
 * @brief Performs a I2C master send function.
 * @param[in] address I2C slave address
 * @param[in] buffer pointer to data buffer
 * @param[in] length number of bytes to send
 * @return number of bytes sent successfully (0xFFFFFFFF if slave did not response on bus)
 */
uint32_t i2c0_send ( uint8_t address, uint8_t* buffer, uint32_t length );

/**
 * @brief Performs a I2C master receive function.
 * @param[in] address I2C slave address
 * @param[in] buffer pointer to data buffer
 * @param[in] length number of bytes to receive
 * @return number of bytes received successfully (0xFFFFFFFF if slave did not response on bus)
 */
uint32_t i2c0_receive ( uint8_t address, uint8_t* buffer, uint32_t length );


#endif /* _I2C0_H_ */
