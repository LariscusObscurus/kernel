/*
 * gdt.h
 *
 *  Created on: 26.04.2015
 *      Author: leonhardt
 */

#ifndef GDT_H_
#define GDT_H_

#include "stdint.h"

void gdt_init(void);
void gdt_set_descriptor(int i, uint32_t base, uint32_t limit, uint32_t accessflags, uint8_t flags);

#endif /* GDT_H_ */
