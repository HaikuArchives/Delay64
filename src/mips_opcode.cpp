/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_opcode.h"
#include "mips_gpr.h"
#include "mips_cop.h"
#include "mips_memory.h"
#include "mips_exception.h"
#include <stdio.h>
#include <math.h>

#define FP_VALUE_FS() \
	fs =(cop_reg[0].cp0.sr.fr ? \
		 cop_reg[1].cp1.r[mips_curr_inst.f_type.fs+0] : \
		(cop_reg[1].cp1.r[mips_curr_inst.f_type.fs+1]<<32 | \
		 cop_reg[1].cp1.r[mips_curr_inst.f_type.fs+0]&0xFFFFFFFF ) \
	)
	
#define FP_VALUE_FT() \
	ft =(cop_reg[0].cp0.sr.fr ? \
		 cop_reg[1].cp1.r[mips_curr_inst.f_type.ft+0] : \
		(cop_reg[1].cp1.r[mips_curr_inst.f_type.ft+1]<<32 | \
		 cop_reg[1].cp1.r[mips_curr_inst.f_type.ft+0]&0xFFFFFFFF ) \
	)
	
#define FP_VALUE_FD() \
	fd =(cop_reg[0].cp0.sr.fr ? \
		 cop_reg[1].cp1.r[mips_curr_inst.f_type.fd+0] : \
		(cop_reg[1].cp1.r[mips_curr_inst.f_type.fd+1]<<32 | \
		 cop_reg[1].cp1.r[mips_curr_inst.f_type.fd+0]&0xFFFFFFFF ) \
	)

#define FP_STORE_FD() \
	if(cop_reg[0].cp0.sr.fr) { \
		cop_reg[1].cp1.r[mips_curr_inst.f_type.fd+0]=(int64)fd; \
	} \
	else { \
		cop_reg[1].cp1.r[mips_curr_inst.f_type.fd+1]=(int64)((int32)(fd>>32)); \
		cop_reg[1].cp1.r[mips_curr_inst.f_type.fd+0]=(int64)((int32)fd); \
	}
	
#define FD_FLOAT  *((float  *)&fd)
#define FT_FLOAT  *((float  *)&ft)
#define FS_FLOAT  *((float  *)&fs)

#define FD_DOUBLE *((double *)&fd)
#define FT_DOUBLE *((double *)&ft)
#define FS_DOUBLE *((double *)&fs)

#define FD_INT32  *((int32  *)&fd)
#define FT_INT32  *((int32  *)&ft)
#define FS_INT32  *((int32  *)&fs)

#define FD_INT64  *((int64  *)&fd)
#define FT_INT64  *((int64  *)&ft)
#define FS_INT64  *((int64  *)&fs)

#define MIPS_DYN_FUNC_START(func) \
	asm( \
		"##func##_start:\n" \
		"\tpusha\n" \
	);

#define MIPS_DYN_FUNC_END(func) \
	asm( \
		"\tpopa\n" \
		"##func##_end:\n" \
	);

mips_inst mips_curr_inst;

static void mips_cp1_co_opcode_00() // ADD
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_00);
	
	uint64 fd=0;
	uint64 ft; FP_VALUE_FT();
	uint64 fs; FP_VALUE_FS();
	
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = FS_FLOAT + FT_FLOAT;
				break;
		case	17:
				FD_DOUBLE = FS_DOUBLE + FT_DOUBLE;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_00);
}

static void mips_cp1_co_opcode_01() // SUB
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_01);

	uint64 fd=0;
	uint64 ft; FP_VALUE_FT();
	uint64 fs; FP_VALUE_FS();
	
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = FS_FLOAT - FT_FLOAT;
				break;
		case	17:
				FD_DOUBLE = FS_DOUBLE - FT_DOUBLE;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_01);
}

static void mips_cp1_co_opcode_02() // MUL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_02);

	uint64 fd=0;
	uint64 ft; FP_VALUE_FT();
	uint64 fs; FP_VALUE_FS();
	
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = FS_FLOAT * FT_FLOAT;
				break;
		case	17:
				FD_DOUBLE = FS_DOUBLE * FT_DOUBLE;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_02);
}

static void mips_cp1_co_opcode_03() // DIV
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_03);

	uint64 fd=0;
	uint64 ft; FP_VALUE_FT();
	uint64 fs; FP_VALUE_FS();
	
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				if(FT_FLOAT != 0) {
					FD_FLOAT = FS_FLOAT / FT_FLOAT;
				}
				else {
					// divide by zero
				}
				break;
		case	17:
				if(FT_FLOAT != 0) {
					FD_DOUBLE = FS_DOUBLE / FT_DOUBLE;
				}
				else {
					// divide by zero
				}
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_03);
}

static void mips_cp1_co_opcode_04() // SQRT
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_04);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();
	
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = sqrt(FS_FLOAT);
				break;
		case	17:
				FD_DOUBLE = sqrt(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_04);
}

static void mips_cp1_co_opcode_05() // ABS
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_05);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();
	
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = fabs(FS_FLOAT);
				break;
		case	17:
				FD_DOUBLE = fabs(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_05);
}

static void mips_cp1_co_opcode_06() // MOV
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_06);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = FS_FLOAT;
				break;
		case	17:
				FD_DOUBLE = FS_DOUBLE;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_06);
}

static void mips_cp1_co_opcode_07() // NEG
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_07);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = -FS_FLOAT;
				break;
		case	17:
				FD_DOUBLE = -FS_DOUBLE;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_07);
}

static void mips_cp1_co_opcode_08() // ROUNDL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_08);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();
	double fraction;

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT64 = (int32)modf(FS_FLOAT+.5,&fraction);
				if(fraction == 0.) {
					FD_INT64 &= ~1;
				}
				break;
		case	17:
				FD_INT64 = (int32)modf(FS_DOUBLE+.5,&fraction);
				if(fraction == 0.) {
					FD_INT64 &= ~1;
				}
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_08);
}

static void mips_cp1_co_opcode_09() // TRUNCL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_09);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT64 = (int64)(FS_FLOAT);
				break;
		case	17:
				FD_INT64 = (int64)(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_09);
}

static void mips_cp1_co_opcode_0A() // CEILL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_0A);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT64 = (int64)ceil(FS_FLOAT);
				break;
		case	17:
				FD_INT64 = (int64)ceil(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_0A);
}

static void mips_cp1_co_opcode_0B() // FLOORL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_0B);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT64 = (int64)floor(FS_FLOAT);
				break;
		case	17:
				FD_INT64 = (int64)floor(FS_DOUBLE);
				break;
	}

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_0B);
}

static void mips_cp1_co_opcode_0C() // ROUNDW
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_0C);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();
	double fraction;

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT32 = (int32)modf(FS_FLOAT+.5,&fraction);
				if(fraction == 0.) {
					FD_INT32 &= ~1;
				}
				break;
		case	17:
				FD_INT32 = (int32)modf(FS_DOUBLE+.5,&fraction);
				if(fraction == 0.) {
					FD_INT32 &= ~1;
				}
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_0C);
}

static void mips_cp1_co_opcode_0D() // TRUNCW
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_0D);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT32 = (int32)(FS_FLOAT);
				break;
		case	17:
				FD_INT32 = (int32)(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_0D);
}

static void mips_cp1_co_opcode_0E() // CEILW
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_0E);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT32 = (int32)ceil(FS_FLOAT);
				break;
		case	17:
				FD_INT32 = (int32)ceil(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_0E);
}

