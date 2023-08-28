
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#include "wm_hostspi.h"

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
      LOG (" %s = TLS_SPI_STATUS_EMODENOSUPPORT mode is not support.\n", help);
      break;
    case TLS_SPI_STATUS_ECLKNOSUPPORT:
      LOG (" %s = TLS_SPI_STATUS_ECLKNOSUPPORT	fclk is not support.\n", help);
      break;
    case TLS_SPI_STATUS_EBUSY:
      LOG (" %s = TLS_SPI_STATUS_EBUSY	SPI is already initialized.\n", help);
      break;
    }
}


static void sdio_spi_init (u32 fclk);
static int sdio_spi_send (const u8 *buf, u32 len);

// *** Hardware specific functions ***
void
UTFT__hw_special_init ()
{
  if (display_serial_mode == SERIAL_5PIN && _spi_freq != 0)
    {
      if (_spi_freq > 20000000)
        {
          sdio_spi_init (_spi_freq);
        }
      else
        {
          /*MASTER SPI configuratioin*/
          wm_spi_cs_config (WM_IO_PB_14);
          wm_spi_ck_config (WM_IO_PB_15);
          wm_spi_di_config (WM_IO_PB_16);
          wm_spi_do_config (WM_IO_PB_17);

          LOG ("MASTER SPI configuratioin cs--PB14, ck--PB15, di--PB16, "
               "do--PB17; "
               "spi_freq=%d \n",
               _spi_freq);
          tls_spi_trans_type (SPI_DMA_TRANSFER); // byte , word, dma
          // tls_spi_trans_type (SPI_BYTE_TRANSFER); // byte , word, dma
          SPI_print_retval (tls_spi_setup (_spi_mode,      // TLS_SPI_MODE_1
                                           TLS_SPI_CS_LOW, // TLS_SPI_CS_LOW,
                                           _spi_freq),
                            "tls_spi_setup");
        }
    }
}

//#include "wm_regs.h"
//#include "wm_irq.h"
//#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_dma.h"
//#include "wm_dbg.h"
//#include "wm_mem.h"
//#include "wm_cpu.h"
//#include "wm_spi_hal.h"
//#include "wm_wl_task.h"
//#include "tls_common.h"
//#include "core_804.h"
#include "wm_pmu.h"

extern int spiWaitIdle (void);

int
n_spi_write (const u8 *buf, u32 len)
{

  if (_spi_freq > 20000000)
    {
      return sdio_spi_send (buf, len);
    }

  if (len <= 4)
    {
      u32 data32 = 0;
      u16 txBitLen;
      u32 rdval1 = 0;
      u32 i;
      tls_open_peripheral_clock (TLS_PERIPHERAL_TYPE_LSPI);
      SPIM_CHCFG_REG = SPI_CLEAR_FIFOS;
      while (SPIM_CHCFG_REG & SPI_CLEAR_FIFOS)
        ;
      for (i = 0; i < len; i++)
        {
          data32 |= (((u8) (buf[i])) << (i * 8));
        }
      SPIM_TXDATA_REG = data32;
      txBitLen = 8 * len;
      rdval1 = SPI_FORCE_SPI_CS_OUT | SPI_CS_LOW | SPI_TX_CHANNEL_ON
               | SPI_RX_CHANNEL_ON | SPI_CONTINUE_MODE | SPI_START
               | SPI_VALID_CLKS_NUM (txBitLen);
      SPIM_CHCFG_REG = rdval1;
      spiWaitIdle ();
      SPIM_CHCFG_REG |= SPI_CS_HIGH;

      SPIM_CHCFG_REG = 0x00000000;
      SPIM_MODECFG_REG = 0x00000000;
      tls_close_peripheral_clock (TLS_PERIPHERAL_TYPE_LSPI);
      return TLS_SPI_STATUS_OK;
    }
  else
    return tls_spi_write (buf, len);
}

void
UTFT_LCD_Writ_Bus_SPI (const u8 *buf, u32 len)
{
  if (_spi_freq != 0)
    n_spi_write (buf, len);
}

