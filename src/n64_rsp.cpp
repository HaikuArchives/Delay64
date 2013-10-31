/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_rsp.h"
#include <string.h>

n64_rsp_reg rsp_reg;

void n64_init_rsp_regs()
{
	memset(&rsp_reg,0,sizeof(n64_rsp_reg));
	rsp_reg.dl_ct 	= -1;
	rsp_reg.dl_pc 	= 0;
	rsp_reg.pc 		= 0;
	rsp_reg.hlt		= 1;
}
