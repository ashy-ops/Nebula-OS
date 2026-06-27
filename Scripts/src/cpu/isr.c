#include "idt.h"
#include "isr.h"
#include "io.h"
#include "isr_setup.h"
#include "display.h"


isr_handler_t handlers[256];     //function pointer table

void isr_initialize(void)
{
  isr_initialize_gates();
  for(int i =0; i<256; i++)
  {
    idt_gate_enable(i);
  }

}
void __attribute((cdecl)) isr_handler(registers_t* regs)
{
  uint8_t n = regs->interrupt;

  if(n>=32 && n <48)
  {
    outb(0x20,0x20);
    if(n>=40)
    {
      outb(0xA0,0x20);
    }
  }

}
