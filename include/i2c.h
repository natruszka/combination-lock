#ifndef _I2C_H_
#define _I2C_H_

#include "Driver_I2C.h"

/**
 * @brief Init procedure for I2C0 bus
 * @note Requires external driver: ARM_DRIVER_I2C Driver_I2C0 and configuration
 * of the I2C0 pins in the RTE_Device.h file for pin P0_27 and P0_28
 * @return none
 */
void i2c0_init ( void );

/**
 * @brief Deinit procedure for I2C0 bus
 * @note none
 * @return none
 */
void i2c0_deinit ( void );

/**
 * @brief Read function for I2C0 bus
 * @param addr I2C address
 * @param reg Register address
 * @param buf Pointer to data buffer
 * @param len Length of data buffer
 * @note none
 * @return Status code
 *         - 0 Success
 *         - 1 Could not initiate transmission
 *         - 2 Could not read data
 *         - 3 Data count mismatch
 */
uint8_t i2c0_read ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len );

/**
 * @brief Read function for I2C0 bus with 16-bit register address
 * @param addr I2C address
 * @param reg Register address (16-bit)
 * @param buf Pointer to data buffer
 * @param len Length of data buffer
 * @note none
 * @return Status code
 *         - 0 Success
 *         - 1 Could not initiate transmission
 *         - 2 Could not read data
 *         - 3 Data count mismatch
 */
uint8_t i2c0_read_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len );

/**
 * @brief Write function for I2C0 bus
 * @param addr I2C address
 * @param reg Register address
 * @param buf Pointer to data buffer
 * @param len Length of data buffer
 * @note none
 * @return Status code
 *         - 0 Success
 *         - 1 Could not initiate transmission
 *         - 3 Data count mismatch
 */
uint8_t i2c0_write ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len );

/**
 * @brief Write function for I2C0 bus with 16-bit register address
 * @param addr I2C address
 * @param reg Register address (16-bit)
 * @param buf Pointer to data buffer
 * @param len Length of data buffer
 * @note none
 * @return Status code
 *        - 0 Success
 *        - 1 Could not initiate transmission
 *        - 3 Data count mismatch
 */
uint8_t i2c0_write_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len );

#endif /* _I2C0_H_ */
