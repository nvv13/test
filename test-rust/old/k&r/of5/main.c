#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned char u8;
typedef unsigned int uint32_t;

// pack(push,1) - Byte alignment ?
#pragma pack(push, 1)
// MP3 Header
typedef struct tagMP3
{
  char ID3[3]; // = "ID3"
  u8 ver;
  u8 sum_ver;
  u8 flag;
  u8 header_size[4];

} MP3HDR, *PMP3HDR;
#pragma pack(pop)

#define MP3_FRAME_LEN 4 * 1152
static u8 file_buf_mp3[MP3_FRAME_LEN] = { 0 };
// static u16 mp3_hz;
// static u16 mp3_out_len = 0;
// static u16 mp3_used = 0;

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

  //******************************************************************
  size_t num_items
      = fread (file_buf_mp3, sizeof (char), sizeof (MP3HDR), fptrRead);
  if (num_items == sizeof (MP3HDR))
    {
      // Parse BMP header to get the information we need
      PMP3HDR aHead = (PMP3HDR)file_buf_mp3;
      if (strstr (aHead->ID3, "ID3") != NULL)
        {
          printf ("MP3 header ok\r\n");
          if (!(aHead->ver > 0 && aHead->ver < 6))
            {
              printf ("Wrong `version` value %d\r\n", aHead->ver);
            }

          uint32_t start;
          start = 0ul;
          start |= (0x7F & aHead->header_size[0]);
          start <<= 7;
          start |= (0x7F & aHead->header_size[1]);
          start <<= 7;
          start |= (0x7F & aHead->header_size[2]);
          start <<= 7;
          start |= (0x7F & aHead->header_size[3]);

          if (fseek (fptrRead, start, SEEK_CUR))
            printf ("fseek error! %d\r\n", start);
          else
            printf ("fseek successfully! %d\r\n", start);
        }
    }
  //******************************************************************

  unsigned char buffer[16];
  num_items = 1;
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
    printf ("\r\n\r\ninput - file_name.mp3\r\n");

  return 0;
}
