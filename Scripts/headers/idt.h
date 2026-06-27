#pragma once
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS       256
#define IDT_MAX_CPU_EXCEPTIONS    32

typedef struct 
{
  uint16_t base_low;
  uint16_t segment_selector;
  uint8_t reserved;
  uint8_t attributes;
  uint16_t base_high;

}__attribute__((packed)) idt_entry_t;


typedef struct
{
  uint16_t size;
  idt_entry_t* base;

}__attribute__((packed)) idtr_t;

extern void __attribute__((cdecl)) load_idt (idtr_t* idtr_p);


typedef enum
{

//The flags are from bit 48-40 defined as a single uint8_t

//40-43 is gate type(4 bit value)
//46-45 DPL (2 bit value)
//47 Present Bit(1 bit value)

//Hence these specific values and for the descriptor to be valid the last bit P should be 1
//Therefore we used 0x80 here, or you can do 1<<7 to get it to 8th bit

  //GATE TYPE
  IDT_FLAG_TASK_GATE                    =0x05,
  IDT_FLAG_INTERRUPT_GATE_16BIT         =0x06,
  IDT_FLAG_INTERRUPT_GATE_32BIT         =0x0E,
  IDT_FLAG_TRAP_GATE_16BIT              =0x07,
  IDT_FLAG_TRAO_GATE_32BIT              =0x0F,

  //PRESENT BIT
  IDT_FLAG_PRESENT                      =0x80,
  
  //DPL FLAGS
  IDT_FLAG_DPL_0                        =(0 << 5), //KERNEL
  IDT_FLAG_DPL_1                        =(1 << 5),
  IDT_FLAG_DPL_2                        =(2 << 5),
  IDT_FLAG_DPL_3                        =(3 << 5)
} IDT_FLAGS;


void idt_set_gate(uint8_t interrupt, uint16_t base_low, uint8_t segment_selector, uint8_t attributes, uint16_t base_high);
void idt_initialize(void);

void idt_gate_enable(uint8_t interrupt);
void idt_gate_disable(uint8_t interrupt);

