#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define BUF_BYTE_CHUNK_SIZE 64 //

  u8 *CreateRingBuf (void);

  u8 PushRingBufChunk (u8 *Buffer);

  u8 PopRingBufChunk (u8 *Buffer);

  void ResetRingBuf (void);

  int RingBufChunkAvailable (void);

  u8 *HeadRingBuf (void);

  int RingBufChunkTotal (void);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */
/** @} */
