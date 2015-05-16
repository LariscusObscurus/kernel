/*
 * kmalloc.h
 *
 *  Created on: 16.05.2015
 *      Author: leonhardt
 */

#ifndef KMALLOC_H_
#define KMALLOC_H_

#include "stdint.h"
#include "stdbool.h"

uint32_t kmalloc_aligned(uint32_t size);

uint32_t kmalloc_physical(uint32_t size, uint32_t *physical_address);

uint32_t kmalloc_aligned_physical(uint32_t size, uint32_t *physical_address);

uint32_t kmalloc(uint32_t size);

#endif /* KMALLOC_H_ */