static void mips_cp1_co_opcode_0F() // FLOORW
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_0F);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT32 = (int32)floor(FS_FLOAT);
				break;
		case	17:
				FD_INT32 = (int32)floor(FS_DOUBLE);
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_0F);
}

static void mips_cp1_co_opcode_20() // CVTS
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_20);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_FLOAT = (float)FS_FLOAT;
				break;
		case	17:
				FD_FLOAT = (float)FS_DOUBLE;
				break;
		case	20:
				FD_FLOAT = (float)FS_INT32;
				break;
		case	21:
				FD_FLOAT = (float)FS_INT64;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_20);
}

static void mips_cp1_co_opcode_21() // CVTD
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_21);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_DOUBLE = (double)FS_FLOAT;
				break;
		case	17:
				FD_DOUBLE = (double)FS_DOUBLE;
				break;
		case	20:
				FD_DOUBLE = (double)FS_INT32;
				break;
		case	21:
				FD_DOUBLE = (double)FS_INT64;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_21);
}

static void mips_cp1_co_opcode_24() // CVTW
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_24);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT32 = (int32)FS_FLOAT;
				break;
		case	17:
				FD_INT32 = (int32)FS_DOUBLE;
				break;
		case	20:
				FD_INT32 = (int32)FS_INT32;
				break;
		case	21:
				FD_INT32 = (int32)FS_INT64;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_24);
}

static void mips_cp1_co_opcode_25() // CVTL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_25);

	uint64 fd=0;
	uint64 fs; FP_VALUE_FS();

	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				FD_INT64 = (int64)FS_FLOAT;
				break;
		case	17:
				FD_INT64 = (int64)FS_DOUBLE;
				break;
		case	20:
				FD_INT64 = (int64)FS_INT32;
				break;
		case	21:
				FD_INT64 = (int64)FS_INT64;
				break;
	}
	FP_STORE_FD();

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_25);
}

static void mips_cp1_co_opcode_30() // CF
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_30);

	puts(">>>>> CF");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_30);
}

static void mips_cp1_co_opcode_31() // CUN
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_31);

	puts(">>>>> CUN");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_31);
}

static void mips_cp1_co_opcode_32() // CEQ
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_32);

	uint64 fs; FP_VALUE_FS();
	uint64 ft; FP_VALUE_FT();
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				cop_reg[1].cp1.sr.c = (FS_FLOAT == FT_FLOAT);
				break;
		case	17:
				cop_reg[1].cp1.sr.c = (FS_DOUBLE == FT_DOUBLE);
				break;
	}

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_32);
}

static void mips_cp1_co_opcode_33() // CUEQ
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_33);

	puts(">>>>> CUEQ");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_33);
}

static void mips_cp1_co_opcode_34() // COLT
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_34);

	puts(">>>>> COLT");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_34);
}

static void mips_cp1_co_opcode_35() // CULT
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_35);

	puts(">>>>> CULT");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_35);
}

static void mips_cp1_co_opcode_36() // COLE
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_36);

	puts(">>>>> COLE");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_36);
}

static void mips_cp1_co_opcode_37() // CULE
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_37);

	puts(">>>>> CULE");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_37);
}

static void mips_cp1_co_opcode_38() // CSF
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_38);

	puts(">>>>> CSF");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_38);
}

static void mips_cp1_co_opcode_39() // CNGLE
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_39);

	puts(">>>>> CNGLE");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_39);
}

static void mips_cp1_co_opcode_3A() // CSEQ
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_3A);

	puts(">>>>> CSEQ");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_3A);
}

static void mips_cp1_co_opcode_3B() // CNGL
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_3B);

	puts(">>>>> CNGL");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_3B);
}

static void mips_cp1_co_opcode_3C() // CLT
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_3C);

	uint64 fs; FP_VALUE_FS();
	uint64 ft; FP_VALUE_FT();
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				cop_reg[1].cp1.sr.c = (FS_FLOAT < FT_FLOAT);
				break;
		case	17:
				cop_reg[1].cp1.sr.c = (FS_DOUBLE < FT_DOUBLE);
				break;
	}

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_3C);
}

static void mips_cp1_co_opcode_3D() // CNGE
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_3D);

	puts(">>>>> CNGE");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_3D);
}

static void mips_cp1_co_opcode_3E() // CLE
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_3E);

	uint64 fs; FP_VALUE_FS();
	uint64 ft; FP_VALUE_FT();
	switch(mips_curr_inst.f_type.fmt)
	{
		case	16:
				cop_reg[1].cp1.sr.c = (FS_FLOAT <= FT_FLOAT);
				break;
		case	17:
				cop_reg[1].cp1.sr.c = (FS_DOUBLE <= FT_DOUBLE);
				break;
	}

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_3E);
}

static void mips_cp1_co_opcode_3F() // CNGT
{
	MIPS_DYN_FUNC_START(mips_cp1_co_opcode_3F);

	puts(">>>>> CNGT");

	MIPS_DYN_FUNC_END(mips_cp1_co_opcode_3F);
}

static void mips_illegal_cp1_co_opcode()
{
	MIPS_DYN_FUNC_START(mips_illegal_cp1_co_opcode);

	printf("Illegal cp1 co opcode %02x\n",(int)mips_curr_inst.c_type.funct);
	mips_do_exception(MIPS_EXCEPTION_RI,0);

	MIPS_DYN_FUNC_END(mips_illegal_cp1_co_opcode);
}

static mips_opcode_func cp1_co_opcode_func[64] =
{
	&mips_cp1_co_opcode_00,
	&mips_cp1_co_opcode_01,
	&mips_cp1_co_opcode_02,
	&mips_cp1_co_opcode_03,
	&mips_cp1_co_opcode_04,
	&mips_cp1_co_opcode_05,
	&mips_cp1_co_opcode_06,
	&mips_cp1_co_opcode_07,
	&mips_cp1_co_opcode_08,
	&mips_cp1_co_opcode_09,
	&mips_cp1_co_opcode_0A,
	&mips_cp1_co_opcode_0B,
	&mips_cp1_co_opcode_0C,
	&mips_cp1_co_opcode_0D,
	&mips_cp1_co_opcode_0E,
	&mips_cp1_co_opcode_0F,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_cp1_co_opcode_20,
	&mips_cp1_co_opcode_21,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_cp1_co_opcode_24,
	&mips_cp1_co_opcode_25,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_illegal_cp1_co_opcode,
	&mips_cp1_co_opcode_30,
	&mips_cp1_co_opcode_31,
	&mips_cp1_co_opcode_32,
	&mips_cp1_co_opcode_33,
	&mips_cp1_co_opcode_34,
	&mips_cp1_co_opcode_35,
	&mips_cp1_co_opcode_36,
	&mips_cp1_co_opcode_37,
	&mips_cp1_co_opcode_38,
	&mips_cp1_co_opcode_39,
	&mips_cp1_co_opcode_3A,
	&mips_cp1_co_opcode_3B,
	&mips_cp1_co_opcode_3C,
	&mips_cp1_co_opcode_3D,
	&mips_cp1_co_opcode_3E,
	&mips_cp1_co_opcode_3F,
};

static void mips_cp1_opcode_00()	// MFC1
{
	uint64 fs; FP_VALUE_FS();
	gpr_reg.r[mips_curr_inst.f_type.ft] = (int64)(int32)fs;
}

static void mips_cp1_opcode_01() // DMFC1
{
	uint64 fs; FP_VALUE_FS();
	gpr_reg.r[mips_curr_inst.f_type.ft] = (int64)fs;
}

