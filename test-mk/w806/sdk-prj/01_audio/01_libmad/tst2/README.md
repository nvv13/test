взято из 
https://github.com/pschatzmann/arduino-libmad.git

HKL-W806-KIT-V1.0 + max98357a

надо соеденить по схеме:
~~~
connect to
W80x   модуль max98357a, вид сверху, 
GPIO   контакты
----   ------     ---------------------------------------------------------------------------------------------
PA09     RLclk      word select line   i2s LRclk
PA08     Bclk       clock line         i2s Bclk
PA10     Din,       Data Input
PB01      Gain_Slot, conect VDD - 6db, Unconnected - 9db, conntct GND 100kOm - 15db, connecd VDD 100kOm - 3db
PB00      SD_MODE,   0 - off, VDD - on
GND      GND
VCC      VCC
----   ------     ---------------------------------------------------------------------------------------------



или модуль PCM5102A (перемычка на модуле, в положении i2s)
connect to
W80x   модуль PCM5102A
GPIO   контакты
----   ------     ---------------------------------------------------------------------------------------------
        SCK       есть перемычка на плате - Подтянивает sck на землю для для тактирования от внутреннего генератора, запаять
PA08    BCK     
PA10    DIN
PA09    LRCK    
GND     GND
VCC     VIN
----   ------     ---------------------------------------------------------------------------------------------


sdio host
Подключаем SD Card по схеме
W80x	Pin #	SD 4-bit Mode	Описание
-----  -------------------------------------------------
PB10	9	DAT[2]		Data Line 2 /Read Wait
 PB11	1	CD/DAT[3]	Data Line 3
 PB07	2	CMD		Command Line
 Gnd	3	VSS1		Ground
 3.3v	4	VDD		Supply Voltage
 PB06	5	CLK		Clock
 Gnd	6	Vss2		Ground
 PB08	7	DAT[0]		Data Line 0
 PB09	8	DAT[1]		Data Line 1 / Interrupt
-----  -------------------------------------------------


SD Card 
Вид со стороны контактов (с низу)
  ---------
 |12345678|
|9        |
|         |
| SD Card |
|         |
-----------

рядом с считывателем
Контакты 1,2,7,8,9 - подключаем через резисторы, примерно ~10кОм...100кОм к +3.3v контакт 4
К контактам Gnd(3,6) и +3.3v(4) - подключам конденсаторы, керамический ~0,047мКф и электролит ~10мКф от помех...





~~~












Сборка  (это не конкретно этого проекта, а к примеру)
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

$ mkdir w806

$ cd w806  

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

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/w806/sdk-prj/01_blink/01_blink_task/app/Makefile

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/w806/sdk-prj/01_blink/01_blink_task/app/main.c

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

  и тут програма вероятнее всего попросит нажать reset на платке для синхронизации

  нажимаем reset
   

    Всё!


~~~





Прочее:

  подцепляемся к платке, если интересует вывод на консоль (используеться в проекте)

$ picocom --echo -b 115200 /dev/ttyUSB0








