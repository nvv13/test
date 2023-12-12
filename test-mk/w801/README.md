платка 
HLK-W801-KIT-V1.1

~~~
описание есть на https://www.cnx-software.com/2021/11/30/hlk-w801-board-features-alibaba-xuantie-xt804-based-mcu-with-wifi-4-bluetooth-le-4-2

HLK-W801 board specifications:

MCU – WinnerMicro W801 32-bit Alibaba XT804 microcontroller @ 240 MHz (Xuantie E804 IP, c-sky 4-конвеера)
  with 2MB Flash, 288KB RAM, FPU, DSP, crypto engine, 2.4 GHz radio

Connectivity
 802.11b/g/n WiFi 4 up to 150 Mbps supporting station, soft AP and station+soft AP modes, WPA2 security
 Bluetooth/BLE 4.2

Expansion – 2x 24-pin headers with
 1x SDIO host with support for SDIO 2.0, SDHC, MMC 4.2 (Configurable interface clock rate, support host rate 0~50MHz)
 1x SDIO device up to 200 Mbps (for 4 data line)
 Up to 6x UART
 1x I2C working frequency support can be configured, 100K-400K.
 1x high-speed SPI interface, supporting up to 50MHz
 1x master/slave SPI up to 20MHz (8-word-deep FIFOs for transmit and receive paths,
    Support full duplex and half duplex, The master device supports bit transmission,
    up to 65536 bit transmission (8192 byte) )
 LCD controller with support for 4×32 interface
 1x I2S
 Up to 4x ADC (16-bit, 1KHz sampling rate)
 Up to 44x GPIOs, 5x PWM
 Up to 15 touch sensors
 ISO/IEC 7816 smart card interface
 Reset, 3.3V, and GND signals
Debugging – CH340 via USB-C port
Misc – Reset and Boot buttons, 7x user LEDs including two connected to PWM capable pins
Power Supply – 5V via USB-C port
Power consumption – Standby: Less than 10 uA for W801 MCU, not the board itself
Dimensions – About 70 x 26 mm


 Timers - Microsecond and millisecond timing (the number of counts is configured according to the clock frequency),
          and six configurable 32-bit counters are realized. When the counting configured by the corresponding
          calculator is completed When successful, a corresponding interrupt is generated.

 Watchdog Controller

 PWM Controller - 5-channel PWM signal generation function
          2-channel input signal capture function (PWM0 and PWM4 two channels)
          Frequency range: 3Hz~160KHz
          Maximum accuracy of duty cycle: 1/256, counter width for inserting dead zone: 8bit


~~~




фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/jpg/HLK-W801-KIT-V1.1.jpg" alt="HLK-W801-KIT-V1.1" title="HLK-W801-KIT-V1.1" /></p>

фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/jpg/HLK-W801-KIT-V1-1.jpg" alt="HLK-W801-KIT-V1-1" title="HLK-W801-KIT-V1-1" /></p>

w801_pinout
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/W801_SDK-linux/doc/w801_pinout.png" alt="HLK W801 pinout" title="HLK W801 pinout" /></p>
w801_pinout
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/jpg/w801_pinout.png" alt="HLK W801 pinout" title="HLK W801 pinout" /></p>


WinnerMicro W801 Block Diagram
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/jpg/winnermicro-w801-block-diagram.jpg" alt="WinnerMicro W801 Block Diagram" title="WinnerMicro W801 Block Diagram" /></p>


W800 Clock Structure
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/jpg/W800%20Clock%20Structure.jpg" alt="W800 Clock Structure" title="W800 Clock Structure" /></p>


W800 address mapping
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/jpg/W800%20address%20mapping.jpg" alt="W800 address mapping" title="W800 address mapping" /></p>


подробнее на https://chowdera.com/2022/03/202203272225090142.html

про SDK, инициализацию и старт https://javamana.com/2022/03/202203272225056680.html


вот наиболее свежий SDK что я нашел 
версия SDK от 2023/08/25 и бывают обновления !!, ищите на странице https://www.winnermicro.com/html/1/156/158/558.html





еще приложил в папку doc, документы переведенные с китайского на английский, с помощью - Machine Translated by Google.

и исходные, если смотреть в исходный и переведённый, вроде что то становиться понятно.

-----------

делал по инструкциям из этого проекта ( у меня Linux Fedora 33 x86_64 )
https://github.com/droppingy/W801_SDK-linux
-----------

