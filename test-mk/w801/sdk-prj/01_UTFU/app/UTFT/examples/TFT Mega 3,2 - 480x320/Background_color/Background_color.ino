//Цветной графический дисплей 3.2 TFT 480x320 MEGA http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-2-tft-480x320.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT32MEGA, 38,39,40,41);           // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT32MEGA и номера выводов Arduino к которым подключён дисплей: RS, WR, CS, RST. Выводы параллельной шины данных не указываются
//          TFT32MEGA_2                        // если изображение на дисплее отображается зеркально, значит для инициализации Вашего дисплея, нужно указать не TFT32MEGA, а TFT32MEGA_2.
                                               // (тип TFT32MEGA - для дисплеев на базе чипа ILI9481, а тип TFT32MEGA_2 - для дисплеев на базе чипа HX8357С).
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
