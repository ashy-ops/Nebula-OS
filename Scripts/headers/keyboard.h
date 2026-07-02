#pragma once
#include <stdint.h>
#include "isr.h"
#include "io.h"
#include "display.h"
#include "shell.h"

#define PS2_DATA    0x60 // Read or Write
#define PS2_STATUS  0x64 // Read
#define PS2_WRITE   0x64 // Write
#define BUFFER_SIZE 2000
//declared here and initialized in keyboard.c 


extern char keyboard_buffer[BUFFER_SIZE];
extern size_t buffer_len;
extern size_t buffer_pos; 
void keyboard_handler(registers_t* regs __attribute__((unused)));
