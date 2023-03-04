!!!В РАБОТЕ!!!
!!!underway!!!





board HLK-W801-KIT-V1.1 

project audio


Источник
https://github.com/schreibfaul1/ESP32-audioI2S.git





~~~

с Линковкой cpp были проблемы, 
поэтому в Makefile добавлено :

CXXFLAGS += -fno-threadsafe-statics 

и ниже,

LINKLIB = -lstdc++ \
    ...............	


по первому пункту ( -fno-threadsafe-statics ), обьяснение :
https://stackoverflow.com/questions/38779413/arduinojson-undefined-reference-to-cxa-guard-acquire
https://github.com/bblanchon/ArduinoJson/issues/356

Explanation:

-std=c++11 enables support for the new language features introduced in the C++11 standard, like nullptr
-fno-threadsafe-statics disables the mutex around initialization of local static variables,
  which is useless when you program is single threaded. Ну да, w801 имеет только одно ядро, подходит.


это 
CXXFLAGS_STD += -std=gnu++11
не добавлял, так как и так работает


по второму пункту ( -lstdc++ ), обьяснение :
https://www.appsloveworld.com/cplus/100/28/undefined-reference-to-gxx-personality-sj0

Explanation:

 use gcc to compile C++ programs, but if you use any features of the standard library or C++ runtime 
 (including exception handling) then you need to link to the C++ runtime with -lstdc++ 
 (or -lsupc++ for just the runtime).

~~~



--------




Сборка  ( видео https://www.youtube.com/watch?v=uMJ7SQkhtYc ):
~~~
делал по инструкциям из этого проекта https://github.com/droppingy/W801_SDK-linux
   у меня Linux Fedora 33 x86_64, думаю для всех Linux x86_64 подойдет

1) сначала, один раз, ставим утилиты для сборки c-sky (архитектура w806, w801 мк)
   благодаря пользователю droppingy это очень просто!  

$ sudo git clone https://github.com/droppingy/hlk-w80x-toolchain.git /opt/w80x-tools

   так чтобы в итоге получилось /opt/w80x-tools/bin
   это чтоб в menuconfig не переделывать, такой там путь по умолчанию
   (еще, в этом мануале https://github.com/IOsetting/wm-sdk-w806, есть ссылки как получить данный Toolchain на другие архитектуры, типа i386)

2) далее качаем SDK

    перемещаемся в домашнюю директорию
 
$ cd ~

    делаем директорию для проекта

$ mkdir w801

$ cd w801  

$ git clone https://github.com/droppingy/W801_SDK-linux.git

3) далее, собираем приложение

$ cd W801_SDK-linux

     если тут запустить make, то собирается демо приложение которое идет вместе с SDK, об этом я писал кратко в https://github.com/nvv13/test/tree/main/test-mk/w801 
     но мы хотим собрать другой проект
     для этого удаляем, лишние директории, 
      где app demo - директории с демо приложением
          bin - директория где в результате будет собранное приложение, создастся заново в процессе сборки

$ rm -R app demo bin

     делаем директорию для приложения

$ mkdir app

     и, в данном случае, качаем файлики приложения, 
     для примера, возьмем отсюда https://github.com/nvv13/test/tree/main/test-mk/w801/sdk-prj/02_task_blink
     но, принцип тот же самый и для других...(если нужно много файлов, то можно воспользоваться утилитой https://downgit.github.io/)

$ cd app

$ wget https://github.com/nvv13/test/raw/main/test-mk/w801/sdk-prj/02_task_blink/app/Makefile

$ wget https://github.com/nvv13/test/raw/main/test-mk/w801/sdk-prj/02_task_blink/app/main.c

     возвращаемся в директорию SDK
$ cd ..

     заменяем головной wget Makefile на наш (удалено обращение к директории demo)

$ rm Makefile

$ wget https://github.com/nvv13/test/raw/main/test-mk/w801/sdk-prj/02_task_blink/Makefile


     перед сборкой, очистить проект, иногда надо
$ make distclean


    сборка
$ make 


    перед прошивкой, стереть флэш в микроконтроллере, иногда надо, !когда, допустим был проект с wifi, а стал без него!
$ make erase

    записать прошивку
$ make flash

    Всё!


~~~





-----------
Прочее:

  подцепляемся к платке, если интересует вывод на консоль (используеться в проекте)

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))

-----------

Прочее (Linux):

 компилируем, и перенаправляем консоль ошибок(2) в файл

$ make 2> a.txt 

-----------





