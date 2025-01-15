Соединения 

фото milkv-duo and wifi
![photo](../jpg/milkv-duo%20and%20wifi.jpg)

фото milkv-duo 256m and wifi
![photo](../jpg/milkv-duo%20256m%20and%20wifi.jpg)



~~~~~~

качаем
$ wget https://github.com/milkv-duo/duo-buildroot-sdk/archive/refs/tags/Duo-V1.1.2.tar.gz

распаковываем
$ tar xzf Duo-V1.1.2.tar.gz


далее заменить файлы приложенными в директории 
https://github.com/nvv13/test/tree/main/test-mk/MilkV-Duo/wifi/duo-buildroot-sdk-Duo-V1.1.2

там файлики пропаченные по инструкции "03 MilkV-Duo and usb Wifi.md"
для вариантов сборки milkv-duo-sd и milkv-duo256m-sd  + чип usb WiFi RTL8188FU
я такой модуль нашел на Али BL-M8188FU1 бывают еще похожие...

!!не забудте поставить свой пароль на WiFi в файле!! 
duo-buildroot-sdk-Duo-V1.1.2/device/milkv-duo-sd/overlay/etc/wpa_supplicant/wpa_supplicant.conf
duo-buildroot-sdk-Duo-V1.1.2/device/milkv-duo256m-sd/overlay/etc/wpa_supplicant/wpa_supplicant.conf



далее - сборка 

перейти в SDK
$ cd duo-buildroot-sdk-Duo-V1.1.2


!останавливаем и удаляем старый образ контейнера duodocker для сборки, если он есть!
 смотрим его "CONTAINER ID"
$ docker ps -a | grep duodocker
 допустим "CONTAINER ID" = 52bf3ab8dc6d
$ docker stop 52bf3ab8dc6d
$ docker rm 52bf3ab8dc6d


скачать и запустить образ контейнера для сборки (просто в терминале)
$ docker run -itd --name duodocker -v $(pwd):/home/work milkvtech/milkv-duo:latest /bin/bash


запускаем сборку (просто в терминале), для варианта milkv-duo-sd
$ docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh milkv-duo-sd"

или для варианта milkv-duo256m-sd
$ docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh milkv-duo256m-sd"




!после компиляции, останавливаем и удаляем образ контейнера duodocker для сборки!
 смотрим его "CONTAINER ID"
$ docker ps -a | grep duodocker
 допустим "CONTAINER ID" = 52bf3ab8dc6d
$ docker stop 52bf3ab8dc6d
$ docker rm 52bf3ab8dc6d

~~~~~~
