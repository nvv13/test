


# Basic CH32V003 RISCV Makefile project
 https://github.com/wuxx/CH32V003-makefile-example

Requirements:
 - xpack riscv toolchain (riscv-none-embed-) (https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack/releases)

Set up udev rules for WCH link
```
sudo cp ./50-wch.rules   /etc/udev/rules.d  
sudo udevadm control --reload-rules
```

Make project
```
make
```


# Licence

Unless otherwise stated files are licensed as BSD 2-Clause

Files under `vendor/` are from openwch (https://github.com/openwch/ch32v307) Licensed under Apache-2.0
Makefile is based on an example here: https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

# Reference
modify from greg's CH32V307 makefile project
https://github.com/gregdavill/CH32V307-makefile-example.git

#src vendor
 https://www.wch.cn/downloads/CH583EVT_ZIP.html
 https://github.com/openwch/ch583.git