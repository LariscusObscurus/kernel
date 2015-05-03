/*
 * console.c
 *
 *  Created on: 25.04.2015
 *      Author: leonhardt
 */

#include "console.h"
#include "string.h"

static size_t console_row;
static size_t console_column;
static uint8_t console_color;
static uint16_t* console_buffer;

uint8_t combine_color(vga_color_t fg, vga_color_t bg)
{
	return fg | bg << 4;
}

void console_set_color(vga_color_t fg, vga_color_t bg)
{
	console_color = combine_color(fg, bg);
}

void console_putchar_at(char c, uint8_t color, uint8_t x, uint8_t y)
{
	uint16_t index = y * VGA_WIDTH + x;
	console_buffer[index] = (((uint16_t) color) << 8) | c;
}

void console_clear(void)
{
	console_row = 0;
	console_column = 0;
	console_buffer = (uint16_t*) 0xB8000;
	console_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			console_putchar_at(' ', console_color, x, y);
		}
	}
}

void console_putchar(char c)
{
	if (c == '\n') {
		console_column = 0;
		console_row++;
		return;
	}
	if (console_row == VGA_HEIGHT) {
		console_scroll_up();
	}
	console_putchar_at(c, console_color, console_column, console_row);
	console_column++;
	if (console_column == VGA_WIDTH) {
		console_column = 0;
		console_row++;
		if (console_row == VGA_HEIGHT) {
			console_scroll_up();
		}
	}
}

void console_write_string(const char* string)
{
	while(*string) {
		console_putchar(*string++);
	}
}

void console_putnumber(uint32_t n, uint32_t base)
{
	char buffer[65];
	const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";

	if (base > 36)
		return;

	char* p = buffer + 64;
	*p = '\0';
	do {
		*--p = digits[n % base];
		n /= base;
	} while (n);
	console_write_string(p);

}

void console_scroll_up(void)
{
	//Copy lines up a line
	memcpy(console_buffer, console_buffer + VGA_WIDTH, (VGA_HEIGHT-1) * VGA_WIDTH*2);

	uint16_t space = (console_color << 8) | 0x20;
	memsetw(console_buffer + (VGA_HEIGHT - 1) * VGA_WIDTH, space, VGA_WIDTH);
	console_row = VGA_HEIGHT -1;
	console_column = 0;
}