1) первое замечание, насчет конденсатора
 Board : HLK-W801-KIT-V1.0 (add a capacitor (4.7uF) between GND and RESET)
   про это есть обьяснение в статье
   https://www.cnblogs.com/milton/p/15621540.html
   
   поскольку у меня платка HLK-W801-KIT-V1.1 , я конденсатор не припаял, и .. работает

-----------

2) второе, установка :
 Toolchain: csky-elfabiv2-tools-x86_64-minilibc-20210423  https://github.com/droppingy/hlk-w80x-toolchain 
   я его скачал как zip архив
   потом распаковал в директорию /opt 
   так чтобы в итоге получилось /opt/w80x-tools/bin
   это чтоб в menuconfig не переделывать, такой там путь по умолчанию
 (еще, в этом мануале https://github.com/IOsetting/wm-sdk-w806, есть ссылки как получить данный Toolchain на другие архитектуры, типа i386)

-----------

3) далее, из домашней директории

$ mkdir w800

$ cd w800

$ git clone https://github.com/droppingy/W801_SDK-linux.git

  тут все по умолчанию подошло ( порт подключённой платки оказалься /dev/ttyUSB0 )

$ make menuconfig

$ make


  далее прошивка, даже кнопку boot не пришлось нажимать ( когда пробовал проект на Arduino, там потребовалось, это я про https://github.com/Hi-LinkDuino/w80x_arduino )

$ make flash

  далее подцепляемся к платке, для ипользования демо проекта (60 функций!!!)

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ... или в опция --lower-rts 

$ picocom --echo -b 115200 /dev/ttyUSB0 --lower-rts

          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))
  и еще, у меня не удавалось в терминале, просто набрать комманду,
  но, если выделить, и вставить комманду через буфер обмена (дополнив чем надо - иногда) все работает!

  можно прямо отсюда выделять-(подправлять)-копировать...
~~~
Sequence  Command             Description
1         t-connect           Test connecting ap;t-connect("ssid","pwd"); For open ap, pwd should be empty
2         t-oneshot           Test Oneshot  configuration
3         t-webcfg            Test web server configuration
4         t-apsta             Test connecting with AP by apsta mode;
5         t-asskt             Test socket communication in apsta mode;
6         t-softap            Test softap create & station join monitor;
7         t-scan              Test wifi scan
8         t-uart              Test uart tx/rx; For example t-uart=(9600,0,0),baudrate 9600 ,parity none and 1 stop bit
9         t-gpioirq           Test gpio interrupt services
10        t-gpio              Test gpio read and write
11        t-flash             Test Read/Write Flash 
12        t-crypt             Test Encryption/Decryption API
13        t-rsa               Test RSA Encryption/Decryption API
14        t-rtc               Test rtc
15        t-timer             Test timer
16        t-pwm               Test PWM output, for example t-pwm=(0,20,99,1,0) to test ALLSYC mode.
17        t-pmuT0             Test power management unit with timer0
18        t-pmuT1             Test power management unit with timer1
19        t-i2c               Test I2C module, for example t-i2c to W&R AT24CXX.
20        t-i2sioinit         Initialize I2S IO.
21        t-i2s               Test I2S module, for example t-i2s=(0,1,44100,16,0,0) to send data.
22        t-mspi-s            Test SPI Master sending data(Note: need another module acts as a client device)
23        t-mspi-r            Test SPI Master receiving data(Note: need another module acts as a client device)
24        t-sspi              Test slave HSPI,t-sspi=(0),(Note: need another module support as a master device)
25        t-sdh               Test sdio host write & read sd card
26        t-adctemp           (ADC)Test chip temperature
27        t-adcvolt           (ADC)Test input voltage,0-PA1(chan0), 1-PA4(chan1),8-different
28        t-adcpower          (ADC)Sample power supply voltage
29        t-7816              Test 7816 tx/rx function
30        t-sockc             Test data stream as [STANDARD SOCKET] CLIENT(working after connecting with AP successfully)
31        t-skcsnd            Test socket client send data, len:send len, uart_trans: is or not use uart retransmission
32        t-socks             Test data stream as [STANDARD SOCKET] SERVER(working after connecting with AP successfully)
33        t-skssnd            Test socket server send data skt_no:socket num, len:send len, uart_trans: is or not use uart retransmission
34        t-client            Test socket client; t-client("ssid","pwd",port,"ip")
35        t-server            Test socket server; t-server("ssid","pwd",port,)
36        t-udp               Test data stream as UDP(working after connecting with AP successfully)
37        t-sndudp            Test udp send data
38        t-ntp               Test NTP
39        t-setntps           Set NTP server ip;For example:t-setntps("cn.ntp.org.cn", "ntp.sjtu.edu.cn", "192.168.1.101"),max server num is 3
40        t-queryntps         Query the NTP server domain
41        t-httpfwup          Test firmware update via HTTP, like this t-httpfwup=(http://192.168.1.100:8080/WM_W600_SEC.img)
42        t-httpget           Test HTTP get method, like this t-httpget
43        t-httppost          Test HTTP post method, like this t-httppost=(user=winnermicro)
44        t-httpput           Test HTTP put method, like this t-httpput=(user=winnermicro)
45        t-ssl-server        Test ssl server,remember to turn on TLS_CONFIG_SERVER_SIDE_SSL
46        t-websockets        websockets demo test
47        t-https             Test https request
48        t-mbedtls           Test mbedtls ssl
49        t-mqtt              Test mqtt
50        t-fatfs             Test fatfs on sd card
51        t-dsp               DSP demo:0-fir,1-matrix,2-rfft,3-sin,4-variance
52        t-bt-on             Test enable bt system
53        t-bt-off            Test destroy bt system
54        t-ble-server-on     Test enable ble server
55        t-ble-server-off    Test disable ble server
56        t-ble-client-on     Test enable ble client
57        t-ble-client-off    Test disable ble client
58        t-ble-adv           Test start connectable/unconnectable/stop ble advertisement,eg: t-ble-adv=(1/2/0)
59        t-ble-scan          Test start/stop ble scan,eg: t-ble-scan=(1/0)
60        t-touch             Test Touch sensor function,0:all, 1:touch sensor 1... 15:touch sensor 15
61        demohelp            Display Help information
62        reset
~~~

например , берём t-connect("ssid","pwd") в блокнот
подправляем t-connect("mywifi2","94510984")
и вставляем в терминал...
                         
-----------

4) что дальше?
  например, изучаем пиложенный файлик
  ~/w800/W801_SDK-linux/doc.w800_apis.chm
  так-же каталог doc
  а еще исходники этой demo программы, очень неплохо


