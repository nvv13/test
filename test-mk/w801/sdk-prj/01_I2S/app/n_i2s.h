/**
 * @file    n_i2s.h
 *
 * @brief   play wav file to i2s interface
 *
 * @author  nvv13
 *
 * Volume Algoritm used from
 * https://github.com/pschatzmann/arduino-audio-tools.git
 * https://github.com/pschatzmann/arduino-audio-tools/blob/main/License.txt
 * GNU General Public License v3.0
 *
 */

#ifndef N_I2S_H
#define N_I2S_H

#if defined(__cplusplus)
extern "C"
{
#endif

#include "ff.h"

  typedef enum n_i2s_status
  {
    N_I2S_NONE = 0,
    N_I2S_HW_INIT = 1,
    N_I2S_PLAY = 2,
    N_I2S_END_FILE = 3,
    N_I2S_STOP = 4,
    N_I2S_QUERY_TO_STOP = 5,
  } _n_i2s_status;

  /**
   * @brief          This function is used to init i2s interface
   *
   * @param[in]      None
   *
   * @return         None
   *
   * @note          PA08  clock line         i2s Bclk
                    PA09  word select line   i2s LRclk
                    PA10  Dout
                    PA11  Din - not used from Play
   */
  void n_i2s_init_hw (void);

  /**
   * @brief          This function is used to Play Wav File
   *
   * @param[in]      filename
   *
   * @return         FRESULT
   *
   * @note
   */
  FRESULT n_i2s_PlayWav (char *filename);

  /**
   * @brief          This function is used to Play Mp3 File
   *
   * @param[in]      filename
   *
   * @return         FRESULT
   *
   * @note
   */
  FRESULT n_i2s_PlayMp3 (char *filename);

  /**
   * @brief          This function return status
   *
   * @param[in]      none
   *
   * @return         enum n_i2s_status
   *
   * @note
   */
  enum n_i2s_status n_i2s_get_status (void);

  /**
   * @brief          This function is used to Stop Play Wav File
   *
   * @param[in]      none
   *
   * @return         none
   *
   * @note
   */
  void n_i2s_stop (void);

  /**
   * @brief          This function is used to Set Volume
   *
   * @param[in]      Procent Of Voleme 0..100 %
   *
   * @return         none
   *
   * @note
   */
  void n_i2s_SetVolume (u8 procVol);

#if defined(__cplusplus)
}
#endif

#endif /* N_I2S_H */
