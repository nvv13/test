#ifndef IR_SCAN_H
#define IR_SCAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wm_gpio.h"

#include "wm_osal.h"

#define IR_QUEUE_SIZE 32


/**
 * @brief     interface IR_Scan_create
 *
 * @param[in] ir_pin - куда подключен датчик
 * @param[in]  ir_code_msg_q - очередь сообщений * с результатом сканирования
 *      если      ir_code_msg_q = NULL -  то тогда используем переменную 
 *                      extern volatile u32 u32_IR_scan_result;
 *
 * @retval         WM_SUCCESS    success
 * @retval         WM_FAILED     failed
 * @note      none
 */

int IR_Scan_create (enum tls_io_name ir_pin, tls_os_queue_t *ir_code_msg_q);

extern volatile u32 u32_IR_scan_result;

#ifdef __cplusplus
}
#endif

#endif /* IR_SCAN_H */