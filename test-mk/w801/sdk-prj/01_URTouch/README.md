
project URTouch

главный источник: 
http://www.rinkydinkelectronics.com/library.php?id=92


-----------------------------


-----------------------------

board HLK-W801-KIT-V1.1 + Резистивный сенсорный экран 3.2 TFT дисплей 320X240 16bit bus

PORTRAIT  = URTouch_set_calibrate: calx=378F66, caly=3C34155, cals=EF13F



Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft_back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_URTouch/app/test/TFT_320QDT_9341

соединения

надо соеденить по схеме:
~~~
вид с низу, разьем слева
connect to TFT_320QDT_9341
---- ------------ ----
W801 LCD   LCD    W801
---- ------------ ----
       20  40
       NC  NC
3.3v LEDA  NC
       NC  SD_CS
PA04 REST  SD_DIN
     F_CS  SD_CLK
PA03   CS  SD_DO
PB21 DB15  T_IRQ  PA9
PB22 DB14  T_DO   PA8
PB23 DB13  NC
PB24 DB12  T_DIN  PA7
PB25 DB11  T_CS   PA6
PB26 DB10  T_CLK  PA5
PB18  DB9  DB7    PB16
PB17  DB8  DB6    PB01
3.3v   RD  DB5    PB27
PA02   WR  DB4    PB15 
PA01   RS  DB3    PB14
       NC  DB2    PB13 
 
5V    VCC  DB1    PB12
gnd   GND  DBO    PB00
        1   21
---- ------------ ----
W801  LCD  LCD    W801
---- ------------ ----



~~~



------------------------------------------------

-----------------------------

board HLK-W801-KIT-V1.1 + 3.97 TFT_397T_NT35510 800x480 16bit bus

 LANDSCAPE = URTouch_set_calibrate: calx=2C8F31, caly=3DDC053, cals=1DF31F
 PORTRAIT  = URTouch_set_calibrate: calx=31CEF2, caly=3ECC03D, cals=1DF31F

Видео
https://youtu.be/sM8lupmjGR8?t=87

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.97_TFT_NT35510_800x480_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.97_TFT_NT35510_800x480.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 
 01_URTouch/app/test/TFT_397T_NT35510
 01_URTouch/app/test_btn/TFT_397T_NT35510
 01_URTouch/app/test_cl/TFT_397T_NT35510

соединения

надо соединить по схеме:
~~~
вид с низу, разьем слева
connect to TFT_397T_NT35510
---- ------------ ----
W801 LCD      LCD W801
---- ------------ ----
PA01 RS      CS   PA03
3.3v RD      WR   PA02
PB00 DBO     RST  PA04 
PB13 DB2     DB1  PB12
PB15 DB4     DB3  PB14
PB01 DB6     DB5  PB27
PB17 DB8     DB7  PB16
PB26 DB10    DB9  PB18
PB24 DB12    DB11 PB25
PB22 DB14    DB13 PB23
     NC      DB15 PB21
3.3v VCC       BL 3.3v 
gnd  GND      VCC 3.3v 
     NC       GND gnd 
PA07 MOSI    MISO PA08
     NC       PEN PA09
PA05 CLK     T_CS PA06
---- ------------ ----
W801 LCD      LCD W801
---- ------------ ----
~~~






---------------------------------------------

board HLK-W801-KIT-V1.1 + 2.4inch 16BIT Module ILI9341 SKU:MRB2408 320x240 16bit bus


LANDSCAPE = URTouch_set_calibrate: calx=3CC40D9, caly=2BCE3A, cals=EF13F
PORTRAIT  = URTouch_set_calibrate: calx=3C00111, caly=200E6C, cals=EF13F


Видео
https://youtu.be/sM8lupmjGR8

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/MRB2408_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/MRB2408.jpg" alt="top side" title="top side" /></p>

wiki
http://www.lcdwiki.com/2.4inch_16BIT_Module_ILI9341_SKU:MRB2408

для него тест (main.c) лежит в директории 
 01_URTouch/app/test/MRB2408
 01_URTouch/app/test_btn/MRB2408
 01_URTouch/app/test_cl/MRB2408

соединения

надо соединить по схеме:
~~~
вид с низу, разъём слева
connect to TFT_24_MRB2408
---- ------------ ----
W801 LCD     LCD  W801
---- ------------ ----
PA01 RS        CS PA03
3.3v RD        WR PA02
PB00 D1       RST PA04 
PB13 D3        D2 PB12
PB15 D5        D4 PB14
PB01 D7        D6 PB27
PB17 D10       D8 PB16
PB26 D12      D11 PB18
PB24 D14      D13 PB25
PB22 D16      D15 PB23
     SCS      D17 PB21
3.3v VDD       BL 3.3v 
gnd  GND      VDD 3.3v 
     5V       GND gnd 
PA07 MOSI    MISO PA08
     FCS      PEN PA09
PA05 CLK      TCS PA06
---- ------------ ----
W801 LCD      LCD W801
---- ------------ ----
~~~
-----------------------------













------------------------------------------------


board HLK-W801-KIT-V1.1 + 2.4 TFT SPI 240x320 V1.3

PORTRAIT  = URTouch_set_calibrate: calx=4E4EE3, caly=3DF80B2, cals=EF13F
LANDSCAPE = URTouch_set_calibrate: calx=3F8F2E, caly=3D1C0C3, cals=EF13F

Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT2_4SP_9341_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT2_4SP_9341.jpg" alt="top side" title="top side" /></p>


для него тест (main.c) лежит в директории 
  01_URTouch/app/test_cl/TFT2_4SP_9341

соединения