также в директории

sdk-prj

сделал несколько тестовых проектов
~~~
01_blink           - пробовал, какие частоты может выдавать плата
01_task_blink      - rtos blink
01_IR_scan         - IR remote control
01_hd44780_gpio    - подключение LCD display 16x2
01_i2c_scanner     - опрос i2c шины
01_I2S             - воспроизведение WAV файла  
01_SD_Card         - выводим raw картинки с sd card
01_tm1637          - подключение display tm1637
01_u8g2_ssd1309_spi- подключение display ssd1309 по spi + библиотека u8g2
01_UTFT            - библиотека UTFT, подключение различных TFT дисплеев, jpeg картинки
01_URTouch         - библиотека URTouch + примеры использования
01_psram_disk      - подключение микросхемы psram и использование 4мб в качестве диска (кэш файлов и т.д.)
01_VS1053          - библиотека для микросхемы vs1053b + пример воспроизведения с SD Card
02_WebRadio1       - использование vs1053b для создания WiFi Radio
03_ws2812b_RGB_LED_SPI2 - управлени адресной лентой ws2812b 
04_wifi_ntp        - часы по NTP протоколу     
05_https_weather   - запрос погоды по протоколу https, пример ОТА 
01_HTTP_Server     - примеры использования Mongoose networking library 
~~~


-----------

5) еще, 
очистить проект

$ make distclean


стереть флэш в микроконтроллере

$ make erase



-----------

6) отформатировать исходный текст (ставим $ dnf install clang-tools-extra)

$ clang-format --style=GNU --verbose -i *.c* *.h*

или (я добавил в make)

$ make fmt


-----------

7) еще, для 
make menuconfig

  надо поставить ncurses-devel

$ dnf install ncurses-devel

-----------

8) Прочее (Linux):

 компилируем, и перенаправляем консоль ошибок(2) в файл

$ make 2> a.txt 


-----------

02-2023 SDK обновил из:
https://github.com/leoay/wm-sdk-w80x-2023.git
и добавил перевод изменений, w801_pinout, из
https://github.com/Gusev-Roman/wm-sdk-w80x-2023.git
интерестная статья по теме
http://we.easyelectronics.ru/part/w801-zhk-ekranchik-i-lozhechka-degtya.html

-----------


про DSP
https://ai.nationalchip.com/dsp/DSP_en/html/csky__rfft__q15_8c.html


-----------
