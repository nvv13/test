/***************************************************************************** 
* 
* File Name : main.c
* 
* Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
* 
* Date : 2022-05-20
*****************************************************************************/ 


#include <stdio.h>
#include "wm_type_def.h"
#include "wm_cpu.h"
#include "wm_regs.h"
#include "wm_gpio.h"
#include "wm_osal.h"

#include "csi_core.h"
extern uint32_t csi_coret_get_load(void);
extern uint32_t csi_coret_get_value(void);

/*
static void my_mdelay(void)
{
    uint32_t load = csi_coret_get_load();
    uint32_t start = csi_coret_get_value();
    uint32_t cur;
    uint32_t cnt;
    tls_sys_clk sysclk;

    tls_sys_clk_get(&sysclk);
    cnt = sysclk.cpuclk;// * 1000;

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

void mymdelay(uint32_t ms)
{
    if (ms == 0) {
        return;
    }

    while (ms--) {
        my_mdelay();
    }
}

*/

static void tic_delay(uint32_t cnt)
{
    if (cnt == 0) {
        return;
    }
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



void UserMain(void)
{
        enum tls_io_name gpio_pin=WM_IO_PB_01;

	printf("user task\n");

	tls_sys_clk_set(CPU_CLK_240M); // freq = 5,714309 MHz
	//tls_sys_clk_set(CPU_CLK_80M);//   freq = 3,636382 MHz
	
	tls_gpio_cfg(gpio_pin, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);

	tls_sys_clk sysclk;
	tls_sys_clk_get(&sysclk);

	printf("  sysclk.apbclk %d\n",sysclk.apbclk);
	printf("  sysclk.apbclk*UNIT_MHZ %d\n",sysclk.apbclk*UNIT_MHZ);

	printf("  sysclk.cpuclk %d\n",sysclk.cpuclk);
	printf("  sysclk.cpuclk*UNIT_MHZ %d\n",sysclk.cpuclk*UNIT_MHZ);


	u8 u8_led_state=0;


	u32 cpu_sr = 0;
	u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (gpio_pin >= WM_IO_PB_00)
        {
           pin    = gpio_pin - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = gpio_pin;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin));

	reg = tls_reg_read32(HR_GPIO_DATA + offset);
	while(1)
	{
	if(u8_led_state)
		tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));/* write low */
        	else
		tls_reg_write32(HR_GPIO_DATA + offset, reg |  (1 << pin));	/* write high */
//	    tls_gpio_write(gpio_pin, u8_led_state);	// freq 577 KHz CPU_CLK_240M
	    //tls_os_time_delay(0); // freq = 2.000 MHz
	    //tls_os_time_delay(1); // freq = 377.360 KHz CPU_CLK_240M
            // mdelay(1); // uint32_t ms freq 499 Hz
            //mymdelay(1);// uint32_t ms freq 400 KHz
            //tic_delay(1);// freg 3.636 MHz CPU_CLK_240M
            //tic_delay(2);// freg 3.333 MHz CPU_CLK_240M
            //tic_delay(10);// freg 2.500 MHz CPU_CLK_240M
            //tic_delay(20);// freg 2.222 MHz CPU_CLK_240M
            //tic_delay(50);// freg 1.428573  MHz CPU_CLK_240M
     //    tic_delay(60);// freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
            //tic_delay(100);// freg 869.560 KHz CPU_CLK_240M
            //tic_delay(130);// freg 714.283  KHz CPU_CLK_240M
            //tic_delay(140);// freg 689.648  KHz CPU_CLK_240M
            //tic_delay(160);// freg 606.054  KHz CPU_CLK_240M     half period 0.8 us
            //tic_delay(170);// freg 588.233  KHz CPU_CLK_240M     half period 0.83 us
            tic_delay(173);// freg 571.420  KHz CPU_CLK_240M     half period 0.85 us
            //tic_delay(180);// freg 555.549  KHz CPU_CLK_240M     half 0.88 period us

	    u8_led_state=~u8_led_state;
	}

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en);

	tls_os_release_critical(cpu_sr); // enable Interrupt



}
