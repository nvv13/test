
#include "md5.h"
#include <stdio.h>
#include <string.h>

#define LEN_BUF 4096

void
md5_file (char *filenameBin)
{
  FILE *fptrRead;

  // Open a file
  fptrRead = fopen (filenameBin, "rb");

  md5_state_t state;
  md5_byte_t digest[16];
  char hex_output[16 * 2 + 1];

  md5_init (&state);

  unsigned char buffer[LEN_BUF];
  int num_items = 1;
  while (num_items != 0)
    {
      num_items = fread (buffer, sizeof (char), LEN_BUF, fptrRead);
      if (num_items != 0)
        {
          md5_append (&state, (const md5_byte_t *)buffer, num_items);
        }
    }

  md5_finish (&state, digest);

  int di;
  for (di = 0; di < 16; ++di)
    sprintf (hex_output + di * 2, "%02x", digest[di]);
  printf ("%s\r\n", hex_output);

  // Close the file
  fclose (fptrRead);
}

int
main (int argc, char **argv)
{

  if (argc == 2)
    {
      md5_file (argv[1]);
    }
  else
    printf ("Usage:\r\n   md5_file file_name\r\n");

  return 0;
}
