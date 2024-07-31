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

--------

примеры для sdk and rp2040

структура каталогов

из корневой директории, делаем поддиректории с проектами
например 01_usb_srial

в корневой директоии, должны лежать файлы

CMakeLists.txt - что включаем в сборку, пример что в нем
--------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(sdk-prj C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


pico_sdk_init()

add_subdirectory(01_usb_serial)
--------------------------------------------------------------------------------------------

pico_sdk_import.cmake - взять из pico-examples https://github.com/raspberrypi/pico-examples.git

----------------------
для сборки, 
делаем дир
mkdir build

переходим
cd build

запускаем
cmake ..


потом, в директорию с проектом (в директории build будет)
cd 01_usb_srial

а можно и не переходить в 01_usb_srial, все равно соберет

и собрать
make

--------------------------
