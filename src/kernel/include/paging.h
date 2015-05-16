/*
 * paging.h
 *
 *  Created on: 16.05.2015
 *      Author: leonhardt
 */

#ifndef PAGING_H_
#define PAGING_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct {
	uint32_t present 	: 1;
	uint32_t rw			: 1;
	uint32_t user		: 1;
	uint32_t accessed	: 1;
	uint32_t dirty		: 1;
	uint32_t unused		: 7;
	uint32_t frame		: 20;
} page_t;

typedef struct {
	page_t pages[1024];
} page_table_t;

typedef struct {
	page_table_t *tables[1024];
	uint32_t tables_physical[1024];
	uint32_t physical_address;
} page_directory_t;

void paging_init(void);

void switch_page_directory(page_directory_t *dir);

page_t *get_page(uint32_t address, bool create, page_directory_t *dir);

#endif /* PAGING_H_ */
