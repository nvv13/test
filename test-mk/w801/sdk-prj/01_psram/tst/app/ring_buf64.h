#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define BUF_SIZE                                                              \
  64 // 8,16,32,64,128,256,512,1024 ... размер буфера обязательно должен быть
// равен степени двойки! 4096 = 262144 byte
#define BUF_MASK (BUF_SIZE - 1)
#define BUF_BYTE_CHUNK_SIZE 64 // 

/* 256 x 32 = 8192, буффер, работает без сбоев, странно... */
/* 64 x 32 = 2048, буффер, работает без сбоев, странно... */
/* 64 x 64 = 4096, буффер, работает без сбоев, странно... */
/* 128 x 64 = 8192, сбоит! что то аппаратное наверно */

  u8 *CreateRingBuf (void);

  u8 *PushRingBuf (void);

  u8 *PopRingBuf (void);

  void ResetRingBuf (void);

  int RingBufSpacesAvailable (void);

  u8 *HeadRingBuf (void);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */
/** @} */
