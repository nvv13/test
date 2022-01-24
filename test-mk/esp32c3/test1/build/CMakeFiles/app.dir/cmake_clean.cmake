file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "project_elf_src_esp32c3.c"
  "test1prj.bin"
  "test1prj.map"
  "CMakeFiles/app"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/app.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
