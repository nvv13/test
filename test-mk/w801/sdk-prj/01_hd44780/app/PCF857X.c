//#include "wm_type_def.h"
//#include "wm_io.h"

#include "wm_i2c.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#include "PCF857X.h"

/*
у PCF8574 и PCF8574A – разная адресация, которую, в дополнение, можно ещё и
менять тремя резисторами на плате.
Резисторы обеспечивают низкий уровень на линиях A0-A2, при их отсутствии
на линиях будет высокий уровень,
что даст для PCF8574A адрес 7Eh для записи и 7Fh для чтения.
Для PCF8574 же это будут адреса 4Eh для записи и 4Fh для чтения.

*/

#define PCF8575_BASE_ADDR                                                     \
  (0x20) /**< PCF8575 I2C slave base address.                                 \
              Addresses are then in range from                                \
              0x20 to 0x27 */
#define PCF8574_BASE_ADDR                                                     \
  (0x4e) /**< PCF8574 I2C slave base address.                                 \
              Addresses are then in range from                                \
              0x20 to 0x27 */
#define PCF8574A_BASE_ADDR                                                    \
  (0x38) /**< PCF8574A I2C slave base address.                                \
              Addresses are then in range from                                \
              0x38 to 0x3f */

#define PCF8575_GPIO_PIN_NUM (16) /**< PCF8575 has 16 I/O pins */
#define PCF8574_GPIO_PIN_NUM (8)  /**< PCF8574 has 8 I/O pins */
#define PCF8574A_GPIO_PIN_NUM (8) /**< PCF8574A has 8 I/O pins */

#define I2C_FREQ (100000) // I2C speed 100kHz

#define PCF857X_GPIO_PIN_NUM (8) /**< PCF8574, PCF8574 provide 8 I/O pins */
typedef uint8_t pcf857x_data_t;  /**< type that can mask all expander pins */

static u16 u16_addr;
static u8 u8_data;

void
PCF857X_Init (u32 u32_FREQ, enum tls_io_name pinSCL, enum tls_io_name pinSDA)
{
  wm_i2c_scl_config (pinSCL);
  wm_i2c_sda_config (pinSDA);
  tls_i2c_init (u32_FREQ);
  u16_addr = PCF8574_BASE_ADDR;
  u8_data = 0;
}

static void
WriteOneByte (u16 addr, u8 data)
{
  printf ("WriteOneByte addr=%d data=%x  %d\n", addr, data, data);

  //  tls_i2c_write_byte (0XA0, 1);
  //  tls_i2c_wait_ack ();
  // tls_i2c_write_byte ((addr<<1) | 0x01 , 1);
  tls_i2c_write_byte (addr, 1);
  tls_i2c_wait_ack ();
  tls_i2c_write_byte (data, 0);
  tls_i2c_wait_ack ();
  tls_i2c_stop ();
  tls_os_time_delay (1);
}

void
PCF857X_gpio_write (u8 num_pin, u8 value)
{
  if (value)
    u8_data = u8_data | (1 << num_pin);
  else
    u8_data = u8_data & (~(1 << num_pin));

  WriteOneByte (u16_addr, u8_data);
}

void
PCF857X_buf_write (u8 num_pin, u8 value)
{
  if (value)
    u8_data = u8_data | (1 << num_pin);
  else
    u8_data = u8_data & (~(1 << num_pin));
}

void
PCF857X_buf_to_gpio_write (void)
{
  WriteOneByte (u16_addr, u8_data);
}
