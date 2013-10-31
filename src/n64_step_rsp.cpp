/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_step_rsp.h"
#include "n64_rsp.h"
#include "n64_opcode_rsp.h"
#include "mips_cop.h"
#include "mips_interrupt.h"
#include "mips_exception.h"
#include "mips_memory.h"

void n64_step_rsp()
{
	// Make sure that we are within the allowed area
	rsp_reg.pc &= 0xFFF;

	// Fetch next instruction
	n64_curr_inst.data = mips_read_uint32(rsp_reg.pc | 0x04001000);

	// Process instruction
	opcode_rsp_func[n64_curr_inst.i_type.op]();

	// Make sure that it stays 0
	rsp_reg.r[0]=0;

	// Process jumps and program counter
	if(rsp_reg.dl_ct >= 0) {
		// branch delay, check status
		if(rsp_reg.dl_ct == 0) {
			rsp_reg.pc = rsp_reg.dl_pc;
		}
		else {
			// Not yet, usual proceeding
			rsp_reg.pc += 4;
		}
		rsp_reg.dl_ct --;
	}
	else {
		// no branch delay, so usual proceeding
		rsp_reg.pc += 4;
	}
	// update cycle counter
	rsp_reg.cc ++; 
}