надо соединить по схеме:
~~~
connect to TFT2_4SP_9341
---- ------
W801 LCD   
---- ------
PA09   T_IRQ
PA08   T_DO
PA07   T_DIN
PA06   T_CS
PA05   T_CLK
PB16  SDO(MISO)
3.3v  LED
PB15  SCK         синхросигнал
PB17  SDI(MOSI)
PB23  DC          комманда/данные
PB21  RESET       сброс
PB14  CS          выбор чипа
gnd   GND
3.3v  VCC
---- ------
W801 LCD   
---- ------


~~~


-----------------------------




------------------------------------------------



board HLK-W801-KIT-V1.1 + 3.2 TFT SPI 240x320 V1.0

PORTRAIT  = URTouch_set_calibrate: calx=2A8EF6, caly=3D9C02B, cals=EF13F
LANDSCAPE = URTouch_set_calibrate: calx=260EC4, caly=3CD0050, cals=EF13F

Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft_spi_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft_spi.jpg" alt="top side" title="top side" /></p>


для него тест (main.c) лежит в директории 
 01_URTouch/app/test_cl/TFT3_2SP_9341

соединения

надо соединить по схеме:
~~~
connect to TFT3_2SP_9341
---- ------
W801  LCD   
---- ------
PA09   T_IRQ
PA08   T_DO
PA07   T_DIN
PA06   T_CS
PA05   T_CLK
PB16  SDO(MISO)
3.3v  LED
PB15  SCK         синхросигнал
PB17  SDI(MOSI)
PB23  DC          комманда/данные
PB21  RESET       сброс
PB14  CS          выбор чипа
gnd   GND
3.3v  VCC
---- ------
W801  LCD   
---- ------


~~~

-----------------------------


------------------------------------------------


board HLK-W801-KIT-V1.1 + 3.5 TFT 320x480  V2.2

назывался "3,5-дюймовый TFT ЖК SPI IPS ILI9488 с емкостным касанием FT6236"

PORTRAIT  = URTouch_set_calibrate: калибровка не требуется, похоже их калибруют при производстве
LANDSCAPE = URTouch_set_calibrate: 

Видео
https://youtu.be/COopMgUwCrc


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT3_5SP_9488_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT3_5SP_9488.jpg" alt="top side" title="top side" /></p>


для него тест (main.c) лежит в директории 
 01_URTouch/app/test_btn/TFT3_5SP_9488

соединения

надо соединить по схеме:
~~~
connect to TFT3_5SP_9488
---- ------
W801  LCD   
---- ------
PB21  CTP_RST     сброс CTP (не должен висеть "в воздухе"!)
PA09  CTP_INT     прерывание
PA01  CTP_SCL     
PA04  CTP_SDA     
      NC/3V3 
PB16  SDO         MISO
3.3v  BL          подсветка
PB15  SCK         синхросигнал
PB17  SDI         MOSI
PB23  D/C         комманда/данные
PB21  RST         сброс
PB14  CS          выбор чипа
gnd   GND         земля
3v3   VDD         питание (3.3v - 5v)
---- ------
W801  LCD   
---- ------


*подготовка изображений для SD Card + используется библиотека libTJPEG.a

# convert *.jpg -resize 480x320! j%03d-480x320.jpg

~~~





--------------------------------------------------

------------------------------------------------


board HLK-W801-KIT-V1.1 + 2.8 TFT 240x320 MSP2834


Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/MSP2834_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/MSP2834.jpg" alt="top side" title="top side" /></p>


wiki
http://www.lcdwiki.com/2.8inch_IPS_SPI_Module_ILI9341

для него тест (main.c) лежит в директории 01_UTFU/app/test/MSP2834

соединения

надо соединить по схеме:
~~~
connect to MSP2834
---- ------
W801  LCD   
---- ------
      SD_CS
PA09  CTP_INT     прерывание
PA04  CTP_SDA     
PB21  CTP_RST     сброс CTP  (не должен висеть "в воздухе"!)
PA01  CTP_SCL     
PB16  SDO(MISO)
3.3v  LED         подсветка
PB15  SCK         синхросигнал
PB17  SDI(MOSI)
PB23  LCD_RS      комманда/данные
PB21  LCD_RST     сброс
PB14  LCD_CS      выбор чипа
gnd   GND         земля
3v3   VDD         питание (3.3v - 5v)
---- ------
W801  LCD   
---- ------


*подготовка изображений для SD Card + используется библиотека libTJPEG.a
# convert *.jpg -resize 320x240! j%03d-320x240.jpg

~~~





--------------------------------------------------








-----------------------------
после сборки, файл libUTFT.a 
лежит ./bin/build/w800/lib/libURTouch.a
нужно закинуть в общие библиотеки ./lib/w800/mod1

также фалик URTouch.h 
 лежит ./app/UTFT
 скоприровать в директорию ./include/mod1

это для других проектов

-----------------------------





-----------------------------
-----------------------------


Сборка  (это не конкретно этого проекта, а к примеру видео https://www.youtube.com/watch?v=uMJ7SQkhtYc 
          https://rutube.ru/video/664a1f911677ecb82c431247c4a5b3b7/?r=wd ):
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

$ git clone https://github.com/leoay/wm-sdk-w80x-2023.git

3) далее, собираем приложение

$ cd wm-sdk-w80x-2023/

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







Прочее:

  подцепляемся к платке, если интересует вывод на консоль (используеться в проекте)

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))







--------------------------------

главный источник: 
## License and Credits 
This project is based on the original [URTouch library](http://www.rinkydinkelectronics.com/library.php?id=92) developed and maintained by Henning Karlsen, and licensed under [CC BY-NC-SA 3.0](http://creativecommons.org/licenses/by-nc-sa/3.0/).

+ещё [UTFT_Buttons library](http://www.rinkydinkelectronics.com/library.php?id=61)

--------------------------------


