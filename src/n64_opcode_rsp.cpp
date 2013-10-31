/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_opcode_rsp.h"
#include "n64_rsp.h"
#include "n64_memory.h"
#include "mips_memory.h"
#include "mips_exception.h"
#include <stdio.h>

mips_inst n64_curr_inst;

inline uint32 n64_read_rsp_uint32(uint32 addr)
{
	if(mips_host_little_endian())
		return swap_uint32(*((uint32 *)(n64_memory.spdmem+(addr&0xfff))));	
	else
		return *((uint32 *)(n64_memory.spdmem+(addr&0xfff)));	
}

inline uint16 n64_read_rsp_uint16(uint32 addr)
{
	if(mips_host_little_endian())
		return swap_uint16(*((uint16 *)(n64_memory.spdmem+(addr&0xfff))));	
	else
		return *((uint16 *)(n64_memory.spdmem+(addr&0xfff)));	
}

inline uint8 n64_read_rsp_uint8(uint32 addr)
{
	return *((uint8 *)(n64_memory.spdmem+(addr&0xfff)));	
}

inline int32 n64_read_rsp_int32(uint32 addr)
{
	return (int32)n64_read_rsp_uint32(addr);
}

inline int16 n64_read_rsp_int16(uint32 addr)
{
	return (int16)n64_read_rsp_uint32(addr);
}

inline int8 n64_read_rsp_int8(uint32 addr)
{
	return (int8)n64_read_rsp_uint32(addr);
}

inline void n64_write_rsp_uint32(uint32 addr, uint32 value)
{
	if(mips_host_little_endian())
		*((uint32 *)(n64_memory.spdmem+(addr&0xfff)))=swap_uint32(value);	
	else
		*((uint32 *)(n64_memory.spdmem+(addr&0xfff)))=value;	
}

inline void n64_write_rsp_uint16(uint32 addr, uint16 value)
{
	if(mips_host_little_endian())
		*((uint16 *)(n64_memory.spdmem+(addr&0xfff)))=swap_uint16(value);	
	else
		*((uint16 *)(n64_memory.spdmem+(addr&0xfff)))=value;	
}

inline void n64_write_rsp_uint8(uint32 addr, uint8 value)
{
	*((uint8 *)(n64_memory.spdmem+(addr&0xfff)))=value;	
}

inline void n64_write_rsp_int32(uint32 addr, int32 value)
{
	n64_write_rsp_uint32(addr,value);
}

inline void n64_write_rsp_int32(uint32 addr, int16 value)
{
	n64_write_rsp_uint16(addr,value);
}

inline void n64_write_rsp_int32(uint32 addr, int8 value)
{
	n64_write_rsp_uint8(addr,value);
}

static void n64_cp0_opcode_rsp_00()	// MFC0
{
	if(n64_curr_inst.c_type.rd < 8) {
		rsp_reg.r[n64_curr_inst.c_type.rt]=mips_read_int32(0x04040000|(n64_curr_inst.c_type.rd<<2));
	}
	else {
		rsp_reg.r[n64_curr_inst.c_type.rt]=mips_read_int32(0x04040000|((n64_curr_inst.c_type.rd&(~0x08))<<2));
	}
}

static void n64_cp0_opcode_rsp_04() // MTC0
{
	if(n64_curr_inst.c_type.rd < 8) {
		mips_write_int32(0x04040000|(n64_curr_inst.c_type.rd<<2),rsp_reg.r[n64_curr_inst.c_type.rt]);
	}
	else {
		mips_write_int32(0x04040000|((n64_curr_inst.c_type.rd&(~0x08))<<2),rsp_reg.r[n64_curr_inst.c_type.rt]);
	}
}

static void n64_illegal_cp0_opcode_rsp()
{
	printf("Illegal cp0 rsp opcode %02x\n",(int)n64_curr_inst.c_type.funct);
}

static mips_opcode_func cp0_opcode_rsp_func[32] =
{
	&n64_cp0_opcode_rsp_00,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_cp0_opcode_rsp_04,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
	&n64_illegal_cp0_opcode_rsp,
};

static void n64_special_opcode_rsp_00() // SLL
{
	if(n64_curr_inst.data != 0) {
		rsp_reg.r[n64_curr_inst.r_type.rd] =
			(int32)((uint32)rsp_reg.r[n64_curr_inst.r_type.rt] << n64_curr_inst.r_type.sa);
	}
}

static void n64_special_opcode_rsp_02() // SRL
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		(int32)((uint32)rsp_reg.r[n64_curr_inst.r_type.rt] >> n64_curr_inst.r_type.sa);
}

