@REM 
@REM  для Windows
@REM  "поробовать" risc-v
@REM  
@REM  качаем файлики
@REM 
@REM   https://repo.openeuler.org/openEuler-preview/RISC-V/openEuler-22.09-riscv64/QEMU/fw_payload_oe_qemuvirt.elf
@REM   https://cdimage.ubuntu.com/releases/20.04.2/release/ubuntu-20.04.2-preinstalled-server-riscv64.img.xz
@REM   
@REM   распаковать
@REM   ubuntu-20.04.2-preinstalled-server-riscv64.img.xz 
@REM   
@REM   
@REM   сделаем побольше 
@REM   qemu-img resize ubuntu-20.04.2-preinstalled-server-riscv64.img +10G
@REM 
@REM   login: ubuntu
@REM   password: ubuntu
@REM 
@REM   при первом старте требует сделать новый пароль, сделал ubuntu100
@REM 
@REM   login: ubuntu
@REM   password: ubuntu100


C:\PROGRA~1\qemu\qemu-system-riscv64 ^
   -nographic -machine virt ^
   -m 4G ^
   -smp cpus=4 ^
   -kernel fw_payload_oe_qemuvirt.elf ^
   -bios none ^
          -drive file=ubuntu-20.04.2-preinstalled-server-riscv64.img,format=raw,id=hd0,if=none ^
          -netdev user,id=eth0,hostfwd=tcp::5900-:5900,hostfwd=tcp::10000-:22 ^
          -device virtio-blk-device,drive=hd0 ^
          -device virtio-net-device,netdev=eth0 ^
          -device virtio-keyboard -device virtio-tablet ^
          -append "root=/dev/vda1"

