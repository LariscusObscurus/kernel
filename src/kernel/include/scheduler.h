/*
 * scheduler.h
 *
 *  Created on: 03.05.2015
 *      Author: leonhardt
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include "idt.h"
#include "stdint.h"
#include "stddef.h"
#include "paging.h"

typedef struct {
	cpu_context_t * ctx;
	page_directory_t * dir;
} task_t;

void task_create(cpu_context_t** tasks, void* task, uint8_t* stack, size_t stack_size, uint8_t* userspace_stack, size_t userspace_stack_size);
cpu_context_t* schedule(cpu_context_t* ctx);

#endif /* SCHEDULER_H_ */
