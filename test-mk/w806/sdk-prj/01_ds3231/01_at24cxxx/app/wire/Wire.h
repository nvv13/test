/*

Взято из
https://github.com/RIOT-OS/RIOT

*/


#ifndef WIRE_H
#define WIRE_H

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    I2C_SPEED_LOW = 10000,            /**< low speed mode: ~10kbit/s */
    I2C_SPEED_NORMAL = 100000,        /**< normal mode: ~100kbit/s */
    I2C_SPEED_FAST = 400000,          /**< fast mode: ~400kbit/sj */
   // не для w801 I2C_SPEED_FAST_PLUS = 1000000,    /**< fast plus mode: ~1Mbit/s */
   // не для w801 I2C_SPEED_HIGH = 3400000,         /**< high speed mode: ~3.4Mbit/s */
} i2c_speed_t;

typedef enum {
    I2C_ADDR10  = 0x01,     /**< use 10-bit device addressing */
    I2C_REG16   = 0x02,     /**< use 16-bit register addressing, big-endian */
    I2C_NOSTOP  = 0x04,     /**< do not issue a STOP condition after transfer */
    I2C_NOSTART = 0x08,     /**< skip START sequence, ignores address field */
} i2c_flags_t;

typedef struct {
    enum tls_io_name i2c_scl; /* WM_IO_PA_01 or WM_IO_PB_20 */
    enum tls_io_name i2c_sda; /* WM_IO_PA_04 or WM_IO_PB_19 */
    u32 i2c_freq; /* частота i2c в герцах, если = 0 - то софтверный вариант, любые gpio можно использовать */
    bool hw_wire;
} i2c_param_t;

typedef i2c_param_t* i2c_t;
/**
 * @brief   Initialize the given I2C bus
 *
 * The given I2C device will be initialized with the parameters as specified in
 * the boards periph_conf.h, using the pins and the speed value given there.
 *
 * The bus MUST not be acquired before initializing it, as this is handled
 * internally by the i2c_init function!
 *
 * @param[in] dev       the device to initialize
 */
void i2c_init(i2c_t dev);

/**
 * @brief   Get mutually exclusive access to the given I2C bus
 *
 * In case the I2C device is busy, this function will block until the bus is
 * free again.
 *
 * @pre     Given device is valid, otherwise an assertion blows up
 *          (if assertions are enabled).
 *
 * @param[in] dev           I2C device to access
 */
void i2c_acquire(i2c_t dev);

/**
 * @brief   Release the given I2C device to be used by others
 *
 * @param[in] dev           I2C device to release
 */
void i2c_release(i2c_t dev);

/**
 * @brief   Convenience function for reading one byte from a given register
 *          address
 *
 * @note    This function is using a repeated start sequence for reading from
 *          the specified register address.
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in]  dev          I2C peripheral device
 * @param[in]  reg          register address to read from (8- or 16-bit,
 *                          right-aligned)
 * @param[in]  addr         7-bit or 10-bit device address (right-aligned)
 * @param[out] data         memory location to store received data
 * @param[in]  flags        optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */

int i2c_read_reg(i2c_t dev, uint16_t addr, uint16_t reg,
                 void *data, uint8_t flags);

/**
 * @brief   Convenience function for reading several bytes from a given
 *          register address
 *
 * @note    This function is using a repeated start sequence for reading from
 *          the specified register address.
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in]  dev          I2C peripheral device
 * @param[in]  reg          register address to read from (8- or 16-bit,
 *                          right-aligned)
 * @param[in]  addr         7-bit or 10-bit device address (right-aligned)
 * @param[out] data         memory location to store received data
 * @param[in]  len          the number of bytes to read into @p data
 * @param[in]  flags        optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */
int i2c_read_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  void *data, size_t len, uint8_t flags);

/**
 * @brief   Convenience function for reading one byte from a device
 *
 * @note    This function is using a repeated start sequence for reading from
 *          the specified register address.
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in]  dev          I2C peripheral device
 * @param[in]  addr         7-bit or 10-bit device address (right-aligned)
 * @param[out] data         memory location to store received data
 * @param[in]  flags        optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */

int i2c_read_byte(i2c_t dev, uint16_t addr, void *data, uint8_t flags);

/**
 * @brief   Convenience function for reading bytes from a device
 *
 * @note    This function is using a repeated start sequence for reading from
 *          the specified register address.
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in]  dev          I2C peripheral device
 * @param[in]  addr         7-bit or 10-bit device address (right-aligned)
 * @param[out] data         memory location to store received data
 * @param[in]  len          the number of bytes to read into @p data
 * @param[in]  flags        optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */

int i2c_read_bytes(i2c_t dev, uint16_t addr,
                   void *data, size_t len, uint8_t flags);

/**
 * @brief   Convenience function for writing a single byte onto the bus
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in] dev           I2C peripheral device
 * @param[in] addr          7-bit or 10-bit device address (right-aligned)
 * @param[in] data          byte to write to the device
 * @param[in] flags         optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */
int i2c_write_byte(i2c_t dev, uint16_t addr, uint8_t data, uint8_t flags);

/**
 * @brief   Convenience function for writing several bytes onto the bus
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in] dev           I2C peripheral device
 * @param[in] addr          7-bit or 10-bit device address (right-aligned)
 * @param[in] data          array holding the bytes to write to the device
 * @param[in] len           the number of bytes to write
 * @param[in] flags         optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */
int i2c_write_bytes(i2c_t dev, uint16_t addr, const void *data,
                    size_t len, uint8_t flags);

/**
 * @brief   Convenience function for writing one byte to a given
 *          register address
 *
 * @note    This function is using a continuous sequence for writing to the
 *          specified register address. It first writes the register then data.
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in]  dev          I2C peripheral device
 * @param[in]  reg          register address to read from (8- or 16-bit,
 *                          right-aligned)
 * @param[in]  addr         7-bit or 10-bit device address (right-aligned)
 * @param[in]  data         byte to write
 * @param[in]  flags        optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */
int i2c_write_reg(i2c_t dev, uint16_t addr, uint16_t reg,
                  uint8_t data, uint8_t flags);

/**
 * @brief   Convenience function for writing data to a given register address
 *
 * @note    This function is using a continuous sequence for writing to the
 *          specified register address. It first writes the register then data.
 *
 * @pre     i2c_acquire must be called before accessing the bus
 *
 * @param[in]  dev          I2C peripheral device
 * @param[in]  reg          register address to read from (8- or 16-bit,
 *                          right-aligned)
 * @param[in]  addr         7-bit or 10-bit device address (right-aligned)
 * @param[out] data         memory location to store received data
 * @param[in]  len          the number of bytes to write
 * @param[in]  flags        optional flags (see @ref i2c_flags_t)
 *
 * @return                  0 When success
 * @return                  -EIO When slave device doesn't ACK the byte
 * @return                  -ENXIO When no devices respond on the address sent on the bus
 * @return                  -ETIMEDOUT  When timeout occurs before device's response
 * @return                  -EINVAL When an invalid argument is given
 * @return                  -EOPNOTSUPP When MCU driver doesn't support the flag operation
 * @return                  -EAGAIN When a lost bus arbitration occurs
 */
int i2c_write_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  const void *data, size_t len, uint8_t flags);



#ifdef __cplusplus
}
#endif

#endif /* WIRE_H */
