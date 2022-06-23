!!!В РАБОТЕ!!!
!!!NO WORK!!!

!!!В РАБОТЕ!!!
!!!NO WORK!!!



project

board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button

При старте, подсоеденяемся к сети Wifi,
далее, синхронизация времени с ntp сервером в интернете, и еще, повторы синхронизации в 3 часа ночи

В дополнеии, есть кнопка изменения яркости дисплея.
4 уровня яркости, после установки - запоминает во внутреннеё флэш.
После 23:00 и до 6:00, яркость переключаеться на минимальную.

фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_quad_digit_led_display.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1" /></p>

фото 2 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_4_1.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" /></p>

фото 3 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_4_2.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + button" /></p>

видео 
https://youtu.be/yVC3PTPEEaM


соединения

дисплей

LCD display 5643AS-1
     12 pin

надо соеденить по схеме:
~~~
connect to
W801  5643AS-1 
GPIO  PIN
PB_21 1
PB_22 2
PB_23 3
PB_24 4
PB_25 5
PB_26 6
PB_18 7
PB_17 8
PB_16 9
PB_11 10
PB_10 11
PB_15 12

к выводу 
PA_1 подсоеденяем кнопку замыкающую на корпус + подтянуть вывод PA_1 резистором на питание 3.3
                (я взял резистор 2 ком, то что под рукой валялось)
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
