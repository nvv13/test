есть какие то проблемы с FreeRTOS
https://github.com/gabor-budai/FreeRTOS_PICO2350.git

или нет
https://github.com/FreeRTOS/FreeRTOS-Kernel-Community-Supported-Ports.git



--------
ранее
dnf groupinstall "Development Tools" "Development Libraries"

--------

перед этим, где то должен быть установлен pico-sdk
cd ~
mkdir pico
cd pico
git clone https://github.com/raspberrypi/pico-sdk.git
git clone https://github.com/raspberrypi/pico-examples.git
----------

и в файле .bashrc  указано где

export PICO_SDK_PATH=/home/pi/pico/pico-sdk
export PICO_EXAMPLES_PATH=/home/pi/pico/pico-examples
export PICO_EXTRAS_PATH=/home/pi/pico/pico-extras
export PICO_PLAYGROUND_PATH=/home/pi/pico/pico-playground



~~~

для rp2350 RISK-V надо поставить (один раз) TOOLCHAIN
на сайте https://www.embecosm.com/resources/tool-chain-downloads/#riscv-stable
нашел ссылку подхдящий для Fedora Linux

далее

$ sudo -i
# mkdir /opt/riscv32
# cd /opt/riscv32/
# wget https://buildbot.embecosm.com/job/riscv32-gcc-centos7-release/19/artifact/riscv32-embecosm-centos7-gcc13.2.0.tar.gz 
# tar xf ./riscv32-embecosm-centos7-gcc13.2.0.tar.gz 

потом, добавим в CMakeLists.txt
set(PICO_TOOLCHAIN_PATH /opt/riscv32/riscv32-embecosm-centos7-gcc13.2.0/bin)
set(PICO_PLATFORM rp2350-riscv)

~~~


--------

примеры для freertos and rp2350

структура каталогов

в корневой директоии, должны лежать файлы
CMakeLists.txt - что включаем в сборку
pico_sdk_import.cmake - взять из pico-examples https://github.com/raspberrypi/pico-examples.git

из корневой директории, делаем поддиректории с проектами
например 01_TaskGPIO

из корневой директории, делаем директорию
freertos - там должны лежать файлы
  CMakeLists.txt   - 
  FreeRTOSConfig.h -
так же, переходим в эту директорию и делаем git clone https://github.com/FreeRTOS/FreeRTOS-Kernel

из корневой директории, делаем директорию
build
 переходим в неё и выполняем cmake ..
 после этого, создадуться директории под проекты, переходим в них и делаем make



01)https://learnembeddedsystems.co.uk/freertos-on-rp2040-boards-pi-pico-etc-using-vscode
  git clone --recurse-submodules https://github.com/LearnEmbeddedSystems/rp2040-freertos-project
  

02)https://learnembeddedsystems.co.uk/freertos-part-2-tasks-and-queues-tutorial

03)https://learnembeddedsystems.co.uk/freertos-on-the-rp2040-part-3-source-code

04)https://learnembeddedsystems.co.uk/freertos-on-the-rp2040-part-4-source-code


05)https://learnembeddedsystems.co.uk/freertos-on-the-rp2040-part-5-semaphores
   https://metanit.com/sharp/tutorial/11.8.php

   Is configUSE_MUTEXES set to 1 in FreeRTOSConfig.h?



06)https://learnembeddedsystems.co.uk/using-the-rp2040-on-board-temperature-sensor


07)
   git clone https://github.com/FreeRTOS/FreeRTOS-SMP-Demos.git --recurse-submodules