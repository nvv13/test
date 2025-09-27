#!/bin/bash
#
# для генерации и загрузки файла для файловой системы spifss

# находим адрес загрузки и длинну из фйла partitions.csv
spifssS=$(cat partitions.csv | grep spiffs)
spifssS=${spifssS//,/ }
spifssA=$(echo "$spifssS" | awk '{print $4}')
spifssL=$(echo "$spifssS" | awk '{print $5}')

echo 'Adress='$spifssA
echo 'Length='$spifssL

# create myfs.bin - создание файла myfs.bin для файловой системы, размером $spifssL как указано в partitions.csv
~/.arduino15/packages/esp32/tools/mkspiffs/0.2.3/mkspiffs -c data -b 4096 -p 256 -s $spifssL myfs.bin

# write-flash myfs.bin - запись файла с файловой системой myfs.bin по адресу $spifssA как указано в partitions.csv
~/.arduino15/packages/esp32/tools/esptool_py/5.0.0/esptool --verbose --chip esp32s3 --port /dev/ttyACM0 --baud 921600 write-flash -z $spifssA myfs.bin

# delete myfs.bin
rm myfs.bin

# read-flash loadfs.bin 
#~/.arduino15/packages/esp32/tools/esptool_py/5.0.0/esptool --verbose --chip esp32s3 --port /dev/ttyACM0 --baud 921600 read_flash $spifssA $spifssL loadfs.bin

