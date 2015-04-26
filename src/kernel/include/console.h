/*
 * console.h
 *
 *  Created on: 25.04.2015
 *      Author: leonhardt
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "stddef.h"
#include "stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 24

typedef enum {
	COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
} vga_color;

uint8_t combine_color(vga_color fg, vga_color bg);
void console_clear(void);
void console_set_color(vga_color fg, vga_color bg);
void console_putchar_at(char c, uint8_t color, uint8_t x, uint8_t y);
void console_putchar(char c);
void console_write_string(const char* string);
void console_scroll_up(void);
void console_putnumber(uint32_t n, uint32_t base);

#endif /* CONSOLE_H_ */
