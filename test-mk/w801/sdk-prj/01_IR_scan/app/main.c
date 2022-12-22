/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-12-21
 *****************************************************************************/

#include "wm_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_cpu.h"
//#include "wm_watchdog.h"

#include "HD44780LCD.h"
#include "IR_Scan.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  const u8 u8_row = 2;
  const u8 u8_col = 16;
  HD44780LCD_HD44780LCD (u8_row, u8_col); //
  HD44780LCD_LCDInit (LCDCursorTypeOn);
  HD44780LCD_LCDClearScreen ();
  HD44780LCD_LCDBackLightSet (true);
  printf ("HD44780LCD_LCDInit u8_row=%d, u8_col=%d\n", u8_row, u8_col);

  tls_os_queue_t *ir_code_msg_q = NULL;
  tls_os_queue_create (&ir_code_msg_q, IR_QUEUE_SIZE);
  if (ir_code_msg_q == NULL)
    printf ("tls_os_queue_create - error\n");
  else
    printf ("tls_os_queue_create - recive_ir_msg_q\n");

  IR_Scan_create (WM_IO_PA_01, ir_code_msg_q);
  printf ("IR_Scan_create\n");

  u32 scan_IR_msg = 0;
  u16 u16_total_count = 0;

  for (;;)
    {
      tls_os_queue_receive (ir_code_msg_q, (void **)&scan_IR_msg, 0, 0);
      printf ("\n tls_os_queue_receive: 0x%08x \n", scan_IR_msg);
      HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);
      HD44780LCD_printf ("0x%08x  %d", scan_IR_msg, ++u16_total_count);
    }
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
