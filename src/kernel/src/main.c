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
#include "stdbool.h"
#include "gdt.h"
#include "idt.h"
#include "scheduler.h"

cpu_context_t* tasks[2];
uint8_t task1_stack[4096];
uint8_t task2_stack[4096];
uint8_t task1_userspace_stack[4096];
uint8_t task2_userspace_stack[4096];

void task_print_task1(void)
{
	//asm volatile ("cli;hlt"); //Should cause general protection fault
	while(true)
		kprintf("Task1\n");

}

void task_print_task2(void)
{
	while(true)
		kprintf("Task2\n");
}

void kmain(void)
{
	console_clear();
	gdt_init();
	idt_init();

	task_create(tasks, task_print_task1, task1_stack, sizeof task1_stack, task1_userspace_stack, sizeof task1_userspace_stack);
	task_create(tasks, task_print_task2, task2_stack, sizeof task2_stack, task2_userspace_stack, sizeof task2_userspace_stack);

	while(true) {

	}
}
