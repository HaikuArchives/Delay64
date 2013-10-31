/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_interrupt.h"
#include "mips_exception.h"
#include "mips_cop.h"
#include <stdio.h>

mips_interrupt_func interrupt_func[MIPS_MAX_INTERRUPT_FUNC];

static int32 timer_interrupt_func(void)
{
	cop_reg[0].cp0.count.value++;
	return (cop_reg[0].cp0.count.value == cop_reg[0].cp0.compare.value) ? 0x80 : -1;
}

void mips_add_interrupt(mips_interrupt_func func)
{
	for(int32 c=0;c<MIPS_MAX_INTERRUPT_FUNC;c++) {
		if(interrupt_func[c]==0) {
			interrupt_func[c]=func;
			break;
		}
	}
}

void mips_check_interrupts()
{
	for(int32 c=0;c<MIPS_MAX_INTERRUPT_FUNC;c++) {
		if(interrupt_func[c]) {
			int32 intr = interrupt_func[c]();
			if(intr>=0) {
				mips_do_interrupt(intr&0xFF,(intr&0x100)?true:false);
			}
		}
		else {
			break;
		}
	}
}

void mips_init_interrupts()
{	
	for(int32 c=0;c<8;c++) interrupt_func[c]=0;
	mips_add_interrupt(&timer_interrupt_func);
}

void mips_exit_interrupts()
{
}