static void n64_special_opcode_rsp_03() // SRA
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		((int32)rsp_reg.r[n64_curr_inst.r_type.rt] >> n64_curr_inst.r_type.sa);
}

static void n64_special_opcode_rsp_04() // SLLV
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		(int32)((uint32)rsp_reg.r[n64_curr_inst.r_type.rt] << (uint32)(rsp_reg.r[n64_curr_inst.r_type.rs]&0x1F));
}

static void n64_special_opcode_rsp_06() // SRLV
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		(int32)((uint32)rsp_reg.r[n64_curr_inst.r_type.rt] >> (uint32)(rsp_reg.r[n64_curr_inst.r_type.rs]&0x1F));
}

static void n64_special_opcode_rsp_07() // SRAV
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		((int32)rsp_reg.r[n64_curr_inst.r_type.rt] >> (int32)(rsp_reg.r[n64_curr_inst.r_type.rs]&0x1F));
}

static void n64_special_opcode_rsp_08() // JR
{
	rsp_reg.dl_pc = 0x04000000 | (rsp_reg.r[n64_curr_inst.r_type.rs] & 0x1fff);
	rsp_reg.dl_ct = 1; 
}

static void n64_special_opcode_rsp_09() // JALR
{
	rsp_reg.dl_pc = 0x04000000 | (rsp_reg.r[n64_curr_inst.r_type.rs] & 0x1fff);
	rsp_reg.dl_ct = 1; 
	rsp_reg.r[n64_curr_inst.r_type.rd] = rsp_reg.pc + 8;
}

static void n64_special_opcode_rsp_0D() // BREAK
{
	puts(">>>> BREAK");
//	DO SP REG interrupt
//	mips_do_exception(MIPS_EXCEPTION_BP,0);
}

