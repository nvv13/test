#ifndef LCD_PARAM_H
#define LCD_PARAM_H

#define MODULE_PCF8574 1
#define MODULE_PCF857X 1

#define PCF857X_PARAM_ADDR	(7)
#define PCF857X_PARAM_DEV       I2C_DEV(0)

// 4*20  2004
#define HD44780_PARAM_COLS              (20U)
#define HD44780_PARAM_ROWS              (4U)

// 2*16  1602
//#define HD44780_PARAM_COLS              (16U)
//#define HD44780_PARAM_ROWS              (2U)

#endif