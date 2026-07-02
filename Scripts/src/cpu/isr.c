#include "isr.h"

isr_handler_t handlers[256];     //function pointer table

void isr_initialize(void)
{

  for(int i=0;i<256;i++)
  {
    handlers[i] = default_handler;
  }

  isr_initialize_gates(); //in isr_gen.c

  for(int i =0; i<256; i++)
  {
    idt_gate_enable(i);
  }

}

void isr_register_handler(int interrupt, isr_handler_t handler)
{
  handlers[interrupt] = handler; 
}

void default_handler(registers_t* reg)
{
  uint32_t interrupt = reg->interrupt;
  WRITE("Unhandeled Ineterrupt: %d\n",WHITE,interrupt);
}

void __attribute((cdecl)) isr_handler(registers_t* regs) //called by the assembly code
{
  uint8_t n = regs->interrupt;
  handlers[n](regs);
  if(n>=32 && n <48)
  {
    outb(0x20,0x20);
    if(n>=40)
    {
      outb(0xA0,0x20);
    }
  }

}
