#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_gpio.h"
#include "wm_irq.h"
#include "wm_osal.h"
#include "wm_timer.h"

#include "IR_Scan.h"

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

#define TIC_PAUSE 12500

static volatile u32 u32_us_tic = 0;
static volatile int i_tic_pause = 0;

static volatile u32 u32_start = 0;
#define RAWBUF_LEN 256
static volatile int i_rawbuf[RAWBUF_LEN]; // raw data
static volatile u16 u16_raw_buf_pos = 0;

#define ST_IDLE 0
#define ST_START 1
#define ST_LOAD1 2
#define ST_LOAD2 3
#define ST_LOAD21 4
#define ST_LOAD3 5
static volatile u8 u8_status = ST_IDLE;

volatile u32 u32_IR_scan_result = 0;
static volatile u32 u32_scan_code = 0;
static volatile u8 u8_scan_pos = 0;

static tls_os_queue_t *recive_ir_msg_q = NULL;

// #define IR_SCAN_SERIAL_DEBUG

static void
tic_timer_irq (
    u8 *arg) // прерывание таймера 1 us, стараемся тут все побыстрому сделать
{
  u32_us_tic++; // подсчет us, переполнения учтены
  if (i_tic_pause >= 0) // подсчет до паузы - разрешён
    {
      if (i_tic_pause++
          > TIC_PAUSE) // наступила пауза, можно анализировать что в буффере
        {
          i_tic_pause = -1; // запрет паузы
          if (u16_raw_buf_pos > 0)
            {
#ifdef IR_SCAN_SERIAL_DEBUG
              printf ("i_rawbuf[%d]: ", u16_raw_buf_pos);
#endif
              for (int i = 0; i < u16_raw_buf_pos; i++)
                {
#ifdef IR_SCAN_SERIAL_DEBUG
                  printf (",%d", i_rawbuf[i]);
#endif
                  if (u8_status == ST_LOAD21) // это ST_LOAD2
                    u8_status = ST_LOAD2;

                  if (u8_status == ST_LOAD3) // прием 1 это или 0
                    {
                      if (i_rawbuf[i] > -650 && i_rawbuf[i] < -30)
                        {
                          u8_status = ST_LOAD21; // отложенный на след. цикл
                                                 // ST_LOAD21

                          if (i_rawbuf[i] > -140) // это 0
                            {
                              u8_scan_pos++;
#ifdef IR_SCAN_SERIAL_DEBUG
                              printf (" 0 ");
#endif
                            }
                          else
                            {
                              u32_scan_code = u32_scan_code | 1 << u8_scan_pos;
#ifdef IR_SCAN_SERIAL_DEBUG
                              printf (" 1 ");
#endif
                              u8_scan_pos++;
                              if (u8_scan_pos
                                  > 31) //все биты считаны (предполагееться
                                        //такой протокол)
                                {
                                  u8_status = ST_IDLE;
                                  u32_IR_scan_result = u32_scan_code;
                                  if (recive_ir_msg_q != NULL)
                                    {
                                      if (tls_os_queue_send (
                                              recive_ir_msg_q,
                                              (void *)u32_scan_code, 0)
                                          == TLS_OS_ERROR)
                                        {
#ifdef IR_SCAN_SERIAL_DEBUG
                                          printf (
                                              "\n DEBUG: tls_os_queue_send "
                                              "Error\n");
#endif
                                        }
                                    }

#ifdef IR_SCAN_SERIAL_DEBUG
                                  printf (
                                      "\n DEBUG: u8_scan_pos > 31 0x%08x\n",
                                      u32_scan_code);
#endif
                                  u32_scan_code = 0; // обнулить для накопления
                                  u8_scan_pos = 0; // позиция принимаемого
                                                   // бита, порядок LSB
                                }
                            }
                        }
                      else
                        u8_status = ST_IDLE;
                    }

                  if (u8_status == ST_LOAD2)
                    {
                      if (i_rawbuf[i] > 50
                          && i_rawbuf[i] < 150) // что ожидалось
                        u8_status = ST_LOAD3; // считываем 1 это или 0
                      else
                        u8_status = ST_IDLE;
                    }

                  if (u8_status == ST_LOAD1)
                    {
                      if (i_rawbuf[i] > -1000
                          && i_rawbuf[i] < -300) // что ожидалось
                        u8_status = ST_LOAD2;
                      else
                        u8_status = ST_IDLE;
                    }

                  if (u8_status == ST_START)
                    {
                      if (i_rawbuf[i] > 600
                          && i_rawbuf[i] < 2000) // что ожидалось
                        u8_status = ST_LOAD1;
                      else
                        u8_status = ST_IDLE;
                    }

                  if (u8_status == ST_IDLE
                      && i_rawbuf[i] < -1000) // детектор старта
                    {
                      u8_status = ST_START;
                      if (u32_scan_code
                          != 0) // есть предыдущий результат, отправим
                        {
                          u32_IR_scan_result = u32_scan_code;
                          if (recive_ir_msg_q != NULL)
                            {
                              if (tls_os_queue_send (recive_ir_msg_q,
                                                     (void *)u32_scan_code, 0)
                                  == TLS_OS_ERROR)
                                {
#ifdef IR_SCAN_SERIAL_DEBUG
                                  printf (
                                      "\n DEBUG: tls_os_queue_send Error\n");
#endif
                                }
                            }
                        }
#ifdef IR_SCAN_SERIAL_DEBUG
                      printf ("\n DEBUG: 0x%08x\n", u32_scan_code);
#endif
                      u32_scan_code = 0; // обнулить для накопления
                      u8_scan_pos
                          = 0; // позиция принимаемого бита, порядок LSB
                    }
                }
            }
          u16_raw_buf_pos = 0;
        }
    }
}

