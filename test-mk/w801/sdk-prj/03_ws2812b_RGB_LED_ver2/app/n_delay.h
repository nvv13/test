#ifndef N_DELAY_H
#define N_DELAY_H

#if defined(__cplusplus)
extern "C"
{
#endif

#include "wm_type_def.h"

  /**
   * @brief     interface delay ms (Milliseconds 10^-3)
   * @param[in] ms
   * @note      none
   */
  void n_delay_ms (uint32_t ms);

  /**
   * @brief     interface delay us (Microseconds 10^-6)
   * @param[in] us
   * @note      none
   */
  void n_delay_us (uint32_t us);

#if defined(__cplusplus)
}
#endif

#endif /* N_DELAY_H */
