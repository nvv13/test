#ifndef N_CONVERT_UTF8_TO_WIN1251_H
#define N_CONVERT_UTF8_TO_WIN1251_H

#if defined(__cplusplus)
extern "C"
{
#endif

#include "wm_type_def.h"

  /**
   * @brief     convert utf8 to windows1251
   * @param[in] const char* utf8, char* windows1251, size_t n
   *            return true - sucsess
   * @note      none
   */
  bool n_convert_utf8_to_windows1251 (const char *utf8, char *windows1251,
                                      size_t n);

#if defined(__cplusplus)
}
#endif

#endif /* N_CONVERT_UTF8_TO_WIN1251_H */