static void mips_cp1_opcode_02() // CFC1
{
	gpr_reg.r[mips_curr_inst.f_type.ft] = 
		(int64)(int32)cop_reg[1].cpx.c[mips_curr_inst.f_type.fs];
}

static void mips_cp1_opcode_04() // MTC1
{
	cop_reg[1].cpx.r[mips_curr_inst.f_type.fs] = 
		(int64)(int32)gpr_reg.r[mips_curr_inst.f_type.ft];
}

static void mips_cp1_opcode_05() // DMTC1
{
	if(cop_reg[0].cp0.sr.fr) {
		cop_reg[1].cpx.r[mips_curr_inst.f_type.fs] = gpr_reg.r[mips_curr_inst.f_type.fs];
	}
	else {
		if((mips_curr_inst.f_type.ft&1)==0) {
			cop_reg[1].cpx.r[mips_curr_inst.f_type.fs+1] = 
				(int64)(((uint64)gpr_reg.r[mips_curr_inst.f_type.ft])>>32);
			cop_reg[1].cpx.r[mips_curr_inst.f_type.fs+0] = 
				(int64)(((uint64)gpr_reg.r[mips_curr_inst.f_type.ft])&0xFFFFFFFF);
		}
	}
}

static void mips_cp1_opcode_06() // CTC1
{
	if(mips_curr_inst.f_type.ft != 0) {
		cop_reg[1].cpx.c[mips_curr_inst.f_type.fs] = gpr_reg.r[mips_curr_inst.f_type.ft];

		puts(">>>>> Possible interrupt bug");
		// interrupt/exception
	}
}

static void mips_cp1_opcode_08() // BC1
{
	puts(">>>>> BC1");
}

static void mips_cp1_opcode_10() // CO1
{
	cp1_co_opcode_func[mips_curr_inst.f_type.funct]();
}

static void mips_illegal_cp1_opcode()
{
	printf("Illegal cp1 opcode %02x\n",(int)mips_curr_inst.c_type.funct);
	mips_do_exception(MIPS_EXCEPTION_RI,0);
}

static mips_opcode_func cp1_opcode_func[32] =
{
	&mips_cp1_opcode_00,
	&mips_cp1_opcode_01,
	&mips_cp1_opcode_02,
	&mips_illegal_cp1_opcode,
	&mips_cp1_opcode_04,
	&mips_cp1_opcode_05,
	&mips_cp1_opcode_06,
	&mips_illegal_cp1_opcode,
	&mips_cp1_opcode_08,
	&mips_illegal_cp1_opcode,
	&mips_illegal_cp1_opcode,
	&mips_illegal_cp1_opcode,
	&mips_illegal_cp1_opcode,
	&mips_illegal_cp1_opcode,
	&mips_illegal_cp1_opcode,
	&mips_illegal_cp1_opcode,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
	&mips_cp1_opcode_10,
};

static void mips_cp0_co_opcode_01()	// TLBR
{
	puts(">>>>> TLBR");
}

static void mips_cp0_co_opcode_02()	// TLBWI
{
	puts(">>>>> TLBWI");
}

static void mips_cp0_co_opcode_06()	// TLBWR
{
	puts(">>>>> TLBWR");
}

static void mips_cp0_co_opcode_08()	// TLBP
{
	puts(">>>>> TLBP");
}

static void mips_cp0_co_opcode_18()	// ERET
{
	if(cop_reg[0].cp0.sr.erl) 
	{
		cop_reg[0].cp0.sr.erl = 0;
		gpr_reg.pc = cop_reg[0].cp0.errorepc - 4;
	}
	else
	{
		cop_reg[0].cp0.sr.exl = 0;
		gpr_reg.pc = cop_reg[0].cp0.epc - 4;
	}
	gpr_reg.dl_ct=-1;
	gpr_reg.ll=false;
}

static void mips_illegal_cp0_co_opcode()
{
	printf("Illegal cp0 co opcode %02x\n",(int)mips_curr_inst.c_type.funct);
	mips_do_exception(MIPS_EXCEPTION_RI,0);
}

static mips_opcode_func cp0_co_opcode_func[64] =
{
	&mips_illegal_cp0_co_opcode,
	&mips_cp0_co_opcode_01,
	&mips_cp0_co_opcode_02,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_cp0_co_opcode_06,
	&mips_illegal_cp0_co_opcode,
	&mips_cp0_co_opcode_08,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
    &mips_cp0_co_opcode_18,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode,
	&mips_illegal_cp0_co_opcode
};

static void mips_cp0_opcode_00()	// MFC0
{
	gpr_reg.r[mips_curr_inst.c_type.rt] = (int64)(int32)cop_reg[0].cpx.r[mips_curr_inst.c_type.rd];
}

static void mips_cp0_opcode_01() // DMFC0
{
	gpr_reg.r[mips_curr_inst.c_type.rt] = cop_reg[0].cpx.r[mips_curr_inst.c_type.rd];
}

static void mips_cp0_opcode_02() // CFC0
{
	gpr_reg.r[mips_curr_inst.c_type.rt] = (int64)cop_reg[0].cpx.c[mips_curr_inst.c_type.rd];
}

static void mips_cp0_opcode_04() // MTC0
{
	// If write to cause register, keep ip0 and ip1 bits
	if(mips_curr_inst.c_type.rd == 13) {

		// only IP(0..1) are writeable
		cop_reg[0].cpx.r[mips_curr_inst.c_type.rd] = 
			(cop_reg[0].cpx.r[mips_curr_inst.c_type.rd] & (~0x300)) |
			(gpr_reg.r[mips_curr_inst.c_type.rt] & 0x300);
			
		// ToDo: Check CU field
		
		// ToDo: Check RE bit
		
	}
	else {
	 	// If write to compare register, clear IP7 in cause reg
		if(mips_curr_inst.c_type.rd == 11) {
			cop_reg[0].cp0.cause.ip &= ~ 0x80;
		}
		cop_reg[0].cpx.r[mips_curr_inst.c_type.rd] = gpr_reg.r[mips_curr_inst.c_type.rt];
	}
}

static void mips_cp0_opcode_05() // DMTC0
{
	mips_cp0_opcode_04(); // MTC0
}

static void mips_cp0_opcode_06() // CTC0
{
	cop_reg[0].cpx.c[mips_curr_inst.c_type.rd] = gpr_reg.r[mips_curr_inst.c_type.rt];
}

static void mips_cp0_opcode_08() // BC0
{
	puts(">>>>> BC0");
}

static void mips_cp0_opcode_10() // CO0
{
	cp0_co_opcode_func[mips_curr_inst.o_type.funct]();
}

static void mips_illegal_cp0_opcode()
{
	printf("Illegal cp0 opcode %02x\n",(int)mips_curr_inst.c_type.funct);
	mips_do_exception(MIPS_EXCEPTION_RI,0);
}

static mips_opcode_func cp0_opcode_func[32] =
{
	&mips_cp0_opcode_00,
	&mips_cp0_opcode_01,
	&mips_cp0_opcode_02,
	&mips_illegal_cp0_opcode,
	&mips_cp0_opcode_04,
	&mips_cp0_opcode_05,
	&mips_cp0_opcode_06,
	&mips_illegal_cp0_opcode,
	&mips_cp0_opcode_08,
	&mips_illegal_cp0_opcode,
	&mips_illegal_cp0_opcode,
	&mips_illegal_cp0_opcode,
	&mips_illegal_cp0_opcode,
	&mips_illegal_cp0_opcode,
	&mips_illegal_cp0_opcode,
	&mips_illegal_cp0_opcode,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
	&mips_cp0_opcode_10,
};

