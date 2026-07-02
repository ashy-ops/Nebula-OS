#include "shell.h"

static const char* programs[] = 
{
    "ccT [num]: to change terminal color (0-15)",
    "cct [num]: to change text color (0-15)"
};

void INIT_SHELL()
{
    SHELL_ACTIVE = true;
    CLEAR_SCREEN();
    WRITE("WELCOME TO THE SHELL :>, AVAILABLE COMMANDS ARE:\n1.)echo\n2.)ls\n3.)clear\n",WHITE);
    WRITE("neublaOS:~/$ ",text_color);

    //clear keyboard buffer
    for(int i=0; i<BUFFER_SIZE; i++)
    {
        keyboard_buffer[i] = 0;
    }
    buffer_len = 0;
    buffer_pos = 0;
}


void parse_command()
{
    WRITE("\n",text_color); //new line to print the output from next line!
    size_t state = STATE_READ_COMMAND; 
    char command[32] = {0};
    char args[100] = {0};
    int cmd_idx = 0;
    int arg_idx = 0;

    for(size_t i =0; i<buffer_len; i++)
    {
        char c = keyboard_buffer[i];

        if(state == STATE_READ_COMMAND)
        {
            if(c == ' ')state = 1;
            else if(cmd_idx < 31) command[cmd_idx++] = c;
        }
        else
        {
            if(arg_idx < 99) args[arg_idx++] = c;
        }
    }

    if(command[0] == 0)
    {
        //empty command
        //empty code to break if else chain
    }

    //echo command
    else if(command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o' && command[4] == 0)
    {
        if(args[0] != 0) WRITE("%s\n", text_color, args);
        else WRITE("\n",text_color);

    }

    //ls commmand
    else if(command[0] == 'l' && command[1] == 's' && command[2] == 0)
    {
        
        WRITE("AVAILABLE PROGRAMS:\n", text_color);

        size_t num_programs = sizeof(programs) / sizeof(programs[0]);
        for(size_t i = 0; i<num_programs; i++)
        {
            WRITE("%d: %s\n",text_color,i+1,programs[i]);
        }

    }

else if(command[0] == 'c' && command[1] == 'c' && command[2] == 'T' && command[3] == 0)
    {
        int color_val = 0;
        int i = 0;
        bool valid = false;

        while(args[i] == ' ') i++;

        if (args[i] >= '0' && args[i] <= '9') {
            valid = true;
            while(args[i] >= '0' && args[i] <= '9') {
                color_val = color_val * 10 + (args[i] - '0');
                i++;
            }
        }

        if(valid && color_val >= 0 && color_val <= 15)
        {
            UPDATE_TERMINAL_COLOR((uint8_t) color_val);
            WRITE("TERMINAL COLOR CHANGED TO %d!\n", text_color, color_val);
        }
        else
        {
            WRITE("Error: Invalid color! Choose a number from 0-15.\n", text_color);
        }

    }

    else if(command[0] == 'c' && command[1] == 'c' && command[2] == 't' && command[3] == 0)
    {
        int color_val = 0;
        int i = 0;
        bool valid = false;

        while(args[i] == ' ') i++;

        if (args[i] >= '0' && args[i] <= '9') {
            valid = true;
            while(args[i] >= '0' && args[i] <= '9') {
                color_val = color_val * 10 + (args[i] - '0');
                i++;
            }
        }

        if(valid && color_val >= 0 && color_val <= 15)
        {
            UPDATE_TEXT_COLOR((uint8_t) color_val);
            WRITE("TEXT COLOR CHANGED TO %d!\n", text_color, color_val);
        }
        else
        {
            WRITE("Error: Invalid color! Choose a number from 0-15.\n", text_color);
        }

        
    }
    else if(command[0] =='c' && command[1] =='l' && command[2]=='e' && command[3] =='a' && command[4] == 'r' && command[5] == 0)
    {
        CLEAR_SCREEN();
    }
    else WRITE("INVALID COMMAND :<\n",text_color);

    WRITE("neublaOS:~/$ ",text_color);


}