file(REMOVE_RECURSE
  "blink.bin"
  "blink.map"
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "project_elf_src_esp32c3.c"
  "CMakeFiles/uf2-app"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/uf2-app.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
