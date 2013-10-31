/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_OPCODE_H
#define _MIPS_OPCODE_H

#ifndef _MIPS_TYPES_H
#include "mips_types.h"
#endif

typedef union
{
	struct
	{
		uint32 funct:6;
		uint32 fd:5;
		uint32 fs:5;
		uint32 ft:5;
		uint32 fmt:5;
		uint32 op:6;
	} f_type;  // FLOATING POINT
	
	struct
	{
		uint32 im:16;		// 16
		uint32 rt:5;		// 21
		uint32 rs:5;		// 26
		uint32 op:6;		// 32
	} i_type;	// IMMEDIATE

	struct
	{
		uint32 im:16;		// 16
		uint32 funct:5;		// 21
		uint32 rs:5;		// 26
		uint32 op:6;		// 32
	} g_type;	// REGIMM

	struct
	{
		uint32 funct:6;		// 6
		uint32 sa:5;		// 11
		uint32 rd:5;		// 16
		uint32 rt:5;		// 21
		uint32 rs:5;		// 26
		uint32 op:6;		// 32
	} r_type; 	// STANDARD
	
	struct 
	{
		uint32 of:21;		// 21
		uint32 funct:5;		// 26
		uint32 op:6;		// 32
	} b_type;	// OFFSET

	struct 
	{
		uint32 sa:11;		// 11
		uint32 rd:5;		// 16
		uint32 rt:5;		// 21
		uint32 funct:5;		// 26
		uint32 op:6;		// 32
	} c_type;	// SPECIAL
	
	struct
	{
		uint32 target:26;	// 26
		uint32 op:6;		// 32
	} j_type;	// JUMPS
	
	struct
	{
		uint32 funct:6;		// 6
		uint32 data:19;		// 25
		uint32 co:1;		// 26
		uint32 op:6;		// 32
	} o_type;

	uint32 data;
} mips_inst;

typedef void (*mips_opcode_func)();

extern mips_inst mips_curr_inst;
extern mips_opcode_func opcode_func[64];

#endif
