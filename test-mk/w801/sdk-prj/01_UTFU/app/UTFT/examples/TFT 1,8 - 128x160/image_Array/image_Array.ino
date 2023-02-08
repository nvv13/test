//Цветной графический дисплей 1,8 TFT 128x160     http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-tft-ekran-128-10---1-8-rdquo.html

#include <UTFT.h>                                   // подключаем библиотеку UTFT для работы с дисплеем
UTFT myGLCD(TFT18SHLD,5,4,8,7,6);                   // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT18SHLD и номера выводов Arduino к которым подключён дисплей: SDA, SCK, CS, RESET, A0. Можно использовать любые выводы Arduino.
extern unsigned short image_128x64[0x2000];         // объявляем массив image_128x64 который был определён во внешнем файле
                                                    //
void setup(){                                       //
   myGLCD.InitLCD();                                // инициируем дисплей
   myGLCD.clrScr();                                 // стираем всю информацию с дисплея
   myGLCD.drawBitmap(176,128,128,64,image_128x64);  // выводим на дисплей картинку начиная с координаты 176,128 размером 128,64 из массива image_128x64
}                                                   //
                                                    //
void loop(){}                                       //
