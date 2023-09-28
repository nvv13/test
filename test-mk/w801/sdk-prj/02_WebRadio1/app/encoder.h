/*

Взято из
https://gitee.com/beewu123/w80x-sdk-lcd.git
там Apache License  Version 2.0, January 2004
*/


#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "wm_type_def.h"

#include "wm_io.h"


typedef struct
 {
    enum tls_io_name ENCODER_S; 
    enum tls_io_name ENCODER_A; 
    enum tls_io_name ENCODER_B; 
 } libENCODER_t;


void bsp_encoder_init(libENCODER_t *set_pin);

bool get_encoder_btn_state();
int32_t get_encoder_diff();
void set_encoder_diff(int32_t val);



#endif