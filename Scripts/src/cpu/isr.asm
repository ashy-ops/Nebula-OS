[bits 32]

extern ISR_HANDLER

%macro ISR_ERROR 1
global ISR%+%1:
ISR%1:
  push %1             ;Pushing the Interrupt Number, The error code is pushed itself BEFORE IT
  jmp isr_common
%endmacro


%macro ISR_NO_ERROR 1
global ISR%+%1:
ISR%1:
  push 0              ;Dummy Error Code
  push %1             ;Pushing the Interrupt Number
  jmp isr_common 
%endmacro


%include "isr_asm_gen.inc"


isr_common:
  pusha                   ;Saving all the values of the general purpose register

  xor eax,eax             
  mov ax, ds              ;Storing the DS value to ax before swithing to kernel DS(in my case its all the same but for future design)
  push eax                ;we need to store the other segments as well but since currently all of them have the same value, I only store DS

  mov ax, 0x10            ;Ensuring that the segments always use the kernel Data segment
  mov ds,ax
  mov es,ax
  mov fs,ax
  mov gs,ax

  push esp                ;Pushes the stack pointer as an argument to the C function
  call ISR_HANDLER
  
  add esp,4               ;removes the esp argument

  pop eax                 ;Pops the original ds value 
  mov ds,ax               ;Restoring the segments with the original segment values
  mov es,ax
  mov fs,ax
  mov gs,ax

  popa                   ;Restoring the state of the program before the interrupt occured
  add esp,8              ;moving up by 8, because CPU pushes the Error Code and the Interrupt Number and to maintain uniformity
                         ;we push the a dummy error code/actual error code can by pushed as well
                         ;Note that in 32-bit mode the stack is in chunks of 4, so pushing one value results in the stack moving down by a count of 4
                         ;as each address is one byte
                         ;Hence to remove the interrupt number and error code we did +8
  iret                   

;Note that for some interrupts the CPU pushes the error code itself, and for some it does not
