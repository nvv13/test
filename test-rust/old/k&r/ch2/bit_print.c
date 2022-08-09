#include <stdio.h>

void bit_print(unsigned char ch1)
{
printf("%3d 0x%xh 0x",ch1,ch1);
unsigned char mask=0x80;
for(int i=0;i<8;i++)
 {
 if((ch1 & mask)==0)
  printf("0");
  else
  printf("1");
 mask = mask >> 1;
 }
printf("b\r\n");
}


