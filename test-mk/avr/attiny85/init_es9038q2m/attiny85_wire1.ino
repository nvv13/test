/*
прг. для включения режима 16-bit data words, для es9038q2m

для мк Attiny85 (была в наличии, показалось что норм. + )

делал по статье https://alexgyver.ru/lessons/naked-chip/

для ATTinyCore – ядро для поддержки и расширенной настройки МК ATtiny 441/841, 44/84, 45/85, 461/861, 48/88, 828, 1634, 87, 167. 
    Ссылка для менеджера плат: http://drazzy.com/package_drazzy.com_index.json

плата выбрана "Attiny25/45/85 (no bootloader)"    

ATtiny85 as an I2C Master. cpu @ 8MHz   
ATtiny Pin 4 = Gnd
ATtiny Pin 5 = SDA 
ATtiny Pin 7 = SCK 
ATtiny Pin 8 = VCC +3.3 

например, инит можно посмотреть тут, взял оттуда корстанты
https://forum.chipdip.ru/index.php?threads/es9038q2m-%D0%B0%D1%83%D0%B4%D0%B8%D0%BE-%D0%A6%D0%90%D0%9F.474/page-21#post-15544

*/

// Адрес ЦАПА //
#define ES90381 0x48

// Регистры для инициализации //
#define Mad_AutoSelect 0x01

#include <Wire.h>

void setup() {
  delay(1000);
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.setClock(400000);
  delay(5);
  Wire.beginTransmission(ES90381); // transmit to device
  Wire.write(Mad_AutoSelect);      // sends one byte
  Wire.write(4);                   // sends one byte
  Wire.endTransmission();          // stop transmitting
  //Wire.end(); не отпускать шину!
}

void loop() {
  delay(1000);
}
