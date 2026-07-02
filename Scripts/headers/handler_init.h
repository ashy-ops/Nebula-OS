#pragma once
#include "display.h"
#include "isr.h"
#include "io.h"
#include "pit.h"
#include "keyboard.h"

#define KEYBOARD_INTERRUPT 0x21
#define TIMER_INTERRUPT 0x20


void INIT_HANDLERS();