static void mips_special_opcode_00() // SLL
{
	if(mips_curr_inst.data != 0) {
		gpr_reg.r[mips_curr_inst.r_type.rd] =
			(uint64)(int32)((uint32)gpr_reg.r[mips_curr_inst.r_type.rt] << mips_curr_inst.r_type.sa);
	}
}

static void mips_special_opcode_02() // SRL
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(int64)(int32)((uint32)gpr_reg.r[mips_curr_inst.r_type.rt] >> mips_curr_inst.r_type.sa);
}

static void mips_special_opcode_03() // SRA
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(int64)((int32)gpr_reg.r[mips_curr_inst.r_type.rt] >> mips_curr_inst.r_type.sa);
}

static void mips_special_opcode_04() // SLLV
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(int64)(int32)((uint32)gpr_reg.r[mips_curr_inst.r_type.rt] << (gpr_reg.r[mips_curr_inst.r_type.rs]&0x1F));
}

static void mips_special_opcode_06() // SRLV
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(int64)(int32)((uint32)gpr_reg.r[mips_curr_inst.r_type.rt] >> (gpr_reg.r[mips_curr_inst.r_type.rs]&0x1F));
}

static void mips_special_opcode_07() // SRAV
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(int64)((int32)gpr_reg.r[mips_curr_inst.r_type.rt] >> (int32)(gpr_reg.r[mips_curr_inst.r_type.rs]&0x1F));
}

static void mips_special_opcode_08() // JR
{
	if(!(gpr_reg.r[mips_curr_inst.r_type.rs]&3)) {
		gpr_reg.dl_pc = (uint32)gpr_reg.r[mips_curr_inst.r_type.rs];
		gpr_reg.dl_ct = 1; 
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_ADEL,0);
	}
}

static void mips_special_opcode_09() // JALR
{
	if(!(gpr_reg.r[mips_curr_inst.r_type.rs]&3)) {
		gpr_reg.dl_pc = (uint32)gpr_reg.r[mips_curr_inst.r_type.rs];
		gpr_reg.dl_ct = 1; 
		gpr_reg.r[mips_curr_inst.r_type.rd] = gpr_reg.pc + 8;
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_ADEL,0);
	}
}

static void mips_special_opcode_0C() // SYSCALL
{
	mips_do_exception(MIPS_EXCEPTION_SYS,0);
}

static void mips_special_opcode_0D() // BREAK
{
	mips_do_exception(MIPS_EXCEPTION_BP,0);
}

static void mips_special_opcode_0F() // SYNC
{
	puts(">>>> SYNC");
}

static void mips_special_opcode_10() // MFHI
{
	gpr_reg.r[mips_curr_inst.r_type.rd] = gpr_reg.hi;
}

static void mips_special_opcode_11() // MTHI
{
	gpr_reg.hi = gpr_reg.r[mips_curr_inst.r_type.rs];
}

static void mips_special_opcode_12() // MFLO
{
	gpr_reg.r[mips_curr_inst.r_type.rd] = gpr_reg.lo;
}

static void mips_special_opcode_13() // MTLO
{
	gpr_reg.lo = gpr_reg.r[mips_curr_inst.r_type.rs];
}

static void mips_special_opcode_14() // DSLLV
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(uint64)gpr_reg.r[mips_curr_inst.r_type.rt] << (uint64)(gpr_reg.r[mips_curr_inst.r_type.rs]&0x1F);
}

static void mips_special_opcode_16() // DSRLV
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(uint64)gpr_reg.r[mips_curr_inst.r_type.rt] >> (uint64)(gpr_reg.r[mips_curr_inst.r_type.rs]&0x1F);
}

static void mips_special_opcode_17() // DSRAV
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(int64)gpr_reg.r[mips_curr_inst.r_type.rt] >> (int64)(gpr_reg.r[mips_curr_inst.r_type.rs]&0x1F);
}

static void mips_special_opcode_18() // MULT
{
	int64 r = ((int64)((int32)gpr_reg.r[mips_curr_inst.r_type.rs]) * (int64)((int32)gpr_reg.r[mips_curr_inst.r_type.rt]));
	gpr_reg.lo = (int64)(int32)(r);
	gpr_reg.hi = (int64)(int32)(r>>32);
}

static void mips_special_opcode_19() // MULTU
{
	uint64 r = ((uint64)((uint32)gpr_reg.r[mips_curr_inst.r_type.rs]) * (uint64)((uint32)gpr_reg.r[mips_curr_inst.r_type.rt]));
	gpr_reg.lo = (int64)(int32)(r);
	gpr_reg.hi = (int64)(int32)(r>>32);
}

static void mips_special_opcode_1A() // DIV
{
	if(((int32)gpr_reg.r[mips_curr_inst.r_type.rt])!=0) {
		gpr_reg.lo = (int64)(int32)((int32)gpr_reg.r[mips_curr_inst.r_type.rs] / (int32)gpr_reg.r[mips_curr_inst.r_type.rt]);
		gpr_reg.hi = (int64)(int32)((int32)gpr_reg.r[mips_curr_inst.r_type.rs] % (int32)gpr_reg.r[mips_curr_inst.r_type.rt]);
	}
}

static void mips_special_opcode_1B() // DIVU
{
	if(((uint32)gpr_reg.r[mips_curr_inst.r_type.rt])!=0) {
		gpr_reg.lo = (int64)(int32)((uint32)gpr_reg.r[mips_curr_inst.r_type.rs] / (uint32)gpr_reg.r[mips_curr_inst.r_type.rt]);
		gpr_reg.hi = (int64)(int32)((uint32)gpr_reg.r[mips_curr_inst.r_type.rs] % (uint32)gpr_reg.r[mips_curr_inst.r_type.rt]);
	}
}

static void mips_special_opcode_1C() // DMULT
{
	gpr_reg.lo = gpr_reg.r[mips_curr_inst.r_type.rs] * gpr_reg.r[mips_curr_inst.r_type.rt];
	gpr_reg.hi = 0;
}

static void mips_special_opcode_1D() // DMULTU
{
	gpr_reg.lo = (uint64)gpr_reg.r[mips_curr_inst.r_type.rs] * (uint64)gpr_reg.r[mips_curr_inst.r_type.rt];
	gpr_reg.hi = 0;
}

static void mips_special_opcode_1E() // DDIV
{
	if(((int64)gpr_reg.r[mips_curr_inst.r_type.rt])!=0) {
		gpr_reg.lo = gpr_reg.r[mips_curr_inst.r_type.rs] / gpr_reg.r[mips_curr_inst.r_type.rt];
		gpr_reg.hi = gpr_reg.r[mips_curr_inst.r_type.rs] % gpr_reg.r[mips_curr_inst.r_type.rt];
	}
}

static void mips_special_opcode_1F() // DDIVU
{
	if(((uint64)gpr_reg.r[mips_curr_inst.r_type.rt])!=0) {
		gpr_reg.lo = (uint64)gpr_reg.r[mips_curr_inst.r_type.rs] / (uint64)gpr_reg.r[mips_curr_inst.r_type.rt];
		gpr_reg.hi = (uint64)gpr_reg.r[mips_curr_inst.r_type.rs] % (uint64)gpr_reg.r[mips_curr_inst.r_type.rt];
	}
}

static void mips_special_opcode_20() // ADD
{
	int32 a = gpr_reg.r[mips_curr_inst.r_type.rs];
	int32 b = gpr_reg.r[mips_curr_inst.r_type.rt];
	int32 r = a+b;
	if((a>>31)^(b>>31)) {
		gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)r;
	}
	else {
		if((a>>31)^(r>>31)) {
			mips_do_exception(MIPS_EXCEPTION_OV, 0);
		}
		else {
			gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)r;
		}
	}
}

