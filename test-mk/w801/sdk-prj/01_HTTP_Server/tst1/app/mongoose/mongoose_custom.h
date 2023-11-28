/*
источник

https://github.com/cesanta/mongoose.git
*/


#ifndef MONGOOSE_CUSTOM_H
#define MONGOOSE_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_rtc.h"
#include "wm_socket.h"
#include "wm_sockets.h"
#include "wm_timer.h"

typedef uint64_t u64_t;
typedef int64_t s64_t;

#define MG_ARCH MG_ARCH_CUSTOM
#define MG_ENABLE_FILE 0           // Disable POSIX filesystem
#define MG_IO_SIZE 256

#define MG_ENABLE_FATFS 1

#define MG_ENABLE_CUSTOM_MILLIS 1  // Let user implement mg_millis()

#define FD_CLOEXEC 0
#define F_SETFD 0

#include <FreeRTOS.h>
#include <task.h>


#ifdef __cplusplus
}
#endif
#endif  // MONGOOSE_CUSTOM_H
