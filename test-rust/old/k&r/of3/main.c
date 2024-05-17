#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BabyElephantWalk60_mp3.h"

void store_arr_to_file (
  char* filename,
  const unsigned char* buffer,
  size_t len)
{
FILE *fptr;

// Create a file
fptr = fopen(filename, "w");

int nlen;
fwrite(buffer, len, 1, fptr);

// Close the file
fclose(fptr);
}


void
main ()
{
  printf ("save file\r\n");

  store_arr_to_file ("BabyElephantWalk60.mp3",BabyElephantWalk60_mp3, BabyElephantWalk60_mp3_len);

  printf ("\r\n\r\nOk\r\n");
}