void
UTFT_LCD_Writ_Bus (char VH, char VL, byte mode)
{
  switch (mode)
    {
    case 1:
      /* в этом режиме VH==1 значит комманда, 0 - данные, передаем */
      if (display_serial_mode == SERIAL_4PIN)
        { /* SERIAL_4PIN значит 1 передаваемый бит - означает комманда/данные
           */
          if (VH == 1)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
        }
      else
        { /* SERIAL_5PIN, значит есть отдельный пин, который говорит, данные
             это или комманда */
          if (VH == 1)
            sbi (P_RS, B_RS);
          else
            cbi (P_RS, B_RS);
        }

      if (display_serial_mode == SERIAL_5PIN && _spi_freq != 0)
        n_spi_write ((u8 *)&VL, 1);
      else
        {

          if (VL & 0x80)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x40)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x20)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x10)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x08)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x04)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x02)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
          if (VL & 0x01)
            sbi_SDA ();
          else
            cbi_SDA ();
          pulse_low_SCL ();
        }

      break;
    case 8:
      /* в этом режиме VH и VL - данные */
      tls_gpio_write (PIN0, VH & 0x80 ? 1 : 0);
      tls_gpio_write (PIN1, VH & 0x40 ? 1 : 0);
      tls_gpio_write (PIN2, VH & 0x20 ? 1 : 0);
      tls_gpio_write (PIN3, VH & 0x10 ? 1 : 0);
      tls_gpio_write (PIN4, VH & 0x08 ? 1 : 0);
      tls_gpio_write (PIN5, VH & 0x04 ? 1 : 0);
      tls_gpio_write (PIN6, VH & 0x02 ? 1 : 0);
      tls_gpio_write (PIN7, VH & 0x01 ? 1 : 0);
      pulse_low_WR ();
      tls_gpio_write (PIN0, VL & 0x80 ? 1 : 0);
      tls_gpio_write (PIN1, VL & 0x40 ? 1 : 0);
      tls_gpio_write (PIN2, VL & 0x20 ? 1 : 0);
      tls_gpio_write (PIN3, VL & 0x10 ? 1 : 0);
      tls_gpio_write (PIN4, VL & 0x08 ? 1 : 0);
      tls_gpio_write (PIN5, VL & 0x04 ? 1 : 0);
      tls_gpio_write (PIN6, VL & 0x02 ? 1 : 0);
      tls_gpio_write (PIN7, VL & 0x01 ? 1 : 0);
      pulse_low_WR ();
      break;
    case 16:
      {
        /* в этом режиме VH и VL - данные, 16 разрядов порта B, биты 0-15  */
        //!ВНИМАНИЕ! PIN0 - PIN15, использовать только PB порт GPIO !ВНИМАНИЕ!
        u32 cpu_sr = 0;
        u32 reg;
        u32 reg_en;
        cpu_sr = tls_os_set_critical (); // disable Interrupt !!!
        reg_en = tls_reg_read32 (HR_GPIO_DATA_EN + TLS_IO_AB_OFFSET);
        tls_reg_write32 (
            HR_GPIO_DATA_EN + TLS_IO_AB_OFFSET,
            reg_en | (1 << (PIN0 - WM_IO_PB_00)) | (1 << (PIN1 - WM_IO_PB_00))
                | (1 << (PIN2 - WM_IO_PB_00)) | (1 << (PIN3 - WM_IO_PB_00))
                | (1 << (PIN4 - WM_IO_PB_00)) | (1 << (PIN5 - WM_IO_PB_00))
                | (1 << (PIN6 - WM_IO_PB_00)) | (1 << (PIN7 - WM_IO_PB_00))
                | (1 << (PIN8 - WM_IO_PB_00)) | (1 << (PIN9 - WM_IO_PB_00))
                | (1 << (PIN10 - WM_IO_PB_00)) | (1 << (PIN11 - WM_IO_PB_00))
                | (1 << (PIN12 - WM_IO_PB_00)) | (1 << (PIN13 - WM_IO_PB_00))
                | (1 << (PIN14 - WM_IO_PB_00))
                | (1 << (PIN15
                         - WM_IO_PB_00))); // enabled control reg from need pin

        reg = tls_reg_read32 (HR_GPIO_DATA
                              + TLS_IO_AB_OFFSET); // load all pins from port

        if (VH & 0x80)
          reg |= (1 << (PIN0 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN0 - WM_IO_PB_00)));
        if (VH & 0x40)
          reg |= (1 << (PIN1 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN1 - WM_IO_PB_00)));
        if (VH & 0x20)
          reg |= (1 << (PIN2 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN2 - WM_IO_PB_00)));
        if (VH & 0x10)
          reg |= (1 << (PIN3 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN3 - WM_IO_PB_00)));
        if (VH & 0x08)
          reg |= (1 << (PIN4 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN4 - WM_IO_PB_00)));
        if (VH & 0x04)
          reg |= (1 << (PIN5 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN5 - WM_IO_PB_00)));
        if (VH & 0x02)
          reg |= (1 << (PIN6 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN6 - WM_IO_PB_00)));
        if (VH & 0x01)
          reg |= (1 << (PIN7 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN7 - WM_IO_PB_00)));

        if (VL & 0x80)
          reg |= (1 << (PIN8 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN8 - WM_IO_PB_00)));
        if (VL & 0x40)
          reg |= (1 << (PIN9 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN9 - WM_IO_PB_00)));
        if (VL & 0x20)
          reg |= (1 << (PIN10 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN10 - WM_IO_PB_00)));
        if (VL & 0x10)
          reg |= (1 << (PIN11 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN11 - WM_IO_PB_00)));
        if (VL & 0x08)
          reg |= (1 << (PIN12 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN12 - WM_IO_PB_00)));
        if (VL & 0x04)
          reg |= (1 << (PIN13 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN13 - WM_IO_PB_00)));
        if (VL & 0x02)
          reg |= (1 << (PIN14 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN14 - WM_IO_PB_00)));
        if (VL & 0x01)
          reg |= (1 << (PIN15 - WM_IO_PB_00));
        else
          reg &= (~(1 << (PIN15 - WM_IO_PB_00)));

        tls_reg_write32 (HR_GPIO_DATA + TLS_IO_AB_OFFSET,
                         reg); /* write reg all enabled pin port B */

        tls_reg_write32 (HR_GPIO_DATA_EN + TLS_IO_AB_OFFSET,
                         reg_en);         // reg_en return
        tls_os_release_critical (cpu_sr); // enable Interrupt

        pulse_low_WR ();
      }
      break;
    }
}

