#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>


#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

#define MAX_BUFFER_SIZE 80*25
#define NORMAL 0
#define SPECIFIER 1

extern size_t cur_row;
extern size_t cur_column;
extern uint8_t terminal_color;
extern uint8_t text_color;
extern size_t cursor;

enum vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
  VGA_COLOR_COUNT_DUMMY
};


void putc(const char c);
void puts(const char* s);
void puti(int i);
void puth(uint64_t n);

void scroll();
void terminal_initialize(void);
void write_to_terminal(const char* str,enum vga_color color, ...);

