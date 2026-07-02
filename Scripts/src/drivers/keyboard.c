#include "keyboard.h"

bool SHIFT_ACTIVE = false;
bool ENABLE_WRITE = false;
bool SHELL_ACTIVE = false;

char keyboard_buffer[2000] = {0};
size_t buffer_len = 0;
size_t buffer_pos = 0;

// Scancode set 1
static const char scancode_to_ascii_lowercase[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',   0, ' '
};

static const char scancode_to_ascii_uppercase[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',   0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',   0, ' '
};
const char* lookup = scancode_to_ascii_lowercase;

void keyboard_handler(registers_t* regs __attribute__((unused)))
{
    uint8_t scancode = inb(PS2_DATA);

    //shift key release 
    if(scancode & 0x80)
    {
        uint8_t make_code = scancode & 0x7F;
        if(make_code == 0x2A || make_code == 0x36)
        {
            lookup = scancode_to_ascii_lowercase;
            SHIFT_ACTIVE = false;
        }
        return;
    }

    //checking for shift key press
    if(scancode == 0x2A || scancode == 0x36)
    {
        lookup = scancode_to_ascii_uppercase;
        SHIFT_ACTIVE = true;
        return;
    }

    //check for escape press
    if(scancode == 1 && SHELL_ACTIVE == false)
    {
        INIT_SHELL();
        SHELL_ACTIVE = true;
        ENABLE_WRITE = true;
        return;
    }

    //write to screen only when enabled
    if(!ENABLE_WRITE)
    {
        return;
    }

    //check for enter
    if(scancode == 0x1C && SHELL_ACTIVE == true)
    {
        if (buffer_len < BUFFER_SIZE) {
            keyboard_buffer[buffer_len] = 0; //0 is same as \0!
        }
        parse_command();
        buffer_len = 0;
        buffer_pos = 0;
        return;
    }

    //check for left arrow key
    if(scancode == 0x4B)
    {
        if(buffer_pos > 0)
        {
            buffer_pos--;
            cursor_write--;
        }
        return;
    }

    //check for right arrow key
    if(scancode == 0x4D)
    {
        if(buffer_pos < buffer_len)
        {
            buffer_pos++;
            cursor_write++;
        }
        return;
    }

    //backspace
    if(scancode == 0x0E)
    {
        if(buffer_pos > 0)
        {

            uint8_t current_attr = vga_entry_color(text_color, terminal_color);
            uint16_t clean_space = vga_entry(' ', current_attr);

            //if cursor at the very end no shifting needed
            if(cursor_write == cursor_free)
            {
                cursor_write--;
                cursor_free--;
                buffer_len--;
                buffer_pos--;
                
                //clear the text at previous position (we did -- above!)
                buffer[cursor_write] = clean_space;
            }
            else
            {
                //cursor is in the middle of text, hence shift everything left
                for(size_t i = cursor_write - 1; i < cursor_free - 1; i++)
                {
                    buffer[i] = buffer[i+1];
                }
                for(size_t i = buffer_pos - 1; i < buffer_len - 1; i++)
                {
                    keyboard_buffer[i] = keyboard_buffer[i+1];
                }
                
                buffer_len--;
                buffer_pos--;
                cursor_write--;
                cursor_free--;
                
                // Blank out the trailing slot left behind by the shift
                buffer[cursor_free] = clean_space; 
            }
        }
        return;
    }

    if(scancode < sizeof(scancode_to_ascii_lowercase))
    {
        char ascii_char = lookup[scancode];

        if(ascii_char != 0 && ascii_char != 27)
        {
            if(buffer_len < BUFFER_SIZE)
            {
                //shift the buffer right
                for(size_t i = cursor_free; i > cursor_write; i--)
                {
                    buffer[i] = buffer[i-1];
                }
                
                //put the character at the cursor position
                putc(ascii_char, text_color);

                //shift the keyboard buffer right
                for(size_t i = buffer_len; i > buffer_pos; i--)
                {
                    keyboard_buffer[i] = keyboard_buffer[i-1];
                }
                
                //put the character at that position in keyboard buffer
                keyboard_buffer[buffer_pos] = ascii_char;
                
                buffer_len++;
                buffer_pos++;
            }
        }
    }
}