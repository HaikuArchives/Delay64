/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_COP_H
#define _MIPS_COP_H

#ifndef _MIPS_TYPES_H
#include "mips_types.h"
#endif

union mips_cop_reg
{
	struct // CPX
	{
		int64 r[32];
		int64 c[32];
	} cpx;
	
	struct // CP0
	{
		uint64 index;
		uint64 random;
		uint64 entrylo0;
		uint64 entrylo1;
		uint64 context;
		uint64 pagemask;
		uint64 wired;
		uint64 reserved07;
		uint64 badvaddr;
		
		struct 
		{
			uint32 value:32;	// 32
			uint32 dummy:32;	// 64
		} count;
		
		uint64 entryhi;

		struct 
		{
			uint32 value:32;	// 32
			uint32 dummy:32;	// 64
		} compare;
		
		struct
		{
			uint32 ie:1;		// 1
			uint32 exl:1;		// 2
			uint32 erl:1;		// 3
			uint32 ksu:2;		// 5
			uint32 ux:1;		// 6
			uint32 sx:1;		// 7
			uint32 kx:1;		// 8
			uint32 im:8;		// 16

			// DS field
			uint32 de:1;		// 17
			uint32 ce:1;		// 18
			uint32 ch:1;		// 19
			uint32 r1:1;		// 20
			uint32 sr:1;		// 21
			uint32 ts:1;		// 22
			uint32 bev:1;		// 23
			uint32 r0:2;		// 25

			uint32 re:1;		// 26
			uint32 fr:1;		// 27
			uint32 rp:1;		// 28
			uint32 cu:4;		// 32
			
			uint32 dummy:32;	// 64
		} sr;
		
		struct 
		{
			uint32 r3:2;		// 2
			uint32 exccode:5;	// 7
			uint32 r2:1;		// 8
			uint32 ip:8;		// 16
			uint32 r1:12;		// 28
			uint32 ce:2;		// 30
			uint32 r0:1;		// 31
			uint32 bd:1;		// 32	
			
			uint32 dummy:32;	// 64
		} cause;

		uint64 epc;
		uint64 prid;
		uint64 config;
		uint64 lladdr;
		uint64 watchlo;
		uint64 watchhi;
		uint64 xcontext;
		uint64 reserved15;
		uint64 reserved16;
		uint64 reserved17;
		uint64 reserved18;
		uint64 reserved19;
		uint64 ecc;
		uint64 cacheerr;
		uint64 taglo;
		uint64 taghi;
		uint64 errorepc;
		uint64 reserved1f;
		uint64 handler[32];
	} cp0;
	
	struct
	{
		int64 r[32];
		
		struct
		{
			uint32 rev:8;
			uint32 imp:8;
			uint32 rsv:16;
		} imprev;
		
		int64 dummy[30];
		
		struct
		{
			uint32 rm:2;	// 2
			
			uint32 f_i:1;	// 3
			uint32 f_u:1;	// 4
			uint32 f_o:1;	// 5
			uint32 f_z:1;	// 6
			uint32 f_v:1;	// 7
	
			uint32 e_i:1;	// 8
			uint32 e_u:1;	// 9
			uint32 e_o:1;	// 10
			uint32 e_z:1;	// 11
			uint32 e_v:1;	// 12
	
			uint32 c_i:1;	// 13
			uint32 c_u:1;	// 14
			uint32 c_o:1;	// 15
			uint32 c_z:1;	// 16
			uint32 c_v:1;	// 17
			uint32 c_e:1;	// 18
			
			uint32 rsv0:5;	// 23
			
			uint32 c:1;		// 24
			uint32 fs:1;	// 25
			
			uint32 rsv1:7;	// 32	
		}sr;
		
	} cp1;
};

/* coprocessor registers */
extern	mips_cop_reg cop_reg[3];

void mips_init_cop_regs();

#endif
