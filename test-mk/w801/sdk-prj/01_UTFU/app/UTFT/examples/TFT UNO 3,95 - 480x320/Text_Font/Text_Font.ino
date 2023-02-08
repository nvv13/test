//Цветной графический дисплей 3.95 TFT 480x320 UNO http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-95-tft-480x320-uno-sensornyy.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT395UNO,A2,A1,A3,A4,A0);         // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT395UNO и номера выводов Arduino к которым подключён дисплей: RS, WR, CS, RST, SER. Выводы параллельной шины данных не указываются
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
