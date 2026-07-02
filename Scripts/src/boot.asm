[org 0x7c00]
mov [BOOT_DISK],dl    ;Storing the boot-drive
;Store it at the very top as any bios interrupt may overwrite it

CODE_SEG equ 0x08           ;Used for execution of code
DATA_SEG equ 0x10           ;Used whenever we read or write memory
MEM_MAP_BUFFER equ 0x90001   ;Used to store the Memory Map
KERNEL_STUB equ 0x7e00      ;The address where kernel stub(entry point is loaded)
MULTI_BOOT_ENTRY_COUNT equ 0x90000

[bits 16]

start_bootloader:

  cli
  xor ax,ax
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov sp,0x7c00
  sti

load_e820:
  mov ax,0x9000
  mov es,ax
  mov di,0x0010
  
  xor ebx,ebx

.next_entry:
  mov eax,0xe820
  mov edx,0x534d4150
  mov ecx,24
  int 0x15

  jc .error


  add [ENTRY_COUNT],1
  add di,24

  cmp ebx,0
  jz .finish

  jmp .next_entry

.error:
  mov al,'E'
  mov ah, 0x0E
  int 0x10
  ret
.finish:
  mov si,msg_check2
  call printRM


  ;16-bit success message
  mov si,msg16_bit  ;Load string into si(Source Index Register)
  call printRM
  ;Creating a short delay,BIOS ticks 18.2 times per second
  ;This count is tracked by the RTC hardware component(Real-Time-Clock)

  mov ah,0x00   ;Read RTC
  int 0x1A      ;Interrupt
  ;Stores the value in dx register
  
  mov bx,dx
  add bx, 18*3  ;1seconds approx



wait_loop:
  mov ah,0x00 ;Read RTC
  int 0x1A    ;Interrupt

  cmp dx,bx     ;Check if value is greater or equal to bx
  jb wait_loop  ;Jump back if Less
  

  ;Enabing A20 line using the BIOS method, you can do it with the fast gate
  ;method as well, this one only works in real mode, fast A20 works in any mode

  mov ax,2401
  int 0x15


load_sectors:
  mov bx,0                ;setup es
  mov es,bx               ;es segment set to KERNEL_LOAD_SEG
  mov bx,KERNEL_STUB      ;Memory address where sectors will be stored

  mov ah,2                ;Sector read function
  mov al,128              ;Number of sectors to read(128 sectors and each is 512 bytes=64Kb, we can only read a max of 128 sectors (64kb) at a time!)
  mov ch,0                ;Cylinder 0
  mov cl,2                ;Start reading from sector 2
  mov dh,0                ;head 0
  mov dl,[BOOT_DISK]      ;ideally 0x80h (80 is the first drive/HDD)
  int 0x13                ;Interrupt



  ; Set video mode 3 (80x25 text) – clears screen
  mov ah, 0x00
  mov al, 0x03
  int 0x10



  ;Switch to Protected Mode
  cli
  lgdt [gdt_descriptor]

  ;Enable 32-bit real mode
  mov eax,cr0
  or eax,1
  mov cr0, eax

  jmp CODE_SEG:init_pm  ;This is executable memory at init_pm, go and start executing code there
                        ;Hence use the CODE_SEG
  

[bits 32]

init_pm:
  mov ax,DATA_SEG
  mov ds,ax
  mov ss,ax
  mov es,ax
  mov fs,ax
  mov gs,ax

  mov ebp,0x90000
  mov esp,ebp

  mov al,[ENTRY_COUNT]
  mov [MULTI_BOOT_ENTRY_COUNT],al
  jmp CODE_SEG:KERNEL_STUB ;Jump to the kernel entry stub

;---------------------GDT---------------------------------------
gdt:
  dq 0x0000000000000000   ;NULL
  dq 0x00CF9A000000FFFF   ;CODE
  dq 0x00CF92000000FFFF   ;DATA
.end:

gdt_descriptor:
  dw gdt.end - gdt - 1
  dd gdt
;---------------------FUNCTIONS-----------------------------------


printRM:
  lodsb    ;Load byte at DS:[SI] into al,then increment SI
  cmp al,0  ;Check if NULL terminator
  je .done  ;If al=0 jump out of loop
  
  mov ah,0x0e ;BIOS teltype 
  int 0x10    ;BIOS interrupt

  jmp printRM
.done:
  ret


;---------------------DECLARATIONS-----------------------------------
msg16_bit:
  db "NEBULA-OS 1.1.0",0x0A,0x0D,0
msg_check1:
  db "Timer continued",0

msg_check2:
  db "Memory Map Loaded",0x0A,0x0D,0

BOOT_DISK:
  db 0

ENTRY_COUNT:
  db 0
;----------------------------------------------------------------------

times 510-($-$$) db 0
dw 0xaa55
