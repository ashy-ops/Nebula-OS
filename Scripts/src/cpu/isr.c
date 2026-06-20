#include "idt.h"
#include "isr.h"
#include "io.h"
#include "isr_setup.h"

int colors[] = {0x01,0x02,0x03,0x04,0x05,0x06};
static int cntr = 0;
volatile char* VGA_BUFFER_2 = (volatile char*)0xB8000;
ISRHandler handlers[256];     //function pointer table

void ISR_Initialize()
{
  ISR_INITIALIZE_GATES();
  for(int i =0; i<256; i++)
  {
    IDT_GATE_ENABLE(i);
  }
  VGA_BUFFER_2[0]= 'B';
  VGA_BUFFER_2[1] = 0x02;
}
void __attribute((cdecl)) ISR_HANDLER(Registers* regs)
{
  uint8_t n = regs->interrupt;
  if (n < 32) {
      // Something went wrong (e.g., divide by zero, page fault).
      // Print an error to a different part of the screen so it doesn't get overwritten.
      VGA_BUFFER_2[160] = 'E'; // Exception!
      VGA_BUFFER_2[162] = (n / 10) + '0';
      VGA_BUFFER_2[164] = (n % 10) + '0';
      //while(1); // Halt
  }
  VGA_BUFFER_2[0] = (n / 10) + '0';
  VGA_BUFFER_2[2] = (n % 10) + '0';
  VGA_BUFFER_2[1] = colors[cntr];
  cntr = (cntr+1)%6;
  VGA_BUFFER_2[3] = colors[cntr];

  if(n>=32 && n <48)
  {
    outb(0x20,0x20);
    if(n>=40)
    {
      outb(0xA0,0x20);
    }
  }

}
