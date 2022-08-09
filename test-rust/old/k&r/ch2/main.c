#include <stdio.h>
#include "bit_print.h"


/* getbits: get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{return (x >> (p+1-n)) & ~(~0 << n);
}

unsigned setbits(unsigned x,int p,int n,int y)
{
/*
Упражнение 2-6. Напишите функцию setbits(x,p,n,y), которая возвращает x с n битами, начинающимися с
позиция p устанавливается на крайние правые n битов y, оставляя остальные биты без изменений.
*/
unsigned k= ~(~0 << (p+1)) & (~0 << (p+1-n)) ; // маска
return (x & ~k) | ((y<<(p+1-n)) & k) ;

}

unsigned invert(unsigned x,int p,int n)
{
/*
Упражнение 2-7. Напишите функцию invert(x,p,n), которая возвращает x с n битами, начинающимися с
позиция p инвертируется (т. е. 1 превращается в 0 и наоборот), оставляя остальные без изменений.
 позиции бит 7..0 !!!
*/
unsigned k= ~(~0 << (p+1)) & (~0 << (p+1-n)) ; // маска
return (x & ~k) | (~x & k) ;
}

unsigned rightrot(unsigned x,int n)
{
/*Упражнение 2-8. Напишите функцию rightrot(x,n), которая возвращает значение целого числа x, повернутого
  вправо на n позиций.*/
while(n--)
 {
 if(x & 1)
  x=(x >> 1) | 0x80;
  else
  x=x >> 1;
 }
return x;
}


void main()
{
printf("hello\r\n");
                         //   76543210
//unsigned char b1 = 0x88; // 0x10001000b
unsigned char b1 = 0x91; // 0x10010001b
//unsigned char b1 = 0xC3; // 0x11000011b
printf("             b1:");bit_print(b1);
printf("getbits(b1,0,1):");bit_print(getbits(b1,0,1));
printf("getbits(b1,1,2):");bit_print(getbits(b1,1,2));
printf("getbits(b1,7,2):");bit_print(getbits(b1,7,2));


printf("\r\n");
printf("Упражнение 2-6. Напишите функцию setbits(x,p,n,y), которая возвращает x с n битами, начинающимися с\r\n");
printf("позиция p устанавливается на крайние правые n битов y, оставляя остальные биты без изменений.\r\n");
b1 = 0xC3; 
printf("               b1:");bit_print(b1);
printf("setbits(b1,1,2,3):");bit_print(setbits(b1,1,2,3));
printf("setbits(b1,4,2,3):");bit_print(setbits(b1,4,2,3));

printf("\r\n");
printf("Упражнение 2-7. Напишите функцию invert(x,p,n), которая возвращает x с n битами, начинающимися с\r\n");
printf("позиция p инвертируется (т. е. 1 превращается в 0 и наоборот), оставляя остальные без изменений.\r\n");
printf(" позиции бит 7..0 !!!\r\n");
b1 = 0xC3; 
printf("            b1:");bit_print(b1);
printf("invert(b1,1,2):");bit_print(invert(b1,1,2));
printf("invert(b1,4,2):");bit_print(invert(b1,4,2));
b1 = (unsigned char)~0xC3; 
printf("            b1:");bit_print(b1);
printf("invert(b1,1,2):");bit_print(invert(b1,1,2));
printf("invert(b1,4,2):");bit_print(invert(b1,4,2));


printf("\r\n");
printf("Упражнение 2-8. Напишите функцию rightrot(x,n), которая возвращает значение целого числа x, повернутого\r\n");
printf("вправо на n позиций.\r\n");

b1 = 0x1; 
printf("            b1:");bit_print(b1);
printf("rightrot(b1,0):");bit_print(rightrot(b1,0));
printf("rightrot(b1,1):");bit_print(rightrot(b1,1));
printf("rightrot(b1,2):");bit_print(rightrot(b1,2));
printf("rightrot(b1,3):");bit_print(rightrot(b1,3));
printf("rightrot(b1,4):");bit_print(rightrot(b1,4));
printf("rightrot(b1,5):");bit_print(rightrot(b1,5));
printf("rightrot(b1,6):");bit_print(rightrot(b1,6));
printf("rightrot(b1,7):");bit_print(rightrot(b1,7));
printf("rightrot(b1,8):");bit_print(rightrot(b1,8));

}

