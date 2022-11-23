#ifndef N_DELAY_H
#define N_DELAY_H

#include "csi_core.h"
#include "wm_type_def.h"

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void n_delay_ms (uint32_t ms);

/**
 * @brief     interface delay us
 * @param[in] us
 * @note      none
 */
void n_delay_us (uint32_t us);

void n_delay_tic (uint32_t cnt);

#endif /* N_DELAY_H */