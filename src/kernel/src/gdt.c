/*
 * gdt.c
 *
 *  Created on: 26.04.2015
 *      Author: leonhardt
 */

#include "gdt.h"
#include "kprintf.h"
#include "string.h"

#define GDT_ENTRIES 6

#define RING0 0
#define RING3 0x60
#define S_FLAG 0x10
#define P_FLAG 0x80
#define GRANULARITY_4K 0x80
#define DB_FLAG_32BIT 0x40

#define DATA_RO 0x0
#define DATA_RW 0x02
#define DATA_RO_EXPAND 0x04
#define DATA_RW_EXPAND 0x06
#define CODE_X 0x08
#define CODE_RX 0x0A
#define CODE_XC 0x0C
#define CODE_RXC 0x0E
#define TSS 0x09

typedef struct __attribute__ ((packed)) {
	uint32_t	prev_tss;
	uint32_t	esp0;
	uint32_t	ss0;
	uint32_t	esp1;
	uint32_t	ss1;
	uint32_t	esp2;
	uint32_t	ss2;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	eflags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint32_t	es;
	uint32_t	cs;
	uint32_t	ss;
	uint32_t	ds;
	uint32_t	fs;
	uint32_t	gs;
	uint32_t	ldt;
	uint16_t	trap;
	uint16_t	iomap_base;
} tss_entry_t;

typedef struct __attribute__((packed)){
	uint16_t segment_limit_low;
	uint16_t base_address_low;
	uint8_t base_address_middle;
	uint8_t access_info;
	uint8_t segment_limit_high; //Also contains granularity, default operand size L and AVL flags
	uint8_t base_address_high;
} gdt_descriptor_t;

typedef struct __attribute__((packed)) {
	uint16_t limit;
	void* pointer;
} gdt_pointer_t;

static tss_entry_t tss_entry;
static gdt_descriptor_t gdt[GDT_ENTRIES];
static gdt_pointer_t gdtp = {
		.limit = GDT_ENTRIES * sizeof (gdt_descriptor_t) - 1,
		.pointer = gdt
};

void gdt_set_descriptor(int i, uint32_t base, uint32_t limit, uint32_t accessflags, uint8_t flags)
{
	gdt[i].segment_limit_low = (limit & 0xFFFF);
	gdt[i].base_address_low = (base & 0xFFFF);
	gdt[i].base_address_middle = (base >> 16) & 0xFF;
	gdt[i].base_address_high = (base >> 24) & 0xFF;

	gdt[i].access_info = accessflags;
	gdt[i].segment_limit_high = (limit >> 16) & 0x0F;

	gdt[i].segment_limit_high |= ((flags) & 0xF0);
}

static void tss_create(uint32_t i, uint16_t ss0, uint32_t esp0)
{
	uint32_t base_address = (uint32_t)&tss_entry;
	uint32_t limit = base_address + sizeof tss_entry;
	gdt_set_descriptor(i, base_address, limit, (P_FLAG |RING3 |TSS), 0x0);

	memset(&tss_entry, 0x0, sizeof(tss_entry));
	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;
	tss_entry.cs = 0x08 | 0x03;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x03;
	tss_entry.iomap_base = sizeof(tss_entry);

}

void gdt_init(void)
{
	gdt_set_descriptor(0, 0, 0, 0, 0);
	gdt_set_descriptor(1, 0, 0xFFFFF, (P_FLAG | S_FLAG | CODE_RX), (GRANULARITY_4K | DB_FLAG_32BIT));
	gdt_set_descriptor(2, 0, 0xFFFFF, (P_FLAG | S_FLAG | DATA_RW), (GRANULARITY_4K | DB_FLAG_32BIT));
	gdt_set_descriptor(3, 0, 0xFFFFF, (P_FLAG | RING3 | S_FLAG | CODE_RX), (GRANULARITY_4K | DB_FLAG_32BIT));
	gdt_set_descriptor(4, 0, 0xFFFFF, (P_FLAG | RING3 | S_FLAG | DATA_RW), (GRANULARITY_4K | DB_FLAG_32BIT));
	tss_create(5, 0x10, 0x0);

	asm volatile ("lgdt %0" : : "m" (gdtp));

	asm volatile (
			"mov $0x10, %ax;"
			"mov %ax, %ds;"
			"mov %ax, %es;"
			"mov %ax, %ss;"
			"ljmp $0x8, $.out;"
			".out:"
	);

	asm volatile (
	        "mov $0x2B, %ax;"
	        "ltr %ax"
	);

}


void tss_set_esp(uint32_t stack)
{
    tss_entry.esp0 = stack;
}

