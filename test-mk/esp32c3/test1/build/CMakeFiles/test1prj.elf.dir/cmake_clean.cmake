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
  "CMakeFiles/test1prj.elf.dir/project_elf_src_esp32c3.c.obj"
  "project_elf_src_esp32c3.c"
  "test1prj.elf"
  "test1prj.elf.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/test1prj.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