static enum tls_io_name recive_pin = 0;
static void
isr_callback_tsop48 (void *context) // прерывание с датчика IR вида TSOP48, по
                                    // обеим фронтам изменения
{
  u16 ret = tls_get_gpio_irq_status (recive_pin);
  if (ret) //прерывание случилось
    {
      i_tic_pause = 0; // сброс счётчика паузы, подсчет до паузы - разрешён...

      int i_cnt_tic = 0; // вычислим сколько us прошло
      if (u32_us_tic < u32_start)
        i_cnt_tic = UINT32_MAX - u32_us_tic + u32_start;
      else
        i_cnt_tic = u32_us_tic - u32_start;

      tls_clr_gpio_irq_status (recive_pin);

      if (tls_gpio_read (recive_pin))          // считать пин
        i_rawbuf[u16_raw_buf_pos] = i_cnt_tic; // считали 1
      else
        i_rawbuf[u16_raw_buf_pos] = i_cnt_tic * -1; // считали 0

      if (u16_raw_buf_pos++ > RAWBUF_LEN) // вот этого, не должно было случится
        u16_raw_buf_pos = 0; // хотя бы так...

      u32_start = u32_us_tic; // сохраним для сравнения
    }
}

int
IR_Scan_create (enum tls_io_name ir_pin, tls_os_queue_t *ir_code_msg_q)
{
  recive_pin = ir_pin;
  if (recive_pin == 0)
    {
#ifdef IR_SCAN_SERIAL_DEBUG
      printf ("IR_Scan_create : error recive_pin\n");
#endif
      return WM_FAILED;
    }

  recive_ir_msg_q = ir_code_msg_q;
  if (recive_ir_msg_q == NULL)
    {
#ifdef IR_SCAN_SERIAL_DEBUG
      printf ("IR_Scan_create : ir_code_msg_q = NULL , no query send ! \n");
#endif
    }

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_US; /**< microsecond level(us) */
  timer_cfg.timeout = 8;              // 8 us = 125KHz
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)tic_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  if (timer_id == WM_TIMER_ID_INVALID)
    return WM_FAILED;
  tls_timer_start (timer_id);
#ifdef IR_SCAN_SERIAL_DEBUG
  printf ("timer %d us start\n", timer_cfg.timeout);
#endif

  tls_gpio_cfg (recive_pin, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (recive_pin, isr_callback_tsop48, NULL);
  tls_gpio_irq_enable (
      recive_pin,
      WM_GPIO_IRQ_TRIG_DOUBLE_EDGE); /**< both rising edge and falling edge
                                        arise the interrupt */
#ifdef IR_SCAN_SERIAL_DEBUG
  printf ("\nisr_callback_tsop48 gpio %d DOUBLE_EDGE isr\n", recive_pin);
#endif

  return WM_SUCCESS;
}
