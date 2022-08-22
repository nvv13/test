project

board HLK-W801-KIT-V1.1 + wifi + tm1637 + WDT + rtos + https weather + ntp + OTA upgrade 

видео:
https://youtu.be/Hg9dpII8_EY

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
 

фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_quad_digit_led_display.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1" /></p>

фото 2 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_4_1.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" /></p>

фото 3 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_4_2.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" /></p>



соединения

дисплей

LCD display tm1637
     4 pin

надо соеденить по схеме:
~~~
connect to
W801  tm1637
GPIO  PIN
PB_21 CLK
PB_22 DIO
3.3V  VCC
GND   GND
~~~


~~~
для OTA, развернул сервер, ( подробнее типа https://habr.com/ru/company/ruvds/blog/528428/ )
sudo apt-get install apache2 phpy libapache2-mod-php
sudo service apache2 restart

в каталог /var/www/ota/weather2

ложим файл прошивки, вида:
w800_ota.img
~~~






сборка (кратко):

1) качаем

   https://github.com/droppingy/W801_SDK-linux.git

   там же есть инструкции как его собирать, или читаем https://github.com/nvv13/test/blob/main/test-mk/w801/README.md


2) заменяем папку app на нашу app из проекта


3) далее сборка make, и прошивка make flash



  далее подцепляемся к платке, если интересует вывод на консоль

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))




PS:
исходники драйвера tm1637 - https://github.com/avishorp/TM1637.







