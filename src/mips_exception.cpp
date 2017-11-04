/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_exception.h"
#include "mips_cop.h"
#include "mips_gpr.h"
#include <stdio.h>
#include <stdlib.h>

static int32 mips_exception_priority[32] =
{
	31,	//MIPS_EXCEPTION_INT
	27,	//MIPS_EXCEPTION_MOD
	26,	//MIPS_EXCEPTION_TLBL
	25,	//MIPS_EXCEPTION_TLBS
	24,	//MIPS_EXCEPTION_ADEL
	23,	//MIPS_EXCEPTION_ADES
	21,	//MIPS_EXCEPTION_IBE
	30,	//MIPS_EXCEPTION_DBE
	22,	//MIPS_EXCEPTION_SYS
	22,	//MIPS_EXCEPTION_BP
	22,	//MIPS_EXCEPTION_RI
	22,	//MIPS_EXCEPTION_CPU
	22,	//MIPS_EXCEPTION_OV
	22,	//MIPS_EXCEPTION_TR
	20,	//MIPS_EXCEPTION_VCEI
	22,	//MIPS_EXCEPTION_FPE
	99,
	99,
	99,
	99,
	99,
	99,
	99,
	28,	//MIPS_EXCEPTION_WATCH
	99,
	99,
	99,
	99,
	99,
	99,
	99,
	29	//MIPS_EXCEPTION_VCED
};

void mips_do_interrupt(int32 itr, bool clear)
{
	if(clear) {
		cop_reg[0].cp0.cause.ip &= ~itr;
	}
	else {
		cop_reg[0].cp0.cause.ip |=  itr;
	}
}

void mips_do_exception(int32 code, int32 cop)
{
	// Check if we are already in exception mode
	if((cop_reg[0].cp0.sr.exl==0) && (cop_reg[0].cp0.sr.erl==0)) {

		// Check priority of exception
		if(mips_exception_priority[code]<=mips_exception_priority[cop_reg[0].cp0.cause.exccode]) {

			// Set the cause
			cop_reg[0].cp0.cause.exccode = code;

			// Set the coprocessor
			cop_reg[0].cp0.cause.ce = cop;

			// Let mips_process_exception() know that he got a normal exception
			gpr_reg.ex = 1;
		}
	}
}

void mips_do_reset()
{
	cop_reg[0].cp0.sr.bev = 1;
	cop_reg[0].cp0.sr.ts = 0;
	cop_reg[0].cp0.sr.sr = 0;
	cop_reg[0].cp0.sr.erl = 1;
	cop_reg[0].cp0.cacheerr = 0;
	cop_reg[0].cp0.errorepc = gpr_reg.pc;
	cop_reg[0].cp0.config = 0;
	gpr_reg.pc = 0xBFC00380;
	gpr_reg.dl_ct = -1;
}

bool mips_process_exception()
{
	if(
		// Normal exception
		(gpr_reg.ex)||

		// Interrupt needs to be handeled?
		((cop_reg[0].cp0.sr.ie &&
		 (cop_reg[0].cp0.cause.ip & cop_reg[0].cp0.sr.im) &&
		 (cop_reg[0].cp0.sr.exl == 0) &&
		 (cop_reg[0].cp0.sr.erl == 0)))
	)
	{
		// We have an interrupt here!
		if(!gpr_reg.ex) {
			cop_reg[0].cp0.cause.exccode = MIPS_EXCEPTION_INT;
		} else {
			// No need to recall this one
			gpr_reg.ex = 0;
		}

		// Are we in a branch delay?
		cop_reg[0].cp0.cause.bd = (gpr_reg.dl_ct==0) ? 1 : 0;

		// Set epc correctly
		cop_reg[0].cp0.epc = gpr_reg.pc - ((gpr_reg.dl_ct==0) ? 4 : 0);

		// Set exception mode
		cop_reg[0].cp0.sr.exl = 1;

		// Set vector
		if(cop_reg[0].cp0.sr.bev) {
			gpr_reg.pc = 0xBFC00380;
		} else {
			gpr_reg.pc = 0x80000180;
		}

		// Clear possible branch delay mode
		gpr_reg.dl_ct = -1;

		return true;
	}
	return false;
}
