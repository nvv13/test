#ifndef W_FLASH_CFG_H
#define W_FLASH_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define USER_CNT_REC_STANTION_NAME                                            \
  44 // 28/4*256 = 1792 +256 = 2048
     // 44/4*256 = 2816 +256 = 3072

  int flash_cfg_load_u16 (u16 *u16_value, u8 u8_index);
  int flash_cfg_store_u16 (u16 u16_value, u8 u8_index);

  int flash_cfg_load_stantion_name (char *name_buf_len29, u8 u8_index);
  int flash_cfg_store_stantion_name (char *name_buf_len28, u8 u8_index);

  int flash_cfg_load_stantion_uuid (char *uuid_buf_len37, u8 u8_index);
  int flash_cfg_store_stantion_uuid (char *uuid_buf_len36, u8 u8_index);

  int flash_cfg_find_stantion_id_by_uuid (char *uuid_buf_len36);

#ifdef __cplusplus
}
#endif

#endif /* W_FLASH_CFG_H */