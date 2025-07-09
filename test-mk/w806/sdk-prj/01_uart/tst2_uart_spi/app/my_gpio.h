#ifndef MY_GPIO_H
#define MY_GPIO_H

#include "wm_io.h"
#include "wm_type_def.h"

u8 gpioStateFill (void);

void init_my_gpio (enum tls_io_name gpio_pin);

#endif /* end of MY_GPIO_H */
