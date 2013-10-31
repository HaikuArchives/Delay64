/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _N64_RSP_H
#define _N64_RSP_H

#ifndef _MIPS_GPR_H
#include "mips_gpr.h"
#endif

struct n64_rsp_reg
{
	int32 r[32];
	uint32 pc;		// pc, 64bit needed?

	int32 dl_pc;	// delay pc offset, 64bit needed?
	int32 dl_ct;	// delay counter, -1 == inactive
	int32 ll;		// load linked
	int32 hlt;		// halt main loop
	int32 cc;		// cycles
};

/* RSP regs */
extern n64_rsp_reg rsp_reg;

void n64_init_rsp_regs();
#endif
