#ifndef LAUT_LOG
#define LAUT_LOG


#ifdef __cplusplus
extern "C" {
#endif

#include "tjpgdcnf.h"

#include "wm_mem.h"

#define luat_heap_malloc tls_mem_alloc 
#define luat_heap_free tls_mem_free 
//#define realloc luat_heap_realloc
//#define zalloc luat_heap_zalloc
//#define calloc luat_heap_calloc


#define LUAT_LOG_DEBUG 1
#define LUAT_LOG_INFO  2
#define LUAT_LOG_WARN  3
#define LUAT_LOG_ERROR 4
#define LUAT_LOG_CLOSE 7

#define LLOGE(format, ...) luat_log_printf(LUAT_LOG_ERROR, "E/" LUAT_LOG_TAG " " format "\n", ##__VA_ARGS__)
#define LLOGW(format, ...) luat_log_printf(LUAT_LOG_WARN,  "W/" LUAT_LOG_TAG " " format "\n", ##__VA_ARGS__)
#define LLOGI(format, ...) luat_log_printf(LUAT_LOG_INFO,  "I/" LUAT_LOG_TAG " " format "\n", ##__VA_ARGS__)
#define LLOGD(format, ...) luat_log_printf(LUAT_LOG_DEBUG, "D/" LUAT_LOG_TAG " " format "\n", ##__VA_ARGS__)
void luat_log_printf(int level, const char* _fmt, ...);


#include "ff.h"
#define FILE FIL

FILE* luat_fs_fopen(const char *filename, const char *mode);
size_t luat_fs_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int luat_fs_fseek(FILE* stream, long int offset, int origin);
int luat_fs_fclose(FILE* stream);


typedef struct luat_lcd_conf {
/*    uint8_t port;
    uint8_t pin_dc;
    uint8_t pin_pwr;
    uint8_t pin_rst;

    uint32_t w;
    uint32_t h;
    uint32_t buffer_size;
    uint32_t dc_delay_us;
    uint8_t xoffset;//偏移
    uint8_t yoffset;//偏移
    uint8_t auto_flush;
    uint8_t direction;//方向
    u8g2_t luat_lcd_u8g2 ;
    struct luat_lcd_opts* opts;
    luat_spi_device_t* lcd_spi_device;
    int lcd_spi_ref;
*/
    void* userdata;

    // buff 相关
//    luat_color_t* buff;
/*    int buff_ref;
    uint16_t flush_y_min;
    uint16_t flush_y_max;
    uint8_t is_init_done;
*/
 
} luat_lcd_conf_t;

luat_lcd_conf_t* default_conf;

int luat_lcd_draw(luat_lcd_conf_t* conf, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, luat_color_t* color);





#ifdef __cplusplus
}
#endif

#endif /* LAUT_LOG */