void
UTFT__set_direction_registers (byte mode)
{

  tls_gpio_cfg (PIN0, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN1, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN2, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN3, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN4, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN5, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN6, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN7, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?

  if (mode == 16)
    {
      tls_gpio_cfg (PIN8, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN9, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN10, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN11, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN12, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN13, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN14, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (PIN15, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    }
}
void
UTFT__fast_fill_16 (byte VH, byte VL, long pix)
{
  if (pix > 0)
    UTFT_LCD_Writ_Bus (VH, VL, 16);

  if (pix > 1)
    pulse_low_WR_repeat (pix - 1);
}

void
UTFT__fast_fill_8 (int ch, long pix)
{
  if (pix > 0)
    UTFT_LCD_Writ_Bus (ch, ch, 8);

  if (pix > 1)
    pulse_low_WR_repeat ((pix - 1) * 2);
}

// extern void n_delay_ms (uint32_t ms);
// static void delay(int ms){n_delay_ms(ms);};

static void
delay (u32 ms)
{
  u32 tick = ms / (1000 / HZ);
  if (ms > (tick * (1000 / HZ)))
    tick++; //в данном случае в большую сторону сделаем
  tls_os_time_delay (tick);
};

#include "wm_cpu.h"
#include "wm_dma.h"
#include "wm_mem.h"
#include "wm_sdio_host.h"

extern void delay_cnt (int count);
extern int wm_sd_card_dma_config (u32 *mbuf, u32 bufsize, u8 dir);

/*
source
https://github.com/openLuat/luatos-soc-air101.git
MIT License

static void sdio_host_reset(void)

*/
static void
sdio_host_reset (void)
{
  tls_bitband_write (HR_CLK_RST_CTL, 27, 0);
  delay_cnt (1000);
  tls_bitband_write (HR_CLK_RST_CTL, 27, 1);
  while (tls_bitband_read (HR_CLK_RST_CTL, 27) == 0)
    ;
}

/*
source
https://github.com/openLuat/luatos-soc-air101.git
MIT License

void sdio_spi_init(u32 fclk);

*/
static void
sdio_spi_init (u32 fclk)
{

  //wm_spi_cs_config (WM_IO_PB_23);              // CS ?
  tls_io_cfg_set (WM_IO_PB_06, WM_IO_OPTION2); /*CK    -> SCL */
  tls_io_cfg_set (WM_IO_PB_07, WM_IO_OPTION2); /*CMD   -> MOSI */
  //tls_io_cfg_set (WM_IO_PB_08, WM_IO_OPTION2); /*D0*/
  tls_open_peripheral_clock (TLS_PERIPHERAL_TYPE_SDIO_MASTER);

  sdio_host_reset ();

  tls_sys_clk sysclk;
  tls_sys_clk_get (&sysclk);

  SDIO_HOST->MMC_CARDSEL = 0xC0 | (sysclk.cpuclk / 2 - 1); // enable module, enable mmcclk

  uint8_t ti = (sysclk.cpuclk / 2 / (fclk / 1000000) - 1);
  if(ti>7)ti=0;
  u32 itog_clk=0;
  switch(ti)
   {
    case 0: itog_clk = sysclk.cpuclk / 2 * 1000000; break;
    case 1: itog_clk = sysclk.cpuclk / 4 * 1000000; break;
    case 2: itog_clk = sysclk.cpuclk / 6 * 1000000; break;
    case 3: itog_clk = sysclk.cpuclk / 8 * 1000000; break;
    case 4: itog_clk = sysclk.cpuclk / 10 * 1000000; break;
    case 5: itog_clk = sysclk.cpuclk / 12 * 1000000; break;
    case 6: itog_clk = sysclk.cpuclk / 14 * 1000000; break;
    case 7: itog_clk = sysclk.cpuclk / 16 * 1000000; break;
   }
  LOG ("sdio_spi_init sysclk.cpuclk = %d, fclk = %d, ti = %d, itog_clk=%d \n",sysclk.cpuclk,fclk, ti, itog_clk);

//  sdio_spi_init sysclk.cpuclk = 240, fclk = 50000000, ti = 0 

  SDIO_HOST->MMC_CTL = 0x542 | (ti << 3);
  
  //SDIO_HOST->MMC_CTL = 0x542; // 000 1/2     119 Mhz (240Mhz sys)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b001 << 3); // 001 1/4   60.24 Mhz (240Mhz sys) (~39 fps)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b010 << 3); // 010 1/6   40.48 Mhz (240Mhz sys)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b011 << 3); // 011 1/8    29.85 Mhz (240Mhz sys)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b100 << 3); // 100 1/10   23.80 Mhz (240Mhz sys)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b101 << 3); // 101 1/12  20 Mhz (240Mhz sys)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b110 << 3); // 110 1/14   17.09 Mhz (240Mhz sys)
  //SDIO_HOST->MMC_CTL = 0x542 | (0b111 << 3); // 111 1/16  14.92 Mhz (240Mhz sys) (~11 fps)

  SDIO_HOST->MMC_INT_MASK = 0x100; // unmask sdio data interrupt.
  SDIO_HOST->MMC_CRCCTL = 0x00;
  SDIO_HOST->MMC_TIMEOUTCNT = 0;
  SDIO_HOST->MMC_BYTECNTL = 0;
}

/*
source
https://github.com/openLuat/luatos-soc-air101.git
MIT License

int sdio_spi_send(const u8 * buf, u32 len);

    int ret;
    if (spi_id==5){
        ret = sdio_spi_send(send_buf, length);
        return ret?-1:length;
    }

*/
static int
sdio_spi_send (const u8 *buf, u32 len)
{
  if ((buf == NULL) || (len == 0))
    {
      return -1;
    }
  if (len < 4)
    { // Direct transfer, the reason for this is that DMA cannot continuously
      // transfer data of less than 4 bytes
      SDIO_HOST->BUF_CTL = 0x4820;
      SDIO_HOST->DATA_BUF[0] = *((u32 *)buf);
      SDIO_HOST->MMC_BYTECNTL = len;
      SDIO_HOST->MMC_IO = 0x01;
      while (1)
        {
          if ((SDIO_HOST->MMC_IO & 0x01) == 0x00)
            break;
        }
    }
  else
    { 
      // DMA transfer
      u32 sendlen, txlen;
      txlen = len & 0xfffffffc; // If there are not enough words, I will post
                                // it separately at the end
      sendlen = txlen / 4;

/*
      SDIO_HOST->BUF_CTL = 0x4000; // disable dma,
      unsigned char sdio_spi_dma_channel
          = wm_sd_card_dma_config ((u32 *)buf, sendlen, 1);
      SDIO_HOST->BUF_CTL = 0xC20;      // enable dma, write sd card
      SDIO_HOST->MMC_INT_SRC |= 0x7ff; // clear all firstly
      SDIO_HOST->MMC_BYTECNTL = txlen;
      SDIO_HOST->MMC_IO = 0x01;
      tls_dma_free (sdio_spi_dma_channel);
      while (1)
        {
          if ((SDIO_HOST->MMC_IO & 0x01) == 0x00)
            break;
        }
*/
      u32 _sendlen=0;
      while(_sendlen<sendlen)
       {
          SDIO_HOST->BUF_CTL = 0x4820;
          SDIO_HOST->DATA_BUF[0] = *((u32 *)buf + _sendlen);
          SDIO_HOST->MMC_BYTECNTL = 4;
          SDIO_HOST->MMC_IO = 0x01;
          while (1)
            {
              if ((SDIO_HOST->MMC_IO & 0x01) == 0x00)
                break;
            }
         _sendlen++;
       }

      if (len > txlen)
        {
          SDIO_HOST->BUF_CTL = 0x4820;
          SDIO_HOST->DATA_BUF[0] = *((u32 *)buf + sendlen);
          SDIO_HOST->MMC_BYTECNTL = len - txlen;
          SDIO_HOST->MMC_IO = 0x01;
          while (1)
            {
              if ((SDIO_HOST->MMC_IO & 0x01) == 0x00)
                break;
            }
        }
    }
  return 0;
}
