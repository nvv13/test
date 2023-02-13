#ifndef HW_SKY_DEFINES_H
#define HW_SKY_DEFINES_H



//********************************************************************



// *** Hardwarespecific defines ***
//#define cbi(reg, bitmask) *reg &= ~bitmask
//#define sbi(reg, bitmask) *reg |= bitmask
#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

//#define cport(port, data) port &= data
//#define sport(port, data) port |= data

#define swap_word(i, j) {word t = i; i = j; j = t;}
#define swap_int(i, j) {int t = i; i = j; j = t;}

#define fontbyte(x) cfont.font[x]  

#define pgm_read_word(data) *data
#define pgm_read_byte(data) *data
#define bitmapdatatype unsigned short*

//#define regtype volatile uint32_t
#define regtype uint32_t
#define regsize u8

//!ВНИМАНИЕ! PIN0 - PIN15, использовать только PB порт GPIO !ВНИМАНИЕ!
#define PIN0 WM_IO_PB_21
#define PIN1 WM_IO_PB_22
#define PIN2 WM_IO_PB_23
#define PIN3 WM_IO_PB_24
#define PIN4 WM_IO_PB_25
#define PIN5 WM_IO_PB_26
#define PIN6 WM_IO_PB_18
#define PIN7 WM_IO_PB_17

#define PIN8  WM_IO_PB_16
#define PIN9  WM_IO_PB_01  // WM_IO_PB_11 Освободил контакты для SD Card
#define PIN10 WM_IO_PB_02  // WM_IO_PB_10 Освободил контакты для SD Card
#define PIN11 WM_IO_PB_15
#define PIN12 WM_IO_PB_14
#define PIN13 WM_IO_PB_13
#define PIN14 WM_IO_PB_12
#define PIN15 WM_IO_PB_00  // WM_IO_PB_09 Освободил контакты для SD Card
//!ВНИМАНИЕ! PIN0 - PIN15, использовать только PB порт GPIO !ВНИМАНИЕ!


#endif /* HW_SKY_DEFINES_H */

