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


void puts(const char* s)
{
  while(*s)
  {
    putc(*s);
    s++;
  }
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

/*
%c - char(1 byte)
%d - int(4 byte)
%hd - short(2 byte)
%hu - Ushort(2 byte)
%hh - char sized(1 byte)
*/

void write_to_terminal(const char* str,enum vga_color color, ...)
{
  text_color = color;
  va_list args;
  va_start(args,color);
  uint8_t state = STATE_NORMAL;
  uint8_t length = LEN_DEF;
  uint8_t radix = 10;
  bool sign = false;

  while(*str)
  {
    switch (state)
    {

      case STATE_NORMAL:
        switch(*str)
        {
          case '%': state = STATE_LENGTH;
                    break;

          default: putc(*str);
                    break;
        }

        break;
      
      case STATE_LENGTH:
        switch (*str)
        {
        case 'h': length = LEN_S;
                  state = STATE_LENGTH_S;
                  break;
        
        case 'l': length = LEN_L;
                  state = STATE_LENGTH_L;
                  break;

        default:  goto PRINT_STATE_SPECIFIER;
                  break;
        }

        break;
      
      case STATE_LENGTH_S:
        switch (*str)
        {
        case 'h': state  = STATE_SPECIFIER;
                  length = LEN_SS;
                  break;

        default:  goto PRINT_STATE_SPECIFIER;
        }
        break;

      case STATE_LENGTH_L:
        switch(*str)
        {
          case 'l': state  = STATE_SPECIFIER;
                    length = LEN_LL;
                    break;

          default : goto PRINT_STATE_SPECIFIER;
        }
        break;
      
      
        
      case STATE_SPECIFIER:

        PRINT_STATE_SPECIFIER:

        switch(*str)
        {
          case 'c': char c = (char)va_arg(args,int); //automatic promotion of char to int at runtime by C when passed as variadic function!
                    putc(c);
                    break;

          case 's': char* s = (char *)va_arg(args,char *);
                    puts(s); //when I pass a string as a parameter,C automatically addsa a '\0'
                    break;

          case '%': putc('%');
                    break;

          case 'd':
          case 'i': 
                    radix = 10;
                    sign  = true;
                    print_num(&args,length,sign,radix);

                    break;

          case 'u': radix = 10;
                    sign  = false;
                    print_num(&args,length,sign,radix);

                    break;
          
          case 'p':          
          case 'x':
          case 'X': radix = 16;
                    sign  = false;
                    print_num(&args,length,sign,radix);

                    break;
        
          case 'o': radix = 8;
                    sign = false;
                    print_num(&args,length,sign,radix);
                    break;
          default : break; //ignore invalid specifiers

        }

      
      state  = STATE_NORMAL;
      length = LEN_DEF;
      radix  = 10;
      sign   = false;

      break;
    
    }
    
    str++;


  }

  va_end(args);
  cur_row++;
  cur_column=0;
}

const char hex_chars[] = "0123456789ABCDEF";

//sign here represents weather its unsigned or not, not weather its negative or positive
void print_num(va_list* args,uint8_t len,bool sign, uint8_t radix)
{
  char buffer[32];
  uint64_t num = 0;
  int num_sign = 1;
  int pos = 0;

  switch(len)
  {
    case LEN_DEF:
    case LEN_SS:
    case LEN_S:
    case LEN_L: //for now it is 32-bit
    {
      int n = va_arg(*args,int); //everything smaller than 32-bits is stored as an int(32-bit)
      if(sign && n<0)
      {
        n = -n;
        num_sign = -1;
      }
      num = (uint64_t)n;

      break;
    } //enclosing in brackets to make declaration of n local to LEN_L otherwise its being re-decalred in LEN_LL
    case LEN_LL:
      int64_t n = va_arg(*args,int64_t);
      if(sign && n<0)
      {
        n =-n;
        num_sign = -1;
      }
      num = (uint64_t)n;
      break;
    default:
      num = 0;
      break;
  }

  do
  {
    uint32_t rem = num%radix;
    num/=radix;
    buffer[pos++] = hex_chars[rem];


  } while(num > 0); //do to ensure that if number is 0 it also gets printed

  if(sign && num_sign==-1) buffer[pos++]='-';

  while(--pos>=0) putc(buffer[pos]);

}
//umoddi3