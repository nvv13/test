#include "wm_gpio.h"

#include "my_gpio.h"

#include "my_config.h"

static enum tls_io_name in_gpio_pin;

u8
gpioStateFill (void)
{
  return tls_gpio_read (in_gpio_pin);
}

void
init_my_gpio (enum tls_io_name gpio_pin)
{
  in_gpio_pin = gpio_pin;
  tls_gpio_cfg (in_gpio_pin, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
}