/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-06-05
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wm_type_def.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
#include "wm_cpu.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"
//#include "csi_core.h"

#include "HD44780LCD.h"
#include "n_delay.h"

// Section: Defines
#define DISPLAY_DELAY_1 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000

// Section: Function Prototypes

void helloWorld (void);
void cursorMoveTest (void);
void scrollTest (void);
void gotoTest (void);
void clearLineTest (void);
void cursorTest (void);
void entryModeTest (void);
void writeNumTest (void);
void customChar (void);
void backLightTest (void);
void endTest (void);

const u8 u8_row = 2;
const u8 u8_col = 16;

void
UserMain (void)
{
  printf ("user task LCD test u8_row=%d, u8_col=%d\n", u8_row, u8_col);

  // tls_sys_clk_set (CPU_CLK_240M);
  // tls_sys_clk_set (CPU_CLK_160M);
  // tls_sys_clk_set (CPU_CLK_80M);
  // tls_sys_clk_set (CPU_CLK_40M);
  // tls_sys_clk_set (CPU_CLK_2M);

  HD44780LCD_HD44780LCD (u8_row, u8_col); // instantiate an object

  HD44780LCD_LCDInit (LCDCursorTypeOn);
  HD44780LCD_LCDClearScreen ();
  HD44780LCD_LCDBackLightSet (true);

  tls_sys_clk sysclk;

  for (;;)
    {
  tls_sys_clk_get (&sysclk);
  printf ("  sysclk.apbclk %d\n", sysclk.apbclk);
  printf ("  sysclk.cpuclk %d\n", sysclk.cpuclk);

  HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);
  HD44780LCD_printf ("clk.apbclk %d", sysclk.apbclk);
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0); // B
  HD44780LCD_printf ("clk.cpuclk %d", sysclk.cpuclk);
  n_delay_ms (DISPLAY_DELAY);
  HD44780LCD_LCDClearScreen ();

      helloWorld ();
      cursorMoveTest ();
      scrollTest ();
      gotoTest ();
      clearLineTest ();
      cursorTest ();    // Cursor type and screen reset test(4 off ) check,
                        //   cursor mode is changed by calling reset.
      entryModeTest (); // Text entry mode (4 off ) Note: if screen is reset
                        // non default entry mode setting will also be reset
      writeNumTest ();  // Print numerical data using print() test
      customChar ();    // Custom character from CGRAM test
      backLightTest ();

    switch(sysclk.cpuclk) // восстанавливаем частоту
    {
     case 2:
       {
       tls_sys_clk_set(CPU_CLK_40M); 
       };break;
     case 40:
       {
       tls_sys_clk_set(CPU_CLK_80M); 
       };break;
     case 80:
       {
       tls_sys_clk_set(CPU_CLK_160M); 
       };break;
     case 160:
       {
       tls_sys_clk_set(CPU_CLK_240M); 
       };break;
     case 240:
       {
       tls_sys_clk_set(CPU_CLK_2M); 
       };break;
    }


    }
}

// Section :  Functions

void
helloWorld (void)
{
  char teststr1[] = "Hello";
  char teststr2[] = "World";
  HD44780LCD_LCDGOTO (u8_row > 2 ? LCDLineNumberTwo : LCDLineNumberOne, 0);
  HD44780LCD_LCDSendString (teststr1);
  HD44780LCD_LCDGOTO (u8_row > 2 ? LCDLineNumberThree : LCDLineNumberTwo, 0);
  HD44780LCD_LCDSendString (teststr2); // Display a string
  HD44780LCD_LCDSendChar ('!');        // Display a single character
  n_delay_ms (DISPLAY_DELAY_1);
}

void
cursorMoveTest (void)
{
  HD44780LCD_LCDMoveCursor (LCDMoveRight, 2);
  n_delay_ms (DISPLAY_DELAY);
  HD44780LCD_LCDMoveCursor (LCDMoveLeft, 2);
}

void
scrollTest (void)
{
  for (uint8_t i = 0; i < 5; i++)
    {
      HD44780LCD_LCDScroll (LCDMoveRight, 1);
      n_delay_ms (DISPLAY_DELAY_2);
    }
  HD44780LCD_LCDScroll (LCDMoveLeft, 5);
  n_delay_ms (DISPLAY_DELAY_2);
}

void
gotoTest (void)
{
  char teststr1[] = "Line 1";
  char teststr2[] = "Line 2";
  char teststr3[] = "Line 3";
  char teststr4[] = "Line 4";
  HD44780LCD_LCDClearScreen ();
  HD44780LCD_LCDGOTO (LCDLineNumberOne, 2);
  HD44780LCD_LCDSendChar ('A');
  HD44780LCD_LCDSendString (teststr1);
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 2);
  HD44780LCD_LCDSendString (teststr2);

  if (u8_row > 2)
    {
      HD44780LCD_LCDGOTO (LCDLineNumberThree, 2);
      HD44780LCD_LCDSendString (teststr3);
      HD44780LCD_LCDGOTO (LCDLineNumberFour, 2);
      HD44780LCD_LCDSendString (teststr4);
    }
  n_delay_ms (DISPLAY_DELAY);
}

void
clearLineTest (void)
{
  HD44780LCD_LCDClearLine (LCDLineNumberTwo);
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearLine (LCDLineNumberOne);
  n_delay_ms (DISPLAY_DELAY_2);
}

