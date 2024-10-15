/*
 * @source          RIOT-OS
 * Copyright (C) 2009-2013 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_shell Shell
 * @ingroup     sys
 * @brief       Simple shell interpreter
 *
 * ## Security expectations
 *
 * Access to the shell grants access to the system that may exercise any power
 * the firmware can exercise. While some commands only provide limited access
 * to the system, and it is best practice for commands to validate their input,
 * there is no expectation of security of the system when an attacker gains
 * access to the shell.
 *
 * @{
 *
 * @file
 * @brief       Shell interface definition
 */

#ifndef SHELL_H
#define SHELL_H

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @source          RIOT-OS
   *  Copyright (C) 2009-2013 Freie Universität Berlin
   *  This file is subject to the terms and conditions of the GNU Lesser
   *  General Public License v2.1. See the file LICENSE in the top level
   *
   * @brief           Prototype of a shell callback handler.
   * @details         The functions supplied to shell_run() must use this
   * signature. It is designed to mimic the function signature of `main()`. For
   * this reason, the argument list is terminated with a `NULL`, i.e
   * `argv[argc] == NULL` (which is an ANSI-C requirement, and a detail that
   * newlib's `getopt()` implementation relies on). The function name is passed
   * in `argv[0]`.
   *
   *                  Escape sequences are removed before the function is
   * called.
   *
   *                  The called function may edit `argv` and the contained
   * strings, but it must be taken care of not to leave the boundaries of the
   * array. This functionality is another property that many `getopt()`
   *                  implementations rely on to provide their so-called
   * "permute" feature extension.
   *
   * @param[in]       argc   Number of arguments supplied to the function
   * invocation.
   * @param[in]       argv   The supplied argument list.
   *
   * @return 0 on success
   * @return Anything else on failure
   */
  typedef int (*shell_command_handler_t) (int argc, char **argv);

  /**
   * @source          RIOT-OS
   *  Copyright (C) 2009-2013 Freie Universität Berlin
   *  This file is subject to the terms and conditions of the GNU Lesser
   *  General Public License v2.1. See the file LICENSE in the top level
   *
   * @brief           A single command in the list of the supported commands.
   * @details         The list of commands is NULL terminated,
   *                  i.e. the last element must be ``{ NULL, NULL, NULL }``.
   */
  typedef struct shell_command_t
  {
    const char *name; /**< Name of the function */
    const char *desc; /**< Description to print in the "help" command. */
    shell_command_handler_t handler; /**< The callback function. */
  } shell_command_t;

#ifdef __cplusplus
}
#endif

#endif /* SHELL_H */
