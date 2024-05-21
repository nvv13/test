#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void
store_fileBin_to_fileArrC (char *filenameBin)
{
  FILE *fptr, *fptrRead;
  char filename[256] = { 0 };

  strcat (filename, filenameBin);
  strcat (filename, ".h");

  // Open a file
  fptrRead = fopen (filenameBin, "rb");

  // Create a file
  fptr = fopen (filename, "w");

  int nlen;
  // fwrite(buffer, len, 1, fptr);

  fputs ("// converted with xxd\n", fptr);
  fputs ("const unsigned char Arr_mp3[] = {\n", fptr);

  unsigned char buffer[16];
  size_t num_items = 1;
  while (num_items != 0)
    {
      num_items = fread (buffer, sizeof (char), 16, fptrRead);
      if (num_items != 0)
        {
          for (int i = 0; i < num_items - 1; i++)
            {
              fprintf (fptr, " 0x%x,", buffer[i]);
            }
          fprintf (fptr, " 0x%x, \n", buffer[num_items - 1]);
          nlen += num_items;
        }
    }
  fputs (" 0x00 };\n", fptr);
  fprintf (fptr, "unsigned int Arr_mp3_len =%d;\n", nlen);

  //  fputs ("write test\n", fptr);

  // Close the file
  fclose (fptr);
  fclose (fptrRead);
}

int
main (int argc, char **argv)
{
  printf ("store_fileBin_to_fileArrC\r\n");

  if (argc == 2)
    {
      store_fileBin_to_fileArrC (argv[1]);
      printf ("\r\n\r\nOk\r\n");
    }
  else
    printf ("\r\n\r\ninput - file name\r\n");

  return 0;
}
