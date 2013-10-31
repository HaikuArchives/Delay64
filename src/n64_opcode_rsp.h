/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_OPCODE_RSP_H
#define _MIPS_OPCODE_RSP_H

#ifndef _MIPS_OPCODE_H
#include "mips_opcode.h"
#endif

extern mips_inst n64_curr_inst;
extern mips_opcode_func opcode_rsp_func[64];

#endif
