file(REMOVE_RECURSE
  "bootloader.bin"
  "bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "project_elf_src_esp32s2.c"
  "CMakeFiles/dfu-flash"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/dfu-flash.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