static void mips_special_opcode_21() // ADDU
{
	int64 a = gpr_reg.r[mips_curr_inst.r_type.rs];
	int64 b = gpr_reg.r[mips_curr_inst.r_type.rt];
	int64 r = a+b;
	gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)(int32)r;
}

static void mips_special_opcode_22() // SUB
{
	int32 a = gpr_reg.r[mips_curr_inst.r_type.rs];
	int32 b = gpr_reg.r[mips_curr_inst.r_type.rt];
	int32 r = a-b;
	if((a>>31)^(b>>31)) {
		gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)r;
	}
	else {
		if((a>>31)^(r>>31)) {
			mips_do_exception(MIPS_EXCEPTION_OV, 0);
		}
		else {
			gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)r;
		}
	}
}

static void mips_special_opcode_23() // SUBU
{
	int64 a = gpr_reg.r[mips_curr_inst.r_type.rs];
	int64 b = gpr_reg.r[mips_curr_inst.r_type.rt];
	int64 r = a-b;
	gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)(int32)r;
}

static void mips_special_opcode_24() // AND
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rs]&
		gpr_reg.r[mips_curr_inst.r_type.rt];
}

static void mips_special_opcode_25() // OR
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rs]|
		gpr_reg.r[mips_curr_inst.r_type.rt];
}

static void mips_special_opcode_26() // XOR
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rs]^
		gpr_reg.r[mips_curr_inst.r_type.rt];
}

static void mips_special_opcode_27() // NOR
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		~(gpr_reg.r[mips_curr_inst.r_type.rs]|
		  gpr_reg.r[mips_curr_inst.r_type.rt]);
}

static void mips_special_opcode_2A() // SLT
{
	if(gpr_reg.r[mips_curr_inst.r_type.rs] < gpr_reg.r[mips_curr_inst.r_type.rt]) {
		gpr_reg.r[mips_curr_inst.r_type.rd] = 1;
	}
	else {
		gpr_reg.r[mips_curr_inst.r_type.rd] = 0;
	}
}

static void mips_special_opcode_2B() // SLTU
{
	if((uint64)(gpr_reg.r[mips_curr_inst.r_type.rs]) < (uint64)(gpr_reg.r[mips_curr_inst.r_type.rt])) {
		gpr_reg.r[mips_curr_inst.r_type.rd] = 1;
	}
	else {
		gpr_reg.r[mips_curr_inst.r_type.rd] = 0;
	}
}

static void mips_special_opcode_2C() // DADD
{
	int64 a = (int64)gpr_reg.r[mips_curr_inst.r_type.rs];
	int64 b = (int64)gpr_reg.r[mips_curr_inst.r_type.rt];
	int64 r = a+b;
	
//	if((a>>63)^(b>>63)) {
		gpr_reg.r[mips_curr_inst.r_type.rd] = r;
//	}
//	else {
//		if((a>>63)^(r>>63)) {
//			mips_do_exception(MIPS_EXCEPTION_OV, 0);
//		}
//		else {
//			gpr_reg.r[mips_curr_inst.r_type.rd] = r;
//		}
//	}
}

static void mips_special_opcode_2D() // DADDU
{
	gpr_reg.r[mips_curr_inst.r_type.rd] = 
		gpr_reg.r[mips_curr_inst.r_type.rs] +
		gpr_reg.r[mips_curr_inst.r_type.rt];
}

static void mips_special_opcode_2E() // DSUB
{
	int64 a = (int64)gpr_reg.r[mips_curr_inst.r_type.rs];
	int64 b = (int64)gpr_reg.r[mips_curr_inst.r_type.rt];
	int64 r = a-b;
//	if((a>>63)^(b>>63)) {
		gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)r;
//	}
//	else {
//		if((a>>63)^(r>>63)) {
//			mips_do_exception(MIPS_EXCEPTION_OV, 0);
//		}
//		else {
//			gpr_reg.r[mips_curr_inst.r_type.rd] = (int64)r;
//		}
//	}
}

static void mips_special_opcode_2F() // DSUBU
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rs] -
		gpr_reg.r[mips_curr_inst.r_type.rt];
}

