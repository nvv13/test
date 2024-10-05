#ifndef SPI_N_H
#define SPI_N_H

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_hostspi.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

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

  static void
  SPI_print_retval (int i_retval, char *help)
  {
    switch (i_retval)
      {
      case TLS_SPI_STATUS_OK:
        // LOG (" %s = TLS_SPI_STATUS_OK success.\n", help);
        break;
      case TLS_SPI_STATUS_EINVAL:
        LOG (" %s = TLS_SPI_STATUS_EINVAL argument is invalid.\n", help);
        break;
      case TLS_SPI_STATUS_ENOMEM:
        LOG (" %s = TLS_SPI_STATUS_ENOMEM no enough memory.\n", help);
        break;
      case TLS_SPI_STATUS_ESHUTDOWN:
        LOG (" %s = TLS_SPI_STATUS_ESHUTDOWN SPI driver has not been "
             "installed.\n",
             help);
        break;
      case TLS_SPI_STATUS_EMODENOSUPPORT:
        LOG (" %s = TLS_SPI_STATUS_EMODENOSUPPORT mode is not support.\n",
             help);
        break;
      case TLS_SPI_STATUS_ECLKNOSUPPORT:
        LOG (
            " %s = TLS_SPI_STATUS_ECLKNOSUPPORT	fclk is not support.\n", help);
        break;
      case TLS_SPI_STATUS_EBUSY:
        LOG (" %s = TLS_SPI_STATUS_EBUSY	SPI is already initialized.\n",
             help);
        break;
      }
  }

  static int
  SPI_Settings (SPI_cfg_t *par)
  {
    wm_spi_cs_config (par->spi_cs);
    wm_spi_ck_config (par->spi_ck);
    wm_spi_di_config (par->spi_di);
    wm_spi_do_config (par->spi_do);
    // tls_spi_trans_type (SPI_BYTE_TRANSFER);
    tls_spi_trans_type (par->type); // SPI_DMA_TRANSFER
    // tls_spi_trans_type (SPI_WORD_TRANSFER);
    // SPI_DMA_TRANSFER);
    // SPI_WORD_TRANSFER=spi_set_endian(0)=SPI_LITTLE_ENDIAN;
    // default SPI_BYTE_TRANSFER=spi_set_endian(1)=SPI_BIG_ENDIAN;
    // byte,word, dma
    // используемая последовательность вывода бит.
    // MSBFIRST (Most Significant Bit First) — слева - с первого (левого) бита
    // (старшего) или LSBFIRST (Least Significant Bit First) — справа - с
    // последнего бита (младшего)
    //  надо чтобы было  MSBFIRST, SPI_MODE0
    int retval = tls_spi_setup (par->mode, par->cs_active, par->fclk);
    /**< SPI transfer mode: mode_0(CPHA=0, CHOL=0),
           mode_1(CPHA=0, CHOL=1), mode_2(CPHA=1,
               CHOL=0), mode_3(CPHA=1, CHOL=1). */
    SPI_print_retval (retval, "tls_spi_setup");
    return retval;
  };
  // static void SPI_beginTransaction (SPI_cfg_t *par){}; // Prevent other SPI
  // users static void SPI_endTransaction (SPI_cfg_t *par){};             //
  // Allow other SPI users

  /*
  static int
  SPI_transfer (const u8 *txbuf, u32 n_tx, u8 *rxbuf, u32 n_rx)
  {
    // @brief          This function is used to synchronously write command and
    // then read data from SPI.
    // int tls_spi_read_with_cmd(const u8 * txbuf, u32 n_tx, u8 * rxbuf, u32
    // n_rx);
    int retval = tls_spi_read_with_cmd (txbuf, n_tx, rxbuf, n_rx);
    SPI_print_retval (retval, "SPI_transfer");
    return retval;
  };
  */

  static int
  SPI_read_buf (u8 *rxbuf, u32 n_rx)
  {
    int retval = tls_spi_read (rxbuf, n_rx);
    SPI_print_retval (retval, "SPI_read");
    return retval;
  };

  /*
  static u16
  SPI_read16 (u16 a)
  {
    // u8 txbuf[2] = { a };
    u8 rxbuf[2] = { 0, 0 };
    SPI_print_retval (tls_spi_read (rxbuf, 2), "SPI_read16");
    //  @brief          This function is used to synchronously write command
  and
    //  then read data from SPI.
    // int tls_spi_read_with_cmd(const u8 * txbuf, u32 n_tx, u8 * rxbuf, u32
    // n_rx);
    // SPI_print_retval (tls_spi_read_with_cmd (txbuf, 1, rxbuf, 1),
    //                  "SPI_transfer");
    return rxbuf[0] << 8 | rxbuf[1];
  };*/
  static void
  SPI_write (u8 a)
  {
    u8 buf[1];
    buf[0] = a;
    SPI_print_retval (tls_spi_write (buf, 1), "SPI_write");
  };
  /*static void
  SPI_write16 (u16 a)
  {
    u8 buf[2];
    union
    {
      u16 w;
      u8 b[2];
    } un;
    un.w = a;
    // CPU:xt804 - little-endiann
    buf[0] = un.b[1]; // first Send hi byte
    buf[1] = un.b[0]; // second Send lo byte
    SPI_print_retval (tls_spi_write (buf, 2), "SPI_write16");
  };*/ // Send 16 bits data
  static void
  SPI_writeBytes (u8 *data, size_t chunk_length)
  {
    SPI_print_retval (tls_spi_write (data, chunk_length), "SPI_writeBytes");
  };

  //-----------------------------------------------------

#ifdef __cplusplus
}
#endif

#pragma GCC diagnostic pop

#endif /* SPI_N_H */
