#ifndef fm24clxx_fram_H
#define fm24clxx_fram_H

#include <stdint.h>

#include "lpc1768_driver_fm24clxx_interface.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __DEBUG__
  /**
   * @brief Simple test function for the FM24CLXX FRAM.
   * It writes a string to the FRAM and then reads it back few times.
   * @return status code
   *        - 0 success
   *       - 1 test failed
   * @note none
   */
  uint8_t fm24clxx_test ( void );
#endif
  /**
   * @brief     init
   * @param[in] type is the chip type
   * @param[in] address is the chip address pin
   * @return    status code
   *            - 0 success
   *            - 1 init failed
   * @note      none
   */
  uint8_t fm24clxx_fram_init ( fm24clxx_t type, fm24clxx_address_t address );

  /**
   * @brief  deinit
   * @return status code
   *         - 0 success
   *         - 1 deinit failed
   * @note   none
   */
  uint8_t fm24clxx_fram_deinit ( void );

  /**
   * @brief      basic example read
   * @param[in]  address is the register address
   * @param[out] *buf points to a data buffer
   * @param[in]  len is the buffer length
   * @return     status code
   *             - 0 success
   *             - 1 read data failed
   * @note       none
   */
  uint8_t fm24clxx_fram_read ( uint16_t address, uint8_t* buf, uint16_t len );

  /**
   * @brief     basic example write
   * @param[in] address is the register address
   * @param[in] buf points to a data buffer
   * @param[in] len is the buffer length
   * @return    status code
   *            - 0 success
   *            - 1 write data failed
   * @note      none
   */
  uint8_t fm24clxx_fram_write ( uint16_t address, uint8_t* buf, uint16_t len );

#ifdef __cplusplus
}
#endif

#endif
