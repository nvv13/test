#include "wm_type_def.h"

#include "wm_i2c.h"
#include "wm_regs.h"

#include "wm_gpio.h"
#include "wm_pmu.h"
#include "wm_timer.h"

#include <stdio.h>
#include <string.h>

#include "wm_gpio_afsel.h"

#include "mod1/UTFT.h"
#include "n_delay.h" //нужна для UTFT

#include "ff.h"

#define SERIAL_DEBUG
#define SERIAL_DEBUG_ALL

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#include "n_i2s.h"

void
init_ff (void)
{
  wm_sdio_host_config (0);
}

extern uint8_t SmallFont[]; // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор

FRESULT
scan_files (
    char *path /* Start node to be scanned (***also used as work area***) */
)
{ /* http://elm-chan.org/fsw/ff/doc/readdir.html */
  FRESULT res;
  DIR dir;
  UINT i;
  static FILINFO fno;

  res = f_opendir (&dir, path); /* Open the directory */
  if (res == FR_OK)
    {
      for (;;)
        {
          res = f_readdir (&dir, &fno); /* Read a directory item */
          if (res != FR_OK || fno.fname[0] == 0)
            break; /* Break on error or end of dir */
          if (fno.fattrib & AM_DIR)
            { /* It is a directory */
              i = strlen (path);
              sprintf (&path[i], "/%s", fno.fname);
              res = scan_files (path); /* Enter the directory */
              if (res != FR_OK)
                break;
              path[i] = 0;
            }
          else
            { /* It is a file. */
              printf ("%s/%s\n", path, fno.fname);
              char FileName[256];
              if (strlen (path) != 0)
                sprintf (FileName, "0:%s/%s", path, fno.fname);
              else
                sprintf (FileName, "0:%s", fno.fname);
              if (strstr (FileName, ".wav") != NULL
                  || strstr (FileName, ".WAV") != NULL)
                {
                  UTFT_clrScr (); // стираем всю информацию с дисплея
                  UTFT_setFont (SmallFont);
                  UTFT_setColor2 (VGA_FUCHSIA);
                  UTFT_print (fno.fname, CENTER, 300, 0);
                  n_i2s_PlayWav (FileName);
                }
            }
        }
      f_closedir (&dir);
    }

  return res;
}

static u8 u8_volume = 0; //

//****************************************************************************************************//

#define KNOOB_SW WM_IO_PA_11
#define KNOOB_DT WM_IO_PA_12
#define KNOOB_CLK WM_IO_PA_13

volatile static u16 i_dreb_CLK = 0; // от дребезга
// static u16 i_dreb_DT = 0;  // от дребезга
static const u16 i_pos_dreb_CLK = 3;
// static const u16 i_pos_dreb_DT  = 5;
static const u16 i_pos_dreb_SW = 300; //кнопка
static int i_rotar = 10;
volatile static u16 i_rotar_zero = 0;
volatile static u16 i_rotar_one = 0;
static u8 i_rotar_value = 0;
volatile static u8 i_dreb_SW = 0; // от дребезга кнопки
static char buf_str_ind[100];
static u8 u8_enc_state = 0;

