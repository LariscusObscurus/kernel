/*
 * main.c
 *
 *  Created on: 25.04.2015
 *      Author: leonhardt
 */
#if defined(__linux__)
#error "Cross-compiler required, aborting!"
#endif

#if !defined(__i686__)
#error "i686-elf compiler required, aborting!"
#endif

#include "console.h"

void kmain(void)
{
	console_clear();
	while(1) {
		console_write_string("Hello World12!\n");
		console_write_string("Hello World!\n");
	}
}
