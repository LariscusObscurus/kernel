/*
 * kmalloc.c
 *
 *  Created on: 16.05.2015
 *      Author: leonhardt
 */

#include "kmalloc.h"
extern uint32_t end;
uint32_t placement_address = (uint32_t) &end;

static uint32_t kmalloc_internal(uint32_t size, bool align, uint32_t *physical_address)
{
	if (true == align && (placement_address & 0xFFFFF000)) {

		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}

	if (physical_address) {
		*physical_address = placement_address;
	}

	uint32_t tmp = placement_address;
	placement_address += size;
	return  tmp;
}

uint32_t kmalloc_aligned(uint32_t size)
{
	return kmalloc_internal(size, true, 0);
}

uint32_t kmalloc_physical(uint32_t size, uint32_t *physical_address)
{
	return kmalloc_internal(size, false, physical_address);
}

uint32_t kmalloc_aligned_physical(uint32_t size, uint32_t *physical_address)
{
	return kmalloc_internal(size, true, physical_address);
}

uint32_t kmalloc(uint32_t size)
{
	return kmalloc_internal(size, false, 0);
}
