#pragma once

#include <stdbool.h>

#include "CUsart.hpp"
#include "shell.h"

class CShell
{
private:
  const shell_command_t *Fshell_cmd;

protected:
public:
  CShell (const shell_command_t *shell_cmd);
  void print_help (void);
  void Idle (void);
  CUsart *oUsart;
};
