#include "i2c.h"
#include <string.h>

extern ARM_DRIVER_I2C Driver_I2C0;
static ARM_DRIVER_I2C* I2Cdrv = &Driver_I2C0;

void i2c0_init ( void )
{
	I2Cdrv->Initialize ( NULL );
	I2Cdrv->PowerControl ( ARM_POWER_FULL );
	I2Cdrv->Control ( ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD );
	I2Cdrv->Control ( ARM_I2C_BUS_CLEAR, 0 );
}

void i2c0_deinit ( void )
{
	I2Cdrv->Uninitialize ();
}

uint8_t i2c0_read ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len )
{
	int status;
	uint8_t msg[2];

	msg[0] = addr;
	msg[1] = reg;

	status = I2Cdrv->MasterTransmit ( addr, msg, 2, false );
	if (status != 0)
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != 1 )
	{
		// Data count mismatch
		return 3;
	}

	status = I2Cdrv->MasterReceive ( addr, buf, len, false );
	if ( status != 0 )
	{
		// could not read data
		return 2;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != ( int ) len )
	{
		// Data count mismatch
		return 3;
	}

	return 0;
}

uint8_t i2c0_read_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len )
{
	int status;
	uint8_t msg[3];

	msg[0] = addr;
	msg[1] = ( reg >> 8 ) & 0xFF;
	msg[2] = reg & 0xFF;

	status = I2Cdrv->MasterTransmit ( addr, msg, 3, false );
	if (status != 0)
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != 1 )
	{
		// Data count mismatch
		return 3;
	}

	status = I2Cdrv->MasterReceive ( addr, buf, len, false );
	if ( status != 0 )
	{
		// could not read data
		return 2;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != ( int ) len )
	{
		// Data count mismatch
		return 3;
	}

	return 0;
}

uint8_t i2c0_write ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len )
{
	int status;

	uint8_t msg[len + 2];

	msg[0] = addr;
	msg[1] = reg;
	memcpy(msg + 2, buf, len);

	status = I2Cdrv->MasterTransmit ( addr, msg, len + 2, false );
	if (status != 0)
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != 1 )
	{
		// mismatch in data count
		return 3;
	}

	return 0;
}

uint8_t i2c0_write_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len )
{
	int status;
	uint8_t msg[len + 3];

	msg[0] = addr;
	msg[1] = ( reg >> 8 ) & 0xFF;
	msg[2] = reg & 0xFF;
	memcpy(msg + 3, buf, len);

	status = I2Cdrv->MasterTransmit ( addr, msg, len + 3, false );
	if (status != 0)
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != 1 )
	{
		// data count mismatch
		return 3;
	}

	return 0;
}
