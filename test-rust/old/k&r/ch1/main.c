#include <stdio.h>

void my1( char * ch1);

void main()
{
printf("hello\r\n");

char msg[] = "m0123456789m";

printf("msg: %s\r\n", msg);
my1(msg);
printf("msg: %s\r\n", msg);

}

void my1( char * ch1)
{
if(00)
 {
 ch1[3]='_';
 }
}

