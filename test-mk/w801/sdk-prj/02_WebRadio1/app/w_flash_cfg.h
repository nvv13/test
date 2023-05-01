#ifndef W_FLASH_CFG_H
#define W_FLASH_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif

  int flash_cfg_load_u16 (u16 *u16_value, u8 u8_index);
  int flash_cfg_store_u16 (u16 u16_value, u8 u8_index);

#ifdef __cplusplus
}
#endif

#endif /* W_FLASH_CFG_H */