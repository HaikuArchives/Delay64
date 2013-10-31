/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_EXCEPTION_H
#define _MIPS_EXCEPTION_H

#ifndef _MIPS_TYPES_H
#include "mips_types.h"
#endif

enum mips_exception_code
{
	MIPS_EXCEPTION_INT 		= 0,
	MIPS_EXCEPTION_MOD 		= 1,
	MIPS_EXCEPTION_TLBL		= 2,
	MIPS_EXCEPTION_TLBS 	= 3,
	MIPS_EXCEPTION_ADEL 	= 4,
	MIPS_EXCEPTION_ADES		= 5,
	MIPS_EXCEPTION_IBE 		= 6,
	MIPS_EXCEPTION_DBE 		= 7,
	MIPS_EXCEPTION_SYS 		= 8,
	MIPS_EXCEPTION_BP 		= 9,
	MIPS_EXCEPTION_RI 		= 10,
	MIPS_EXCEPTION_CPU 		= 11,
	MIPS_EXCEPTION_OV 		= 12,
	MIPS_EXCEPTION_TR 		= 13,
	MIPS_EXCEPTION_VCEI 	= 14,
	MIPS_EXCEPTION_FPE 		= 15,
	MIPS_EXCEPTION_WATCH 	= 23,
	MIPS_EXCEPTION_VCED		= 31
};

void mips_do_interrupt(int32 itrmask, bool clear);
void mips_do_exception(int32 code, int32 cop);
bool mips_process_exception();
void mips_do_reset();

#endif
