/*
 * scheduler.c
 *
 *  Created on: 03.05.2015
 *      Author: leonhardt
 */

#include "scheduler.h"

static int32_t task_current = -1;
static uint32_t task_count = 0;
static cpu_context_t** task_ctx;

void task_create(cpu_context_t** tasks, void* task, uint8_t* stack, size_t stack_size)
{
	task_ctx = tasks;
	cpu_context_t ctx_new = {
			.eax = 0,
			.ebx = 0,
			.ecx = 0,
			.edx = 0,
			.esi = 0,
			.edi = 0,
			.ebp = 0,

			.eip = (uint32_t)task,
			.cs = 0x08,
			.eflags = 0x200
	};

	cpu_context_t* ctx =(void*) (stack + stack_size - sizeof ctx_new);
	*ctx = ctx_new;
	task_ctx[task_count++] = ctx;
}

cpu_context_t* schedule(cpu_context_t* ctx)
{
	if (task_current >= 0) {
		task_ctx[task_current] = ctx;
	}

	task_current++;
	task_current %= task_count;

	ctx = task_ctx[task_current];

	return ctx;
}