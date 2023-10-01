/*

Взято из
https://gitee.com/beewu123/w80x-sdk-lcd.git
там Apache License  Version 2.0, January 2004
*/

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>


#include "encoder.h"


#include "wm_gpio.h"


static int32_t encoder_diff=0;
static bool encoder_btn_state=1;
static u8 last_val = 255;
static libENCODER_t *Fset_pin;

bool get_encoder_btn_state()
{
    return encoder_btn_state;
}

int32_t get_encoder_diff()
{
    return encoder_diff;
}

void set_encoder_diff(int32_t val)
{
    encoder_diff = val;
}

static void btn_isr_callback(void *context)
{
	u16 ret;
	ret = tls_get_gpio_irq_status(Fset_pin->ENCODER_S);
	if(ret)
	{
		tls_clr_gpio_irq_status(Fset_pin->ENCODER_S);
		encoder_btn_state = tls_gpio_read(Fset_pin->ENCODER_S);
        // printf("---> btn: %d\n", encoder_btn_state);
	}
}

static void encoder_isr_callback(void *context)
{
    // printf("--->:%d %s\r\n", __LINE__, __func__);
    s8 flag_a = tls_get_gpio_irq_status(Fset_pin->ENCODER_A);
    s8 flag_b = tls_get_gpio_irq_status(Fset_pin->ENCODER_B);
    bool a = 0;
    bool b = 0;
    u8 val = 0;

    if(flag_a || flag_b)
    {
        if(flag_a) {
            tls_clr_gpio_irq_status(Fset_pin->ENCODER_A);
        }
        if(flag_b) {
            tls_clr_gpio_irq_status(Fset_pin->ENCODER_B);
        }

        a = tls_gpio_read(Fset_pin->ENCODER_A);
        b = tls_gpio_read(Fset_pin->ENCODER_B);

        val = (a<<1)+b;
        // printf("---> %d\n",val);
        if(last_val == 0)
        {
            if(val == 1){
                encoder_diff++;
                // printf("111111111111\n");
            }
            if(val == 2){
                encoder_diff--;
                // printf("222222222222\n");
            }
        }
        last_val = val;
    }
}

void bsp_encoder_init(libENCODER_t *set_pin)
{
    Fset_pin=set_pin;

    tls_gpio_cfg(Fset_pin->ENCODER_A, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
    tls_gpio_isr_register(Fset_pin->ENCODER_A, encoder_isr_callback, NULL);
    tls_gpio_irq_enable(Fset_pin->ENCODER_A, WM_GPIO_IRQ_TRIG_DOUBLE_EDGE);

    tls_gpio_cfg(Fset_pin->ENCODER_B, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
    tls_gpio_isr_register(Fset_pin->ENCODER_B, encoder_isr_callback, NULL);
    tls_gpio_irq_enable(Fset_pin->ENCODER_B, WM_GPIO_IRQ_TRIG_DOUBLE_EDGE);

    tls_gpio_cfg(Fset_pin->ENCODER_S, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
    tls_gpio_isr_register(Fset_pin->ENCODER_S, btn_isr_callback, NULL);
    tls_gpio_irq_enable(Fset_pin->ENCODER_S, WM_GPIO_IRQ_TRIG_DOUBLE_EDGE);

}

