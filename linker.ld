OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    . = 0x7e00;

    .text : ALIGN(16)
    {
        *(.text.entry)
        *(.text)
    }

    .data : ALIGN(16)
    {
        *(.data)
    }

    .bss : ALIGN(16)
    {
        *(.bss)
        *(COMMON)
    }
    
}