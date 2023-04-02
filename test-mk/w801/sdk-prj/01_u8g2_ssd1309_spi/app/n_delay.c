/*

 mc - Milliseconds 10^-3

 µs - Microseconds 10^-6
 us - Microseconds 10^-6

 ns - Nanoseconds  10^-9

 1000 ms = 1 µs OR 1 us

*/

#include "csi_core.h"

#include "n_delay.h"
#include "wm_cpu.h"

extern uint32_t csi_coret_get_load (void);
extern uint32_t csi_coret_get_value (void);

/**
 * @brief     interface delay ms (Milliseconds 10^-3)
 * @param[in] ms
 * @note      none
 */
void
n_delay_ms (uint32_t ms)
{
  if (ms == 0)
    return;
  do
    {
      n_delay_us (1000);
    }
  while ((--ms) > 0);
}

/**
 * @brief     interface delay us (Microseconds 10^-6)
 * @param[in] us
 * @note      none
 */
void
n_delay_us (uint32_t us)
{
  if (us > 1000) //надо так, иначе зависнет, уже 2000 перебор, поэтому так
    {
      n_delay_ms (us / 1000);
      us = us % 1000;
    }
  if (us == 0)
    return;

  uint32_t load = csi_coret_get_load ();
  uint32_t start = csi_coret_get_value ();
  uint32_t cur;
  uint32_t cnt;
  tls_sys_clk sysclk;

  tls_sys_clk_get (&sysclk);
  cnt = sysclk.cpuclk * us;
  /*
  при 240MHz значение sysclk.cpuclk=240
  то есть
  1 cpuclk = 1 микросекунда
  или 1MHz,
  то есть
  System Tick Timer (CORET) - меняеться с частотой CPU в сторону уменьшения
  csi_coret_get_value() - текущее значение CORET

  при достижении нуля, возникает прерываение, и загрузка текущего значения из
  переменой csi_coret_get_load() - настраевает OS при смене частоты процессора

  значение load от частоты:
  240MHz load=479999
  160MHz load=319999
  80MHz  load=159999
  40MHz  load=79999

  если частоту в герцах поделить на значение load,
  то это и будет 500 герц (т.е. период 2 милисекунды)
  */

  while (1)
    {
      cur = csi_coret_get_value ();

      if (start > cur)
        {
          if (start - cur >= cnt)
            {
              return;
            }
        }
      else
        {
          if (load - cur + start > cnt)
            {
              return;
            }
        }
    }
}
