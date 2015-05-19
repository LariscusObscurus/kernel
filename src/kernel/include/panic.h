/*
 * panic.h
 *
 *  Created on: 19.05.2015
 *      Author: scl83wi
 */

#ifndef PANIC_H_
#define PANIC_H_

#include "logger.h"

#define PANIC(msg) debug_print(PANIC, __FILE__, __LINE__, msg); \
    while(1){ asm ("hlt"); }

#endif /* PANIC_H_ */
