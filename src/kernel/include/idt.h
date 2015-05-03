/*
 * idt.h
 *
 *  Created on: 02.05.2015
 *      Author: leonhardt
 */

#ifndef IDT_H_
#define IDT_H_
#include "stdint.h"

typedef struct {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;

	uint32_t interrupt_num;
	uint32_t error_code;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;

} cpu_context_t;

void idt_init(void);

#endif /* IDT_H_ */
