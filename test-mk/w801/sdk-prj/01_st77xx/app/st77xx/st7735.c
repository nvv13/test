//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------

//#include <stm32f10x_gpio.h>
//#include <stm32f10x_rcc.h>

#include <delay.h>
#include <dispcolor.h>
#include <st77xx.h>
#include <st7735.h>
#include <gpio.h>
#include <spim.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


//==============================================================================
// Процедура инициализации дисплея
//==============================================================================
void ST7735_Init(uint8_t Width, uint8_t Height)
{
  ST77xx_Width = Width;
  ST77xx_Height = Height;
  ST77xx_X_Start = ST7735_X_Start;
  ST77xx_Y_Start = ST7735_Y_Start;

  // Инициализация ножек интерфейса управления дисплеем
  ST77xx_GPIO_init();
  // Инициализация интерфейса SPI
  spim_init(ST77xx_SPI_periph, 1, 8);
  
  // Задержка после подачи питания
  delay_ms(40);
 
  // Сброс дисплея
  ST77xx_HardReset();
  // Отправляем последовательность инициализирующих команд
  ST77xx_SoftReset();

  ST77xx_SleepMode(0);

//ST77xx_SendCmd(0xfe); //
//ST77xx_SendCmd(0xef); // | Переключаем разрешение
//ST77xx_SendCmd(0xb6);ST77xx_SendData(0x11); // | экрана 80x160
//ST77xx_SendCmd(0xac);ST77xx_SendData(0x0b); ///


  ST77xx_ColorModeSet(ST7735_ColorMode_16bit);
  delay_ms(500);

  ST77xx_MemAccessModeSet(0, 1, 0, 1);
  delay_ms(10);


  ST77xx_InversionMode(1);
  delay_ms(10);

  ST77xx_FillScreen(0);

/*
#define ST77XX_MADCTL 0x36
  ST77xx_SendCmd(ST77XX_MADCTL);// 14: Mem access ctl (directions), 1 arg:
  ST77xx_SendData(0xC0); //     row/col addr, bottom-top refresh
  delay_ms(10);

#define ST77XX_MADCTL 0x36
#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
  ST77xx_SendCmd(ST77XX_MADCTL);
  ST77xx_SendData(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR); // rotation(0)
  delay_ms(10);
*/
  
  // Включаем подсветку
  st77xx_SetBL(100);

  ST77xx_DisplayPower(1);
  delay_ms(100);
}
//==============================================================================
