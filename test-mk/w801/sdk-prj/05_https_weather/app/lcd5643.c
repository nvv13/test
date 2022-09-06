#include "wm_gpio.h"
#include "wm_osal.h"
#include "wm_regs.h"
#include "wm_type_def.h"

#include "decode_cmd.h"
#include "lcd5643.h"

u32
reg_set_num (u32 reg_temp, u8 i_num) //
{
  /*
  PB_21 1
  PB_22 2
  PB_23 3
  PB_24 4
  PB_25 5
  PB_26 6
  PB_18 7
  PB_17 8
  PB_16 9
  PB_11 10
  PB_10 11
  PB_15 12
  */
  // i_num=11.7.4.2.1.10.5
  switch (i_num)
    {
    case 0: // 0=5  1=11 7 4 2 1 10
      {
        reg_temp = reg_temp | (1 << 10) // 11 A
                   | (1 << 18)          // 7  B
                   | (1 << 24)          // 4  C
                   | (1 << 22)          // 2  D
                   | (1 << 21)          // 1  E
                   | (1 << 11)          // 10 F
            ;
        reg_temp = reg_temp & (~(1 << 25)) // 5 G
            ;                              /* write low */
      };
      break;
    case 1: //
      {
        reg_temp = reg_temp | (1 << 18)    // 7  B
                   | (1 << 24)             // 4  C
            ;                              //
        reg_temp = reg_temp & (~(1 << 10)) // 11 A
                   & (~(1 << 22))          // 2  D
                   & (~(1 << 21))          // 1  E
                   & (~(1 << 11))          // 10 F
                   & (~(1 << 25))          // 5 G
            ;                              /* write low */
      };
      break;
    case 2: //
      {
        reg_temp = reg_temp | (1 << 10)    // 11 A
                   | (1 << 18)             // 7  B
                   | (1 << 22)             // 2  D
                   | (1 << 21)             // 1  E
                   | (1 << 25)             // 5 G
            ;                              //
        reg_temp = reg_temp & (~(1 << 11)) // 10 F
                   & (~(1 << 24))          // 4  C
            ;                              /* write low */
      };
      break;
    case 3: //
      {
        reg_temp = reg_temp | (1 << 10)    // 11 A
                   | (1 << 18)             // 7  B
                   | (1 << 22)             // 2  D
                   | (1 << 24)             // 4  C
                   | (1 << 25)             // 5 G
            ;                              //
        reg_temp = reg_temp & (~(1 << 11)) // 10 F
                   & (~(1 << 21))          // 1  E
            ;                              /* write low */
      };
      break;
    case 4: //
      {
        reg_temp = reg_temp | (1 << 18)    // 7  B
                   | (1 << 24)             // 4  C
                   | (1 << 11)             // 10 F
                   | (1 << 25)             // 5 G
            ;                              //
        reg_temp = reg_temp & (~(1 << 10)) // 11 A
                   & (~(1 << 22))          // 2  D
                   & (~(1 << 21))          // 1  E
            ;                              /* write low */
      };
      break;
    case 5: //
      {
        reg_temp = reg_temp | (1 << 10)    // 11 A
                   | (1 << 24)             // 4  C
                   | (1 << 22)             // 2  D
                   | (1 << 11)             // 10 F
                   | (1 << 25)             // 5 G
            ;                              //
        reg_temp = reg_temp & (~(1 << 18)) // 7  B
                   & (~(1 << 21))          // 1  E
            ;                              /* write low */
      };
      break;
    case 6: //
      {
        reg_temp = reg_temp | (1 << 10)    // 11 A
                   | (1 << 24)             // 4  C
                   | (1 << 22)             // 2  D
                   | (1 << 21)             // 1  E
                   | (1 << 11)             // 10 F
                   | (1 << 25)             // 5 G
            ;                              //
        reg_temp = reg_temp & (~(1 << 18)) // 7  B
            ;                              /* write low */
      };
      break;
    case 7: //
      {
        reg_temp = reg_temp | (1 << 10)    // 11 A
                   | (1 << 18)             // 7  B
                   | (1 << 24)             // 4  C
            ;                              //
        reg_temp = reg_temp & (~(1 << 22)) // 2  D
                   & (~(1 << 21))          // 1  E
                   & (~(1 << 11))          // 10 F
                   & (~(1 << 25))          // 5 G
            ;                              /* write low */
      };
      break;
    case 8: //
      {
        reg_temp = reg_temp | (1 << 10) // 11 A
                   | (1 << 18)          // 7  B
                   | (1 << 24)          // 4  C
                   | (1 << 22)          // 2  D
                   | (1 << 21)          // 1  E
                   | (1 << 11)          // 10 F
                   | (1 << 25)          // 5 G
            ;                           //
      };
      break;
    case 9: //
      {
        reg_temp = reg_temp | (1 << 10)    // 11 A
                   | (1 << 18)             // 7  B
                   | (1 << 24)             // 4  C
                   | (1 << 22)             // 2  D
                   | (1 << 11)             // 10 F
                   | (1 << 25)             // 5 G
            ;                              //
        reg_temp = reg_temp & (~(1 << 21)) // 1  E
            ;                              //
      };
      break;
    }
  return reg_temp;
}

static u8 i_off = 0;

