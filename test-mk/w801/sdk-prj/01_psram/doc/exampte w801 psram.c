URL
http://ask.winnermicro.com/article/71.html

W800/W801 PSRAM usage example
Published on 2022-08-26 17:04:46

1.Code modification (possible)
Add or modify the psram config function in wm_gpio_afsel.c (pay attention to whether it is consistent):
void wm_psram_config(uint8_t numsel)
{
    switch(numsel)
    {
        case 0:
            tls_io_cfg_set(WM_IO_PB_00, WM_IO_OPTION4);/*CK*/
            tls_io_cfg_set(WM_IO_PB_01, WM_IO_OPTION4);/*CS*/
            tls_io_cfg_set(WM_IO_PB_02, WM_IO_OPTION4);/*D0*/
            tls_io_cfg_set(WM_IO_PB_03, WM_IO_OPTION4);/*D1*/
            tls_io_cfg_set(WM_IO_PB_04, WM_IO_OPTION4);/*D2*/
            tls_io_cfg_set(WM_IO_PB_05, WM_IO_OPTION4);/*D3*/
            tls_open_peripheral_clock(TLS_PERIPHERAL_TYPE_PSRAM);            
            break;
        case 1:
            tls_io_cfg_set(WM_IO_PA_15, WM_IO_OPTION1);/*CK*/
            tls_io_cfg_set(WM_IO_PB_27, WM_IO_OPTION1);/*CS*/
            tls_io_cfg_set(WM_IO_PB_02, WM_IO_OPTION1);/*D0*/
            tls_io_cfg_set(WM_IO_PB_03, WM_IO_OPTION1);/*D1*/
            tls_io_cfg_set(WM_IO_PB_04, WM_IO_OPTION1);/*D2*/
            tls_io_cfg_set(WM_IO_PB_05, WM_IO_OPTION1);/*D3*/
            tls_open_peripheral_clock(TLS_PERIPHERAL_TYPE_PSRAM);            
            break;
        case 2: //w861
            tls_io_cfg_set(WM_IO_PA_15, WM_IO_OPTION1);/*CK*/
            tls_io_cfg_set(WM_IO_PB_27, WM_IO_OPTION1);/*CS*/
            tls_io_cfg_set(WM_IO_PB_28, WM_IO_OPTION1);/*D0*/
            tls_io_cfg_set(WM_IO_PB_29, WM_IO_OPTION1);/*D1*/
            tls_io_cfg_set(WM_IO_PB_30, WM_IO_OPTION1);/*D2*/
            tls_io_cfg_set(WM_IO_PB_31, WM_IO_OPTION1);/*D3*/
            tls_open_peripheral_clock(TLS_PERIPHERAL_TYPE_PSRAM);   
            break;
        default:
            TLS_DBGPRT_ERR("psram afsel config error!");
            break;
    }
}

2. PSRAM initialization
After wm_main.c starts, 
 initialize the PSRAM (it is recommended to put it in the function wm_gpio_config):
     wm_psram_config(0);//Select 0 or 1 for the actual psram pin, W861 is 2
     //Initialize psram
     psram_init(1);

3. use
Because the built-in memory and psram memory are not continuous,
you can refer to the implementation of heap_dram.c [attachment at the end of the article] 
to manage this memory separately.
 
In this file, you only need to pay attention to the modification of two macros, 
because the starting address of PSRAM is 0x30000000,
you only need to modify the actual PSRAM capacity (DRAM_HEAP_SIZE).
/* Heap should align to DRAM_HEAP_ALIGNMENT */
#define DRAM_HEAP_BASE (0x30000000)
/* Size should be smaller than ~DRAM_HEAP_MAGIC */
#define DRAM_HEAP_SIZE (2*1024 * 1024)


