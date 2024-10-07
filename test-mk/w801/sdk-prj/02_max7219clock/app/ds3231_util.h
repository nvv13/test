#ifndef DS3231_UTIL_H
#define DS3231_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mod1/ds3231.h"

#define ISOSTR_LEN (20U)

  extern ds3231_t ds3231_dev;

  int ds3231_cmd_get (int argc, char **argv);

  int ds3231_cmd_set (int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* DS3231_UTIL_H */
