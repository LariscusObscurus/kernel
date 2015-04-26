/*
 * gdt.c
 *
 *  Created on: 26.04.2015
 *      Author: leonhardt
 */

#include "gdt.h"
#include "kprintf.h"

#define GDT_ENTRIES 5

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


typedef struct __attribute__((packed)){
	uint16_t segment_limit_low;
	uint16_t base_address_low;
	uint8_t base_address_middle;
	uint8_t access_info;
	uint8_t segment_limit_high; //Also contains granularity, default operand size L and AVL flags
	uint8_t base_address_high;
} gdt_descriptor;

typedef struct __attribute__((packed)) {
	uint16_t limit;
	void* pointer;
} gdt_pointer;

static gdt_descriptor gdt[GDT_ENTRIES];
static gdt_pointer gdtp = {
		.limit = GDT_ENTRIES * 8 - 1,
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

void gdt_init(void)
{
	gdt_set_descriptor(0, 0, 0, 0, 0);
	gdt_set_descriptor(1, 0, 0xFFFFF, (P_FLAG | S_FLAG | CODE_RX), (GRANULARITY_4K | DB_FLAG_32BIT));
	gdt_set_descriptor(2, 0, 0xFFFFF, (P_FLAG | S_FLAG | DATA_RW), (GRANULARITY_4K | DB_FLAG_32BIT));
	gdt_set_descriptor(3, 0, 0xFFFFF, (P_FLAG | RING3 | S_FLAG | CODE_RX), (GRANULARITY_4K | DB_FLAG_32BIT));
	gdt_set_descriptor(4, 0, 0xFFFFF, (P_FLAG | RING3 | S_FLAG | DATA_RW), (GRANULARITY_4K | DB_FLAG_32BIT));

	asm volatile ("lgdt %0" : : "m" (gdtp));

	asm volatile (
			"mov $0x10, %ax;"
			"mov %ax, %ds;"
			"mov %ax, %es;"
			"mov %ax, %ss;"
			"ljmp $0x8, $.out;"
			".out:"
	);

}
