/*
 * kprintf.c
 *
 *  Created on: 25.04.2015
 *      Author: leonhardt
 */

#include "kprintf.h"
#include "console.h"

void kprintf(const char* string, ...)
{
	va_list ap;
	uint32_t n;
	const char * s;
	va_start(ap, string);

	while (*string) {
		if (*string == '%') {
			string++;
			switch (*string) {
			case '%':
				console_putchar('%');
				break;
			case 's':
				s = va_arg(ap, char*);
				console_write_string(s);
				break;
			case 'c':
				n = va_arg(ap, int);
				console_putchar(n);
				break;
			case 'u':
			case 'd':
				n = va_arg(ap, uint32_t);
				console_putnumber(n, 10);
				break;
			case 'x':
			case 'p':
				n = va_arg(ap, uint32_t);
				console_write_string("0x");
				console_putnumber(n, 16);
				break;
			case '\0':
				goto out;
				break;
			default:
				console_putchar('%');
				console_putchar(*string);
				break;

			}

		} else {
			console_putchar(*string);
		}

		string++;
	}

out:
	va_end(ap);
}
