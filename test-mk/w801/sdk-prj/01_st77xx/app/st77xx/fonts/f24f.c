//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "font.h"
#include "f24f.h"


// Таблица содержит только цифры
// Первые 2 байта символа - ширина и высота (для моноширинных шрифтов - константы)
const uint8_t f24f_table[f24f_NOFCHARS][48 + 2] = {       
  // 0x30
  {
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ____XXXX,________,
    __XXXXXX,XX______,
    __XXX__X,XX______,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    __XXX__X,XX______,
    __XXXXXX,XX______,
    ____XXXX,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x31
  ,{
    10,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ______XX,X_______,
    ______XX,X_______,
    _____XXX,X_______,
    ___XXXXX,X_______,
    __XXXXXX,X_______,
    __XXX_XX,X_______,
    __X___XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x32
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ___XXXXX,X_______,
    __XXXXXX,XX______,
    _XXXX__X,XXX_____,
    _XXX____,XXX_____,
    ________,XXX_____,
    ________,XXX_____,
    _______X,XX______,
    ______XX,XX______,
    _____XXX,X_______,
    ____XXXX,________,
    ___XXXX_,________,
    ___XXX__,________,
    __XXX___,________,
    _XXXXXXX,XXX_____,
    _XXXXXXX,XXX_____,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x33
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ___XXXXX,________,
    __XXXXXX,X_______,
    _XXX___X,XX______,
    _XXX___X,XX______,
    _______X,XX______,
    ______XX,XX______,
    ____XXXX,X_______,
    ____XXXX,X_______,
    _______X,XX______,
    ________,XXX_____,
    ________,XXX_____,
    _XXX____,XXX_____,
    _XXXX__X,XX______,
    __XXXXXX,XX______,
    ___XXXXX,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x34
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ______XX,X_______,
    _____XXX,X_______,
    _____XXX,X_______,
    ____XXXX,X_______,
    ____XXXX,X_______,
    ___XX_XX,X_______,
    __XX__XX,X_______,
    __XX__XX,X_______,
    _XX___XX,X_______,
    XX____XX,X_______,
    XXXXXXXX,XXX_____,
    XXXXXXXX,XXX_____,
    ______XX,X_______,
    ______XX,X_______,
    ______XX,X_______,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x35
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ___XXXXX,XX______,
    ___XXXXX,XX______,
    __XXX___,________,
    __XXX___,________,
    __XXX___,________,
    __XXXXXX,X_______,
    _XXXXXXX,XX______,
    _XXX___X,XXX_____,
    ________,XXX_____,
    ________,XXX_____,
    ________,XXX_____,
    _XXX____,XXX_____,
    _XXXX__X,XX______,
    __XXXXXX,XX______,
    ___XXXXX,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x36
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ____XXXX,X_______,
    __XXXXXX,XX______,
    __XXX__X,XXX_____,
    _XXX____,X_______,
    _XXX____,________,
    _XXX_XXX,X_______,
    _XXXXXXX,XX______,
    _XXXX__X,XX______,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    __XXX__X,XX______,
    __XXXXXX,XX______,
    ____XXXX,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x37
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    _XXXXXXX,XXX_____,
    _XXXXXXX,XXX_____,
    ________,XXX_____,
    _______X,XX______,
    ______XX,X_______,
    ______XX,X_______,
    _____XXX,________,
    _____XXX,________,
    ____XXX_,________,
    ____XXX_,________,
    ____XXX_,________,
    ___XXX__,________,
    ___XXX__,________,
    ___XXX__,________,
    ___XXX__,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x38
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ___XXXXX,X_______,
    __XXXXXX,XX______,
    _XXXX__X,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    __XXX__X,XX______,
    ___XXXXX,X_______,
    ___XXXXX,X_______,
    __XXX__X,XX______,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXXX__X,XXX_____,
    __XXXXXX,XX______,
    ___XXXXX,X_______,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
  // 0x39
  ,{
    12,
    f24_FLOAT_HEIGHT,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ____XXXX,________,
    __XXXXXX,XX______,
    __XXX__X,XX______,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    _XXX____,XXX_____,
    __XXX__X,XXX_____,
    __XXXXXX,XXX_____,
    ___XXXX_,XXX_____,
    ________,XXX_____,
    __XX____,XXX_____,
    _XXXX__X,XX______,
    __XXXXXX,XX______,
    ___XXXXX,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________,
    ________,________}
};


//==============================================================================
// Функция возвращает указатель на подтаблицу символа Char
// Только цифры
//==============================================================================
uint8_t *f24f_GetCharTable(uint8_t Char)
{
  // Только цифры
  if ((Char >= 0x30) && (Char <= 0x39))
    return (uint8_t *)(&f24f_table[Char - 0x30][0]);
  
  return 0;
}
//==============================================================================
