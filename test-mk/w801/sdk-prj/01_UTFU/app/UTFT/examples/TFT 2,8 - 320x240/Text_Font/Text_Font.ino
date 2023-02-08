// Цветной графический дисплей 2.8 TFT 320x240 https://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-2-8-tft-320x240.html
// подключаем библиотеку UTFT
#include <UTFT.h>

// Определяем выводы используемые для
// управления дисплеем 2.8" TFT 320x240 UNO:
#define dispMISO    8
#define dispSCK     7
#define dispCS      6
#define dispRST     5
#define dispDC      4

// подключаем шрифты
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
UTFT myGLCD(TFT01_24SP, dispMISO,
             dispSCK, dispCS,
             dispRST, dispDC);

void setup(){
    // инициируем дисплей
    myGLCD.InitLCD();
}

void loop(){

    // стираем всю информацию с дисплея
    myGLCD.clrScr();

    // устанавливаем маленький шрифт
    myGLCD.setFont(SmallFont);
    // устанавливаем белый цвет текста
    myGLCD.setColor(VGA_WHITE);
    // выводим текст на дисплей (выравнивание по ширине -
    // центр дисплея, координата по высоте 50 точек)
    myGLCD.print("iarduino.ru | SmallFont", CENTER, 50);

    // устанавливаем большой шрифт
    myGLCD.setFont(BigFont);
    // устанавливаем синий цвет текста
    myGLCD.setColor(VGA_BLUE);
    // выводим текст на дисплей (выравнивание по ширине -
    // центр дисплея, координата по высоте 100 точек)
    myGLCD.print("BigFont", CENTER, 100);
    // выводим текст на дисплей (выравнивание по ширине -
    // центр дисплея, координата по высоте 115 точек)
    myGLCD.print("iarduino.ru", CENTER, 115);

    // устанавливаем шрифт имитирующий семисегментный индикатор
    myGLCD.setFont(SevenSegNumFont);
    // устанавливаем пурпурный цвет текста
    myGLCD.setColor(VGA_FUCHSIA);
    // выводим текст на дисплей (выравнивание по ширине -
    // центр дисплея, координата по высоте 150 точек)
    myGLCD.print("1234567890", CENTER, 150);

    // ждём 20 секунд
    delay(20000);
}
