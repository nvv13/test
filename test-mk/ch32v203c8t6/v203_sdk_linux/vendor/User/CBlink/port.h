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
    PA_08,
    PA_09,
    PA_10,
    PA_11,
    PA_12,
    PA_13,
    PA_14,
    PA_15,

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
    PC_08,
    PC_09,
    PC_10,
    PC_11,
    PC_12,
    PC_13,
    PC_14,
    PC_15,

    PD_00,
    PD_01,
    PD_02,
    PD_03,
    PD_04,
    PD_05,
    PD_06,
    PD_07,
    PD_08,
    PD_09,
    PD_10,
    PD_11,
    PD_12,
    PD_13,
    PD_14,
    PD_15,

    GPIO_UNDEF,
  } pin_t;

#ifdef __cplusplus
}
#endif

#endif /* __PORT_H */