static void mips_special_opcode_30() // TGE
{
	if(gpr_reg.r[mips_curr_inst.r_type.rs] >= gpr_reg.r[mips_curr_inst.r_type.rt])	{
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_special_opcode_31() // TGEU
{
	if((uint64)gpr_reg.r[mips_curr_inst.r_type.rs] >= (uint64)gpr_reg.r[mips_curr_inst.r_type.rt])	{
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_special_opcode_32() // TLT
{
	if(gpr_reg.r[mips_curr_inst.r_type.rs] < gpr_reg.r[mips_curr_inst.r_type.rt])	{
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_special_opcode_33() // TLTU
{
	if((uint64)gpr_reg.r[mips_curr_inst.r_type.rs] < (uint64)gpr_reg.r[mips_curr_inst.r_type.rt])	{
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_special_opcode_34() // TEQ
{
	if(gpr_reg.r[mips_curr_inst.r_type.rs] == gpr_reg.r[mips_curr_inst.r_type.rt])	{
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_special_opcode_36() // TNE
{
	if(gpr_reg.r[mips_curr_inst.r_type.rs] != gpr_reg.r[mips_curr_inst.r_type.rt])	{
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_special_opcode_38() // DSLL
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rt] << mips_curr_inst.r_type.sa;
}

static void mips_special_opcode_3A() // DSRL
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(uint64)gpr_reg.r[mips_curr_inst.r_type.rt] >> (uint64)mips_curr_inst.r_type.sa;
}

static void mips_special_opcode_3B() // DSRA
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rt] >> (int64)(mips_curr_inst.r_type.sa);
}

static void mips_special_opcode_3C() // DSLL32
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rt] << (mips_curr_inst.r_type.sa+32);
}

static void mips_special_opcode_3E() // DSRL32
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		(uint64)gpr_reg.r[mips_curr_inst.r_type.rt] >> (uint64)(mips_curr_inst.r_type.sa+32);
}

static void mips_special_opcode_3F() // DSRA32
{
	gpr_reg.r[mips_curr_inst.r_type.rd] =
		gpr_reg.r[mips_curr_inst.r_type.rt] >> (int64)(mips_curr_inst.r_type.sa+32);
}

static void mips_illegal_special_opcode()
{
	printf("Illegal special opcode %02x\n",(int)mips_curr_inst.r_type.funct);
	mips_do_exception(MIPS_EXCEPTION_RI,0);
}

static mips_opcode_func special_opcode_func[64] =
{
	&mips_special_opcode_00,
	&mips_illegal_special_opcode,
	&mips_special_opcode_02,
	&mips_special_opcode_03,
	&mips_special_opcode_04,
	&mips_illegal_special_opcode,
	&mips_special_opcode_06,
	&mips_special_opcode_07,
	&mips_special_opcode_08,
	&mips_special_opcode_09,
	&mips_illegal_special_opcode,
	&mips_illegal_special_opcode,
	&mips_special_opcode_0C,
	&mips_special_opcode_0D,
	&mips_illegal_special_opcode,
	&mips_special_opcode_0F,
	&mips_special_opcode_10,
	&mips_special_opcode_11,
	&mips_special_opcode_12,
	&mips_special_opcode_13,
	&mips_special_opcode_14,
	&mips_illegal_special_opcode,
	&mips_special_opcode_16,
	&mips_special_opcode_17,
	&mips_special_opcode_18,
	&mips_special_opcode_19,
	&mips_special_opcode_1A,
	&mips_special_opcode_1B,
	&mips_special_opcode_1C,
	&mips_special_opcode_1D,
	&mips_special_opcode_1E,
	&mips_special_opcode_1F,
	&mips_special_opcode_20,
	&mips_special_opcode_21,
	&mips_special_opcode_22,
	&mips_special_opcode_23,
	&mips_special_opcode_24,
	&mips_special_opcode_25,
	&mips_special_opcode_26,
	&mips_special_opcode_27,
	&mips_illegal_special_opcode,
	&mips_illegal_special_opcode,
	&mips_special_opcode_2A,
	&mips_special_opcode_2B,
	&mips_special_opcode_2C,
	&mips_special_opcode_2D,
	&mips_special_opcode_2E,
	&mips_special_opcode_2F,
	&mips_special_opcode_30,
	&mips_special_opcode_31,
	&mips_special_opcode_32,
	&mips_special_opcode_33,
	&mips_special_opcode_34,
	&mips_illegal_special_opcode,
	&mips_special_opcode_36,
	&mips_illegal_special_opcode,
	&mips_special_opcode_38,
	&mips_illegal_special_opcode,
	&mips_special_opcode_3A,
	&mips_special_opcode_3B,
	&mips_special_opcode_3C,
	&mips_illegal_special_opcode,
	&mips_special_opcode_3E,
	&mips_special_opcode_3F
};

static void mips_regimm_opcode_00()	// BLTZ
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]<0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_regimm_opcode_01()	// BGEZ
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]>=0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_regimm_opcode_02()	// BLTZL
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]<0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_regimm_opcode_03()	// BGEZL
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]>=0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_regimm_opcode_08()	// TGEI
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]>=(int64)*((int16 *)&mips_curr_inst.i_type.im)) {
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_regimm_opcode_09()	// TGEIU
{
	if((uint64)gpr_reg.r[mips_curr_inst.g_type.rs]>=(uint64)(int64)*((int16 *)&mips_curr_inst.i_type.im)) {
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_regimm_opcode_0A()	// TLTI
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]<(int64)*((int16 *)&mips_curr_inst.i_type.im)) {
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_regimm_opcode_0B()	// TLTIU
{
	if((uint64)gpr_reg.r[mips_curr_inst.g_type.rs]<(uint64)(int64)*((int16 *)&mips_curr_inst.i_type.im)) {
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_regimm_opcode_0C()	// TEQI
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]==(int64)*((int16 *)&mips_curr_inst.i_type.im)) {
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_regimm_opcode_0E()	// TNEI
{
	if(gpr_reg.r[mips_curr_inst.g_type.rs]!=(int64)*((int16 *)&mips_curr_inst.i_type.im)) {
		mips_do_exception(MIPS_EXCEPTION_TR,0);
	}
}

static void mips_regimm_opcode_10()	// BLTZAL
{
	gpr_reg.r[31] = gpr_reg.pc + 8;
	if(gpr_reg.r[mips_curr_inst.g_type.rs]<0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_regimm_opcode_11()	// BGEZAL
{
	gpr_reg.r[31] = gpr_reg.pc + 8;
	if(gpr_reg.r[mips_curr_inst.g_type.rs]>=0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_regimm_opcode_12()	// BLTZALL
{
	gpr_reg.r[31] = gpr_reg.pc + 8;
	if(gpr_reg.r[mips_curr_inst.g_type.rs]<0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_regimm_opcode_13()	// BGEZALL
{
	gpr_reg.r[31] = gpr_reg.pc + 8;
	if(gpr_reg.r[mips_curr_inst.g_type.rs]>=0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_illegal_regimm_opcode()
{
	printf("Illegal regimm opcode %02x\n",(int)mips_curr_inst.g_type.funct);
	mips_do_exception(MIPS_EXCEPTION_RI,0);
}

static mips_opcode_func regimm_opcode_func[32] =
{
	&mips_regimm_opcode_00,
	&mips_regimm_opcode_01,
	&mips_regimm_opcode_02,
	&mips_regimm_opcode_03,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_regimm_opcode_08,
	&mips_regimm_opcode_09,
	&mips_regimm_opcode_0A,
	&mips_regimm_opcode_0B,
	&mips_regimm_opcode_0C,
	&mips_illegal_regimm_opcode,
	&mips_regimm_opcode_0E,
	&mips_illegal_regimm_opcode,
	&mips_regimm_opcode_10,
	&mips_regimm_opcode_11,
	&mips_regimm_opcode_12,
	&mips_regimm_opcode_13,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode,
	&mips_illegal_regimm_opcode
};

static void mips_opcode_00() // SPECIAL
{
	special_opcode_func[mips_curr_inst.r_type.funct]();
}

static void mips_opcode_01() // REGIMM
{	
	regimm_opcode_func[mips_curr_inst.g_type.funct]();
}

static void mips_opcode_02() // J
{	
	gpr_reg.dl_pc = (gpr_reg.pc & 0xF0000000) | (uint32)mips_curr_inst.j_type.target<<2;
	gpr_reg.dl_ct = 1;
}

static void mips_opcode_03() // JAL
{	
	gpr_reg.dl_pc = (gpr_reg.pc & 0xF0000000) | (uint32)mips_curr_inst.j_type.target<<2;
	gpr_reg.dl_ct = 1;
	gpr_reg.r[31] = gpr_reg.pc + 8;
}

static void mips_opcode_04() // BEQ
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]==gpr_reg.r[mips_curr_inst.i_type.rt]) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_opcode_05() // BNE
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]!=gpr_reg.r[mips_curr_inst.i_type.rt]) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_opcode_06() // BLEZ
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]<=0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_opcode_07() // BGTZ
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]>0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
}

static void mips_opcode_08() // ADDI
{
	int32 a = (int32)gpr_reg.r[mips_curr_inst.i_type.rs];
	int32 b = (int32)*((int16 *)&mips_curr_inst.i_type.im);
	int32 r = a+b;
	if((a>>31)^(b>>31)) {
		gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)r;
	}
	else {
		if((a>>31)^(r>>31)) {
			mips_do_exception(MIPS_EXCEPTION_OV, 0);
		}
		else {
			gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)r;
		}
	}
}

static void mips_opcode_09() // ADDIU
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(int64)((int32)(gpr_reg.r[mips_curr_inst.i_type.rs] +
		((int32)*((int16 *)&mips_curr_inst.i_type.im))));
}

static void mips_opcode_0A() // SLTI
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		((int64)gpr_reg.r[mips_curr_inst.i_type.rs] <
		(int64)*((int16 *)&mips_curr_inst.i_type.im)) ?
		1 : 0;
}

static void mips_opcode_0B() // SLTIU
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		((uint64)gpr_reg.r[mips_curr_inst.i_type.rs] <
		(uint64)*((int16 *)&mips_curr_inst.i_type.im)) ?
		1 : 0;
}

static void mips_opcode_0C() // ANDI
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		gpr_reg.r[mips_curr_inst.i_type.rs] &
		(uint16)mips_curr_inst.i_type.im;
}

static void mips_opcode_0D() // ORI
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		gpr_reg.r[mips_curr_inst.i_type.rs] |
		(uint16)mips_curr_inst.i_type.im;
}

static void mips_opcode_0E() // XORI
{
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		gpr_reg.r[mips_curr_inst.i_type.rs] ^
		(uint16)mips_curr_inst.i_type.im;
}

