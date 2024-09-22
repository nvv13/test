#!/bin/bash
#
# для Linux
# "поробовать" risc-v
#
# install qemu
#
#  dnf install qemu-system-riscv
#  dnf install qemu-img
#  
#   source:
#  https://wiki.ubuntu.com/RISC-V/QEMU
#  
# prepare files
#
#  wget https://repo.openeuler.org/openEuler-preview/RISC-V/openEuler-22.09-riscv64/QEMU/fw_payload_oe_qemuvirt.elf
#  wget https://github.com/smist09/riscv-assembly-language/raw/refs/heads/main/Chapter%201/QEMU/fw_jump.elf
#  wget https://github.com/smist09/riscv-assembly-language/raw/refs/heads/main/Chapter%201/QEMU/uboot.elf
#  
#  wget https://cdimage.ubuntu.com/releases/noble/release/ubuntu-24.04.1-preinstalled-server-riscv64.img.xz
#  xz -dk ubuntu-24.04.1-preinstalled-server-riscv64.img.xz 
#  qemu-img resize ubuntu-24.04.1-preinstalled-server-riscv64.img +10G
#
#  login: ubuntu
#  password: ubuntu
#
#  after start repass:
#  login: ubuntu
#  password: ubuntu100
#
#  after:
#  ssh ubuntu@127.0.0.1 -p 10024
#

qemu-system-riscv64 \
-machine virt -nographic -m 4G -smp 4 \
-bios fw_jump.elf \
-kernel uboot.elf \
-device virtio-net-device,netdev=eth0 -netdev user,id=eth0,hostfwd=tcp::5924-:5900,hostfwd=tcp::10024-:22 \
-device virtio-rng-pci \
-drive file=ubuntu-24.04.1-preinstalled-server-riscv64.img,format=raw,if=virtio







