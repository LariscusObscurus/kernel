/*
 * logger.c
 *
 *  Created on: 19.05.2015
 *      Author: scl83wi
 */
#include "logger.h"
#include "kprintf.h"
#include "string.h"

static char* log_levels[] = {
        "INFO",
        "NOTICE",
        "WARNING",
        "ERROR",
        "CRITICAL",
        "PANIC"
};

void debug_print(log_level_t level, const char * file, uint32_t line, char* msg)
{
    char* msg_type;
    if (level > PANIC) {
        msg_type = log_levels[PANIC];
    } else {
        msg_type = log_levels[level];
    }
    kprintf("%s at %d in %s: %s", msg_type, line, file, msg);
}
