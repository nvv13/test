//Цветной графический дисплей 2.4 TFT 240x320     http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-tft-ekran-240-320---2-4-rdquo.html

#include <UTFT.h>                                   // подключаем библиотеку UTFT для работы с дисплеем
UTFT myGLCD(TFT01_24SP,5,4,8,7,6);                  // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT01_24SP и номера выводов Arduino к которым подключён дисплей: SDI/MOSI, SCK, CS, RESET, DC/RS. Можно использовать любые выводы Arduino.
extern unsigned short image_128x64[0x2000];         // объявляем массив image_128x64 который был определён во внешнем файле
                                                    //
void setup(){                                       //
   myGLCD.InitLCD();                                // инициируем дисплей
   myGLCD.clrScr();                                 // стираем всю информацию с дисплея
   myGLCD.drawBitmap(176,128,128,64,image_128x64);  // выводим на дисплей картинку начиная с координаты 176,128 размером 128,64 из массива image_128x64
}                                                   //
                                                    //
void loop(){}                                       //
