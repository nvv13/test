#ifndef N_DELAY_H
#define N_DELAY_H

#include "wm_type_def.h"
#include "csi_core.h"
#include "wm_cpu.h"

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void n_delay_ms(uint32_t ms);

/**
 * @brief     interface delay us
 * @param[in] us
 * @note      none
 */
void n_delay_us(uint32_t us);


#endif /* N_DELAY_H */