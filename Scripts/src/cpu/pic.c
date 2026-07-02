#include "pic.h"

uint8_t lines_enabled[] = {0,1};
//0 is for Timer, 1 is for Keyboard!


void pic_remap(void) {

    // ICW1 which starts the initialization sequence in cascading mode
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW2 which invovles remapping
    outb(PIC1_DATA, 0x20); // Master PIC vectors now start at 32 (0x20)
    io_wait();
    outb(PIC2_DATA, 0x28); // Slave PIC vectors now start at 40 (0x28)
    io_wait();

    //ICW3 which tells the master PIC that there is a slave PIC at IRQ2 
    outb(PIC1_DATA, 4);   
    io_wait();

    // telling the slave that it is a slave pic and it needs to listen at IRQ2
    outb(PIC2_DATA, 2);
    io_wait();

    // 4. ICW4 which puts both the chips into 8086/88 microprocessor mode
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    //Setting all IRQs line 0 to IRQ line 15!
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
    WRITE("PIC INITIALIZED!\n",WHITE);

    for(int i=0; i<sizeof(lines_enabled)/sizeof(lines_enabled[0]); i++)
    {
        unmask_line(lines_enabled[i]);
    }
}

void mask_line(uint8_t line)
{
    if (line < 8) outb(PIC1_DATA, inb(PIC1_DATA) | (1 << line));
    else outb(PIC2_DATA, inb(PIC2_DATA) | (1 << (line - 8)));
    
}

void unmask_line(uint8_t line)
{
    if (line < 8) outb(PIC1_DATA, inb(PIC1_DATA) & ~(1 << line));
    else outb(PIC2_DATA, inb(PIC2_DATA) & ~(1 << (line - 8)));
}
