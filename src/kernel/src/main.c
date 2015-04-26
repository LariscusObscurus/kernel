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

#include "kprintf.h"
#include "console.h"

void kmain(void)
{
	console_clear();
	int i = 0;
	volatile int w = 0;
	while(1) {
		kprintf("Hello, World! %d\n", i);
		kprintf("Hello, World! %c\n", 'a');
		kprintf("Hello, World! %x\n", i);
		kprintf("Hello, World! %%\n", i);
		while(++w < 90000){}
		w = 0;
		i++;
	}
}
