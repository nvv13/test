/*

Взято из
https://github.com/RIOT-OS/RIOT

*/

/*
 * Copyright (C) 2019 Otto-von-Guericke-Universität Magdeburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_at24cxxx AT24CXXX EEPROM unit
 * @ingroup     drivers_misc
 * @brief       Device driver interface for the AT24CXXX EEPROM units
 *
 * @{
 *
 * @file
 * @brief       Device driver interface for AT24CXXX EEPROM units.
 *
 * @author      Fabian Hüßler <fabian.huessler@ovgu.de>
 *
 */

#ifndef AT24CXXX_H
#define AT24CXXX_H

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_i2c.h"
#include "wm_io.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef GPIO_UNDEF
#define GPIO_UNDEF -1
#endif

#ifndef gpio_is_valid
#define gpio_is_valid(pin) ((pin != GPIO_UNDEF))
#endif

/**
 * @brief   Delay between two poll attempts
 */
#define AT24CXXX_POLL_DELAY_US (1000UL)
/**
 * @brief   Clear byte
 */
#define AT24CXXX_CLEAR_BYTE (0x00)
/**
 * @brief   AT24CXXX default device address
 *
 * Different AT24CXXX types may have a different address byte
 * format. Some may include a portion of the data word address.
 * Some may have a 0, 1, 2 or 3 bit wide address space.
 * But all types have a 7 bit I2C address which starts with
 * 1010. [1, 0, 1, 0, ?, ?, ?, r/w]
 *        \__7 bit address__/
 */
#define AT24CXXX_DEF_DEV_ADDR (0x50)

/**
 * @name AT24C01A constants
 * @{
 */
/**
 * @brief   128 byte memory
 */
#define AT24C01A_EEPROM_SIZE (128U)
/**
 * @brief   16 pages of 8 bytes each
 */
#define AT24C01A_PAGE_SIZE (8U)
/**
 * @brief Delay to complete write operation
 */
#define AT24C01A_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief Number of poll attempts
 */