static void mips_opcode_0F() // LUI
{	
	gpr_reg.r[mips_curr_inst.i_type.rt]=(int64)((int32)(mips_curr_inst.i_type.im<<16));
}

static void mips_opcode_10() // COP0
{	
//	if(cop_reg[0].cp0.sr.cu&1) {
		cp0_opcode_func[mips_curr_inst.c_type.funct]();
//	}
//	else {
//		mips_do_exception(MIPS_EXCEPTION_CPU,0);
//	}
}

static void mips_opcode_11() // COP1
{	
//	if(cop_reg[0].cp0.sr.cu&2) {
		cp1_opcode_func[mips_curr_inst.c_type.funct]();
//	}
//	else {
//		mips_do_exception(MIPS_EXCEPTION_CPU,0);
//	}
}

static void mips_opcode_12() // COP2
{	
	if(cop_reg[0].cp0.sr.cu&4) {
		puts(">>>>> COP2");
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_CPU,0);
	}
}

static void mips_opcode_14() // BEQL
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]==gpr_reg.r[mips_curr_inst.i_type.rt]) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_opcode_15() // BNEL
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]!=gpr_reg.r[mips_curr_inst.i_type.rt]) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_opcode_16() // BLEZL
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]<=0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_opcode_17() // BGTZL
{	
	if(gpr_reg.r[mips_curr_inst.i_type.rs]>0) {
		gpr_reg.dl_pc = gpr_reg.pc + (int32)(*((int16 *)&mips_curr_inst.i_type.im)*4) + 4;
		gpr_reg.dl_ct = 1;
	}
	else {
		gpr_reg.pc += 4;
	}
}

static void mips_opcode_18() // DADDI
{	
	int64 a = gpr_reg.r[mips_curr_inst.i_type.rs];
	int64 b = (int64)*((int16 *)&mips_curr_inst.i_type.im);
	int64 r = a+b;
	
//	if((a>>63)^(b>>63)) {
		gpr_reg.r[mips_curr_inst.i_type.rt] = r;
//	}
//	else {
//		if((a>>63)^(r>>63)) {
//			mips_do_exception(MIPS_EXCEPTION_OV,0);
//		}
//		else {
//			gpr_reg.r[mips_curr_inst.i_type.rt] = r;
//		}
//	}
}

static void mips_opcode_19() // DADDIU
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		gpr_reg.r[mips_curr_inst.i_type.rs] +
		(int64)*((int16 *)&mips_curr_inst.i_type.im);
}

static void mips_opcode_1A() // LDL
{	
	uint32 addr = (uint32)(gpr_reg.r[mips_curr_inst.i_type.rs]+*((int16 *)&mips_curr_inst.i_type.im));
	switch(addr&7)
	{
		case	0:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)mips_read_uint64(addr);
				break;
		case	1:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 8));
				break;
		case	2:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 16));
				break;
		case	3:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 24));
				break;
		case	4:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 32));
				break;
		case	5:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 40));
				break;
		case	6:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFFFFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 48));
				break;
		case	7:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFFFFFFFLL) |
					(mips_read_uint64(addr&0xFFFFFFF8) << 56));
				break;
	}
}

static void mips_opcode_1B() // LDR
{	
	uint32 addr = (uint32)(gpr_reg.r[mips_curr_inst.i_type.rs]+*((int16 *)&mips_curr_inst.i_type.im));
	switch(addr&7)
	{
		case	0:
				break;
		case	1:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFF00000000000000LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 8));
				break;
		case	2:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFF000000000000LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 16));
				break;
		case	3:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFF0000000000LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 24));
				break;
		case	4:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFF00000000LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 32));
				break;
		case	5:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFFF000000LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 40));
				break;
		case	6:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFFFFF0000LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 48));
				break;
		case	7:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFFFFFFFFFF00LL) |
					(mips_read_uint64(addr&0xFFFFFFF8) >> 56));
				break;
	}
}

static void mips_opcode_20() // LB
{
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(int64)mips_read_int8(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_21() // LH
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(int64)mips_read_int16(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_22() // LWL
{	
	uint32 addr = (uint32)(gpr_reg.r[mips_curr_inst.i_type.rs]+*((int16 *)&mips_curr_inst.i_type.im));
	switch(addr&3)
	{
		case	0:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)mips_read_uint32(addr);
				break;
		case	1:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)
					(((uint32)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFF) |
					(mips_read_uint32(addr&0xFFFFFFFC) << 8));
				break;
		case	2:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)
					(((uint32)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFF) |
					(mips_read_uint32(addr&0xFFFFFFFC) << 16));
				break;
		case	3:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)
					(((uint64)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFF) |
					(mips_read_uint32(addr&0xFFFFFFFC) << 24));
				break;
	}
}

static void mips_opcode_23() // LW
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(int64)mips_read_int32(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_24() // LBU
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(uint64)mips_read_uint8(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_25() // LHU
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(uint64)mips_read_uint16(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_26() // LWR
{	
	uint32 addr = (uint32)(gpr_reg.r[mips_curr_inst.i_type.rs]+*((int16 *)&mips_curr_inst.i_type.im));
	switch(addr&3)
	{
		case	0:
				break;
		case	1:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)
					(((uint32)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFF000000) |
					(mips_read_uint32(addr&0xFFFFFFFC) >> 8));
				break;
		case	2:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)
					(((uint32)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFF0000) |
					(mips_read_uint32(addr&0xFFFFFFFC) >> 16));
				break;
		case	3:
				gpr_reg.r[mips_curr_inst.i_type.rt] = (int64)(int32)
					(((uint32)gpr_reg.r[mips_curr_inst.i_type.rt]&0xFFFFFF00) |
					(mips_read_uint32(addr&0xFFFFFFFC) >> 24));
				break;
	}
}

static void mips_opcode_27() // LWU
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		(uint64)mips_read_uint32(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_28() // SB
{
	mips_write_uint8(
		gpr_reg.r[mips_curr_inst.i_type.rs]+
		*((int16 *)&mips_curr_inst.i_type.im),
		gpr_reg.r[mips_curr_inst.i_type.rt]
	);
}

static void mips_opcode_29() // SH
{	
	mips_write_uint16(
		gpr_reg.r[mips_curr_inst.i_type.rs]+
		*((int16 *)&mips_curr_inst.i_type.im),
		gpr_reg.r[mips_curr_inst.i_type.rt]
	);
}

static void mips_opcode_2A() // SWL
{	
	puts(">>>>> SWL");
}

static void mips_opcode_2B() // SW
{
	mips_write_uint32(
		gpr_reg.r[mips_curr_inst.i_type.rs]+
		*((int16 *)&mips_curr_inst.i_type.im),
		gpr_reg.r[mips_curr_inst.i_type.rt]
	);
}

static void mips_opcode_2C() // SDL
{	
	puts(">>>>> SDL");
}

static void mips_opcode_2D() // SDR
{	
	puts(">>>>> SDR");
}

static void mips_opcode_2E() // SWR
{	
	puts(">>>>> SWR");
}

static void mips_opcode_2F() // CACHE
{
	puts(">>>>> CACHE");
}

static void mips_opcode_30() // LL
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		mips_read_int32(
			cop_reg[0].cp0.lladdr = (gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im))
		);
	gpr_reg.ll = true;
}

static void mips_opcode_31() // LWC1
{	
//	if(cop_reg[0].cp0.sr.cu&2) {
		cop_reg[1].cpx.r[mips_curr_inst.i_type.rt] = 
			(int64)mips_read_int32(
				gpr_reg.r[mips_curr_inst.i_type.rs]+
				*((int16 *)&mips_curr_inst.i_type.im)
			);
//	}
//	else {
//		mips_do_exception(MIPS_EXCEPTION_CPU,0);
//	}
}

