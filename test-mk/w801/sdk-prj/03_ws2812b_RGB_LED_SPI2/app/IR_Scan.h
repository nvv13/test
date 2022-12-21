#ifndef IR_SCAN_H
#define IR_SCAN_H

#include "wm_gpio.h"

#include "wm_osal.h"

#define IR_QUEUE_SIZE 32

/**
 * @brief     interface IR_Scan_create
 * @param[in] ir_pin - куда подключен датчик, ir_code_msg_q - очередь сообщений
 * с результатом сканирования
 * @retval         WM_SUCCESS    success
 * @retval         WM_FAILED     failed
 * @note      none
 */
int IR_Scan_create (enum tls_io_name ir_pin, tls_os_queue_t *ir_code_msg_q);

#endif /* IR_SCAN_H */