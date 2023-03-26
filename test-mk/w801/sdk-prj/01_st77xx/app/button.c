//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
//#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include <gpio.h>
//#include <delay.h>

#include <button.h>


#include "wm_type_def.h"

#include "wm_cpu.h"

//#include "wm_watchdog.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"


//==============================================================================
// Процедура инициализации ноги GPIO для подключения кнопки
//==============================================================================
void button_Init()
{
  // Инициализация ножки для перевода в режим обучения новым отпечаткам
//  gpio_PortClockStart(TestButton_Port);
//  gpio_SetGPIOmode_In(TestButton_Port, TestButton_Pin, gpio_PullUp);
}
//==============================================================================


//==============================================================================
// Функция чтения состояния кнопки
//==============================================================================
uint8_t button_GetState(void)
{
  return 0;//GPIO_ReadInputDataBit(TestButton_Port, TestButton_Pin);
}
//==============================================================================


//==============================================================================
// Процедура ожидания надатия (и отпускания) кнопки
//==============================================================================
void button_WaitPress(void)
{
     tls_os_time_delay (HZ * 3);
  // Ждём отжатия кнопки после предыдущего нажатия
//  while (!button_GetState())
//    delay_ms(100);
  
  // Ждём нажатия кнопки
//  while (button_GetState())
//    delay_ms(100);
}
//==============================================================================
