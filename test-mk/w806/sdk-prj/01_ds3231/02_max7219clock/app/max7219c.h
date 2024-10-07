#ifndef MAX7219C_H
#define MAX7219C_H


#ifdef __cplusplus
extern "C"
{
#endif


  void max7219_clear (void);
  int max7219_refresh (void);

  int max7219_int_get (int argc, char **argv);

  int max7219_int_set (int argc, char **argv);

u8 max7219_get_h_1(void);
u8 max7219_get_h_2(void);
u8 max7219_get_m_1(void);
u8 max7219_get_m_2(void);
u8 max7219_get_c_sec(void);

void max7219_set_h_1(u8 val);
void max7219_set_h_2(u8 val);
void max7219_set_m_1(u8 val);
void max7219_set_m_2(u8 val);
void max7219_set_c_sec(u8 val);


  u8 max7219_get_intensity(void);
  void max7219_set_intensity(u8 val);
  //-----------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* MAX7219C_H */
