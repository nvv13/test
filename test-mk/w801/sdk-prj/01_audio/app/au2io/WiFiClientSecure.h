#pragma once

#include "wm_type_def.h"

class WiFiClient
{
protected:
  int socket_num;
  u16 timeout_ms;

  char sock_rx[512] = { 0 };
  char sock_tx[512] = { 0 };

  int recv_ret;
public:
  WiFiClient ();
  u32 available (void); // есть доступные данные в принятом буфере
  char read (); // чтение по 1 char из потока (из буфера?), его и возвращает
  int read (u8 *buf, int len); //возвращает длинну прочтенных байтов в буфер
  u16 readBytes (char *buff, u16 len);
  u16 readBytes (u8 *buff, u16 len);
  bool connected (void); //соединение есть
  bool n_connect (char *host, u16 port);
  void print (char *ch); //пишем в поток сайта к которому подключились
  void stop (void);
  void flush (void); // release memory
  bool n_connect (char *hostwoext, u16 port, u16 m_timeout_ms);
};

class WiFiClientSecure : public WiFiClient
{
public:
  WiFiClientSecure (){};
  void setInsecure (void){};
};