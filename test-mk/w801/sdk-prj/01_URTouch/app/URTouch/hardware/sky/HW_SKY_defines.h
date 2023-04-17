#ifndef HW_SKY_DEFINES_H
#define HW_SKY_DEFINES_H

//********************************************************************


static void cbi (uint32_t *PORT_REG, u8 PIN);
static void sbi (uint32_t *PORT_REG, u8 PIN);
static u8 rbi (uint32_t *PORT_REG, u8 PIN);

#define pulse_high(reg, bitmask)                                              \
  sbi (reg, bitmask);                                                         \
  cbi (reg, bitmask);
#define pulse_low(reg, bitmask)                                               \
  cbi (reg, bitmask);                                                         \
  sbi (reg, bitmask);


#define swap_word(i, j)                                                       \
  {                                                                           \
    word t = i;                                                               \
    i = j;                                                                    \
    j = t;                                                                    \
  }
#define swap_int(i, j)                                                        \
  {                                                                           \
    int t = i;                                                                \
    i = j;                                                                    \
    j = t;                                                                    \
  }


//#define regtype volatile uint32_t
#define regtype uint32_t
#define regsize u8


#endif /* HW_SKY_DEFINES_H */
