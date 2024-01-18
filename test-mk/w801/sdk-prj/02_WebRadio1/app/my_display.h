

static u8g2_t u8g2;

static u8 i_switch_menu = 0;
static int i_menu = 0;
static int i_menu2 = 0;
static int i_menu3 = 0;
static u8 u8_ind_ch_st = 0;

static u16 u16_volume = 0; //
static char buf_str_ind[50];
static char stantion_uuid[39];
static int i_find_stantion_id = -1;
static char stantion_name_temp[50];
static char stantion_uuid_temp[39];

static void display_refresh (void);

#define MENU_STORE_VOLUME 0
#define MENU_STORE_INDEX 1
#define MENU_MAX_POS (USER_CNT_REC_STANTION_NAME * 2)

#define MENU2_MAX_POS (MAX_INDEX_LOAD_FIND - 1)

void
MenuActionClick (void)
{
  if (i_menu == MENU_STORE_VOLUME)
    flash_cfg_store_u16 (u16_volume, MENU_STORE_VOLUME);
  else
    {
      u8 u8_stantion_id = (i_menu / 2);
      if ((u8_stantion_id * 2) == i_menu && u8_stantion_id > 0)
        u8_stantion_id--;
      //
      // printf ("MenuActionClick i_menu %d,i_menu2 %d,u8_stantion_id
      // %d\n",i_menu,i_menu % 2,u8_stantion_id);
      if (i_menu % 2 != 0)
        {
          flash_cfg_load_stantion_uuid (stantion_uuid, u8_stantion_id);
          // printf ("flash_cfg_load stantion_uuid %s,u8_stantion_id
          // %d\n",stantion_uuid,u8_stantion_id);
          VS1053_stop_PlayMP3 ();
        }
      else
        {
          i_find_stantion_id = flash_cfg_find_stantion_id_by_uuid (
              my_recognize_ret_stationuuid (u8_ind_ch_st));
          if (i_find_stantion_id == -1 || i_find_stantion_id == u8_stantion_id)
            {
              flash_cfg_store_stantion_name (
                  my_recognize_ret_name (u8_ind_ch_st), u8_stantion_id);
              flash_cfg_store_stantion_uuid (
                  my_recognize_ret_stationuuid (u8_ind_ch_st), u8_stantion_id);
            }
          else
            { //Если нашло в другом месте, то обмен позиций
              flash_cfg_load_stantion_uuid (stantion_uuid_temp,
                                            u8_stantion_id);
              flash_cfg_load_stantion_name (stantion_name_temp,
                                            u8_stantion_id);

              flash_cfg_store_stantion_name (stantion_name_temp,
                                             i_find_stantion_id);
              flash_cfg_store_stantion_uuid (stantion_uuid_temp,
                                             i_find_stantion_id);

              flash_cfg_store_stantion_name (
                  my_recognize_ret_name (u8_ind_ch_st), u8_stantion_id);
              flash_cfg_store_stantion_uuid (
                  my_recognize_ret_stationuuid (u8_ind_ch_st), u8_stantion_id);
            }
          printf ("i_find_stantion_id=%d, u8_ind_ch_st=%d, u8_stantion_id=%d "
                  "name=%s uuid=%s\n",
                  i_find_stantion_id, u8_ind_ch_st, u8_stantion_id,
                  my_recognize_ret_name (u8_ind_ch_st),
                  my_recognize_ret_stationuuid (u8_ind_ch_st));
          display_refresh ();
        }
    }
}

void
Menu2ActionClick (void)
{
  u8_ind_ch_st = i_menu2;
  sprintf (stantion_uuid, my_recognize_ret_stationuuid (u8_ind_ch_st));
  // printf ("flash_cfg_load stantion_uuid %s,u8_stantion_id
  // %d\n",stantion_uuid,u8_stantion_id);
  VS1053_stop_PlayMP3 ();
}

void
Menu3ActionClick (void)
{
  VS1053_stop_PlayMP3 ();
}

static void
display_menu_stantion_pos (u8 u8_stantion_id, u8 u8_stpos)
{
  u8 u8_Page_Disp = u8_stantion_id / 4; //по 4 станции влезает на дисплей

  bool l_name_choice = ((u8_Page_Disp * 4 + u8_stpos) == u8_stantion_id
                        && i_menu % 2 != 0 && i_menu != 0);
  bool l_store_choice = ((u8_Page_Disp * 4 + u8_stpos) == u8_stantion_id
                         && i_menu % 2 == 0 && i_menu != 0);

  if (l_name_choice)
    u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
  else
    u8g2_SetFont (&u8g2, u8g2_font_4x6_t_cyrillic);
  sprintf (buf_str_ind, "%.2d.", (u8_Page_Disp * 4 + u8_stpos));
  u8g2_DrawStr (&u8g2, 1, 30 + u8_stpos * 10, buf_str_ind);
  flash_cfg_load_stantion_name (buf_str_ind, (u8_Page_Disp * 4 + u8_stpos));
  if (buf_str_ind[0] == 0xd0)
    buf_str_ind[(l_store_choice ? 24 : 28)] = 0;
  else
    buf_str_ind[(l_store_choice ? 12 : 14)] = 0;
  u8g2_DrawUTF8 (&u8g2, 16, 30 + u8_stpos * 10, buf_str_ind);

  if (!l_name_choice)
    {
      if (l_store_choice)
        {
          u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
          sprintf (buf_str_ind, "Store%.2d", (u8_Page_Disp * 4 + u8_stpos));
          u8g2_DrawStr (&u8g2, 80, 30 + u8_stpos * 10, buf_str_ind);
        }
      else
        {
          u8g2_SetFont (&u8g2, u8g2_font_4x6_t_cyrillic);
          sprintf (buf_str_ind, "St%.2d", (u8_Page_Disp * 4 + u8_stpos));
          u8g2_DrawStr (&u8g2, 100, 30 + u8_stpos * 10, buf_str_ind);
        }
    }
}