static void n64_special_opcode_rsp_20() // ADD
{
	rsp_reg.r[n64_curr_inst.r_type.rd] = 
		rsp_reg.r[n64_curr_inst.r_type.rs] + rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_21() // ADDU
{
	rsp_reg.r[n64_curr_inst.r_type.rd] = 
		rsp_reg.r[n64_curr_inst.r_type.rs] + rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_22() // SUB
{
	rsp_reg.r[n64_curr_inst.r_type.rd] = 
		rsp_reg.r[n64_curr_inst.r_type.rs] - rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_23() // SUBU
{
	rsp_reg.r[n64_curr_inst.r_type.rd] = 
		rsp_reg.r[n64_curr_inst.r_type.rs] - rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_24() // AND
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		rsp_reg.r[n64_curr_inst.r_type.rs] & rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_25() // OR
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		rsp_reg.r[n64_curr_inst.r_type.rs] | rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_26() // XOR
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		rsp_reg.r[n64_curr_inst.r_type.rs] ^ rsp_reg.r[n64_curr_inst.r_type.rt];
}

static void n64_special_opcode_rsp_27() // NOR
{
	rsp_reg.r[n64_curr_inst.r_type.rd] =
		~(rsp_reg.r[n64_curr_inst.r_type.rs] | rsp_reg.r[n64_curr_inst.r_type.rt]);
}

static void n64_special_opcode_rsp_2A() // SLT
{
	if(rsp_reg.r[n64_curr_inst.r_type.rs] < rsp_reg.r[n64_curr_inst.r_type.rt]) {
		rsp_reg.r[n64_curr_inst.r_type.rd] = 1;
	}
	else {
		rsp_reg.r[n64_curr_inst.r_type.rd] = 0;
	}
}

static void n64_special_opcode_rsp_2B() // SLTU
{
	if((uint32)(rsp_reg.r[n64_curr_inst.r_type.rs]) < (uint32)(rsp_reg.r[n64_curr_inst.r_type.rt])) {
		rsp_reg.r[n64_curr_inst.r_type.rd] = 1;
	}
	else {
		rsp_reg.r[n64_curr_inst.r_type.rd] = 0;
	}
}

static void n64_illegal_special_opcode_rsp()
{
	printf("Illegal special rsp opcode %02x\n",(int)n64_curr_inst.r_type.funct);
}

static mips_opcode_func special_opcode_rsp_func[64] =
{
	&n64_special_opcode_rsp_00,
	&n64_illegal_special_opcode_rsp,
	&n64_special_opcode_rsp_02,
	&n64_special_opcode_rsp_03,
	&n64_special_opcode_rsp_04,
	&n64_illegal_special_opcode_rsp,
	&n64_special_opcode_rsp_06,
	&n64_special_opcode_rsp_07,
	&n64_special_opcode_rsp_08,
	&n64_special_opcode_rsp_09,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_special_opcode_rsp_0D,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_special_opcode_rsp_20,
	&n64_special_opcode_rsp_21,
	&n64_special_opcode_rsp_22,
	&n64_special_opcode_rsp_23,
	&n64_special_opcode_rsp_24,
	&n64_special_opcode_rsp_25,
	&n64_special_opcode_rsp_26,
	&n64_special_opcode_rsp_27,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_special_opcode_rsp_2A,
	&n64_special_opcode_rsp_2B,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp,
	&n64_illegal_special_opcode_rsp
};

static void n64_regimm_opcode_rsp_00()	// BLTZ
{
	if(rsp_reg.r[n64_curr_inst.g_type.rs]<0) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_regimm_opcode_rsp_01()	// BGEZ
{
	if(rsp_reg.r[n64_curr_inst.g_type.rs]>=0) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_regimm_opcode_rsp_10()	// BLTZAL
{
	rsp_reg.r[31] = rsp_reg.pc + 8;
	if(rsp_reg.r[n64_curr_inst.g_type.rs]<0) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_regimm_opcode_rsp_11()	// BGEZAL
{
	rsp_reg.r[31] = rsp_reg.pc + 8;
	if(rsp_reg.r[n64_curr_inst.g_type.rs]>=0) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_illegal_regimm_opcode_rsp()
{
	printf("Illegal regimm rsp opcode %02x\n",(int)n64_curr_inst.g_type.funct);
}

static mips_opcode_func regimm_opcode_rsp_func[32] =
{
	&n64_regimm_opcode_rsp_00,
	&n64_regimm_opcode_rsp_01,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_regimm_opcode_rsp_10,
	&n64_regimm_opcode_rsp_11,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp,
	&n64_illegal_regimm_opcode_rsp
};

static void n64_opcode_rsp_00() // SPECIAL
{
	special_opcode_rsp_func[n64_curr_inst.r_type.funct]();
}

static void n64_opcode_rsp_01() // REGIMM
{	
	regimm_opcode_rsp_func[n64_curr_inst.g_type.funct]();
}

static void n64_opcode_rsp_02() // J
{	
	rsp_reg.dl_pc = n64_curr_inst.j_type.target << 2;
	rsp_reg.dl_ct = 1;
}

static void n64_opcode_rsp_03() // JAL
{	
	rsp_reg.dl_pc = n64_curr_inst.j_type.target << 2;
	rsp_reg.dl_ct = 1;
	rsp_reg.r[31] = (rsp_reg.pc + 8) & 0xffff;
}

static void n64_opcode_rsp_04() // BEQ
{	
	if(rsp_reg.r[n64_curr_inst.i_type.rs]==rsp_reg.r[n64_curr_inst.i_type.rt]) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_opcode_rsp_05() // BNE
{	
	if(rsp_reg.r[n64_curr_inst.i_type.rs]!=rsp_reg.r[n64_curr_inst.i_type.rt]) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_opcode_rsp_06() // BLEZ
{	
	if(rsp_reg.r[n64_curr_inst.i_type.rs]<=0) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_opcode_rsp_07() // BGTZ
{	
	if(rsp_reg.r[n64_curr_inst.i_type.rs]>0) {
		rsp_reg.dl_pc = rsp_reg.pc + (int32)(*((int16 *)&n64_curr_inst.i_type.im)*4) + 4;
		rsp_reg.dl_ct = 1;
	}
}

static void n64_opcode_rsp_08() // ADDI
{
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		rsp_reg.r[n64_curr_inst.i_type.rs] + (int32)*((int16 *)&n64_curr_inst.i_type.im);
}

static void n64_opcode_rsp_09() // ADDIU
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		rsp_reg.r[n64_curr_inst.i_type.rs] + (int32)*((int16 *)&n64_curr_inst.i_type.im);
}

static void n64_opcode_rsp_0A() // SLTI
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		(rsp_reg.r[n64_curr_inst.i_type.rs] < (int32)*((int16 *)&n64_curr_inst.i_type.im)) ? 1 : 0;
}

static void n64_opcode_rsp_0B() // SLTIU
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		((uint32)rsp_reg.r[n64_curr_inst.i_type.rs] < (uint32)*((int16 *)&n64_curr_inst.i_type.im)) ? 1 : 0;
}

static void n64_opcode_rsp_0C() // ANDI
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		rsp_reg.r[n64_curr_inst.i_type.rs] & (uint16)n64_curr_inst.i_type.im;
}

static void n64_opcode_rsp_0D() // ORI
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		rsp_reg.r[n64_curr_inst.i_type.rs] | (uint16)n64_curr_inst.i_type.im;
}

static void n64_opcode_rsp_0E() // XORI
{
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		rsp_reg.r[n64_curr_inst.i_type.rs] ^ (uint16)n64_curr_inst.i_type.im;
}

static void n64_opcode_rsp_0F() // LUI
{	
	rsp_reg.r[n64_curr_inst.i_type.rt]=(int32)(n64_curr_inst.i_type.im<<16);
}

static void n64_opcode_rsp_10() // COP0
{	
	cp0_opcode_rsp_func[n64_curr_inst.c_type.funct]();
}

static void n64_opcode_rsp_12() // COP2
{	
	puts(">>>>> COP2");
}

static void n64_opcode_rsp_20() // LB
{
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		(int32)n64_read_rsp_int8( 
			rsp_reg.r[n64_curr_inst.i_type.rs]+ 
			*((int16 *)&n64_curr_inst.i_type.im)
		);
}

static void n64_opcode_rsp_21() // LH
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		(int32)n64_read_rsp_int16(
			rsp_reg.r[n64_curr_inst.i_type.rs]+
			*((int16 *)&n64_curr_inst.i_type.im)
		);
}

static void n64_opcode_rsp_23() // LW
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		(int32)n64_read_rsp_int32(
			rsp_reg.r[n64_curr_inst.i_type.rs]+
			*((int16 *)&n64_curr_inst.i_type.im)
		);
}

static void n64_opcode_rsp_24() // LBU
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		(uint32)n64_read_rsp_uint8(
			rsp_reg.r[n64_curr_inst.i_type.rs]+
			*((int16 *)&n64_curr_inst.i_type.im)
		);
}

static void n64_opcode_rsp_25() // LHU
{	
	rsp_reg.r[n64_curr_inst.i_type.rt] = 
		(uint32)n64_read_rsp_uint16(
			rsp_reg.r[n64_curr_inst.i_type.rs]+
			*((int16 *)&n64_curr_inst.i_type.im)
		);
}

static void n64_opcode_rsp_28() // SB
{
	n64_write_rsp_uint8(
		rsp_reg.r[n64_curr_inst.i_type.rs]+
		*((int16 *)&n64_curr_inst.i_type.im),
		rsp_reg.r[n64_curr_inst.i_type.rt]
	);
}

static void n64_opcode_rsp_29() // SH
{	
	n64_write_rsp_uint16(
		rsp_reg.r[n64_curr_inst.i_type.rs]+
		*((int16 *)&n64_curr_inst.i_type.im),
		rsp_reg.r[n64_curr_inst.i_type.rt]
	);
}

static void n64_opcode_rsp_2B() // SW
{
	n64_write_rsp_uint32(
		rsp_reg.r[n64_curr_inst.i_type.rs]+
		*((int16 *)&n64_curr_inst.i_type.im),
		rsp_reg.r[n64_curr_inst.i_type.rt]
	);
}

static void n64_opcode_rsp_32() // LWC2
{	
	puts(">>> RSP LWC2");
}

static void n64_opcode_rsp_3A() // SWC2
{	
	puts(">>> RSP SWC2");
}

static void n64_illegal_opcode()
{
	printf("Illegal rsp opcode %02x\n",(int)n64_curr_inst.i_type.op);
}

mips_opcode_func opcode_rsp_func[64] =
{
	&n64_opcode_rsp_00,
	&n64_opcode_rsp_01,
	&n64_opcode_rsp_02,
	&n64_opcode_rsp_03,
	&n64_opcode_rsp_04,
	&n64_opcode_rsp_05,
	&n64_opcode_rsp_06,
	&n64_opcode_rsp_07,
	&n64_opcode_rsp_08,
	&n64_opcode_rsp_09,
	&n64_opcode_rsp_0A,
	&n64_opcode_rsp_0B,
	&n64_opcode_rsp_0C,
	&n64_opcode_rsp_0D,
	&n64_opcode_rsp_0E,
	&n64_opcode_rsp_0F,
	&n64_opcode_rsp_10,
	&n64_illegal_opcode,
	&n64_opcode_rsp_12,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_opcode_rsp_20,
	&n64_opcode_rsp_21,
	&n64_illegal_opcode,
	&n64_opcode_rsp_23,
	&n64_opcode_rsp_24,
	&n64_opcode_rsp_25,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_opcode_rsp_28,
	&n64_opcode_rsp_29,
	&n64_illegal_opcode,
	&n64_opcode_rsp_2B,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_opcode_rsp_32,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_opcode_rsp_3A,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode,
	&n64_illegal_opcode
};
