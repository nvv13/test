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
# prepare files
#
#  wget https://repo.openeuler.org/openEuler-preview/RISC-V/openEuler-22.09-riscv64/QEMU/fw_payload_oe_qemuvirt.elf
#  wget https://cdimage.ubuntu.com/releases/20.04.2/release/ubuntu-20.04.2-preinstalled-server-riscv64.img.xz
#  xz -dk ubuntu-20.04.2-preinstalled-server-riscv64.img.xz 
#  qemu-img resize ubuntu-20.04.2-preinstalled-server-riscv64.img +10G
#
#  login: ubuntu
#  password: ubuntu
#
#  after start re-pass:
#  login: ubuntu
#  password: ubuntu100

qemu-system-riscv64 \
   -nographic \
   -machine virt \
   -m 4G \
   -smp cpus=4 \
   -kernel fw_payload_oe_qemuvirt.elf \
   -bios none \
          -drive file=ubuntu-20.04.2-preinstalled-server-riscv64.img,format=raw,id=hd0,if=none \
          -netdev user,id=eth0,hostfwd=tcp::5900-:5900,hostfwd=tcp::10000-:22 \
          -device virtio-blk-device,drive=hd0 \
          -device virtio-net-device,netdev=eth0 \
          -device virtio-keyboard -device virtio-tablet \
          -append "root=/dev/vda1"





