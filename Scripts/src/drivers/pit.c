#include "pit.h"

void pit_handler(registers_t* regs __attribute__((unused)))
{
    UPDATE_SCREEN();
}
