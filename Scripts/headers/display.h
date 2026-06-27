#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>


#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

#define MAX_BUFFER_SIZE 80*25
#define STATE_NORMAL 0
#define STATE_LENGTH 1
#define STATE_LENGTH_S 2    //on first h aka %h
#define STATE_LENGTH_L 4    //on first l aka %l
#define STATE_SPECIFIER 5
#define STATE_SPECIAL 6

#define LEN_DEF 0 //32-bit 
#define LEN_SS  1 //8-bit char sized
#define LEN_S   2 //16-bit short
#define LEN_L   3 //32-bit long 
#define LEN_LL  4 //64-bit long long

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
void print_num(va_list* args,uint8_t len,bool sign,uint8_t radix);

void scroll();
void terminal_initialize(void);
void write_to_terminal(const char* str,enum vga_color color, ...);

