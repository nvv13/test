#include "wm_type_def.h"

#pragma once

#define SdCsPin_t u8

#define SD_SCK_MHZ(x) (1000 * x)
#define SS 6

class FsFile
{
public:
  FsFile (){};
  u32
  open (char *FileName)
  {
    return 0;
  };
  u32
  seek (u32 pos)
  {
    return 0;
  };
  u32
  read (u8 *buf, u32 len)
  {
    return 0;
  };
  u32
  position (void)
  {
    return 0;
  };
  operator bool () const
  {
    return true; // Or false!
  }
  u32
  size (void)
  {
    return 0;
  };
  void close (void){};
  void getName (char *buf, u16 len){};
};

class SdFat
{
public:
  SdFat (){};
  bool
  begin (SdCsPin_t csPin, uint32_t maxSck)
  {
    return 0;
  };
  u32
  clusterCount (void)
  {
    return 0;
  };
  u32
  freeClusterCount (void)
  {
    return 0;
  };
  u32
  bytesPerCluster (void)
  {
    return 512;
  };
  bool
  exists (char *audioName)
  {
    return true;
  };
  FsFile
  open (char *audioName)
  {
    FsFile o_FF = FsFile ();
    return o_FF;
  }; // #86
};
