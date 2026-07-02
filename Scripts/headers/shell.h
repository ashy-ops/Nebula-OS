#include <stdint.h>
#include "display.h"
#include "keyboard.h"
#include <stdbool.h>

#define STATE_READ_COMMAND 0
#define STATE_READ_ARGS 1

extern bool SHELL_ACTIVE;

void INIT_SHELL();
void parse_command();
