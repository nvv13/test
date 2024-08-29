/*
 * Copyright (C) 2019 Javier FILEIV <javier.fileiv@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file        main.c
 * @brief       Example using MQTT Paho package from RIOT
 *
 * @author      Javier FILEIV <javier.fileiv@gmail.com>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
//#include "mutex.h"

static const shell_command_t shell_commands[] =
{
    { NULL,     NULL,                                 NULL        }
};


int main(void)
{
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(1);
#endif

    printf("Running \n");


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