#define AT24C01A_MAX_POLLS                                                    \
  (1 + (AT24C01A_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C02 constants
 * @{
 */
/**
 * @brief   256 byte memory
 */
#define AT24C02_EEPROM_SIZE (256U)
/**
 * @brief   32 pages of 8 bytes
 */
#define AT24C02_PAGE_SIZE (8U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C02_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C02_MAX_POLLS                                                     \
  (1 + (AT24C02_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C04 constants
 * @{
 */
/**
 * @brief   512 byte memory
 */
#define AT24C04_EEPROM_SIZE (512U)
/**
 * @brief   32 pages of 16 bytes each
 */
#define AT24C04_PAGE_SIZE (16U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C04_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C04_MAX_POLLS                                                     \
  (1 + (AT24C04_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C08A constants
 * @{
 */
/**
 * @brief   1 kiB memory
 */
#define AT24C08A_EEPROM_SIZE (1024U)
/**
 * @brief   64 pages of 16 bytes each
 */
#define AT24C08A_PAGE_SIZE (16U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C08A_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief Number of poll attempts
 */
#define AT24C08A_MAX_POLLS                                                    \
  (1 + (AT24C08A_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C16A constants
 * @{
 */
/**
 * @brief 2 kiB memory
 */
#define AT24C16A_EEPROM_SIZE (2048U)
/**
 * @brief   128 pages of 16 bytes each
 */
#define AT24C16A_PAGE_SIZE (16U)
/**
 * @brief Delay to complete write operation
 */
#define AT24C16A_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief Number of poll attempts
 */
#define AT24C16A_MAX_POLLS                                                    \
  (1 + (AT24C16A_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C32 constants
 * @{
 */
/**
 * @brief   4 kiB memory
 */
#define AT24C32_EEPROM_SIZE (4096U)
/**
 * @brief   128 pages of 32 bytes each
 */
#define AT24C32_PAGE_SIZE (32U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C32_PAGE_WRITE_DELAY_US (10000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C32_MAX_POLLS                                                     \
  (1 + (AT24C32_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C64 constants
 * @{
 */
/**
 * @brief 8 kiB memory
 */
#define AT24C64_EEPROM_SIZE (8192U)
/**
 * @brief   256 pages of 32 bytes each
 */
#define AT24C64_PAGE_SIZE (32U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C64_PAGE_WRITE_DELAY_US (10000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C64_MAX_POLLS                                                     \
  (1 + (AT24C64_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C128 constants
 * @{
 */
/**
 * @brief 16 kiB memory
 */
#define AT24C128_EEPROM_SIZE (16384U)
/**
 * @brief   256 pages of 64 bytes each
 */
#define AT24C128_PAGE_SIZE (64U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C128_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C128_MAX_POLLS                                                    \
  (1 + (AT24C128_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C256 constants
 * @{
 */
/**
 * @brief 32 kiB memory
 */
#define AT24C256_EEPROM_SIZE (32768U)
/**
 * @brief   512 pages of 64 bytes each
 */
#define AT24C256_PAGE_SIZE (64U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C256_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C256_MAX_POLLS                                                    \
  (1 + (AT24C256_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C512 constants
 * @{
 */
/**
 * @brief 64 kiB memory
 */
#define AT24C512_EEPROM_SIZE (65536U)
/**
 * @brief   512 pages of 128 bytes each
 */
#define AT24C512_PAGE_SIZE (128U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C512_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C512_MAX_POLLS                                                    \
  (1 + (AT24C512_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24C1024 constants
 * @{
 */
/**
 * @brief 128 kiB memory
 */
#define AT24C1024_EEPROM_SIZE (131072U)
/**
 * @brief   512 pages of 256 bytes each
 */
#define AT24C1024_PAGE_SIZE (256U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24C1024_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24C1024_MAX_POLLS                                                   \
  (1 + (AT24C1024_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
/** @} */

/**
 * @name AT24MAC402/602 constants
 * @{
 */
/**
 * @brief   256 byte memory
 */
#define AT24MAC_EEPROM_SIZE (256U)
/**
 * @brief   16 pages of 16 bytes each
 */
#define AT24MAC_PAGE_SIZE (16U)
/**
 * @brief   Delay to complete write operation
 */
#define AT24MAC_PAGE_WRITE_DELAY_US (5000U)
/**
 * @brief   Number of poll attempts
 */
#define AT24MAC_MAX_POLLS                                                     \
  (1 + (AT24MAC_PAGE_WRITE_DELAY_US / AT24CXXX_POLL_DELAY_US))
  /** @} */

  /**
   * @name Set constants depending on module
   * @{
   */
  /*
  #if IS_USED(MODULE_AT24C1024)
  #define AT24CXXX_EEPROM_SIZE            (AT24C1024_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C1024_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C1024_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C512)
  #define AT24CXXX_EEPROM_SIZE            (AT24C512_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C512_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C512_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C256)
  #define AT24CXXX_EEPROM_SIZE            (AT24C256_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C256_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C256_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C128)
  #define AT24CXXX_EEPROM_SIZE            (AT24C128_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C128_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C128_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C64)
  #define AT24CXXX_EEPROM_SIZE            (AT24C64_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C64_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C64_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C32)
  #define AT24CXXX_EEPROM_SIZE            (AT24C32_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C32_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C32_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C16A)
  #define AT24CXXX_EEPROM_SIZE            (AT24C16A_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C16A_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C16A_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C08A)
  #define AT24CXXX_EEPROM_SIZE            (AT24C08A_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C08A_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C08A_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C04)
  #define AT24CXXX_EEPROM_SIZE            (AT24C04_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C04_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C04_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C02)
  #define AT24CXXX_EEPROM_SIZE            (AT24C02_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C02_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C02_MAX_POLLS)
  #elif IS_USED(MODULE_AT24C01A)
  #define AT24CXXX_EEPROM_SIZE            (AT24C01A_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24C01A_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24C01A_MAX_POLLS)
  #elif IS_USED(MODULE_AT24MAC)
  #define AT24CXXX_EEPROM_SIZE            (AT24MAC_EEPROM_SIZE)
  #define AT24CXXX_PAGE_SIZE              (AT24MAC_PAGE_SIZE)
  #define AT24CXXX_MAX_POLLS              (AT24MAC_MAX_POLLS)
  #else */
  /* minimal */
  //#define AT24CXXX_EEPROM_SIZE (128U) /**< EEPROM size */
  //#define AT24CXXX_PAGE_SIZE (4U) /**< page size */
  //#define AT24CXXX_MAX_POLLS (6U) /**< maximum poll attempts */
  //#endif
  /** @} */

  /**
   * @brief   Return values
   */
  enum
  {
    AT24CXXX_OK,
    AT24CXXX_I2C_ERROR
  };

#include "mod1/Wire.h"

  /**
   * @brief   Struct that holds initialization parameters
   */
  typedef struct at24cxxx_params
  {
    i2c_t i2c;
    enum tls_io_name pin_wp;  /**< write protect pin */
    uint32_t eeprom_size;     /**< EEPROM memory capacity */
    uint8_t dev_addr;         /**< I2C device address */
    uint8_t page_size;        /**< page size */
    uint8_t max_polls;        /**< number of ACK poll attempts */
  } at24cxxx_params_t;

  /**
   * @brief   Struct that represents an AT24CXXX device
   */
  typedef struct
  {
    at24cxxx_params_t params; /**< parameters */
  } at24cxxx_t;

  /**
   * @brief   Initialize an AT24CXXX device handle with AT24CXXX parameters
   *
   * @param[in, out]  dev     AT24CXXX device handle
   * @param[in]       params  AT24CXXX parameters
   *
   * @return          AT24CXXX_OK on success
   * @return          -AT24CXXX_I2C_ERROR if i2c could not be acquired
   * @return          -EINVAL if input parameters are NULL
   */
  int at24cxxx_init (at24cxxx_t *dev, const at24cxxx_params_t *params);

  /**
   * @brief   Read a byte at a given position @p pos
   *
   * @param[in]       dev      AT24CXXX device handle
   * @param[in]       pos      Position in EEPROM memory
   * @param[out]      dest     Read byte
   *
   * @return          AT24CXXX_OK on success
   * @return          -ERANGE if @p pos is out of bounds
   * @return          @see i2c_read_regs
   */
  int at24cxxx_read_byte (const at24cxxx_t *dev, uint32_t pos, void *dest);

  /**
   * @brief Sequentially read @p len bytes from a given position @p pos
   *
   * @param[in]       dev     AT24CXXX device handle
   * @param[in]       pos     Position in EEPROM memory
   * @param[out]      data    Read buffer
   * @param[in]       len     Requested length to be read
   *
   * @return          AT24CXXX_OK on success
   * @return          -ERANGE if @p pos + @p len is out of bounds
   * @return          @see i2c_read_regs
   */
  int at24cxxx_read (const at24cxxx_t *dev, uint32_t pos, void *data,
                     size_t len);

  /**
   * @brief   Write a byte at a given position @p pos
   *
   * @param[in]       dev      AT24CXXX device handle
   * @param[in]       pos      Position in EEPROM memory
   * @param[in]       data     Value to be written
   *
   * @return          AT24CXXX_OK on success
   * @return          -ERANGE if @p pos is out of bounds
   * @return          @see i2c_write_regs
   */
  int at24cxxx_write_byte (const at24cxxx_t *dev, uint32_t pos, uint8_t data);

  /**
   * @brief   Sequentially write @p len bytes from a given position @p pos
   *
   * Writing is performed in chunks of size AT24CXXX_PAGE_SIZE.
   *
   * @param[in]       dev       AT24CXXX device handle
   * @param[in]       pos       Position in EEPROM memory
   * @param[in]       data      Write buffer
   * @param[in]       len       Requested length to be written
   *
   * @return          AT24CXXX_OK on success
   * @return          -ERANGE if @p pos + @p len is out of bounds
   * @return          @see i2c_write_regs
   */
  int at24cxxx_write (const at24cxxx_t *dev, uint32_t pos, const void *data,
                      size_t len);

  /**
   * @brief   Set @p len bytes from a given position @p pos to the
   * value @p val
   *
   * Writing is performed in chunks of size AT24CXXX_SET_BUFFER_SIZE.
   *
   * @param[in]       dev       AT24CXXX device handle
   * @param[in]       pos       Position in EEPROM memory
   * @param[in]       val       Value to be set
   * @param[in]       len       Requested length to be written
   *
   * @return          AT24CXXX_OK on success
   * @return          -ERANGE if @p pos + @p len is out of bounds
   * @return          @see i2c_write_byte
   */
  int at24cxxx_set (const at24cxxx_t *dev, uint32_t pos, uint8_t val,
                    size_t len);

  /**
   * @brief Set @p len bytes from position @p pos to
   * AT24CXXX_CLEAR_BYTE
   *
   * This is a wrapper around @see at24cxxx_set.
   *
   * @param[in]       dev       AT24CXXX device handle
   * @param[in]       pos       Position in EEPROM memory
   * @param[in]       len       Requested length to be written
   *
   * @return          @see at24cxxx_set
   */
  int at24cxxx_clear (const at24cxxx_t *dev, uint32_t pos, size_t len);

  /**
   * @brief Set the entire EEPROM memory to AT24CXXX_CLEAR_BYTE
   *
   * This is a wrapper around @see at24cxxx_clear.
   *
   * @param[in]       dev       AT24CXXX device handle
   *
   * @return          @see at24cxxx_set
   */
  int at24cxxx_erase (const at24cxxx_t *dev);

  /**
   * @brief Enable write protection
   *
   * @param[in]       dev       AT24CXXX device handle
   *
   * @return          AT24CXXX_OK on success
   * @return          -ENOTSUP if pin_wp was initialized with GPIO_UNDEF
   */
  int at24cxxx_enable_write_protect (const at24cxxx_t *dev);

  /**
   * @brief Disable write protection
   *
   * @param[in]       dev       AT24CXXX device handle
   *
   * @return          AT24CXXX_OK on success
   * @return          -ENOTSUP if pin_wp was initialized with GPIO_UNDEF
   */
  int at24cxxx_disable_write_protect (const at24cxxx_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* AT24CXXX_H */
/** @} */
