#include <stdio.h>

/*
Exercise 3-5. Write the function itob(n,s,b) that converts the integer n into a base b
character representation in the string s. In particular, itob(n,s,16) formats s as a
hexadecimal integer in s.

Упражнение 3-5. Напишите функцию itob(n,s,b), которая переводит целое число n в основание b.
представление символов в строке s. В частности, itob(n,s,16) форматирует s как
шестнадцатеричное целое в s.
*/
void itob(unsigned int n, unsigned char * s,unsigned char b) //b - от 2 до 16 !!!
{
if(!(b>1 && b<=16))
 {
 s[0]=0;
 return;
 }
unsigned int i_index=0, i_rev=0;
do
 {
 unsigned char ost=n%b; 
 s[i_index++]=(ost<11?ost+'0':ost-10+'a');
 n=n/b; 
 }while(n>0);
s[i_index--]=0;
do
 {
 unsigned char x0=s[i_rev];
 s[i_rev++]=s[i_index];
 s[i_index--]=x0;
 }while(i_index>i_rev);
}




void main()
{
printf("hello\r\n");
unsigned int n = 0x503291; // 0x10010001b
printf("%3d 0x%xh\r\n",n,n);


char s[100];
s[0]=0;

itob(n, s, 16);
printf("b=16 %s\r\n",s);

itob(n, s, 10);
printf("b=10 %s\r\n",s);

itob(n, s, 8);
printf("b=8 %s\r\n",s);

itob(n, s, 5);
printf("b=5 %s\r\n",s);

itob(n, s, 3);
printf("b=3 %s\r\n",s);

itob(n, s, 2);
printf("b=2 %s\r\n",s);

}

