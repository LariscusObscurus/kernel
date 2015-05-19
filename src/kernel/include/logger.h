/*
 * logger.h
 *
 *  Created on: 19.05.2015
 *      Author: scl83wi
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "stdint.h"

typedef enum {
    INFO,
    NOTICE,
    WARNING,
    ERROR,
    CRITICAL,
    PANIC
} log_level_t;

void debug_print(log_level_t level, const char * file, uint32_t line, char* msg);


#endif /* LOGGER_H_ */
