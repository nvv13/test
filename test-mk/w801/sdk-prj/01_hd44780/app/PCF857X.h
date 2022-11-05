/*
 * Copyright (C) 2018 Gunar Schorcht
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef __PCF857X__
#define __PCF857X__

#include "wm_io.h"
#include "wm_type_def.h"

void PCF857X_Init (u32 u32_FREQ, enum tls_io_name pinSCL,
                   enum tls_io_name pinSDA);
void PCF857X_gpio_write (u8 num_pin, u8 value);

void PCF857X_buf_write (u8 num_pin, u8 value);
void PCF857X_buf_to_gpio_write (void);

#endif // __PCF857X__
