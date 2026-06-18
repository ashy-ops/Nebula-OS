[BITS 32]

global _start   ;Makes it visible to the linker
extern kernel_main  ;Tells that kernel_main exists somewhere and is linked later 

_start:
  call kernel_main
  jmp $
