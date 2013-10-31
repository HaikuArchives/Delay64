/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_interrupt.h"
#include "n64_memory.h"
#include "n64_register.h"
#include "n64_screen.h"
#include <stdio.h>

#define VI_RETRACE 0x98968

bool n64_check_mi_interrupt = false;

inline void n64_vi_interrupt()
{
	static int32 counter = VI_RETRACE;
	if(counter>0) {
		counter--;
	}
	else {
		uint32 *mireg =  (uint32 *)n64_memory.mireg;

		if(mips_host_little_endian()) {
			mireg[MI_INTR_MASK_REG] = swap_uint32(mireg[MI_INTR_MASK_REG]);
			mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
		}

		mireg[MI_INTR_REG] |= MI_INTR_REG_VI_MASK;

		if(mips_host_little_endian()) {
			mireg[MI_INTR_MASK_REG] = swap_uint32(mireg[MI_INTR_MASK_REG]);
			mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
		}

		counter=VI_RETRACE;
		n64_update_screen();
		n64_check_mi_interrupt = true;
	}
}

static int32 n64_mi_interrupt()
{
	n64_vi_interrupt();

	if(n64_check_mi_interrupt) {

		int32 ret = -1;
		uint32 *mireg =  (uint32 *)n64_memory.mireg;

		if(mips_host_little_endian()) {
			mireg[MI_INTR_MASK_REG] = swap_uint32(mireg[MI_INTR_MASK_REG]);
			mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
		}

		if(mireg[MI_INTR_REG] & mireg[MI_INTR_MASK_REG]) {
			ret =  0x004;
		}
	
		if(mips_host_little_endian()) {
			mireg[MI_INTR_MASK_REG] = swap_uint32(mireg[MI_INTR_MASK_REG]);
			mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
		}
		
		n64_check_mi_interrupt = false;

		return ret;
	}

	return -1;
}

void n64_init_interrupts()
{
	mips_add_interrupt(&n64_mi_interrupt);
}
