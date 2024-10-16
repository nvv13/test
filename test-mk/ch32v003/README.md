
делал в fedora linux 39, в других возможно немного по другому надо


(1) установка пакетов для сборки riskv кода

# sudo dnf install pnpm

# sudo npm install --location=global xpm@latest

# xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --global --verbose

# export PATH=$HOME/.local/xPacks/@xpack-dev-tools/riscv-none-elf-gcc/14.2.0-2.1/.content/bin:$PATH



(2) установка для прошивки 

собираем из проекта 

https://github.com/cnlohr/ch32v003fun.git

minichlink

копируем оттуда файл
`99-minichlink.rules`
в
`/etc/udev/rules.d/`

$ sudo groupadd plugdev
$ sudo usermod -a -G plugdev cur_user
$ sudo reboot

(3)
 





PS:
 источники
https://github.com/cnlohr/ch32v003fun
https://github.com/wuxx/CH32V003-makefile-example
https://github.com/gregdavill/CH32V307-makefile-example

