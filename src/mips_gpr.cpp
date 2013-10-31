/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include <mips_gpr.h>
#include <string.h>

mips_gpr_reg gpr_reg;

void mips_init_gpr_regs()
{
	memset(&gpr_reg,0,sizeof(mips_gpr_reg));
	gpr_reg.dl_ct 	= -1;
	gpr_reg.dl_pc 	= 0;
	gpr_reg.pc 		= 0xa4000040;
	gpr_reg.r[20] 	= 0x0000000000000001ULL;
	gpr_reg.r[22] 	= 0x000000000000003fULL;
	gpr_reg.r[29] 	= 0x0000000000400000ULL;
}
