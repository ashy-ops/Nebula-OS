#pragma once
#include <stdint.h>

typedef struct {
    uint32_t ds;                                          // Pushed by us
    uint32_t edi, esi, ebp, esp_ignored, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t interrupt;                                   // Pushed by macro
    uint32_t error;                                       // Pushed by macro
    uint32_t eip, cs, eflags;                             // Pushed by CPU
} __attribute__((packed)) registers_t;

typedef void (*isr_handler_t)(registers_t* regs);    //function pointer with regs as argument

void isr_initialize(void);
void isr_register_handler(int interrupt, isr_handler_t handler);