static void
demo_timer_irq (u8 *arg) //
{

  if (i_dreb_CLK != 0)
    {
      if (i_dreb_CLK++ > i_pos_dreb_CLK) //можно отсчитывать временной интервал
        {
          i_dreb_CLK = 0; // от дребезга

          u8_enc_state = ~u8_enc_state;
          if (u8_enc_state)
            {

              if (i_rotar_zero > i_rotar_one)
                i_rotar--;
              else
                i_rotar++;

              if (i_rotar < 0)
                i_rotar = 0;
              if (i_rotar > 100)
                i_rotar = 100;

              u8_volume = 100 - i_rotar;
              n_i2s_SetVolume (u8_volume);
              UTFT_setFont (
                  SevenSegNumFont); // устанавливаем шрифт имитирующий
                                    // семисегментный индикатор
              UTFT_setColor2 (
                  VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
              sprintf (buf_str_ind, "%.3d", u8_volume);
              UTFT_print (
                  buf_str_ind, CENTER, 150,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)
            }
          i_rotar_zero = 0;
          i_rotar_one = 0;
          i_rotar_value = 1;
        }
    }

  /*  if (i_dreb_DT != 0
        && i_dreb_DT++ > i_pos_dreb_DT) //можно отсчитывать временной интервал
      {
        i_dreb_DT = 0; // от дребезга
      }*/

  if (i_dreb_SW != 0
      && i_dreb_SW++ > i_pos_dreb_SW) //можно отсчитывать временной интервал
    {
      i_dreb_SW = 0; // от дребезга
    }
}

static void
KNOOB_SW_isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (KNOOB_SW);
  if (ret)
    {
      tls_clr_gpio_irq_status (KNOOB_SW);
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
          n_i2s_stop ();
        }
    }
}
static void
KNOOB_CLK_isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (KNOOB_CLK);
  if (ret)
    {
      tls_clr_gpio_irq_status (KNOOB_CLK);
      if (i_dreb_CLK == 0)
        {
          if (tls_gpio_read (KNOOB_DT))
            i_rotar_one++;
          else
            i_rotar_zero++;
          i_dreb_CLK = 1;
        }
    }
}
/*static void
KNOOB_DT_isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (KNOOB_DT);
  if (ret)
    {
      tls_clr_gpio_irq_status (KNOOB_DT);
      if (i_dreb_DT == 0)
        {
          if (tls_gpio_read (KNOOB_DT))
            i_rotar_one++;
          else
            i_rotar_zero++;
          i_dreb_DT = 1;
        }
    }
}*/

//****************************************************************************************************//

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  n_i2s_init_hw ();

  init_ff ();

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  // timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.unit = TLS_TIMER_UNIT_US;
  timer_cfg.timeout = 100;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");
  //
  tls_gpio_cfg (KNOOB_SW, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (KNOOB_SW, KNOOB_SW_isr_callback, NULL);
  tls_gpio_irq_enable (KNOOB_SW, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
  //
  tls_gpio_cfg (KNOOB_CLK, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (KNOOB_CLK, KNOOB_CLK_isr_callback, NULL);
  tls_gpio_irq_enable (KNOOB_CLK, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
  //
  tls_gpio_cfg (KNOOB_DT, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  // tls_gpio_isr_register (KNOOB_DT, KNOOB_DT_isr_callback, NULL);
  // tls_gpio_irq_enable (KNOOB_DT, WM_GPIO_IRQ_TRIG_DOUBLE_EDGE);
  //

  u8_volume = 50;
  i_rotar = 100 - u8_volume;
  n_i2s_SetVolume (u8_volume);

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT32MEGA_2, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02, (u8)WM_IO_PA_03,
             (u8)WM_IO_PA_04, 0);
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  UTFT_clrScr (); // стираем всю информацию с дисплея

  FATFS fs;
  FRESULT res_sd;
  char buff[256]; // буффер для названия директории при сканировании файловой
                  // системы
  while (1)
    { //
      // mount SD card
      res_sd = f_mount (&fs, "0:", 1);

      //***********************formatting test****************************
      if (res_sd == FR_NO_FILESYSTEM)
        {
          printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably "
                  "because the file "
                  "initialization failed! error code:%d\r\n",
                  res_sd);
        }
      else if (res_sd != FR_OK)
        {
          printf ("Failed to mount file system! Probably because the file "
                  "initialization failed! error code:%d\r\n",
                  res_sd);
        }
      else
        {
          printf ("The file system is successfully mounted, and the read and "
                  "write test can be performed!\r\n");
        }

      if (res_sd == FR_OK)
        {
          memset (buff, 0, sizeof (buff));
          strcpy (buff, "/");
          res_sd = scan_files (buff);
        }

      // unmount file system
      f_mount (NULL, "0:", 1);
      tls_os_time_delay (HZ * 1);
    } //
}

void
UserMain (void)
{

  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);
  // tls_sys_clk_set (CPU_CLK_160M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
