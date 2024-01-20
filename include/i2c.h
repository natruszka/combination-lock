#ifndef _I2C_H_
#define _I2C_H_

#include "Driver_I2C.h"

void i2c0_init ( void );
void i2c0_deinit ( void );
uint8_t i2c0_read ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len );
uint8_t i2c0_write ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len );
uint8_t i2c0_read_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len );
uint8_t i2c0_write_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len );

#endif /* _I2C0_H_ */
