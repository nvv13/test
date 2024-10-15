#ifndef AT24C32_UTIL_H
#define AT24C32_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"
#include "wm_osal.h"

#include "mod1/at24cxxx.h"

#ifndef AT24CXXX_ERASE
#define AT24CXXX_ERASE (0)
#endif

#define AT24CXXX_EEPROM_SIZE (AT24C32_EEPROM_SIZE)
#define AT24CXXX_PAGE_SIZE (AT24C32_PAGE_SIZE)
#define AT24CXXX_MAX_POLLS (AT24C32_MAX_POLLS)
#define AT24CXXX_PIN_WP (GPIO_UNDEF)
#define AT24CXXX_ADDR (AT24CXXX_DEF_DEV_ADDR + 7)
  // 0xAE addr ACK found! = 0x57

  int at24c_ReadByte (const at24cxxx_t *dev, int iPosition, u8 *u8_value);

  int at24c_WriteByte (const at24cxxx_t *dev, int iPosition, u8 u8_value);

#ifdef __cplusplus
}
#endif

#endif /* AT24C32_UTIL_H */
