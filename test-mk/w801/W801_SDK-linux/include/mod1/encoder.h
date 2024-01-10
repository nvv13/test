/*

Взято из
https://gitee.com/beewu123/w80x-sdk-lcd.git
там Apache License  Version 2.0, January 2004
*/


#ifndef __ENCODER_H__
#define __ENCODER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "wm_type_def.h"

#include "wm_io.h"

#ifdef NO_GPIO_PIN
#undef NO_GPIO_PIN
#endif
#define NO_GPIO_PIN 255

typedef struct
 {
    enum tls_io_name ENCODER_S; 
    enum tls_io_name ENCODER_A; 
    enum tls_io_name ENCODER_B; 
    enum tls_io_name ADD_BUTTON_1; 
    enum tls_io_name ADD_BUTTON_2; 
    enum tls_io_name ADD_BUTTON_3; 
 } libENCODER_t;


void bsp_encoder_init(libENCODER_t *set_pin);

bool get_encoder_btn_state();
int32_t get_encoder_diff();
void set_encoder_diff(int32_t val);

bool get_add_btn_1_state();
bool get_add_btn_2_state();
bool get_add_btn_3_state();


#ifdef __cplusplus
}
#endif

#endif