void
cursorTest (void)
{
  char teststr1[] = "Cursor no 4";
  char teststr2[] = "Cursor no 1";
  char teststr3[] = "Cursor no 2";
  char teststr4[] = "Cursor no 3";

  HD44780LCD_LCDResetScreen (LCDCursorTypeOnBlink); // type 4 cursor
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0);
  HD44780LCD_LCDSendString (teststr1);
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearLine (LCDLineNumberTwo);

  HD44780LCD_LCDResetScreen (LCDCursorTypeOff); // type 1 cursor
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0);
  HD44780LCD_LCDSendString (teststr2);
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearLine (LCDLineNumberTwo);

  HD44780LCD_LCDResetScreen (LCDCursorTypeBlink); // type 2 cursor
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0);
  HD44780LCD_LCDSendString (teststr3);
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearLine (LCDLineNumberTwo);

  HD44780LCD_LCDResetScreen (
      LCDCursorTypeOn); // Back to initial state , type 3
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0);
  HD44780LCD_LCDSendString (teststr4);
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearLine (LCDLineNumberTwo);
}

void
writeNumTest ()
{
  int numPos = 193;
  int numNeg = -8582;
  double myPI = 3.1456;

  HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);
  HD44780LCD_printf ("%d", numPos);
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0);
  HD44780LCD_printf ("%d", numNeg);
  HD44780LCD_LCDMoveCursor (LCDMoveRight, 2);
  HD44780LCD_printf ("%f", myPI);

  n_delay_ms (DISPLAY_DELAY);
  HD44780LCD_LCDClearScreen ();

  uint8_t ch1 = 11;

  HD44780LCD_LCDGOTO (LCDLineNumberOne, 0); // 11
  HD44780LCD_printf ("%d", ch1);
  HD44780LCD_LCDMoveCursor (LCDMoveRight, 2); // 13
  HD44780LCD_printf ("%o", ch1);
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0); // B
  HD44780LCD_printf ("%.2X", ch1);
  HD44780LCD_LCDMoveCursor (LCDMoveRight, 1); // 1011

  char Char[9];
  uint8_t mask = 0x80;
  for (u8 i = 0; i < 8; i++)
    {
      if ((ch1 & mask) == 0)
        Char[i] = '0';
      else
        Char[i] = '1';
      mask = mask >> 1;
    }
  Char[8] = 0;

  HD44780LCD_printf ("0x%sb", Char);

  n_delay_ms (DISPLAY_DELAY);
}

void
customChar (void)
{

  // Data to test custom function
  uint8_t bellChar[8] = { 0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4 };
  uint8_t noteChar[8] = { 0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0 };
  uint8_t clockChar[8] = { 0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0 };
  uint8_t heartChar[8] = { 0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0 };
  uint8_t duckChar[8] = { 0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0 };
  uint8_t checkChar[8] = { 0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0 };
  uint8_t crossChar[8] = { 0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0 };
  uint8_t enterChar[8] = { 0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4 };

  HD44780LCD_LCDClearScreen ();

  // Load the CGRAM with the data , custom characters
  // location argument must be 0 to 7
  HD44780LCD_LCDCreateCustomChar (0, bellChar); // Location 0
  HD44780LCD_LCDCreateCustomChar (1, noteChar);
  HD44780LCD_LCDCreateCustomChar (2, clockChar);
  HD44780LCD_LCDCreateCustomChar (3, heartChar);
  HD44780LCD_LCDCreateCustomChar (4, duckChar);
  HD44780LCD_LCDCreateCustomChar (5, checkChar);
  HD44780LCD_LCDCreateCustomChar (6, crossChar);
  HD44780LCD_LCDCreateCustomChar (7, enterChar); // Location 7

  HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);

  // Print out custom characters from
  // CGRAM locations 0-7
  // location argument must be 0 to 7
  for (uint8_t i = 0; i < 8; i++)
    {
      HD44780LCD_LCDPrintCustomChar (i);
      HD44780LCD_LCDMoveCursor (LCDMoveRight, 1);
    }

  n_delay_ms (DISPLAY_DELAY);
  HD44780LCD_LCDClearScreen ();
}

void
backLightTest (void)
{
  char teststr4[] = "Back Light";
  // Needs another command/data before it changes Light
  HD44780LCD_LCDBackLightSet (false);

  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 1);
  HD44780LCD_LCDSendString (teststr4);
  n_delay_ms (DISPLAY_DELAY);
  HD44780LCD_LCDBackLightSet (true);
  HD44780LCD_LCDClearScreen ();
}

void
entryModeTest (void)
{

  char teststr8[] = "1234";

  HD44780LCD_LCDChangeEntryMode (LCDEntryModeOne);
  HD44780LCD_LCDGOTO (LCDLineNumberOne, 8);
  HD44780LCD_LCDSendString (teststr8); // <-C4321
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearScreenCmd ();

  HD44780LCD_LCDChangeEntryMode (LCDEntryModeTwo);
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 8);
  HD44780LCD_LCDSendString (teststr8); // C4321->
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearScreenCmd ();

  HD44780LCD_LCDChangeEntryMode (LCDEntryModeFour);
  HD44780LCD_LCDGOTO (LCDLineNumberTwo, 8);
  HD44780LCD_LCDSendString (teststr8); // <-1234C
  n_delay_ms (DISPLAY_DELAY_2);
  HD44780LCD_LCDClearScreenCmd ();

  HD44780LCD_LCDChangeEntryMode (
      LCDEntryModeThree); // Set back to default entry mode
  HD44780LCD_LCDClearScreenCmd ();
  n_delay_ms (DISPLAY_DELAY_1);
}