static void mips_opcode_32() // LWC2
{	
	if(cop_reg[0].cp0.sr.cu&4) {
		cop_reg[2].cpx.r[mips_curr_inst.i_type.rt] = 
			(int64)mips_read_int32(
				gpr_reg.r[mips_curr_inst.i_type.rs]+
				*((int16 *)&mips_curr_inst.i_type.im)
			);
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_CPU,0);
	}
}

static void mips_opcode_34() // LLD
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		mips_read_int64(
			cop_reg[0].cp0.lladdr = (gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im))
		);
	gpr_reg.ll = true;
}

static void mips_opcode_35() // LDC1
{	
//	if(cop_reg[0].cp0.sr.cu&2) {
		uint64 value=mips_read_uint64(
			gpr_reg.r[mips_curr_inst.i_type.rs]+ *((int16 *)&mips_curr_inst.i_type.im));
	
		if(cop_reg[0].cp0.sr.fr) {
			cop_reg[1].cpx.r[mips_curr_inst.f_type.ft] = value;	
		}
		else {
			cop_reg[1].cpx.r[mips_curr_inst.f_type.ft+1] = (int64)(int32)(value>>32);
			cop_reg[1].cpx.r[mips_curr_inst.f_type.ft+0] = (int64)(int32)value;
		}
//	}
//	else {
//		mips_do_exception(MIPS_EXCEPTION_CPU,0);
//	}
}

static void mips_opcode_36() // LDC2
{	
	if(cop_reg[0].cp0.sr.cu&4) {
		cop_reg[2].cpx.r[mips_curr_inst.i_type.rt] = 
			mips_read_int64(
				gpr_reg.r[mips_curr_inst.i_type.rs]+
				*((int16 *)&mips_curr_inst.i_type.im)
			);
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_CPU,0);
	}
}

static void mips_opcode_37() // LD
{	
	gpr_reg.r[mips_curr_inst.i_type.rt] = 
		mips_read_int64(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im)
		);
}

static void mips_opcode_38() // SC
{	
	if(gpr_reg.ll) {
		mips_write_int32(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im),
			(int32)gpr_reg.r[mips_curr_inst.i_type.rt]
		);
		gpr_reg.r[mips_curr_inst.i_type.rt] = 1;
		gpr_reg.ll = false;
	}
	else {
		gpr_reg.r[mips_curr_inst.i_type.rt] = 0;
	}
	cop_reg[0].cp0.lladdr = 0;
}

static void mips_opcode_39() // SWC1
{	
//	if(cop_reg[0].cp0.sr.cu&2) {
		mips_write_uint32(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im),
			cop_reg[1].cpx.r[mips_curr_inst.i_type.rt]
		);
//	}
//	else {
//		mips_do_exception(MIPS_EXCEPTION_CPU,0);
//	}
}

static void mips_opcode_3A() // SWC2
{	
	if(cop_reg[0].cp0.sr.cu&4) {
		if(cop_reg[0].cp0.sr.fr) {
			mips_write_uint32(
				gpr_reg.r[mips_curr_inst.i_type.rs]+
				*((int16 *)&mips_curr_inst.i_type.im),
				cop_reg[2].cpx.r[mips_curr_inst.i_type.rt]
			);
		}
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_CPU,0);
	}
}

static void mips_opcode_3C() // SCD
{	
	if(gpr_reg.ll) {
		mips_write_int64(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im),
			gpr_reg.r[mips_curr_inst.i_type.rt]
		);
		gpr_reg.r[mips_curr_inst.i_type.rt] = 1;
		gpr_reg.ll = false;
	}
	else {
		gpr_reg.r[mips_curr_inst.i_type.rt] = 0;
	}
	cop_reg[0].cp0.lladdr = 0;
}

static void mips_opcode_3D() // SDC1
{
//	if(cop_reg[0].cp0.sr.cu&2) {
		uint64 value;
		if(cop_reg[0].cp0.sr.fr) {
			value = (cop_reg[1].cpx.r[mips_curr_inst.f_type.ft+0]);
		}
		else {
			value = (cop_reg[1].cpx.r[mips_curr_inst.f_type.ft+1]<<32) |
					(cop_reg[1].cpx.r[mips_curr_inst.f_type.ft+0]&0xFFFFFFFF);
		}
		mips_write_uint64(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im),
			value
		);
//	}
//	else {
//		mips_do_exception(MIPS_EXCEPTION_CPU,0);
//	}
}

static void mips_opcode_3E() // SDC2
{	
	if(cop_reg[0].cp0.sr.cu&4) {
		mips_write_uint64(
			gpr_reg.r[mips_curr_inst.i_type.rs]+
			*((int16 *)&mips_curr_inst.i_type.im),
			cop_reg[2].cpx.r[mips_curr_inst.i_type.rt]
		);
	}
	else {
		mips_do_exception(MIPS_EXCEPTION_CPU,0);
	}
}

static void mips_opcode_3F() // SD
{	
	mips_write_uint64(
		gpr_reg.r[mips_curr_inst.i_type.rs]+
		*((int16 *)&mips_curr_inst.i_type.im),
		gpr_reg.r[mips_curr_inst.i_type.rt]
	);
}

static void mips_illegal_opcode()
{
	printf("Illegal opcode %02x\n",(int)mips_curr_inst.i_type.op);
	mips_do_exception(MIPS_EXCEPTION_RI,0);
}

mips_opcode_func opcode_func[64] =
{
	&mips_opcode_00,
	&mips_opcode_01,
	&mips_opcode_02,
	&mips_opcode_03,
	&mips_opcode_04,
	&mips_opcode_05,
	&mips_opcode_06,
	&mips_opcode_07,
	&mips_opcode_08,
	&mips_opcode_09,
	&mips_opcode_0A,
	&mips_opcode_0B,
	&mips_opcode_0C,
	&mips_opcode_0D,
	&mips_opcode_0E,
	&mips_opcode_0F,
	&mips_opcode_10,
	&mips_opcode_11,
	&mips_opcode_12,
	&mips_illegal_opcode,
	&mips_opcode_14,
	&mips_opcode_15,
	&mips_opcode_16,
	&mips_opcode_17,
	&mips_opcode_18,
	&mips_opcode_19,
	&mips_opcode_1A,
	&mips_opcode_1B,
	&mips_illegal_opcode,
	&mips_illegal_opcode,
	&mips_illegal_opcode,
	&mips_illegal_opcode,
	&mips_opcode_20,
	&mips_opcode_21,
	&mips_opcode_22,
	&mips_opcode_23,
	&mips_opcode_24,
	&mips_opcode_25,
	&mips_opcode_26,
	&mips_opcode_27,
	&mips_opcode_28,
	&mips_opcode_29,
	&mips_opcode_2A,
	&mips_opcode_2B,
	&mips_opcode_2C,
	&mips_opcode_2D,
	&mips_opcode_2E,
	&mips_opcode_2F,
	&mips_opcode_30,
	&mips_opcode_31,
	&mips_opcode_32,
	&mips_illegal_opcode,
	&mips_opcode_34,
	&mips_opcode_35,
	&mips_opcode_36,
	&mips_opcode_37,
	&mips_opcode_38,
	&mips_opcode_39,
	&mips_opcode_3A,
	&mips_illegal_opcode,
	&mips_opcode_3C,
	&mips_opcode_3D,
	&mips_opcode_3E,
	&mips_opcode_3F
};
