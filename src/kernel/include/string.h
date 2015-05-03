/*
 * string.h
 *
 *  Created on: 02.05.2015
 *      Author: leonhardt
 */

#ifndef STRING_H_
#define STRING_H_

#include "stddef.h"
#include "stdint.h"

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int32_t, size_t);
void* memsetw(void *, int32_t, size_t);
size_t strlen(const char*);

#endif /* STRING_H_ */
