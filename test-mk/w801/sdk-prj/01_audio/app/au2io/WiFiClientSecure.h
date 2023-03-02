#pragma once

#include "wm_type_def.h"

class WiFiClient
{
public:
  WiFiClient (){};
  int
  read ()
  {
    return 0;
  };
  int
  read (u8 *buf, int len)
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
  u32
  available (void)
  {
    return 0;
  };
  bool
  connected (void)
  {
    return true;
  };
  void print (char *ch){};
  bool
  connect (char *host, u16 port)
  {
    return true;
  };
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