#pragma once

#include "wm_type_def.h"

class WiFiClient
{
public:
  WiFiClient (){};

  u32
  available (void) // есть доступные данные в принятом буфере
  {
    return 0;
  };
  int
  read () // чтение по 1 char из потока (из буфера?), его и возвращает
  {
    return 0;
  };
  int
  read (u8 *buf, int len) //возвращает длинну прочтенных байтов в буфер
  {
    return 0;
  };
  u16
  readBytes (char *buff, u16 len)
  {
    return 0;
  };
  u16
  readBytes (u8 *buff, u16 len)
  {
    return 0;
  };
  u16
  readBytes (char *buff, u16 *pos, u16 len)
  {
    return 0;
  };
  u16
  readBytes (u8 *buff, u16 *pos, u16 len)
  {
    return 0;
  };

  bool
  connected (void) //соединение есть
  {
    return true;
  };

  bool
  connect (char *host, u16 port)
  {
    return true;
  };

  void print (char *ch){}; //пишем в поток сайта к которому подключились

  void stop (void){};

  void flush (void){}; // release memory

  int
  connect (char *hostwoext, u16 port, u16 m_timeout_ms)
  {
    return 0;
  };
};

class WiFiClientSecure : public WiFiClient
{
public:
  WiFiClientSecure (){};
  void setInsecure (void){};
};