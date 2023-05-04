
#include "wm_type_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_mem.h"
#include "wm_regs.h"

#include "w_flash_cfg.h"
#include "wm_internal_flash.h"

#define USER_U16_FLASH_BUF_SIZE 256 // 36+28 * 4

int
flash_cfg_load_u16 (u16 *u16_value, u8 u8_index)
{
  if (u8_index > 127)
    {
      printf ("\nflash_cfg_load_u16 error, invalid index (0...127) = %d\n",
              u8_index);
      return WM_FAILED;
    }

  u8 *read_buf = NULL;

  tls_fls_init (); // initialize flash driver

  read_buf = tls_mem_alloc (USER_U16_FLASH_BUF_SIZE);
  if (NULL == read_buf)
    {
      printf ("\nmalloc read buf error\n");
      return WM_FAILED;
    }
  memset (read_buf, 0, USER_U16_FLASH_BUF_SIZE);
  tls_fls_read (0x1F0303, read_buf, USER_U16_FLASH_BUF_SIZE);

  (*u16_value) = ((u16 *)read_buf)[u8_index];

  tls_mem_free (read_buf);

  return WM_SUCCESS;
}

int
flash_cfg_store_u16 (u16 u16_value, u8 u8_index)
{
  if (u8_index > 127)
    {
      printf ("\nflash_cfg_store_u16 error, invalid index (0...127) = %d\n",
              u8_index);
      return WM_FAILED;
    }

  u8 *write_buf = NULL;

  tls_fls_init (); // initialize flash driver

  write_buf = tls_mem_alloc (USER_U16_FLASH_BUF_SIZE);
  if (NULL == write_buf)
    {
      printf ("\nmalloc write buf error\n");
      return WM_FAILED;
    }

  memset (write_buf, 0, USER_U16_FLASH_BUF_SIZE);
  tls_fls_read (0x1F0303, write_buf, USER_U16_FLASH_BUF_SIZE);

  ((u16 *)write_buf)[u8_index] = u16_value;

  tls_fls_write (0x1F0303, write_buf, USER_U16_FLASH_BUF_SIZE);

  tls_mem_free (write_buf);

  return WM_SUCCESS;
}

int
flash_cfg_load_stantion_name (char *name_buf_len29, u8 u8_index)
{
  if (u8_index > 27)
    {
      printf ("\nflash_cfg_load_stantion_name error, invalid index (0...27) = "
              "%d\n",
              u8_index);
      return WM_FAILED;
    }

  u16 u16_mem_start = USER_U16_FLASH_BUF_SIZE * ((u8) (u8_index / 4))
                      + USER_U16_FLASH_BUF_SIZE;

  u8 *read_buf = NULL;

  tls_fls_init (); // initialize flash driver

  read_buf = tls_mem_alloc (USER_U16_FLASH_BUF_SIZE);
  if (NULL == read_buf)
    {
      printf ("\nmalloc read buf error\n");
      return WM_FAILED;
    }
  memset (read_buf, 0, USER_U16_FLASH_BUF_SIZE);
  tls_fls_read (0x1F0303 + u16_mem_start, read_buf, USER_U16_FLASH_BUF_SIZE);

  // uuid	36
  // name	28
  // 36+28=64

  memcpy (name_buf_len29, (read_buf + (u8_index % 4) * 64 + 36), 28);
  name_buf_len29[28] = 0;

  tls_mem_free (read_buf);

  return WM_SUCCESS;
}

