project HLK-W801-KIT-V1.1 + ds3231 + max7219 

<p><img src="https://github.com/nvv13/test/blob/main/test-mk/stm32/bluepill/step05-matrixLED/clock_2.jpg" alt="max7219clock" title="max7219clock" /></p>







Видео
https://youtu.be/TNM8vkSJKMk
https://rutube.ru/video/fea91be3b2752fbb304cc0a20ada9ad7/?r=wd


~~~
компоненты

(1)
module ds3231 с eeprom на борту 24c32
вот такой (для примера только)
https://protosupplies.com/product/ds3231s-rtc-with-eeprom-module/

(2)
MAX7219 8×8 Dot Matrix
вот такой (для примера только)
https://protosupplies.com/product/max7219-8x8-dot-matrix-x4-red-display-module/

(3)
board HLK-W801-KIT-V1.1  
~~~







соединения

надо соединить по схеме:
~~~



connect to
W801   module ds3231
GPIO   PIN    
-----  ------ 
PA02   32K    (32kHz) выход меандра с частотой 32.768кГц
PA03   SQW    (INT/SQW) (INTerrupt / SQuare Wave) прерывание / программируемый меандр
PA01   SCL    i2c clock   
PA04   SDA    i2c data      
+3.3   VCC     
GND    GND    
-----  ------ 


connect to
W801   module max7219(4) + max7219(4) = 8 
GPIO   PIN    
-----  ------ 
PB23   cs
PB24   clk
PB26   din
+5.0   VCC     
GND    GND    
-----  ------ 






~~~




----





----



-------




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





----

PS: 









