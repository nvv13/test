file(REMOVE_RECURSE
  "bootloader.bin"
  "bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "project_elf_src_esp32c3.c"
  "CMakeFiles/save-defconfig"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/save-defconfig.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
