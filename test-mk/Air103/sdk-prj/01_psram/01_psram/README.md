board Air103 v1.1

project psram

psram - припаевается на подготовленое место! см. доки. "Air103_Core Board Design Manual V1.2.pdf"

/**
 * @brief  config the pins used for psram ck cs dat0 dat1 dat2 dat3
 * @param  numsel: config psram ck cs dat0 dat1 dat2 dat3 pins multiplex relation,valid para 0,1
 *			0:                 1: only for 56pin Air103
 *			  psram_ck   PB00    psram_ck   PA15
 *			  psram_cs   PB01    psram_cs   PB27
 *			  psram_dat0 PB02    psram_dat0 PB02
 *			  psram_dat1 PB03    psram_dat1 PB03
 *			  psram_dat2 PB04    psram_dat2 PB04
 *			  psram_dat3 PB05    psram_dat3 PB05

 * @return None

  void wm_psram_config(uint8_t numsel);
 */



надо соединить по схеме:
~~~


Air103 psram     LY68L6400  
------ --------- -------    
PA15   SCLK      (6)        
PB27   CE#       (1)        
PB02   SI/SIO[0] (5)
PB03   SO/SIO[1] (2)
PB04*     SIO[2] (3)
PB05      SIO[3] (7)
3.3V   Vcc       (8)
GND    Vss       (4)
------ --------- -------

* вывод PB04 на макетке board HLK-W801-KIT-V1.1 ошибочно назван PB24 в месте рядом с разьемом type-c

~~~




Сборка  (это не конкретно этого проекта, а к примеру
~~~
делал по инструкциям из этого проекта https://github.com/droppingy/W801_SDK-linux
   у меня Linux Fedora 33 x86_64, думаю для всех Linux x86_64 подойдет

1) сначала, один раз, ставим утилиты для сборки c-sky (архитектура w806, w801 мк)
   благодаря пользователю droppingy это очень просто!  

$ sudo git clone https://github.com/droppingy/hlk-w80x-toolchain.git /opt/w80x-tools

   так чтобы в итоге получилось /opt/w80x-tools/bin
   это чтоб в menuconfig не переделывать, такой там путь по умолчанию
   (еще, в этом мануале https://github.com/IOsetting/wm-sdk-w806, есть ссылки как получить данный Toolchain на другие архитектуры, типа i386)

2) далее качаем-распаковываем SDK

    перемещаемся в домашнюю директорию
 
$ cd ~

    делаем директорию для проекта

$ mkdir Air103

$ cd Air103  

    качаем

  виде архива W806_SDK-linux.tar.gz по ссылке
  https://orangepi.nvv13.keenetic.link/nextcloud/index.php/s/6ZRAwg7taaC6Ho4 

    распаковка

$ tar xvf W806_SDK-linux.tar.gz

3) далее, собираем приложение

$ cd W806_SDK-linux/

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

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/Air103/sdk-prj/01_blink/01_blink_task/app/Makefile

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/Air103/sdk-prj/01_blink/01_blink_task/app/main.c

     возвращаемся в директорию SDK
$ cd ..

     заменяем головной wget Makefile на наш (удалено обращение к директории demo)

$ rm Makefile

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/w806/sdk-prj/01_blink/01_blink_task/Makefile


     перед сборкой, очистить проект, иногда надо
$ make distclean


    сборка
$ make 


    млжно перед прошивкой, стереть флэш в микроконтроллере, иногда надо
$ make erase

    записать прошивку
$ make flash

  

    Всё!


~~~





Прочее:

  подцепляемся к платке, если интересует вывод на консоль (используеться в проекте)

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))





