#include "handler_init.h"

void INIT_HANDLERS()
{
    isr_register_handler(TIMER_INTERRUPT,&pit_handler);

}