#include "display.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}


uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
size_t cur_row = 0;
size_t cur_column = 0;
uint8_t terminal_color = BLUE;
uint8_t text_color = WHITE;
size_t cursor = 0;

void scroll()
{
  terminal_initialize();
}

void putc(const char c)
{
  if(cur_column==VGA_WIDTH)
  {
    cur_row++;
    cur_column = 0;
  }
  if(cur_row==VGA_HEIGHT) scroll();
  cursor = cur_row*VGA_WIDTH + cur_column;
  uint8_t entry_colors = vga_entry_color(text_color,terminal_color);
  terminal_buffer[cursor] = vga_entry(c,entry_colors);

  cur_column++;
  
}
void puth(uint64_t n) {
    char hex_digits[] = "0123456789ABCDEF";
    putc('0');
    putc('x');
    
    // Process 16 hex digits (for 64-bit address)
    for (int i = 15; i >= 0; i--) {
        // Shift right by i * 4 bits to get the current nibble, 
        // then mask with 0xF to get the value 0-15
        uint8_t nibble = (n >> (i * 4)) & 0xF;
        putc(hex_digits[nibble]);
    }
}


void puts(const char* s)
{
  while(*s)
  {
    putc(*s);
    s++;
  }
}
void puti(int i)
{
  if(i<0)
  {
    putc('-');
    i*=-1;
  }
  if(i>=10) puti(i/10);
  putc('0' + (i%10));
}

void terminal_initialize(void)
{

  for(size_t y = 0; y<VGA_HEIGHT; y++)
  {
    for(size_t x = 0; x<VGA_WIDTH; x++)
    {
      const size_t indx = y*VGA_WIDTH + x;
      terminal_buffer[indx] = vga_entry(' ',vga_entry_color(terminal_color,terminal_color));
    }
  }
  
  write_to_terminal("Hello from the OS!",WHITE);
  
}



void write_to_terminal(const char* str,enum vga_color color, ...)
{
  text_color = color;
  va_list args;
  va_start(args,color);
  uint8_t state = NORMAL;

  while(*str)
  {
    switch (state)
    {
      case NORMAL:
        switch(*str)
        {
          case '%': state = SPECIFIER;
                    break;

          default: putc(*str);
                    break;
        }
        break;
      
      case SPECIFIER:
        switch(*str)
        {

          case 'c': char c = (char)va_arg(args,int);
                    putc(c);
                    state = NORMAL;
                    break;
          
          case 's': char *s = va_arg(args, char *);
                    puts(s);
                    state = NORMAL;
                    break;

          case 'd':
          case 'i': int i = va_arg(args,int);
                    puti(i);
                    state = NORMAL;
                    break;
          case 'h': uint64_t n = va_arg(args,uint64_t);
                               puth(n);
                               state=NORMAL;
                               break;
        }

        break;
        
    }

    str++;
  }

  va_end(args);
  cur_row++;
  cur_column=0;

}