int
flash_cfg_store_stantion_name (char *name_buf_len28, u8 u8_index)
{
  if (u8_index > 27)
    {
      printf ("\nflash_cfg_store_stantion_name error, invalid index (0...27) "
              "= %d\n",
              u8_index);
      return WM_FAILED;
    }

  u16 u16_mem_start = USER_U16_FLASH_BUF_SIZE * ((u8) (u8_index / 4))
                      + USER_U16_FLASH_BUF_SIZE;

  u8 *write_buf = NULL;

  tls_fls_init (); // initialize flash driver

  write_buf = tls_mem_alloc (USER_U16_FLASH_BUF_SIZE);
  if (NULL == write_buf)
    {
      printf ("\nmalloc write buf error\n");
      return WM_FAILED;
    }

  memset (write_buf, 0, USER_U16_FLASH_BUF_SIZE);
  tls_fls_read (0x1F0303 + u16_mem_start, write_buf, USER_U16_FLASH_BUF_SIZE);

  // uuid	36
  // name	28
  // 36+28=64

  memcpy ((write_buf + (u8_index % 4) * 64 + 36), name_buf_len28, 28);

  tls_fls_write (0x1F0303 + u16_mem_start, write_buf, USER_U16_FLASH_BUF_SIZE);

  tls_mem_free (write_buf);

  return WM_SUCCESS;
}

int
flash_cfg_load_stantion_uuid (char *uuid_buf_len37, u8 u8_index)
{
  if (u8_index > 27)
    {
      printf ("\nflash_cfg_load_stantion_uuid error, invalid index (0...27) = "
              "%d\n",
              u8_index);
      return WM_FAILED;
    }

  u16 u16_mem_start = USER_U16_FLASH_BUF_SIZE * ((u8) (u8_index / 4))
                      + USER_U16_FLASH_BUF_SIZE;

  u8 *read_buf = NULL;

  tls_fls_init (); // initialize flash driver

  read_buf = tls_mem_alloc (USER_U16_FLASH_BUF_SIZE);
  if (NULL == read_buf)
    {
      printf ("\nmalloc read buf error\n");
      return WM_FAILED;
    }
  memset (read_buf, 0, USER_U16_FLASH_BUF_SIZE);
  tls_fls_read (0x1F0303 + u16_mem_start, read_buf, USER_U16_FLASH_BUF_SIZE);

  // uuid	36
  // name	28
  // 36+28=64

  memcpy (uuid_buf_len37, (read_buf + (u8_index % 4) * 64), 36);
  uuid_buf_len37[36] = 0;

  tls_mem_free (read_buf);

  return WM_SUCCESS;
}

int
flash_cfg_store_stantion_uuid (char *uuid_buf_len36, u8 u8_index)
{
  if (u8_index > 27)
    {
      printf ("\nflash_cfg_store_stantion_uuid error, invalid index (0...27) "
              "= %d\n",
              u8_index);
      return WM_FAILED;
    }

  u16 u16_mem_start = USER_U16_FLASH_BUF_SIZE * ((u8) (u8_index / 4))
                      + USER_U16_FLASH_BUF_SIZE;

  u8 *write_buf = NULL;

  tls_fls_init (); // initialize flash driver

  write_buf = tls_mem_alloc (USER_U16_FLASH_BUF_SIZE);
  if (NULL == write_buf)
    {
      printf ("\nmalloc write buf error\n");
      return WM_FAILED;
    }

  memset (write_buf, 0, USER_U16_FLASH_BUF_SIZE);
  tls_fls_read (0x1F0303 + u16_mem_start, write_buf, USER_U16_FLASH_BUF_SIZE);

  // uuid	36
  // name	28
  // 36+28=64

  memcpy ((write_buf + (u8_index % 4) * 64), uuid_buf_len36, 36);

  tls_fls_write (0x1F0303 + u16_mem_start, write_buf, USER_U16_FLASH_BUF_SIZE);

  tls_mem_free (write_buf);

  return WM_SUCCESS;
}

int
flash_cfg_find_stantion_id_by_uuid (char *uuid_buf_len36)
{
  char uuid_buf_len37[37];
  for (int i_cnt = 0; i_cnt < 28; i_cnt++)
    {
      flash_cfg_load_stantion_uuid (uuid_buf_len37, i_cnt);
      if (strlen(uuid_buf_len37) == 36 && strlen(uuid_buf_len36) == 36 && strstr (uuid_buf_len36, uuid_buf_len37) != NULL)
        {
          return i_cnt;
        }
    }
  return -1;
}
