
документация по платкам:
$ git clone https://github.com/milkv-duo/duo-files.git


~~~~~~

(0) сборка образа для sd

поставить docker (если небыло)
добавить себя в группу docker
$ sudo usermod -a -G docker <имя пользователя>
перезайти в сесию


собирал по инструкции
https://github.com/milkv-duo/duo-buildroot-sdk
с места 
"2. Compiled using Docker"

клонируем SDK
$ git clone https://github.com/milkv-duo/duo-buildroot-sdk.git --depth=1
я не так сделал (потому что сборка выпадала в ошибку)
я скачал релиз, вот этот, последний был на момент
$ wget https://github.com/milkv-duo/duo-buildroot-sdk/archive/refs/tags/Duo-V1.1.2.tar.gz      
распаковка
$ tar xzf Duo-V1.1.2.tar.gz

перейти в SDK
$ cd duo-buildroot-sdk
в моем случае в
$ cd duo-buildroot-sdk-Duo-V1.1.2


!останавливаем и удаляем старый образ контейнера duodocker для сборки, если он есть!
 смотрим его "CONTAINER ID"
$ docker ps -a | grep duodocker
 допустим "CONTAINER ID" = 52bf3ab8dc6d
$ docker stop 52bf3ab8dc6d
$ docker rm 52bf3ab8dc6d


скачать и запустить образ контейнера для сборки (просто в терминале)
$ docker run -itd --name duodocker -v $(pwd):/home/work milkvtech/milkv-duo:latest /bin/bash

смотрим, как там duodocker
$ docker ps -a

смотрим какие есть варианты образов для сборки
$ docker exec -it duodocker /bin/bash -c "cd /home/work && ./build.sh"

допустим нам надо "milkv-duo"
запускаем сборку (просто в терминале), для вырианта milkv-duo-sd
$ docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh milkv-duo-sd"

после компиляции, останавливаем и удаляем образ контейнера для сборки
 смотрим его "CONTAINER ID"
$ docker ps -a | grep duodocker
 допустим "CONTAINER ID" = 52bf3ab8dc6d
$ docker stop 52bf3ab8dc6d
$ docker rm 52bf3ab8dc6d



-----------------------------------------------
PS: если будут ошибки типа 
"make: /bin/sh: Operation not permitted"
то надо подновить сам docker (у меня так было)




~~~~~~
