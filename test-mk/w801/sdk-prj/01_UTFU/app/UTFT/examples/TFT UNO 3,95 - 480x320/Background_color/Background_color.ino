//Цветной графический дисплей 3.95 TFT 480x320 UNO http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-95-tft-480x320-uno-sensornyy.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT395UNO,A2,A1,A3,A4,A0);         // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT395UNO и номера выводов Arduino к которым подключён дисплей: RS, WR, CS, RST, SER. Выводы параллельной шины данных не указываются
                                               //
void setup(){                                  //
   myGLCD.InitLCD();                           // инициируем дисплей
   myGLCD.clrScr();                            // стираем всю информацию с дисплея
}                                              //
                                               //
void loop(){                                   //
   myGLCD.fillScr(VGA_RED   ); delay(1000);    // заливаем дисплей красным,     ждём 1 секунду
   myGLCD.fillScr(VGA_GREEN ); delay(1000);    // заливаем дисплей зелёным,     ждём 1 секунду
   myGLCD.fillScr(VGA_BLUE  ); delay(1000);    // заливаем дисплей синим,       ждём 1 секунду
   myGLCD.fillScr(VGA_SILVER); delay(1000);    // заливаем дисплей серебристым, ждём 1 секунду
   myGLCD.fillScr(VGA_MAROON); delay(1000);    // заливаем дисплей бордовым,    ждём 1 секунду
   myGLCD.fillScr(VGA_NAVY  ); delay(1000);    // заливаем дисплей тем. синим,  ждём 1 секунду
}                                              //
