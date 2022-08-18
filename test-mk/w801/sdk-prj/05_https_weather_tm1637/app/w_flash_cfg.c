

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wm_type_def.h"
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
#include "wm_mem.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
//#include "wm_watchdog.h"
//#include "wm_cpu.h"
//#include "csi_core.h"


#include "wm_internal_flash.h"
#include "w_flash_cfg.h"


#define TEST_FLASH_BUF_SIZE    512

int flash_cfg_load_u16(u16* u16_value,u8 u8_index)
{
    u8 *read_buf = NULL;

    tls_fls_init();									//initialize flash driver

    read_buf = tls_mem_alloc(TEST_FLASH_BUF_SIZE);
    if (NULL == read_buf)
    {
        printf("\nmalloc read buf error\n");
        return WM_FAILED;
    }
    memset(read_buf, 0, TEST_FLASH_BUF_SIZE);
    tls_fls_read(0x1F0303, read_buf, TEST_FLASH_BUF_SIZE);
    
    (*u16_value) = ((u16*)read_buf)[u8_index];

    tls_mem_free(read_buf);

return WM_SUCCESS;
}

int flash_cfg_store_u16(u16 u16_value,u8 u8_index)
{

    u8 *write_buf = NULL;

    tls_fls_init();									//initialize flash driver

    write_buf = tls_mem_alloc(TEST_FLASH_BUF_SIZE);
    if (NULL == write_buf)
    {
        printf("\nmalloc write buf error\n");
        return WM_FAILED;
    }

    memset(write_buf, 0, TEST_FLASH_BUF_SIZE);
    tls_fls_read(0x1F0303, write_buf, TEST_FLASH_BUF_SIZE);

    ((u16*)write_buf)[u8_index]=u16_value;

    tls_fls_write(0x1F0303, write_buf, TEST_FLASH_BUF_SIZE);		

    tls_mem_free(write_buf);

return WM_SUCCESS;
}