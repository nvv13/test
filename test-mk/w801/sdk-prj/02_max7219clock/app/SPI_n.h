#ifndef SPI_N_H
#define SPI_N_H

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_hostspi.h"

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-function"

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG(...) printf (__VA_ARGS__)

  typedef struct
  {

    /**
     *            config spi cs pins name
     *			WM_IO_PA_00
     *			WM_IO_PB_04
     *			WM_IO_PB_14 only for 56pin
     *			WM_IO_PB_23 only for 56pin
     */
    enum tls_io_name spi_cs;

    /**
     *            config spi ck pins name
     *			WM_IO_PB_01
     *			WM_IO_PB_02
     *			WM_IO_PB_15 only for 56pin
     *			WM_IO_PB_24 only for 56pin
     */
    enum tls_io_name spi_ck;

    /**
     *         config spi di pins name
     *			WM_IO_PB_00
     *			WM_IO_PB_03
     *			WM_IO_PB_16 only for 56pin
     *			WM_IO_PB_25 only for 56pin
     */
    enum tls_io_name spi_di; /* miso MISO (Master In Slave Out) pin <- */
                             /**
                              * config the pins used for spi di
                              * WM_IO_PB_00 - не работает,
                              * WM_IO_PB_03 - работает!
                              * WM_IO_PB_16 only for 56pin - не работает, мешает светодиод подключенный
                              * к                          данному контакту на макетке                          WM_IO_PB_25 only
                              * for                          56pin - не работает,                          мешает светодиод
                              * подключенный к данному контакту на                          макетке
                              */

    /**
     *                config spi do pins name
     *			WM_IO_PA_07
     *			WM_IO_PB_05
     *			WM_IO_PB_17 only for 56pin
     *			WM_IO_PB_26 only for 56pin
     */
    enum tls_io_name spi_do; /* mosi MOSI (Master Out Slave In) pin -> */

    u8 mode;      // TLS_SPI_MODE_0 ... TLS_SPI_MODE_3
    u8 cs_active; // TLS_SPI_CS_LOW TLS_SPI_CS_HIGH
    u32 fclk;     // between TLS_SPI_FCLK_MIN and TLS_SPI_FCLK_MAX
    u8 type;      // SPI_BYTE_TRANSFER SPI_DMA_TRANSFER SPI_WORD_TRANSFER

  } SPI_cfg_t;

  void SPI_print_retval (int i_retval, char *help);

  int SPI_Settings (SPI_cfg_t *par);

  int SPI_read_buf (u8 *rxbuf, u32 n_rx);

  void SPI_write (u8 a);

  void SPI_writeBytes (u8 *data, size_t chunk_length);

  //-----------------------------------------------------

#ifdef __cplusplus
}
#endif

//#pragma GCC diagnostic pop

#endif /* SPI_N_H */
