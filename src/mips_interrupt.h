/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_INTERRUPT_H
#define _MIPS_INTERRUPT_H

#ifndef _MIPS_TYPES_H
#include "mips_types.h"
#endif

#define MIPS_MAX_INTERRUPT_FUNC	64

typedef int32 (*mips_interrupt_func) ();	// return interrupt number or -1

void mips_add_interrupt(mips_interrupt_func func);
void mips_check_interrupts();

void mips_init_interrupts();
void mips_exit_interrupts();

extern mips_interrupt_func interrupt_func[MIPS_MAX_INTERRUPT_FUNC];

#endif
