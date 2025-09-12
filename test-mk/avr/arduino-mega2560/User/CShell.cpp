
//#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CShell.hpp"

CShell::CShell (const shell_command_t *shell_cmd) : Fshell_cmd (shell_cmd)
{
  oUsart = CUsart::GetInstance ();
}

void
CShell::print_help (void)
{
  printf ("usage:\r\n");
  int ind_cmd = 0;
  while (Fshell_cmd[ind_cmd].name != NULL)
    {
      printf ("  %s : %s\r\n", Fshell_cmd[ind_cmd].name,
              Fshell_cmd[ind_cmd].desc);
      ind_cmd++;
    }
}

void
CShell::Idle (void)
{
  if (oUsart->is_recive_line ())
    {
      char *rx_buf = oUsart->line_cstr ();
      uint8_t line_len = oUsart->line_len ();
      char *pHeaderEnd = (char *)rx_buf;
      /*
      printf (" line:");
      printf (pHeaderEnd);
      printf ("\r\n");
      */

      if (strstr (pHeaderEnd, "help") == pHeaderEnd)
        {
          print_help ();
        }
      else
        {
          int ind_cmd = 0;
          while (Fshell_cmd[ind_cmd].name != NULL)
            {
              if (strstr (pHeaderEnd, Fshell_cmd[ind_cmd].name) == pHeaderEnd)

                {
                  int argc = 0;
                  char *argv[5];
                  argv[argc] = pHeaderEnd;
                  while (argc < 5)
                    {
                      argc++;
                      char *pPos = strstr (pHeaderEnd, " ");
                      if (pPos != NULL && pPos < ((char *)(rx_buf + line_len)))
                        {
                          *pPos = 0;
                          pPos++;
                          if (*pPos == 0)
                            break;
                          argv[argc] = pPos;
                        }
                      else
                        break;
                    }
                  Fshell_cmd[ind_cmd].handler (argc, argv);
                  break;
                }
              ind_cmd++;
            }
        }
    }
}
