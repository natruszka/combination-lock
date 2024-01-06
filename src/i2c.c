#include "i2c.h"
#include "UART.h"
#include "delay.h"
#include "GPIO_LPC17xx.h"
#include <string.h>

extern ARM_DRIVER_I2C Driver_I2C0;
static ARM_DRIVER_I2C* I2Cdrv = &Driver_I2C0;

static volatile uint32_t I2C_Event;

static void I2C_SignalEvent ( uint32_t event )
{
	I2C_Event |= event;
	if ( event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE )
	{
		send ( "I2C: Less data was transferred than requested" );
	}

	if ( event & ARM_I2C_EVENT_TRANSFER_DONE )
	{
		send ( "I2C: Transfer or receive is finished" );
	}

	if ( event & ARM_I2C_EVENT_ADDRESS_NACK )
	{
		send ( "I2C: Slave address was not acknowledged" );
	}

	if ( event & ARM_I2C_EVENT_ARBITRATION_LOST )
	{
		send ( "I2C: Master lost bus arbitration" );
	}

	if ( event & ARM_I2C_EVENT_BUS_ERROR )
	{
		send ( "I2C: Invalid start/stop position detected" );
	}

	if ( event & ARM_I2C_EVENT_BUS_CLEAR )
	{
		send ( "I2C: Bus clear operation completed" );
	}

	if ( event & ARM_I2C_EVENT_GENERAL_CALL )
	{
		send ( "I2C: Slave was addressed with a general call address" );
	}

	if ( event & ARM_I2C_EVENT_SLAVE_RECEIVE )
	{
		send ( "I2C: Slave addressed as receiver but SlaveReceive operation is not started" );
	}

	if ( event & ARM_I2C_EVENT_SLAVE_TRANSMIT )
	{
		send ( "I2C: Slave addressed as transmitter but SlaveTransmit operation is not started" );
	}
}

void i2c0_init ( void )
{
	I2Cdrv->Initialize ( I2C_SignalEvent );
	I2Cdrv->PowerControl ( ARM_POWER_FULL );
	I2Cdrv->Control ( ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST );
	I2Cdrv->Control ( ARM_I2C_BUS_CLEAR, 0 );
}

void i2c0_deinit ( void )
{
	I2Cdrv->Uninitialize ();
}

uint8_t i2c0_read ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len )
{
	int status;
	uint8_t msg[1];

	msg[0] = reg;

	I2C_Event = 0U;

	status = I2Cdrv->MasterTransmit ( addr, msg, 1, false );
	if ( status != 0 )
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

	I2C_Event = 0U;

	status = I2Cdrv->MasterReceive ( addr, buf, len, false );
	if ( status != 0 )
	{
		// could not read data
		return 2;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != ( int )len )
	{
		// Data count mismatch
		return 3;
	}

	return 0;
}

uint8_t i2c0_read_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len )
{
	int status;
	uint8_t msg[2];

	msg[0] = ( reg >> 8 ) & 0xFF;
	msg[1] = reg & 0xFF;

	I2C_Event = 0U;

	status = I2Cdrv->MasterTransmit ( addr, msg, 2, false );
	if ( status != 0 )
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != 2 )
	{
		// Data count mismatch
		return 3;
	}

	I2C_Event = 0U;

	status = I2Cdrv->MasterReceive ( addr, buf, len, false );
	if ( status != 0 )
	{
		// could not read data
		return 2;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != ( int )len )
	{
		// Data count mismatch
		return 3;
	}

	return 0;
}

uint8_t i2c0_write ( uint8_t addr, uint8_t reg, uint8_t* buf, uint32_t len )
{
	int status;

	uint8_t msg[len + 1];

	msg[0] = reg;
	memcpy ( msg + 1, buf, len );

	I2C_Event = 0U;

	status = I2Cdrv->MasterTransmit ( addr, msg, len + 1, false );
	if ( status != 0 )
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != ( int )len + 1 )
	{
		// mismatch in data count
		return 3;
	}

	return 0;
}

uint8_t i2c0_write_address16 ( uint8_t addr, uint16_t reg, uint8_t* buf, uint32_t len )
{
	int status;
	uint8_t msg[len + 2];

	msg[0] = ( reg >> 8 ) & 0xFF;
	msg[1] = reg & 0xFF;
	memcpy ( msg + 2, buf, len );

	I2C_Event = 0U;

	status = I2Cdrv->MasterTransmit ( addr, msg, len + 2, false );
	if ( status != 0 )
	{
		// could not initiate transmission
		return 1;
	}

	while ( I2Cdrv->GetStatus ().busy );

	if ( I2Cdrv->GetDataCount () != ( int )len + 2 )
	{
		// data count mismatch
		return 3;
	}

	return 0;
}
