#include "debug.h"

#include "lcd5643.hpp"

#include "CBlink.hpp"

CBlink *PoE = NULL;
CBlink *PoD = NULL;
CBlink *PoDP = NULL;
CBlink *PoC = NULL;
CBlink *PoG = NULL;
CBlink *PoDig4 = NULL;
CBlink *PoB = NULL;
CBlink *PoDig3 = NULL;
CBlink *PoDig2 = NULL;
CBlink *PoF = NULL;
CBlink *PoA = NULL;
CBlink *PoDig1 = NULL;

void
lcd5643_init_pin (void)
{

  CBlink oE = CBlink (PD_07);    //  1  E
  CBlink oD = CBlink (PD_04);    //  2  D
  CBlink oDP = CBlink (PD_03);   //  3  DP
  CBlink oC = CBlink (PD_02);    //  4  C
  CBlink oG = CBlink (PD_01);    //  5  G
  CBlink oDig4 = CBlink (PD_00); //  6  Dig4
  CBlink oB = CBlink (PC_07);    //  7  B
  CBlink oDig3 = CBlink (PC_06); //  8  Dig3
  CBlink oDig2 = CBlink (PC_05); //  9  Dig2
  CBlink oF = CBlink (PC_04);    //  10 F
  CBlink oA = CBlink (PC_03);    //  11 A
  CBlink oDig1 = CBlink (PC_00); //  12 Dig1

  PoE = &oE;
  PoD = &oD;
  PoDP = &oDP;
  PoC = &oC;
  PoG = &oG;
  PoDig4 = &oDig4;
  PoB = &oB;
  PoDig3 = &oDig3;
  PoDig2 = &oDig2;
  PoF = &oF;
  PoA = &oA;
  PoDig1 = &oDig1;
}

static void
reg_set_num (u8 i_num) //
{
  /*
PD7  1  E
PD4  2  D
PD3  3  DP
PD2  4  C
PD1  5  G
PD0  6  Dig4
PC7  7  B
PC6  8  Dig3
PC5  9  Dig2
PC4  10 F
PC3  11 A
PC0  12 Dig1
  */
  switch (i_num)
    {
    case 0:
      {
        PoA->On ();
        PoB->On ();
        PoC->On ();
        PoD->On ();
        PoE->On ();
        PoF->On ();
        PoG->Off ();
        PoDP->Off ();
      };
      break;
    case 1: //
      {
        PoA->Off ();
        PoB->On ();
        PoC->On ();
        PoD->Off ();
        PoE->Off ();
        PoF->Off ();
        PoG->Off ();
        PoDP->Off ();
      };
      break;
    case 2: //
      {
        PoA->On ();
        PoB->On ();
        PoC->Off ();
        PoD->On ();
        PoE->On ();
        PoF->Off ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    case 3: //
      {
        PoA->On ();
        PoB->On ();
        PoC->On ();
        PoD->On ();
        PoE->Off ();
        PoF->Off ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    case 4: //
      {
        PoA->Off ();
        PoB->On ();
        PoC->On ();
        PoD->Off ();
        PoE->Off ();
        PoF->On ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    case 5: //
      {
        PoA->On ();
        PoB->Off ();
        PoC->On ();
        PoD->On ();
        PoE->Off ();
        PoF->On ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    case 6: //
      {
        PoA->On ();
        PoB->Off ();
        PoC->On ();
        PoD->On ();
        PoE->On ();
        PoF->On ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    case 7: //
      {
        PoA->On ();
        PoB->On ();
        PoC->On ();
        PoD->Off ();
        PoE->Off ();
        PoF->Off ();
        PoG->Off ();
        PoDP->Off ();
      };
      break;
    case 8: //
      {
        PoA->On ();
        PoB->On ();
        PoC->On ();
        PoD->On ();
        PoE->On ();
        PoF->On ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    case 9: //
      {
        PoA->On ();
        PoB->On ();
        PoC->On ();
        PoD->On ();
        PoE->Off ();
        PoF->On ();
        PoG->On ();
        PoDP->Off ();
      };
      break;
    }
}

static u8 i_off = 0;

void
lcd5643printDigit (u8 i_pos, u8 i_num, u8 iMode)
{
  if (i_pos > OUT_DIG_4 && i_off == 1)
    return; //
            /*
             */

  i_off = 0;
  switch (i_pos)
    {
    case OUT_SEC_IND: // sec indicator, 0=9,11.7.4.2.1.10.5 1=12,8,6 - i_num=3
      {
        if (iMode == MODE_CLOCK)
          {
            PoA->Off ();
            PoB->Off ();
            PoC->Off ();
            PoD->Off ();
            PoE->Off ();
            PoF->Off ();
            PoG->Off ();
            if (i_num)
              {
                PoDP->On ();
                PoDig1->On ();
                PoDig2->Off ();
                PoDig3->On ();
                PoDig4->On ();
              }
            else
              {
                PoDP->Off ();
                PoDig1->On ();
                PoDig2->On ();
                PoDig3->On ();
                PoDig4->On ();
              }
          }
      };
      break;

    case OUT_SIG_1: // digit 1, 0=12 1=9,8,6 - i_num=11.7.4.2.1.10.5
      {
        if (iMode == MODE_CLOCK) // case OUT_DIG_1: //digit 1, 0=12 1=9,8,6 -
                                 // i_num=11.7.4.2.1.10.5
          {
            reg_set_num (i_num);
          }
        else
          {
            PoA->Off ();
            PoB->Off ();
            PoC->Off ();
            PoD->Off ();
            PoE->Off ();
            PoF->Off ();
            PoG->On ();
            PoDP->Off ();
          }
        PoDig1->Off ();
        PoDig2->On ();
        PoDig3->On ();
        PoDig4->On ();
      };
      break;

    case OUT_DIG_2: // digit 2, 0=9 1=12,8,6 - i_num=11.7.4.2.1.10.5
      {
        reg_set_num (i_num);
        PoDig1->On ();
        PoDig2->Off ();
        PoDig3->On ();
        PoDig4->On ();
      };
      break;
    case OUT_DIG_3: // digit 3, 0=8 1=12,9,6 - i_num=11.7.4.2.1.10.5
      {
        reg_set_num (i_num);
        PoDig1->On ();
        PoDig2->On ();
        PoDig3->Off ();
        PoDig4->On ();
      };
      break;

    case OUT_C_4: // digit 4, 0=6 1=12,9,8 - i_num=11.7.4.2.1.10.5
      {
        if (iMode == MODE_CLOCK) // case OUT_DIG_4: //digit 4, 0=6 1=12,9,8 -
                                 // i_num=11.7.4.2.1.10.5
          {
            reg_set_num (i_num);
          }
        else
          {
            PoA->On ();
            PoB->Off ();
            PoC->Off ();
            PoD->On ();
            PoE->On ();
            PoF->On ();
            PoG->Off ();
            PoDP->Off ();
          }
        PoDig1->On ();
        PoDig2->On ();
        PoDig3->On ();
        PoDig4->Off ();
      };
      break;

    default: // off ligth
      {
        PoDig1->On ();
        PoDig2->On ();
        PoDig3->On ();
        PoDig4->On ();

        PoA->Off ();
        PoB->Off ();
        PoC->Off ();
        PoD->Off ();
        PoE->Off ();
        PoF->Off ();
        PoG->Off ();
        PoDP->Off ();
        i_off = 1;
      };
      break;
    }
}
