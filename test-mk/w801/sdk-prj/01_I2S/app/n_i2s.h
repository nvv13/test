/**
 * @file    n_i2s.h
 *
 * @brief   play wav file to i2s interface
 *
 * @author  nvv13
 *
 */

#ifndef N_I2S_H
#define N_I2S_H

#if defined(__cplusplus)
extern "C"
{
#endif

#include "ff.h"



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
void init_i2s (void);


/**
 * @brief          This function is used to Play Wav File
 *
 * @param[in]      filename
 *
 * @return         FRESULT
 *
 * @note         
 */
FRESULT WAV_loadWav (char *filename);


#if defined(__cplusplus)
}
#endif

#endif /* N_I2S_H */
