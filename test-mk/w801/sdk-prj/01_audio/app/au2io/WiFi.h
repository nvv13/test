#pragma once

#define ESP_OK 0
#define ESP_ERR_INVALID_ARG 1

extern "C" size_t xPortGetFreeHeapSize (void);

class my_ESP
{
public:
  my_ESP (){};
  size_t
  getFreeHeap (void)
  {
    return xPortGetFreeHeapSize ();
  };
};

// extern my_ESP ESP;
