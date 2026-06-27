#include <stdint.h>
#include "idt.h"
#include "isr.h"


static idt_entry_t idt_table[IDT_MAX_DESCRIPTORS];

static idtr_t idtr_p =
{
  sizeof(idt_table)-1,
  idt_table //due to pointer decay its equivalent to &idt_table[0]
};

void idt_set_gate(uint8_t interrupt, uint16_t base_low, uint8_t segment_selector, uint8_t attributes, uint16_t base_high)
{
  idt_table[interrupt].base_low = base_low;
  idt_table[interrupt].segment_selector = segment_selector;
  idt_table[interrupt].attributes = attributes;
  idt_table[interrupt].reserved = 0; //Always 0
  idt_table[interrupt].base_high = base_high;
}

void idt_gate_enable(uint8_t interrupt)
{
  idt_table[interrupt].attributes |= 0x80;
}
void idt_gate_disable(uint8_t interrupt)
{
  idt_table[interrupt].attributes &= ~0x80;
}

void idt_initialize(void)
{
  for(int i=0; i< IDT_MAX_DESCRIPTORS;i++)
  {
    idt_set_gate(i,0,0,0,0);
  }
  isr_initialize();
  load_idt(&idtr_p);
}
