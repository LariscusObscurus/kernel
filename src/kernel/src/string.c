/*
 * string.c
 *
 *  Created on: 03.05.2015
 *      Author: leonhardt
 */

#include "string.h"

int memcmp(const void* firstptr, const void* secondptr, size_t size)
{
	const uint8_t* first = (const uint8_t*) firstptr;
	const uint8_t* second = (const uint8_t*) secondptr;
	for (size_t i = 0; i < size; i++)
		if (first[i] < second[i])
			return -1;
		else if (second[i] < first[i])
			return 1;
	return 0;
}

void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size)
{
	uint8_t* dst = (uint8_t*) dstptr;
	const uint8_t* src = (const uint8_t*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size)
{
	uint8_t* dst = (uint8_t*) dstptr;
	uint8_t* src = (uint8_t*) srcptr;

	if (dst < src)
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	else
		for (size_t i = size; i != 0; i--)
			dst[i] = src[i];
	return dstptr;

}

void* memset(void* bufptr, int32_t value, size_t size)
{
	uint8_t* buf = (uint8_t*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (uint8_t) value;
	return bufptr;
}

void* memsetw(void *bufptr, int32_t value, size_t size)
{
        uint16_t* buf = (uint16_t*) bufptr;
        for (size_t i = 0; i < size; i++)
        {
                 buf[i] = (uint16_t) value;
        }
        return bufptr;
}


size_t strlen(const char* string)
{
	size_t rv = 0;
	while (string[rv])
		rv++;
	return rv;
}
