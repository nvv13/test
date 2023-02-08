// подключаем библиотеку UTFT
#include <UTFT.h>

// Определяем выводы используемые для
// управления дисплеем 2.8" TFT 320x240 UNO:
#define dispMISO    8
#define dispSCK     7
#define dispCS      6
#define dispRST     5
#define dispDC      4

// объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
UTFT myGLCD(TFT01_24SP, dispMISO,
             dispSCK, dispCS,
             dispRST, dispDC);

void setup(){
    // инициируем дисплей
    myGLCD.InitLCD();
    myGLCD.clrScr();
}

void loop(){
   int n = random(7);
   int j = random(239);
   for (int i = random(239); i > 0; i--) {
       // Устанавливаем цвет RGB
       myGLCD.setColor(i, -i, j);
       switch (n) {
           case 0:
               // Рисуем прямоугольник
               myGLCD.drawRect(i*j, i, i*j+i, i+j);
               break;

           case 1:
               // Заполняем прямоугольник
               myGLCD.fillRect(i*j, i, i*j+5, i+5);

           case 2:
               // Рисуем круг
               myGLCD.drawCircle(i*j, i, i/5);
               break;

           case 3:
               // Заполняем круг
               myGLCD.fillCircle(i*j, i, 5);
               break;

           case 4:
               // Рисуем прямоугольник
               myGLCD.drawRoundRect(i*j, i, i*j+10, i+10);
               break;

           case 5:
               // Рисуем прямоугольник
               myGLCD.drawLine(i*j, i, i*j+i, i+j);
               break;

           default:
               break;
       }
   }
}
