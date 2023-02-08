//Цветной графический дисплей 2.2 TFT 320x240     http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-tft-ekran-320-240---2-2.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT01_22SP,5,4,8,7,6);             // объявляем объект myGLCD класса библиотеки UTFT  указывая тип дисплея TFT01_22SP и номера выводов Arduino к которым подключён дисплей: SDI/MOSI, SCK, CS, RESET, DC/RS. Можно использовать любые выводы Arduino.
extern uint8_t SmallFont[];                    // подключаем маленький шрифт
extern uint8_t BigFont[];                      // подключаем большой шрифт
extern uint8_t SevenSegNumFont[];              // подключаем шрифт имитирующий семисегментный индикатор
                                               //
void setup(){                                  //
   myGLCD.InitLCD();                           // инициируем дисплей
}                                              //
                                               //
void loop(){                                   //
   myGLCD.clrScr();                            // стираем всю информацию с дисплея
                                               //
   myGLCD.setFont(SmallFont);                  // устанавливаем маленький шрифт
   myGLCD.setColor(VGA_WHITE);                 // устанавливаем белый цвет текста
   myGLCD.print("iarduino.ru | SmallFont", CENTER, 50); // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 50 точек)
                                               //
   myGLCD.setFont(BigFont);                    // устанавливаем большой шрифт
   myGLCD.setColor(VGA_BLUE);                  // устанавливаем синий цвет текста
   myGLCD.print("BigFont", CENTER, 100);       // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 100 точек)
   myGLCD.print("iarduino.ru", CENTER, 115);   // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 115 точек)
                                               //
   myGLCD.setFont(SevenSegNumFont);            // устанавливаем шрифт имитирующий семисегментный индикатор
   myGLCD.setColor(VGA_FUCHSIA);               // устанавливаем пурпурный цвет текста
   myGLCD.print("1234567890", CENTER, 150);    // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 150 точек)
                                               //
   delay(20000);                               // ждём 20 секунд
}                                              //
