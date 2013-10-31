/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_step.h"
#include "mips_gpr.h"
#include "mips_cop.h"
#include "mips_opcode.h"
#include "mips_interrupt.h"
#include "mips_exception.h"
#include "mips_memory.h"
#include "mips_debugger.h"
#include <stdio.h>
#include <OS.h>

void mips_step()
{
	// Check for interrupts	
	mips_check_interrupts();

	// Process all exceptions/interrupts
	if(mips_process_exception()) {
		printf("Exception occured, code: %d\n",cop_reg[0].cp0.cause.exccode);
	}

	// Fetch next instruction
	mips_curr_inst.data = mips_read_uint32(gpr_reg.pc);

	// Process instruction
	opcode_func[mips_curr_inst.i_type.op]();

	// Step here
	mips_step_debugger();

	// Process jumps and program counter
	if(gpr_reg.dl_ct >= 0) {
		// branch delay, check status
		if(gpr_reg.dl_ct == 0) {
			gpr_reg.pc = gpr_reg.dl_pc;
		}
		else {
			// Not yet, usual proceeding
			gpr_reg.pc += 4;
		}
		gpr_reg.dl_ct --;
	}
	else {
		// no branch delay, so usual proceeding
		gpr_reg.pc += 4;
	}

	// Make sure that it stays 0
	gpr_reg.r[0]=0;

	// update cycle counter
	gpr_reg.cc ++; 
}


