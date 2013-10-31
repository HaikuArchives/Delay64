/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _N64_MEMORY_H
#define _N64_MEMORY_H

#ifndef _MIPS_MEMORY_H
#include "mips_memory.h"
#endif

struct n64_memory_areas
{
	uint8	*rdram;
	uint8	*rdreg;
	uint8	*spdmem;
	uint8	*spimem;
	uint8	*spreg;
	uint8	*spmem;
	uint8	*dpcreg;
	uint8	*dpsreg;
	uint8	*mireg;
	uint8	*vireg;
	uint8	*aireg;
	uint8	*pireg;
	uint8	*rireg;
	uint8	*sireg;
};

extern n64_memory_areas n64_memory;

void n64_get_rdram_ptr(uint32 addr, uint8 **ptr, int32 *len);

void n64_init_memory();
void n64_exit_memory();

#endif

