#ifndef PIC_H
#define PIC_H

// Hardware Port Defines
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// ICW1 initialization flags
#define ICW1_ICW4       0x01    // Indicates that ICW4 will be present
#define ICW1_INIT       0x10    // Initialization configuration bit

// ICW4 mode flags
#define ICW4_8086       0x01    // 8086/88 mode instruction bit

void pic_remap(void);
void mask_line(void);
void unmask_line(void);

#endif