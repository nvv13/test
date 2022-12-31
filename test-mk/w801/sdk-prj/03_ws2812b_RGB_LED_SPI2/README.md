
project

board HLK-W801-KIT-V1.1 + ws2812b RGB address LED + wifi + OTA upgrade + IR remote


эффекты взяты отсюда: https://github.com/AlexGyver/WS2812_FX
                      https://alexgyver.ru/ws2812b-fx/ 


~~~

Удаленное управление: можно подцепиться к платке по сети, на порт 5555,
 пример программки для соединения chat_client.py
 команды
          help - данная справка (flash ver- )
          time - время и прочие состояния
          drX - режим ленты, где X, только 1...51."
          lightX - яркость, где X от 1 до 255."
          spd=A - где A - скорость
          mode=Y - режим драйвера ленты, где Y, 0-PIN_MODE, SPI_MODE: 
                                          1-3bit, 2-4bit, 3-5bit, 4-6bit, 5-7bit, 6-8bit
          on=Z, где Z - бит для 1 в SPI_MODE, можно менять для эксперементов 
          от 0 до 8
          off=V,где V - бит для 0
          clk=K,где K - частота в Герцах для SPI_MODE
          upgrade - обновить прошивку по OTA, лезет на "http://192.168.1.69/ota/ws2812b/w800_ota.img"



~~~



соединения

ws2812b
надо соеденить по схеме:
~~~
connect to
W801 GPIO | ws2812b PIN
PB17---200R-_---data
GND   	  220pF-GND
+5v   		+5v
~~~
между PB17 и DATA, резистор 200 ом +
между DATA и GND, конденсатор 200 пФ 
иначе помехи, 
 * еще можно преобразователь уровней 3,3v -> 5v добавить
   хотя работает и без него....




дисплей, (если надо)
LCD display tm1637
     4 pin
надо соеденить по схеме:
~~~
connect to
W801 GPIO | tm1637 PIN
PB_21 		CLK
PB_22 		DIO
3.3V  		VCC
GND   		GND
~~~



кнопка (убрал в прошивке, часто ловила помехи)
~~~
connect to
W801 GPIO | botton PIN
PA_01 		1 pin
GND   		2 pin
~~~



IR sensor
~~~
connect to 
----- --------
W801  tsop48
----- --------
PA_02 Pin1
GND   Pin2
3.3v  Pin3
----- --------
~~~


IR управление, подогнано под такую лентяйку: 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/03_ws2812b_RGB_LED_SPI2/jpg/remote1.jpg" alt="remote1" title="remote1" /></p>


если нужно под другую (найти кода и т.д.), можно начать с 
https://github.com/nvv13/test/tree/main/test-mk/w801/sdk-prj/01_IR_scan








Сборка  (это не конкретно этого проекта, а к примеру видео https://www.youtube.com/watch?v=uMJ7SQkhtYc ):
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






  далее подцепляемся к платке, если интересует вывод на консоль

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))









