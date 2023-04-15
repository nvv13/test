
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

// *** Hardware specific functions ***
void
UTFT__hw_special_init ()
{
  if (display_serial_mode == SERIAL_5PIN && _spi_freq != 0)
    {
      /*MASTER SPI configuratioin*/
      wm_spi_cs_config (WM_IO_PB_14);
      wm_spi_ck_config (WM_IO_PB_15);
      wm_spi_di_config (WM_IO_PB_16);
      wm_spi_do_config (WM_IO_PB_17);

      LOG ("MASTER SPI configuratioin cs--PB14, ck--PB15, di--PB16, do--PB17; "
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
      /* в этом режиме VH==1 значит комменда, 0 - данные, передаем */
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
UTFT__fast_fill_16 (int ch, int cl, long pix)
{
  long blocks;

  tls_gpio_write (PIN0, ch & 0x80 ? 1 : 0);
  tls_gpio_write (PIN1, ch & 0x40 ? 1 : 0);
  tls_gpio_write (PIN2, ch & 0x20 ? 1 : 0);
  tls_gpio_write (PIN3, ch & 0x10 ? 1 : 0);
  tls_gpio_write (PIN4, ch & 0x08 ? 1 : 0);
  tls_gpio_write (PIN5, ch & 0x04 ? 1 : 0);
  tls_gpio_write (PIN6, ch & 0x02 ? 1 : 0);
  tls_gpio_write (PIN7, ch & 0x01 ? 1 : 0);
  tls_gpio_write (PIN8, cl & 0x80 ? 1 : 0);
  tls_gpio_write (PIN9, cl & 0x40 ? 1 : 0);
  tls_gpio_write (PIN10, cl & 0x20 ? 1 : 0);
  tls_gpio_write (PIN11, cl & 0x10 ? 1 : 0);
  tls_gpio_write (PIN12, cl & 0x08 ? 1 : 0);
  tls_gpio_write (PIN13, cl & 0x04 ? 1 : 0);
  tls_gpio_write (PIN14, cl & 0x02 ? 1 : 0);
  tls_gpio_write (PIN15, cl & 0x01 ? 1 : 0);

  blocks = pix / 16;
  for (int i = 0; i < blocks; i++)
    {
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
    }
  if ((pix % 16) != 0)
    for (int i = 0; i < (pix % 16); i++)
      {
        pulse_low_WR ();
      }
}

void
UTFT__fast_fill_8 (int ch, long pix)
{
  long blocks;

  tls_gpio_write (PIN0, ch & 0x80 ? 1 : 0);
  tls_gpio_write (PIN1, ch & 0x40 ? 1 : 0);
  tls_gpio_write (PIN2, ch & 0x20 ? 1 : 0);
  tls_gpio_write (PIN3, ch & 0x10 ? 1 : 0);
  tls_gpio_write (PIN4, ch & 0x08 ? 1 : 0);
  tls_gpio_write (PIN5, ch & 0x04 ? 1 : 0);
  tls_gpio_write (PIN6, ch & 0x02 ? 1 : 0);
  tls_gpio_write (PIN7, ch & 0x01 ? 1 : 0);

  blocks = pix / 16;
  for (int i = 0; i < blocks; i++)
    {
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
      pulse_low_WR ();
    }
  if ((pix % 16) != 0)
    for (int i = 0; i < (pix % 16); i++)
      {
        pulse_low_WR ();
        pulse_low_WR ();
      }
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
