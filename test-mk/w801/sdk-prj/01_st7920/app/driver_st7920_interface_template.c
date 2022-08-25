/**
 *Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_st7920_interface_template.c
 * @brief     driver st7920 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-03-19
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/03/19  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_st7920_interface.h"

#include "n_delay.h"
#include "wm_type_def.h"
#include "wm_io.h"
#include "wm_gpio.h"


#define PIN_CS   WM_IO_PB_21
#define PIN_SCLK WM_IO_PB_22
#define PIN_SID  WM_IO_PB_23

//gpio pin: RS/RW/E/ PB1/PA8/PB0.     CS/SID/CLK


/**
 * @brief  interface cs gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t st7920_interface_cs_gpio_init(void)
{
    tls_gpio_cfg(PIN_CS, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    return 0;
}

/**
 * @brief  interface cs gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t st7920_interface_cs_gpio_deinit(void)
{
    tls_gpio_cfg(PIN_CS, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
    return 0;
}

/**
 * @brief     interface cs gpio write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t st7920_interface_cs_gpio_write(uint8_t value)
{
    //tls_gpio_cfg(PIN_CS, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    if (value)
    {
        tls_gpio_write(PIN_CS, 1);
    }
    else
    {
        tls_gpio_write(PIN_CS, 0);
    }
    //printf("*st7920_interface_cs_gpio_write(%d)", value);
    return 0;
}

/**
 * @brief  interface sclk gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t st7920_interface_sclk_gpio_init(void)
{
    tls_gpio_cfg(PIN_SCLK, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    return 0;
}

/**
 * @brief  interface sclk gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t st7920_interface_sclk_gpio_deinit(void)
{
    tls_gpio_cfg(PIN_SCLK, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
    return 0;
}

/**
 * @brief     interface sclk gpio write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t st7920_interface_sclk_gpio_write(uint8_t value)
{
    if (value)
    {
        tls_gpio_write(PIN_SCLK, 1);
    }
    else
    {
        tls_gpio_write(PIN_SCLK, 0);
    }
    //printf("*st7920_interface_sclk_gpio_write(%d)", value);
    return 0;
}

/**
 * @brief  interface sid gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t st7920_interface_sid_gpio_init(void)
{
    tls_gpio_cfg(PIN_SID, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_write(PIN_SID, 1);
    tls_gpio_cfg(PIN_SID, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_PULLHIGH);
    return 0;
}

/**
 * @brief  interface sid gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t st7920_interface_sid_gpio_deinit(void)
{
    tls_gpio_cfg(PIN_SID, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
    return 0;
}

/**
 * @brief     interface sid gpio write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t st7920_interface_sid_gpio_write(uint8_t value)
{
    tls_gpio_cfg(PIN_SID, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_write(PIN_SID, value);
    //printf("*st7920_interface_sid_gpio_write(%d)", value);
    return 0;
}


/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void st7920_interface_delay_ms(uint32_t ms)
{
    n_delay_ms(ms);
}

/**
 * @brief     interface delay us
 * @param[in] us
 * @note      none
 */
void st7920_interface_delay_us(uint32_t us)
{
    n_delay_us(us);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void st7920_interface_debug_print(const char *const fmt, ...)
{

    char str[256];
    //uint8_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 256, (char const *)fmt, args);
    va_end(args);
        
    //len = strlen((char *)str);
    //(void)uart1_write((uint8_t *)str, len);
    printf("%s", str);
  
}
