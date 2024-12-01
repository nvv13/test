#ifndef __PORT_H
#define __PORT_H

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum
  {
    PA_00 = 0,
    PA_01,
    PA_02,
    PA_03,
    PA_04,
    PA_05,
    PA_06,
    PA_07,
    PB_00,
    PB_01,
    PB_02,
    PB_03,
    PB_04,
    PB_05,
    PB_06,
    PB_07,
    PB_08,
    PB_09,
    PB_10,
    PB_11,
    PB_12,
    PB_13,
    PB_14,
    PB_15,

    PC_00,
    PC_01,
    PC_02,
    PC_03,
    PC_04,
    PC_05,
    PC_06,
    PC_07,
    PD_00,
    PD_01,
    PD_02,
    PD_03,
    PD_04,
    PD_05,
    PD_06,
    PD_07,
    GPIO_UNDEF,
  } pin_t;

#ifdef __cplusplus
}
#endif

#endif /* __PORT_H */
