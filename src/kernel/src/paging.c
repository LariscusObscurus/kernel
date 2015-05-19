/*
 * paging.c
 *
 *  Created on: 16.05.2015
 *      Author: leonhardt
 */
#include "paging.h"
#include "string.h"
#include "kmalloc.h"
#include "console.h"
#include "panic.h"
#include "logger.h"

extern uint32_t placement_address;

page_directory_t *kernel_directory;
page_directory_t *current_directory;

uint32_t *frames;
uint32_t nframes;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame( uint32_t frame_addr)
{
	uint32_t frame  = frame_addr / 0x1000;
	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);
	frames[index] |= (0x1 << offset);
}

static void clear_frame( uint32_t frame_addr)
{
	uint32_t frame  = frame_addr / 0x1000;
	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);
	frames[index] &= ~(0x1 << offset);
}

static uint32_t test_frame(uint32_t frame_addr)
{
	uint32_t frame  = frame_addr / 0x1000;
	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);
	return (frames[index] & (0x1 << offset));
}

static uint32_t find_first_frame(void)
{
	for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if (frames[i] != 0xFFFFFFFF) {

			for (uint32_t j = 0; j < 32; j++) {

				uint32_t test_frame = 0x1 << j;

				if (!(frames[i] & test_frame)) {
					return i * 4 * 8 + j;
				}
			}
		}
	}
	//Overwrote page frames?
	//TODO: PANIC
	return -1;
}

void allocate_frame(page_t *page, bool kernel_space, bool writeable)
{
	if (page->frame != 0) {
		page->present = 1;
		page->rw = (writeable)?1:0;
		page->user = (kernel_space)?0:1;
		return;
	} else {
		uint32_t index = find_first_frame();
		if (index == (uint32_t)-1) {
		    PANIC("No free frames");
		}
		set_frame(index * 0x1000);
		page->frame = index;
		page->present = 1;
		page->rw = (writeable)?1:0;
		page->user = (kernel_space)?0:1;
	}
}

void free_frame(page_t *page)
{
	uint32_t frame;
	if (!(frame = page->frame)) {
		return;
	} else {
		clear_frame(frame);
		page->frame = 0x0;
	}
}

void map_dma_frame(page_t *page, bool kernel_space, bool writeable, uint32_t address)
{
    page->present = 1;
    page->rw = (writeable)?1:0;
    page->user = (kernel_space)?0:1;
    page->frame = address / 0x1000;
    if (address * 4 * 0x400) {
        set_frame(address);
    }
}

void paging_init(void)
{
	uint32_t mem_end_page = 0x1000000;
	nframes = mem_end_page / 0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory_t*)kmalloc_aligned(sizeof (page_directory_t));
	memset(kernel_directory, 0, sizeof (page_directory_t));
	current_directory = kernel_directory;

	//Null pointer should not be mapped
	get_page(0, true, kernel_directory)->present = 0;
	set_frame(0);

	for (uint32_t i = 0x1000; i < 0x100000; i += 0x1000) {
	    map_dma_frame(get_page(i, true, kernel_directory), 1, 0, i);
	}

	uint32_t i = 0x100000;
	while (i < placement_address) {
		allocate_frame( get_page(i, true, kernel_directory), 0, 1);
		i += 0x1000;
	}

	//remap VGA text mode address space
	for (uint32_t i = 0xB8000; i < 0xC0000; i += 0x1000) {
	    map_dma_frame(get_page(i, true, kernel_directory), 0, 1, i);
	}
	//TODO: Page fault handler
	switch_page_directory(kernel_directory);

}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tables_physical));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, bool create, page_directory_t *dir)
{
    address /= 0x1000;
    uint32_t table_index = address / 1024;
    if (dir->tables[table_index]) {
        return &dir->tables[table_index]->pages[address % 1024];
    } else if (create) {
        uint32_t tmp;
        dir->tables[table_index] = (page_table_t*)kmalloc_aligned_physical(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_index], 0, sizeof(page_table_t));
        dir->tables_physical[table_index] = tmp | 0x7;
        return &dir->tables[table_index]->pages[address % 1024];
    } else {
    	return 0;
    }
}