volatile static u16 i_delay_WAIT
    = 0; //если не 0, значит ждем данных с сервера веб станции

static void
display_refresh (void)
{
  u8g2_FirstPage (&u8g2);
  do
    {
      u8g2_SetDrawColor (&u8g2, 1);
      if (i_switch_menu == 0)
        {
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          sprintf (buf_str_ind, "vol:%.3d", u16_volume);
          u8g2_DrawStr (&u8g2, 10, 20, buf_str_ind);
          u8g2_SetFont (&u8g2, u8g2_font_5x7_t_cyrillic);
          u8g2_DrawUTF8 (&u8g2, 1, 30, my_recognize_ret_name (u8_ind_ch_st));
          u8g2_DrawUTF8 (&u8g2, 1, 40, my_recognize_ret_tags (u8_ind_ch_st));
          u8g2_DrawStr (&u8g2, 1, 50, my_recognize_ret_country (u8_ind_ch_st));
          u8g2_DrawStr (&u8g2, 1, 60, my_recognize_ret_codec (u8_ind_ch_st));
          u8g2_DrawStr (&u8g2, 60, 60,
                        my_recognize_ret_bitrate (u8_ind_ch_st));
          u8g2_DrawStr (
              &u8g2, 90, 60,
              (my_recognize_ret_https (u8_ind_ch_st) ? "https" : "http"));
        }
      if (i_switch_menu == 1)
        {
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          sprintf (buf_str_ind, "Menu1");
          u8g2_DrawStr (&u8g2, 0, 20, buf_str_ind);
          //

          u8g2_SetFont (&u8g2, u8g2_font_5x8_t_cyrillic);
          sprintf (buf_str_ind, "%d", i_menu);
          u8g2_DrawStr (&u8g2, 75, 10, buf_str_ind);

          if (i_menu == MENU_STORE_VOLUME)
            u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
          else
            u8g2_SetFont (&u8g2, u8g2_font_5x7_t_cyrillic);
          u8g2_DrawStr (&u8g2, 75, 20, "St.Vol");

          u8 u8_stantion_id = (i_menu / 2);
          if ((u8_stantion_id * 2) == i_menu && u8_stantion_id > 0)
            u8_stantion_id--;
          display_menu_stantion_pos (u8_stantion_id, 0);
          display_menu_stantion_pos (u8_stantion_id, 1);
          display_menu_stantion_pos (u8_stantion_id, 2);
          display_menu_stantion_pos (u8_stantion_id, 3);

          if (i_find_stantion_id >= 0)
            {
              u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
              sprintf (buf_str_ind, "St%.2d.OV(%.2d)", u8_stantion_id,
                       i_find_stantion_id);
              u8g2_DrawStr (&u8g2, 0, 45, buf_str_ind);
              i_find_stantion_id = -1;
              i_delay_WAIT = 1;
            }
        }

      if (i_switch_menu == 2)
        {
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          sprintf (buf_str_ind, "Menu2");
          u8g2_DrawStr (&u8g2, 0, 20, buf_str_ind);

          u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
          sprintf (buf_str_ind, "%d", i_menu2);
          u8g2_DrawStr (&u8g2, 85, 15, buf_str_ind);

          sprintf (buf_str_ind, my_recognize_ret_name (i_menu2));
          u8g2_DrawUTF8 (&u8g2, 1, 38, buf_str_ind);

          u8g2_SetFont (&u8g2, u8g2_font_5x7_t_cyrillic);
          u8g2_DrawStr (&u8g2, 1, 60, my_recognize_ret_codec (i_menu2));
          u8g2_DrawStr (&u8g2, 60, 60, my_recognize_ret_bitrate (i_menu2));
          u8g2_DrawStr (&u8g2, 90, 60,
                        (my_recognize_ret_https (i_menu2) ? "https" : "http"));
        }

      if (i_switch_menu == 3)
        {
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          sprintf (buf_str_ind, "Menu3");
          u8g2_DrawStr (&u8g2, 0, 20, buf_str_ind);

          u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
          sprintf (buf_str_ind, "%d", i_menu3);
          u8g2_DrawStr (&u8g2, 85, 15, buf_str_ind);

          sprintf (buf_str_ind, aUrl[i_menu3*2]);
          u8g2_DrawUTF8 (&u8g2, 1, 38, buf_str_ind);

          u8g2_SetFont (&u8g2, u8g2_font_5x7_t_cyrillic);
//          u8g2_DrawStr (&u8g2, 1, 60, my_recognize_ret_codec (i_menu3));
          u8g2_DrawStr (&u8g2, 60, 1, aUrl[i_menu3*2+1]);
//          u8g2_DrawStr (&u8g2, 90, 60,
//                        (my_recognize_ret_https (i_menu3) ? "https" : "http"));
        }

      if (VS1053_status_get_status () != VS1053_PLAY
          || VS1053_WEB_RADIO_nTotal < 512)
        {
          u8g2_SetDrawColor (&u8g2, 1);
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          u8g2_DrawStr (&u8g2, 30, 45, "WAIT...");
        }
    }
  while (u8g2_NextPage (&u8g2));
}
