/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_GPR_H
#define _MIPS_GPR_H

#ifndef _MIPS_TYPES_H
#include "mips_types.h"
#endif

struct mips_gpr_reg
{
	int64	r[32];
	int64 	hi;
	int64	lo;
	uint32 	pc;
	int32 	dl_pc;	// delay pc offset, 64bit needed?
	int32 	dl_ct;	// delay counter, -1 == inactive
	int32 	ll;		// load linked
	int32 	hlt;	// halt main loop
	int32 	cc;		// cycles
	int32 	ex;		// exception pending
};

/* GPR regs */
extern mips_gpr_reg gpr_reg;

void mips_init_gpr_regs();

#endif
