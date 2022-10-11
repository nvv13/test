!!!В РАБОТЕ!!!
!!!underway!!!


project

board HLK-W801-KIT-V1.1 + ws2812b RGB address LED + wifi + OTA upgrade 


эффекты взяты отсюда: https://github.com/AlexGyver/WS2812_FX
                      https://alexgyver.ru/ws2812b-fx/ 


~~~

Удаленное управление: можно подцепиться к платке по сети, на порт 5555,
 пример программки для соединения chat_client.py
 команды
          help - данная справка (flash ver- )
          time - время и прочие состояния
          drX - режим эф.ленты, где X, только 1-, 2-, 3-выкл.
          spd=A - где A - скорость
          mode=Y - режим драйвера ленты, где Y, 0-PIN_MODE, SPI_MODE: 
                                          1-3bit, 2-4bit, 3-5bit, 4-6bit, 5-7bit, 6-8bit
          on=Z, где Z - бит для 1 в SPI_MODE, можно менять для эксперементов 
          от 0 до 8
          off=V,где V - бит для 0
          clk=K,где K - частота в Герцах для SPI_MODE
          upgrade - обновить прошивку по OTA, лезет на "http://192.168.1.69/ota/ws2812b/w800_ota.img"


~~~





сборка:
~~~
   делал по инструкциям из этого проекта https://github.com/droppingy/W801_SDK-linux
   у меня Linux Fedora 33 x86_64, думаю для всех Linux x86_64 подойдет

1) сначала, один раз, ставим утилиты для сборки c-sky (архитектура w806, w801 мк)
   благодоря пользователю droppingy это очень просто!  

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

     если тут запустить make, то собирёться демо приложение которое идет вместе с SDK, об этом я писал кратко в https://github.com/nvv13/test/tree/main/test-mk/w801 
     но мы хотим собрать другой проект
     для этого удаляем, лишнии директории, 
      где app demo - директории с демо приложением
          bin - директория где в результате будет собранное приложение, создасться заново в процессе сборки

$ rm -R app demo bin

     делаем директорию для приложения

$ mkdir app

     и, в даном случае, качаем файлики приложения, 
     для примера, возьмем отсюда https://github.com/nvv13/test/tree/main/test-mk/w801/sdk-prj/02_task_blink
     но, принцип тот же самый и для другох... (если нужно много файлов, то можно воспользоваться утилитой https://downgit.github.io/)


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






  далее подцепляемся к платке, если интересует вывод на консоль

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))









