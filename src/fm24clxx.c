#include "fm24clxx.h"
#include <stdlib.h>

#ifdef __DEBUG__
#include "UART.h"
#endif


#define USER_PASS_ADDR (0x00)
#define SERVICE_PASS_ADDR (0x04)

static fm24clxx_handle_t handle;

#ifdef __DEBUG__
/**
 * @brief     read test
 * @param[in] type is the chip type
 * @param[in] address is the chip address pin
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t fm24clxx_test ( void )
{
    uint8_t res, i, j;
    uint8_t buf[12];
    uint8_t buf_check[12];
    uint16_t inc;
    fm24clxx_t type = FM24CL16B;
    fm24clxx_address_t addr = FM24CLXX_ADDRESS_A000;

    fm24clxx_fram_init ( type, addr );

    fm24clxx_interface_debug_print ( "fm24clxx: start read test." );
    inc = ( ( uint16_t )type + 1 ) / 8;
    for ( i = 0; i < 8; i++ )
    {
        for ( j = 0; j < 11; j++ )
        {
            buf[j] = ( uint8_t )( 'A' + j );
        }
        buf[11] = '\0';
        fm24clxx_interface_debug_print ( "fm24clxx: write %d bytes of data to address 0x%04X.", 12, i * inc );

        /* write data */
        res = fm24clxx_fram_write ( i * inc, ( uint8_t* )buf, 12 );
        if ( res != 0 )
        {
            fm24clxx_interface_debug_print ( "fm24clxx: write failed." );
            return 1;
        }
        fm24clxx_interface_debug_print ( "fm24clxx: read %d bytes of data from address 0x%04X.", 12, i * inc );

        /* read data */
        res = fm24clxx_fram_read ( i * inc, ( uint8_t* )buf_check, 12 );
        if ( res != 0 )
        {
            fm24clxx_interface_debug_print ( "fm24clxx: read failed." );
            return 1;
        }
        fm24clxx_interface_debug_print ( buf );
        fm24clxx_interface_debug_print ( buf_check );
        for ( j = 0; j < 12; j++ )
        {
            /* check data */
            if ( buf[j] != buf_check[j] )
            {
                fm24clxx_interface_debug_print ( "fm24clxx: check error." );
                return 1;
            }
        }
        fm24clxx_interface_debug_print ( "fm24clxx: 0x%04X read write test passed.", i * inc );
    }

    /* finish read test */

    fm24clxx_interface_debug_print ( "fm24clxx: finish read test." );
    fm24clxx_fram_deinit ();

    return 0;
}
#endif

uint8_t fm24clxx_fram_init ( fm24clxx_t type, fm24clxx_address_t addr )
{
    uint8_t res = 0;

    DRIVER_FM24CLXX_LINK_INIT ( &handle, fm24clxx_handle_t );
    DRIVER_FM24CLXX_LINK_IIC_INIT ( &handle, fm24clxx_interface_iic_init );
    DRIVER_FM24CLXX_LINK_IIC_DEINIT ( &handle, fm24clxx_interface_iic_deinit );
    DRIVER_FM24CLXX_LINK_IIC_READ ( &handle, fm24clxx_interface_iic_read );
    DRIVER_FM24CLXX_LINK_IIC_WRITE ( &handle, fm24clxx_interface_iic_write );
    DRIVER_FM24CLXX_LINK_IIC_READ_ADDRESS16 ( &handle, fm24clxx_interface_iic_read_address16 );
    DRIVER_FM24CLXX_LINK_IIC_WRITE_ADDRESS16 ( &handle, fm24clxx_interface_iic_write_address16 );
    DRIVER_FM24CLXX_LINK_DELAY_MS ( &handle, fm24clxx_interface_delay_ms );
    DRIVER_FM24CLXX_LINK_DEBUG_PRINT ( &handle, fm24clxx_interface_debug_print );

    res = fm24clxx_set_type ( &handle, type );
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: set type failed." );
#endif

        return 1;
    }

    res = fm24clxx_set_addr_pin ( &handle, addr );
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: set addr pin failed." );
#endif

        return 1;
    }

    res = fm24clxx_init ( &handle );
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: init failed." );
#endif

        return 1;
    }

    return 0;
}

uint8_t fm24clxx_fram_read ( uint16_t addr, uint8_t* buf, uint16_t len )
{
    fm24clxx_t type = FM24CL16B;
    fm24clxx_address_t slave_addr = FM24CLXX_ADDRESS_A000;
    int res = 0;

    res = fm24clxx_fram_init ( type, slave_addr );
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: init failed." );
#endif

        return 1;
    }

    if ( fm24clxx_read ( &handle, addr, buf, len ) )
    {
        return 1;
    }

    res = fm24clxx_fram_deinit ();
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: deinit failed." );
#endif

        return 1;
    }

    return 0;
}

uint8_t fm24clxx_fram_write ( uint16_t addr, uint8_t* buf, uint16_t len )
{
    fm24clxx_t type = FM24CL16B;
    fm24clxx_address_t slave_addr = FM24CLXX_ADDRESS_A000;
    int res = 0;

    res = fm24clxx_fram_init ( type, slave_addr );
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: init failed." );
#endif

        return 1;
    }

    if ( fm24clxx_write ( &handle, addr, buf, len ) )
    {
        return 1;
    }

    res = fm24clxx_fram_deinit ();
    if ( res )
    {
#ifdef __DEBUG__
        fm24clxx_interface_debug_print ( "fm24clxx: deinit failed." );
#endif

        return 1;
    }

    return 0;
}

uint8_t fm24clxx_fram_deinit ( void )
{
    if ( fm24clxx_deinit ( &handle ) )
    {
        return 1;
    }

    return 0;
}