void
lcd5643printDigit (u8 i_pos, u8 i_num, u8 iMode)
{
  if (i_pos > OUT_DIG_4 && i_off == 1)
    return; //
            /*
            LCD display 5643AS-1
                 12 pin
          
            connect to
            W801  5643AS-1
            GPIO  PIN
            PB_21 1
            PB_22 2
            PB_23 3
            PB_24 4
            PB_25 5
            PB_26 6
            PB_18 7
            PB_17 8
            PB_16 9
            PB_11 10
            PB_10 11
            PB_15 12
          
            */

  u32 cpu_sr = cpu_sr = tls_os_set_critical (); // disable Interrupt !!!

  u16 offset = TLS_IO_AB_OFFSET;
  u32 reg;
  u32 reg_en;

  reg_en = tls_reg_read32 (HR_GPIO_DATA_EN + offset);
  tls_reg_write32 (HR_GPIO_DATA_EN + offset,
                   reg_en | (1 << 21) | (1 << 22) | (1 << 23) | (1 << 24)
                       | (1 << 25) | (1 << 26) | (1 << 18) | (1 << 17)
                       | (1 << 16) | (1 << 11) | (1 << 10)
                       | (1 << 15)); // enabled control reg from need pin

  reg = tls_reg_read32 (HR_GPIO_DATA + offset); // load all pins from port

  u32 reg_temp = reg;

  i_off = 0;
  switch (i_pos)
    {
    case OUT_SEC_IND: // sec indicator, 0=9,11.7.4.2.1.10.5 1=12,8,6 - i_num=3
      {
        if (iMode == MODE_CLOCK)
          {
            reg_temp = reg_temp | (1 << 15) // 12
                       | (1 << 17)          // 8
                       | (1 << 26);         // 6

            reg_temp = reg_temp & (~(1 << 16)) & (~(1 << 10)) & (~(1 << 18))
                       & (~(1 << 24)) & (~(1 << 22)) & (~(1 << 21))
                       & (~(1 << 11)) & (~(1 << 25)); // write low
            if (i_num == 0)
              reg_temp = reg_temp & (~(1 << 23)); // 3
            else
              reg_temp = reg_temp | (1 << 23); // 3
          }
      };
      break;

    case OUT_SIG_1: // digit 1, 0=12 1=9,8,6 - i_num=11.7.4.2.1.10.5
      {
        if (iMode == MODE_CLOCK) // case OUT_DIG_1: //digit 1, 0=12 1=9,8,6 -
                                 // i_num=11.7.4.2.1.10.5
          {
            reg_temp = reg_temp | (1 << 16)     // 9
                       | (1 << 17)              // 8
                       | (1 << 26);             // 6
            reg_temp = reg_temp & (~(1 << 15)); // 12
            reg_temp = reg_set_num (reg_temp, i_num);
          }
        else
          {

            reg_temp = reg_temp | (1 << 16)       // 9
                       | (1 << 17)                // 8
                       | (1 << 26);               // 6
            reg_temp = reg_temp & (~(1 << 15));   // 12
            if (i_num == 1)                       //
              reg_temp = reg_temp & (~(1 << 25)); // -
            else
              reg_temp = reg_temp | (1 << 25); // -
          }
      };
      break;

    case OUT_DIG_2: // digit 2, 0=9 1=12,8,6 - i_num=11.7.4.2.1.10.5
      {
        reg_temp = reg_temp | (1 << 15)     // 12
                   | (1 << 17)              // 8
                   | (1 << 26);             // 6
        reg_temp = reg_temp & (~(1 << 16)); // 9
        reg_temp = reg_set_num (reg_temp, i_num);
      };
      break;
    case OUT_DIG_3: // digit 3, 0=8 1=12,9,6 - i_num=11.7.4.2.1.10.5
      {
        reg_temp = reg_temp | (1 << 15)     // 12
                   | (1 << 16)              // 9
                   | (1 << 26);             // 6
        reg_temp = reg_temp & (~(1 << 17)); // 8
        reg_temp = reg_set_num (reg_temp, i_num);
      };
      break;

    case OUT_C_4: // digit 4, 0=6 1=12,9,8 - i_num=11.7.4.2.1.10.5
      {
        if (iMode == MODE_CLOCK) // case OUT_DIG_4: //digit 4, 0=6 1=12,9,8 -
                                 // i_num=11.7.4.2.1.10.5
          {
            reg_temp = reg_temp | (1 << 15)     // 12
                       | (1 << 16)              // 9
                       | (1 << 17);             // 8
            reg_temp = reg_temp & (~(1 << 26)); // 6
            reg_temp = reg_set_num (reg_temp, i_num);
          }
        else
          {

            reg_temp = reg_temp | (1 << 15)     // 12
                       | (1 << 16)              // 9
                       | (1 << 17);             // 8
            reg_temp = reg_temp & (~(1 << 26)); // 6
            reg_temp = reg_temp | (1 << 10)     // 11 A
                       | (1 << 22)              // 2  D
                       | (1 << 21)              // 1  E
                       | (1 << 11)              // 10 F
                ;                               //
            reg_temp = reg_temp & (~(1 << 18))  // 1  B
                       & (~(1 << 25))           // 1  G
                       & (~(1 << 24))           // 1  C
                ;
          }
      };
      break;

    default: // off ligth
      {
        reg_temp = reg_temp & (~(1 << 15)) & (~(1 << 17)) & (~(1 << 26))
                   & (~(1 << 16)) & (~(1 << 10)) & (~(1 << 18)) & (~(1 << 24))
                   & (~(1 << 22)) & (~(1 << 21)) & (~(1 << 11)) & (~(1 << 25))
                   & (~(1 << 23)); /* write low */
        i_off = 1;
      };
      break;
    }
  tls_reg_write32 (HR_GPIO_DATA + offset, reg_temp); /* write  */

  tls_reg_write32 (HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

  tls_os_release_critical (cpu_sr); // enable Interrupt
}
