/*

connect to (MODE_COMMON_CATHODE)
v003 5643AS-1 
---- ----- 
PA1  1  E
PD4  2  D
PD3  3  DP
PD2  4  C
PA2  5  G
PD0  6  Dig4
PC7  7  B
PC6  8  Dig3
PC5  9  Dig2
PC4  10 F
PC3  11 A
PC0  12 Dig1
---- ------ 

connect to (MODE_COMMON_ANODE)
v003 8041BS-1 
---- ----- 
PA1  1  E
PD4  2  D
PD3  3  DP
PD2  4  C
PA2  5  G
PD0  6  Dig4
PC7  7  B
PC6  8  Dig3
PC5  9  Dig2
PC4  10 F
PC3  11 A
PC0  12 Dig1
---- ------ 

connect to (MODE_COMMON_ANODE)
v003 8042BS-1F 
---- ----- 
PA1  1  E
PD4  2  D
     3  DP
PD3  8  D1+D2 CATHOD
PD2  4  C
PA2  5  G
PD0  6  Dig4
PC7  9  B
PC6  10 Dig3

PC5  11 Dig2
PC5  7  D1+D2 ANOD

PC4  12 F
PC3  13 A
PC0  14 Dig1
---- ------ 



*/

#ifndef LSD_5643AS_H
#define LSD_5643AS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "port.h"

#define MODE_CLOCK 0
#define MODE_WEATHER 1

#define MODE_COMMON_ANODE   1   // (CA)
#define MODE_COMMON_CATHODE 0   // (CC)

  void lcd5643_init_pin (u8 i_common_mode);

  extern u8 i_5643_hour;
  extern u8 i_5643_min;
  extern u8 i_5643_sec_state;
  extern u8 i_5643_Mode;
  extern int i_5643_t_sign;
  extern int i_5643_t_value;
  extern int i_5643_t_mantissa;

  extern u8 i_5643_out;
#define LCD_VAL_LG_spb_low 254
#define LCD_VAL_LG_low 200
#define LCD_VAL_LG_middle 50
#define LCD_VAL_LG_spb_hi 10
#define LCD_VAL_LG_hi 5
  extern u8 i_5643_max_out; //

  void lcd5643_update_disp (void); // здесь будет вывод на LCD

#ifdef __cplusplus
}
#endif

#endif /* LSD_5643AS_H */
