/**
 * @ingroup     drivers_ws2812b
 * @{
 *
 * @file
 * @brief       WS2812B RGB LED driver implementation
 *
 *
 * @}
 */

#include <string.h>

#include "assert.h"

#include "wm_type_def.h"
#include "wm_cpu.h"
#include "wm_regs.h"
#include "wm_gpio.h"
#include "wm_osal.h"

#include "csi_core.h"
extern uint32_t csi_coret_get_load(void);
extern uint32_t csi_coret_get_value(void);


#include "ws2812b.h"

#define BLUE            (0xff0000)
#define GREEN           (0x00ff00)
#define RED             (0x0000ff)
#define BLUE_SHIFT      (16U)
#define GREEN_SHIFT     (8U)


static void tic_delay(uint32_t cnt)
{
    if (cnt == 0) {
        return;
    }
    // нашел данный алгоритм в исходниках SDK, там все что может понадобиться - есть
    uint32_t load = csi_coret_get_load();
    uint32_t start = csi_coret_get_value();
    uint32_t cur;

    while (1) {
        cur = csi_coret_get_value();

        if (start > cur) {
            if (start - cur >= cnt) {
                return;
            }
        } else {
            if (load - cur + start > cnt) {
                return;
            }
        }
    }
}


/*
Data transfer time( TH+TL=1.25 μs ±600n s )

0:
T0H 0 code ,high voltage time 0.4us ±150ns
T0L 0 code , low voltage time 0.85us ±150ns

1:
T1H 1 code ,high voltage time 0.85us ±150ns
T1L 1 code ,low voltage time 0.4us ±150ns

RES low voltage time Above 50μs
*/

static inline void shift(const u16 offset, const u32 reg, const u8 pin, uint32_t data)
{
    for (int i = 23; i >= 0; i--) {

        if( ((data >> i) & 0x01) )
        {//1
	tls_reg_write32(HR_GPIO_DATA + offset, reg |  (1 << pin));	/* write high */
        tic_delay(170);// freg 571.420  KHz CPU_CLK_240M     half period 0.85 us, + - значения чуть подравлены с учетом операторов в цикле
        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));/* write low */
        tic_delay(61);// freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
        }
        else
        {//0
	tls_reg_write32(HR_GPIO_DATA + offset, reg |  (1 << pin));	/* write high */
        tic_delay(60);// freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
	tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));/* write low */
        tic_delay(163);// freg 571.420  KHz CPU_CLK_240M     half period 0.85 us
        }
    }
}

void ws2812b_init(ws2812b_t *dev, const ws2812b_params_t *params)
{
    assert(dev && params);

    *dev = *params;

    tls_gpio_cfg(dev->data_pin, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
    tls_gpio_write(dev->data_pin, 0);	
    

}

void ws2812b_load_rgba(const ws2812b_t *dev, const color_rgba_t vals[])
{
    assert(dev && vals);

	tls_sys_clk sysclk;
	tls_sys_clk_get(&sysclk);
        if(sysclk.cpuclk!=240)
         {
         tls_sys_clk_set(CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано
         }

	u32 cpu_sr = 0;
	u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (dev->data_pin >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and PB (GPIOB - 32 bit), max power 12ma
        {
          pin    = dev->data_pin - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = dev->data_pin;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin)); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

    for (int i = 0; i < dev->led_numof; i++) {
        uint32_t data = 0;//HEAD;
        /* we scale the 8-bit alpha value to a 5-bit value by cutting off the
         * 3 leas significant bits */
        data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)<< BLUE_SHIFT);
        data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha )<< GREEN_SHIFT);
        data |= vals[i].color.r & (uint32_t)vals[i].alpha;
        shift(offset, reg, pin, data);
    }

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));/* write low from pin */

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

    // RES above 50μs
    tic_delay(13000);

    switch(sysclk.cpuclk) // восстанавливаем частоту
    {
     case 2:
       {
       tls_sys_clk_set(CPU_CLK_2M); 
       };break;
     case 40:
       {
       tls_sys_clk_set(CPU_CLK_40M); 
       };break;
     case 80:
       {
       tls_sys_clk_set(CPU_CLK_80M); 
       };break;
     case 160:
       {
       tls_sys_clk_set(CPU_CLK_160M); 
       };break;
    }


}
