
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

static int
_cmd_get (int argc, char **argv)
{
  (void)argc;
  (void)argv;
  char dstr[ISOSTR_LEN];

  struct tm time;
  ds3231_get_time (&_dev, &time);

  size_t pos = strftime (dstr, ISOSTR_LEN, "%Y-%m-%dT%H:%M:%S", &time);
  dstr[pos] = '\0';
  printf ("The current time is: %s\n", dstr);

  return 0;
}

static int
_cmd_set (int argc, char **argv)
{
  if (argc != 2)
    {
      printf ("usage: %s <iso-date-str YYYY-MM-DDTHH:mm:ss>\n", argv[0]);
      return 1;
    }

  if (strlen (argv[1]) != (ISOSTR_LEN - 1))
    {
      puts ("error: input date string has invalid length");
      return 1;
    }

  struct tm target_time;
  int res = _tm_from_str (argv[1], &target_time);
  if (res != 0)
    {
      puts ("error: unable do parse input date string");
      return 1;
    }

  ds3231_set_time (&_dev, &target_time);

  printf ("success: time set to %s\n", argv[1]);
  return 0;
}
