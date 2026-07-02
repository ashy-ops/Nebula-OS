#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "io.h"


#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define LIMIT VGA_WIDTH*VGA_HEIGHT
#define VGA_MEMORY  0xB8000

#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5

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

extern size_t cursor_write;
extern size_t cursor_free;
extern uint8_t terminal_color;
extern uint8_t text_color;
extern uint16_t buffer[VGA_HEIGHT*VGA_WIDTH];


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


uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);

void putc(const char c,enum vga_color color);
void puts(const char* s,enum vga_color color);
void print_num(va_list* args,uint8_t len,bool sign,uint8_t radix,enum vga_color color);

void UPDATE_TERMINAL_COLOR(uint8_t new_bg_color);
void UPDATE_TEXT_COLOR(uint8_t new_fg_color);

void UPDATE_SCREEN();
void SCROLL();
void CLEAR_SCREEN(void);
void WRITE(const char* str,enum vga_color color, ...);