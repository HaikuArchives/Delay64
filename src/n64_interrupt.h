/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _N64_INTERRUPT_H
#define _N64_INTERRUPT_H

#ifndef _MIPS_INTERRUPT_H
#include "mips_interrupt.h"
#endif

void n64_init_interrupts();

extern bool n64_check_mi_interrupt;

#endif
