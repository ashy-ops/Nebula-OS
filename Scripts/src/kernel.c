#include <stdint.h>
#include "pic.h"

volatile char* VGA_BUFFER = (volatile char*)0xB8000;

void kernel_main()
{
  
  VGA_BUFFER[0]= 'A';
  VGA_BUFFER[1] = 0x02;
  pic_remap();
  while(1)
  {
    
  }
}