project - version 3 

board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + KY-040 (поворотный энкодер)


При старте, подсоединяемся к сети Wifi,
далее, синхронизация времени с ntp сервером в интернете, и еще, повторы синхронизации в 3 часа ночи

так-же, есть энкодер для изменения яркости дисплея.
4 уровня яркости, после установки - нажать энкодер - запоминает во внутренний флэш.
После 23:00 и до 6:00, яркость переключается на минимальную.

+ добавил прием соединения по tcp, все взято из demo sdk, и немного доработано...


фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp/w801_quad_digit_led_display.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1" /></p>

фото 2 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp_ver3/w801_4_3.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + KY-040" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + KY-040" /></p>

фото 3 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp_ver3/w801_4_4.jpg" alt="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + KY-040" title="board HLK-W801-KIT-V1.1 + wifi + ntp + 5643AS-1 + KY-040" /></p>

KY-040 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/04_wifi_ntp_ver3/ky-040.jpg" alt="KY-040" title="KY-040" /></p>

видео 
https://youtu.be/WgEfQ8s8r3g


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



connect to
W801 KY-040 
GPIO PIN
GND  GND
+3.3 +
PA_1 SW
PA_2 DT
PA_3 CLK

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
