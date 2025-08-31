
вот такую платку es9038q2m, решил переделать на 16 бит, для подключения по i2s к соответствующим источникам звука.
![photo](jpg/1.jpg)

подпаял разъем 
![photo](jpg/2.jpg)

ближе
![photo](jpg/3.jpg)

схема
![photo](jpg/4.jpg)

так
![photo](jpg/5.jpg)

к разъёму подключил Attiny85
![photo](jpg/6.jpg)

тестировал на таком стенде, 
 тут i2s берётся с vs1053b,
 просто у меня такая схема лежала собранной
![photo](jpg/7.jpg)

протестировал с yoRadio, работает!
![photo](jpg/8.jpg)



файл, лог скана i2c 

[decoder--250516-164223.txt](decoder--250516-164223.txt)



прога для переключения

[attiny85_wire1.ino](attiny85_wire1.ino)




прг. для включения режима 16-bit data words, для es9038q2m

для мк Attiny85 (была в наличии, показалось что норм. + )

и на Arduino (переделать на что то другое, не проблема....)


делал по статье [https://alexgyver.ru/lessons/naked-chip/](https://alexgyver.ru/lessons/naked-chip/)

~~~
для ATTinyCore – ядро для поддержки и расширенной настройки МК ATtiny 441/841, 44/84, 45/85, 461/861, 48/88, 828, 1634, 87, 167. 
    Ссылка для менеджера плат: http://drazzy.com/package_drazzy.com_index.json

плата выбрана "Attiny25/45/85 (no bootloader)" 

выводы:
ATtiny85 as an I2C Master. cpu @ 8MHz 
ATtiny Pin 4 = Gnd
ATtiny Pin 5 = SDA 
ATtiny Pin 7 = SCK 
ATtiny Pin 8 = VCC +3.3 

~~~
взял оттуда константы для программы (тут обсуждает другую платку, тоже на основе es9038q2m)

[форум chipdip es9038q2m](https://forum.chipdip.ru/index.php?threads/es9038q2m-%D0%B0%D1%83%D0%B4%D0%B8%D0%BE-%D0%A6%D0%90%D0%9F.474/page-21#post-15544)



Всё













---PS: 15-08-2025---

 а нет, не Всё

![photo](jpg/yoRadio_plus_es8038g2m.jpg)

в продолжение темы, сделал плагин для yoRadio, по просьбе одного товарища

смысл тут в том, чтобы отказаться от внешней микросхемы инициализации, и делать это прямо от выводов esp32s3

папку с плагином [es9038q2m_init](other/yoRadio/src/src/plugins/es9038q2m_init)

ложим в папку yoRadio/src/src/plugins


... (компилим, заливаем)

выводы для подключения указываються в [es9038q2m_init.cpp](other/yoRadio/src/src/plugins/es9038q2m_init/es9038q2m_init.cpp)

задано SDA = GPIO-6, SCL = GPIO-7 тут:
~~~
#define EQ_SDA_PIN 6
#define EQ_SCL_PIN 7
~~~

если надо другие, меняем...



---PS: 31-08-2025---


ещё фото, тут еспользован мод [yoradio-Maleksm-v0.9.512m](https://4pda.to/forum/index.php?showtopic=1010378&st=11320#entry125839228)

 + доработка библиотек [liblwip.a](other/yoRadio/esp32s3_5.4.md),
  это есть в описании мода


![photo](jpg/yoradio-(Maleksm)-v0.9.512m.jpg)

