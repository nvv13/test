#ifndef DS3231_UTIL_H
#define DS3231_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

int
ds3231_cmd_get (int argc, char **argv)
{
  struct tm tblock;

  ds3231_get_time (&ds3231_dev, &tblock);
  // получаем текущее время
  if (tblock.tm_year > 80)
    {
      PoCShell->oUsart->SendPSZstring ("cur time ");
      PoCShell->oUsart->SendIntToStr (tblock.tm_year + 1900);
      PoCShell->oUsart->SendPSZstring (".");
      if(tblock.tm_mon + 1 <10)PoCShell->oUsart->SendPSZstring ("0");
      PoCShell->oUsart->SendIntToStr (tblock.tm_mon + 1);
      PoCShell->oUsart->SendPSZstring (".");
      if(tblock.tm_mday <10)PoCShell->oUsart->SendPSZstring ("0");
      PoCShell->oUsart->SendIntToStr (tblock.tm_mday);
      PoCShell->oUsart->SendPSZstring (" ");
      if(tblock.tm_hour <10)PoCShell->oUsart->SendPSZstring ("0");
      PoCShell->oUsart->SendIntToStr (tblock.tm_hour);
      PoCShell->oUsart->SendPSZstring (":");
      if(tblock.tm_min <10)PoCShell->oUsart->SendPSZstring ("0");
      PoCShell->oUsart->SendIntToStr (tblock.tm_min);
      PoCShell->oUsart->SendPSZstring (":");
      if(tblock.tm_sec <10)PoCShell->oUsart->SendPSZstring ("0");
      PoCShell->oUsart->SendIntToStr (tblock.tm_sec);
      PoCShell->oUsart->SendPSZstring ("\r\n");
    }
  return 0;
}

#define ISOSTR_LEN (20U)

/* parse ISO date string (YYYY-MM-DDTHH:mm:ss) to struct tm */
static int
_tm_from_str (const char *str, struct tm *time)
{
  char tmp[5];

  if (strlen (str) != ISOSTR_LEN - 1)
    {
      return -1;
    }
  if ((str[4] != '-') || (str[7] != '-') || (str[10] != 'T')
      || (str[13] != ':') || (str[16] != ':'))
    {
      return -1;
    }

  memset (time, 0, sizeof (struct tm));

  memcpy (tmp, str, 4);
  tmp[4] = '\0';
  str += 5;
  time->tm_year = atoi (tmp) - 1900;

  memcpy (tmp, str, 2);
  tmp[2] = '\0';
  str += 3;
  time->tm_mon = atoi (tmp) - 1;

  memcpy (tmp, str, 2);
  str += 3;
  time->tm_mday = atoi (tmp);

  memcpy (tmp, str, 2);
  str += 3;
  time->tm_hour = atoi (tmp);

  memcpy (tmp, str, 2);
  str += 3;
  time->tm_min = atoi (tmp);

  memcpy (tmp, str, 2);
  time->tm_sec = atoi (tmp);

  return 0;
}

int
ds3231_cmd_set (int argc, char **argv)
{
  if (argc != 2)
    {
      PoCShell->oUsart->SendPSZstring ("usage: ");
      PoCShell->oUsart->SendPSZstring (argv[0]);
      PoCShell->oUsart->SendPSZstring (" YYYY-MM-DDTHH:mm:ss\r\n");
      return 1;
    }

  if (strlen (argv[1]) != (ISOSTR_LEN - 1))
    {
      PoCShell->oUsart->SendPSZstring ("input string - invalid length");
      return 1;
    }

  struct tm target_time;
  int res = _tm_from_str (argv[1], &target_time);
  if (res != 0)
    {
      PoCShell->oUsart->SendPSZstring ("unable do parse string");
      return 1;
    }

  ds3231_set_time (&ds3231_dev, &target_time);

  PoCShell->oUsart->SendPSZstring ("success: time set to ");
  PoCShell->oUsart->SendPSZstring (argv[1]);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  return 0;
}


#ifdef __cplusplus
}
#endif

#endif /* DS3231_UTIL_H */

