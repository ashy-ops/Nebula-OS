#include "io.h"
#include "pic.h"

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

    //Set to 0x00 to enable all hardware lines
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);

}