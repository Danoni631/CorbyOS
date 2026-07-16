#ifndef TIMER_H
#define TIMER_H

#include "idt.h"

void PITIRQ0(struct InterruptRegisters *r);
void sleep(int seconds);
void init_timer();
unsigned int get_ticks();
#endif