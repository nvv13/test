Светодиодный модуль OLED 1,3 дюйма, белый SPI, 128X64, 1,3 дюйма, контроллер sh1106

------

project

board HLK-W801-KIT-V1.1 + wifi + OLED Display + WDT + rtos + https weather + ntp + OTA upgrade 

видео:
https://youtu.be/-Hyt0G5vFQM

------

соединения

дисплей

Светодиодный модуль OLED 1,3 дюйма, белый SPI, 128X64, 1,3 дюйма, контроллер sh1106
     7 pin

надо соеденить по схеме:
~~~
connect to
DISPLAY     w801
1 GND       GND
2 VCC       5v 
3 SCK       PB24
4 SDA       PB26
5 RES       PB22
6 DC        PB18
7 CS        PB21
~~~



~~~
При старте: подсоединяемся к сети Wifi, запрашиваем время по ntp.
В работе: запрашиваем температуру в нашем городе... и так каждые 2-5 минут,  
Отображение: 3 секунды - показывает время, 7 секунды - температуру, по кругу 
            (или можно переключить режим с помощью команд).
После 23:00 и до 6:00, яркость индикатора переключается на минимальную.

Удаленное управление: можно подцепиться к платке по сети, на порт 5555,
 пример программки для соединения chat_client.py
 команды
      "help - данная справка"
      "time - время и прочие состояния"
      "lightXXX - установить/узнать яркость индикатора, где XXX число от 1 до 7, если 0 то выдаст текущее значение яркости"        
      "mode=X - режим, где X, только 1-погода, 2-погода и время, 3-только время"
      "upgrade - обновить прошивку по OTA, лезет на http://192.168.1.69/ota/weather2/w800_ota.img"


~~~




~~~
для OTA, развернул сервер, ( подробнее типа https://habr.com/ru/company/ruvds/blog/528428/ )
sudo apt-get install apache2 phpy libapache2-mod-php
sudo service apache2 restart

в каталог /var/www/ota/weather2

ложим файл прошивки, вида:
w800_ota.img


немного позже, развернул сервер apache2 на устройстве keenetic-giga (KN-1011)
навела на это статья https://kotyara12.ru/iot/keenetic-mqtt/

~~~






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


------

PS:
исходники u8g2, взяты с сайта https://github.com/olikraus/u8g2

пример адаптации взят из https://github.com/RIOT-OS/RIOT, (хотя можно было взять из LuatOS, к примеру)







