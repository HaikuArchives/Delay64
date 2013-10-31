/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_cop.h"
#include <string.h>

mips_cop_reg cop_reg[3];

void mips_init_cop_regs()
{
	memset(&cop_reg,0,sizeof(cop_reg));
	cop_reg[0].cpx.r[12] = 0x70400004;
	cop_reg[0].cpx.r[ 1] = 0x0000002f;
	cop_reg[0].cpx.r[16] = 0x00066463;
	cop_reg[0].cpx.r[15] = 0x00000b00;
	cop_reg[1].cpx.c[ 0] = 0x00000511;
}
