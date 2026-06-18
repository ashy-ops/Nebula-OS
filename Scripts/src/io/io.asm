[bits 32]


;Global declarations to make them visible to the linker
global outb
global inb
global io_wait

outb:
  mov dx,[esp + 4]  ;Storing the PORT number from the stack into the dx register
  mov al,[esp + 8]  ;Storing the VALUE from the stack into the al register

  out dx, al
  ret

inb:
  mov dx,[esp + 4]  ;Storing the PORT number from the stack into the dx register
  in al,dx          ;Reading the byte from the PORT into al

  ;Note in C calling convention the return value is always put in the EAX/AL register
  ret

io_wait:
  mov dx,0x80
  mov al,0

  out dx,al
  ret

;Note that the stack grows downwards, whenever we call a function in C weather its in assembly
;or in some other file/assembly stub, it always pushes the function parameters RIGHT-TO-LEFT!!!,
;and after all the parameters are pushed, it pushes the return address onto the stack
;Since the stack grows downwards we add +4 and +8 to get the values
;Doing +8 assuming that we pass the port paramter first in our C function definition
