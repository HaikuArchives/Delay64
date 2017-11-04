/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_debugger.h"
#include "mips_opcode.h"
#include "mips_memory.h"
#include "mips_cop.h"
#include "mips_gpr.h"
#include "n64_screen.h"
#include "n64_rsp.h"
#include <stdio.h>
#include <string.h>
#include <Application.h>
#include <InterfaceKit.h>

// Don't look at this code, its simply horribly done. The disassembler
// part is taken from the nop disassembler

class MipsDebuggerView;

class MipsDebugger : public BWindow
{
	public:
				MipsDebugger();

		void	MipsShow();
		void	MipsStep();

		bool	QuitRequested();

	private:

		MipsDebuggerView *view;
};

typedef enum { /* All the opcodes in the world - sort of */
  NONE,
  HELP,     /* Eh-oh! */
  SPECIAL,
  REGIMM,
  VECTOR,
  FPU,
  COP0,
  COP1,
  COP2,     /* Below: Try to work out my system! */
  OP,       /* Format: Opcode */
  OPTGT,    /* Format: Opcode Target */
  OPC,      /* Format: Opcode Code (System Call, Break, ?Sync?) */
  OPS,      /* Format: Opcode $RS */
  OPSD,     /* Format: Opcode $RS $RD */
  OPST,     /* Format: Opcode $RS $RT */
  OPSI,     /* Format: Opcode $RS Immediate */
  OPSO,     /* Format: Opcode $RS Offset */
  OPSTO,    /* Format: Opcode $RS $RT Offset */
  OPTI,     /* Format: Opcode $RT Immediate */
  OPTSI,    /* Format: Opcode $RT $RS Immediate */
  OPTOB,    /* Format: Opcode $RT Offset(base) */
  OPTD,     /* Format: Opcode $RT $RD */
  OPD,      /* Format: Opcode $RD */
  OPDST,    /* Format: Opcode $RD $RS $RT */
  OPDTA,    /* Format: Opcode $RD $RT ShiftAmount */
  OPDTS,    /* Format: Opcode $RD $RT $RS */
  OPTSD,    /* Format: Opcode $RT $SD (SD = System Reg) */
  OPTSS,    /* Format: Opcode $RT $SS (SS = System Reg) */
  OPTFD,    /* Format: Opcode $RT $FD (FD = FPU Reg) */
  OPTFS,    /* Format: Opcode $RT $FS (FS = FPU Reg) */
  OPFTOB,   /* Format: Opcode $FT Offset(base) (FT = FPU Reg) */
  OPRTOB,   /* Format: Opcode $RT Offset(base) (RT = RCP Reg) */
  OPFSFT,   /* Format: Opcode $FS $FT */
  OPFDFS,   /* Format: Opcode $FD $FS */
  OPFDFSFT, /* Format: Opcode $FD $FS $FT */
  OPTRD,    /* Format: Opcode $RT $VD (VD = Vector Reg) */
  OPTRS,    /* Format: Opcode $RT $VS (VS = Vector Reg) */
  COPBC,    /* Coprocessor Branch - see mipsOp.bc */
  TLB       /* Translation Lookup Buffer? Lookaside? Something like that */
} optype;

typedef enum {
  NOTHING,
  BRANCH,
  JUMP,
  LABEL
} whatisit;

typedef union
{
  struct
  {
    unsigned func:6;
    unsigned sa:5;
    unsigned rd:5;
    unsigned rt:5;
    unsigned rs:5;
    unsigned op:6;
  } f;
  struct
  {
    unsigned func:6;
    unsigned code:20;
    unsigned op:6;
  } c;
  struct
  {
    unsigned imm:16;
    unsigned rt:5;
    unsigned rs:5;
    unsigned op:6;
  } i;
  struct
  {
    unsigned target:26;
    unsigned op:6;
  } t;
  struct
  {
    unsigned imm:16;
    unsigned tf:1;
    unsigned nd:1;
    unsigned pad:3;
    unsigned rs:5;
    unsigned op:6;
  } bc;
  struct
  {
    unsigned pad:6;
    unsigned dest:5;
    unsigned s1:5;
    unsigned s2:5;
    unsigned el:4;
    unsigned one:1;
    unsigned op:6;
  } v;
  unsigned int data;
  struct
  {
    unsigned b1:8;
    unsigned b2:8;
    unsigned b3:8;
    unsigned b4:8;
  } byte;
  struct
  {
    unsigned w1:16;
    unsigned w2:16;
  } word;
} mipsOp;

typedef struct {
  mipsOp op;
  unsigned int info;
  whatisit what;
} Instr;

typedef struct {
  unsigned int addr;
  unsigned int size;
} Function;


static const char *GPR[32] = {
  "$ze",   "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
  "$t0",   "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
  "$s0",   "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
  "$t8",   "$t9", "$k0", "$k1", "$gp", "$sp", "$s8", "$ra"
};

static const char *SYSR[32] = { /* Cop0 Regs - System Regs*/
  "$Index", "$Random", "$EntryLo0", "$EntryLo1", "$Context", "$PageMask", "$Wired", "$C0-07",
  "$BadVAddr", "$Count", "$EntryHi", "$Compare", "$Status", "$Cause", "$EPC", "$PRevID",
  "$Config", "$LLAddr", "$WatchLo", "$WatchHi", "$XContext", "$C0-21", "$C0-22", "$C0-23",
  "$C0-24", "$C0-25", "$PErr", "$CacheErr", "$TagLo", "$TagHi", "$ErrorEPC", "$C0-31"
};

static const char *mainOps[64] = {
  "special", "regimm ", "j      ", "jal    ", "beq    ", "bne    ", "blez   ", "bgtz   ",
  "addi   ", "addiu  ", "slti   ", "sltiu  ", "andi   ", "ori    ", "xori   ", "lui    ",
  "cop0   ", "cop1   ", "cop2   ", "       ", "beql   ", "bnel   ", "blezl  ", "bgtzl  ",
  "daddi  ", "daddiu ", "ldl    ", "ldr    ", "       ", "       ", "       ", "       ",
  "lb     ", "lh     ", "lwl    ", "lw     ", "lbu    ", "lhu    ", "lwr    ", "lwu    ",
  "sb     ", "sh     ", "swl    ", "sw     ", "sdl    ", "sdr    ", "swr    ", "cache  ",
  "ll     ", "lwc1   ", "lwc2   ", "       ", "lld    ", "ldc1   ", "ldc2   ", "ld     ",
  "sc     ", "swc1   ", "swc2   ", "       ", "scd    ", "sdc1   ", "sdc2   ", "sd     "
};

static const optype mainOpsType[64] = {
  SPECIAL, REGIMM, OPTGT,  OPTGT, OPSTO, OPSTO,  OPSO,   OPSO,
  OPTSI,   OPTSI,  OPTSI,  OPTSI, OPTSI, OPTSI,  OPTI,   OPTI,
  COP0,    COP1,   COP2,   NONE,  OPSTO, OPSTO,  OPSO,   OPSO,
  OPTSI,   OPTSI,  OPTOB,  OPTOB, NONE,  NONE,   NONE,   NONE,
  OPTOB,   OPTOB,  OPTOB,  OPTOB, OPTOB, OPTOB,  OPTOB,  OPTOB,
  OPTOB,   OPTOB,  OPTOB,  OPTOB, OPTOB, OPTOB,  OPTOB,  OPTOB,
  OPTOB,   OPFTOB, OPRTOB, NONE,  OPTOB, OPFTOB, OPRTOB, OPTOB,
  OPTOB,   OPFTOB, OPRTOB, NONE,  OPTOB, OPFTOB, OPRTOB, OPTOB
};

static const char *specialOps[64] = {
  "sll    ", "       ", "srl    ", "sra    ", "sllv   ", "       ", "srlv   ", "srav   ",
  "jr     ", "jalr   ", "       ", "       ", "syscall", "break  ", "       ", "sync   ",
  "mfhi   ", "mthi   ", "mflo   ", "mtlo   ", "dsllv  ", "       ", "dsrlv  ", "dsrav  ",
  "mult   ", "multu  ", "div    ", "divu   ", "dmult  ", "dmultu ", "ddiv   ", "ddivu  ",
  "add    ", "addu   ", "sub    ", "subu   ", "and    ", "or     ", "xor    ", "nor    ",
  "       ", "       ", "slt    ", "sltu   ", "dadd   ", "daddu  ", "dsub   ", "dsubu  ",
  "tge    ", "tgeu   ", "tlt    ", "tltu   ", "teq    ", "       ", "tne    ", "       ",
  "dsll   ", "       ", "dsrl   ", "dsra   ", "dsll32 ", "       ", "dsrl32 ", "dsra32 "
};

static const optype specialOpsType[64] = {
  OPDTA, NONE,  OPDTA, OPDTA, OPDTS, NONE,  OPDTS, OPDTS,
  OPS,   OPSD,  NONE,  NONE,  OPC,   OPC,   NONE,  OP,
  OPD,   OPD,   OPD,   OPD,   OPDTS, NONE,  OPDTS, OPDTS,
  OPST,  OPST,  OPST,  OPST,  OPST,  OPST,  OPST,  OPST,
  OPDST, OPDST, OPDST, OPDST, OPDST, OPDST, OPDST, OPDST,
  NONE,  NONE,  OPDST, OPDST, OPDST, OPDST, OPDST, OPDST,
  OPST,  OPST,  OPST,  OPST,  OPST,  NONE,  OPST,  NONE,
  OPDTA, NONE,  OPDTA, OPDTA, OPDTA, NONE,  OPDTA, OPDTA
};

static const char *regimmOps[32] = {
  "bltz   ", "bgez   ", "bltzl  ", "bgezl  ", "       ", "       ", "       ", "       ",
  "tgei   ", "tgeiu  ", "tlti   ", "tltiu  ", "teqi   ", "       ", "tnei   ", "       ",
  "bltzal ", "bgezal ", "bltzall", "bgezall", "       ", "       ", "       ", "       ",
  "       ", "       ", "       ", "       ", "       ", "       ", "       ", "       "
};

static const optype regimmOpsType[32] = {
  OPSO, OPSO, OPSO, OPSO, NONE, NONE, NONE, NONE,
  OPSI, OPSI, OPSI, OPSI, OPSI, NONE, OPSI, NONE,
  OPSO, OPSO, OPSO, OPSO, NONE, NONE, NONE, NONE,
  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};

static const char *cop0Ops[32] = {
  "mfc0   ", "dmfc0  ", "cfc0   ", "       ", "mtc0   ", "dmtc0  ", "ctc0   ", "       ",
  "bc0    ", "       ", "       ", "       ", "       ", "       ", "       ", "       ",
  "tlb    ", "       ", "       ", "       ", "       ", "       ", "       ", "       ",
  "       ", "       ", "       ", "       ", "       ", "       ", "       ", "       "
};

static const optype cop0OpsType[32] = {
  OPTSD, OPTSD, OPTSS, NONE, OPTSD, OPTSD, OPTSS, NONE,
  COPBC, NONE,  NONE,  NONE, NONE,  NONE,  NONE,  NONE,
  TLB,   NONE,  NONE,  NONE, NONE,  NONE,  NONE,  NONE,
  NONE,  NONE,  NONE,  NONE, NONE,  NONE,  NONE,  NONE
};

static const char *cop1Ops[32] = {
  "mfc1", "dmfc1", "cfc1", "", "mtc1", "dmtc1", "ctc1", "",
  "bc1", "", "", "", "", "", "", "",
  ".s", ".d", "", "", ".w", ".l", "", "",
  "", "", "", "", "", "", "", ""
};

static const optype cop1OpsType[32] = {
  OPTFD, OPTFD, OPTFS, NONE, OPTFD, OPTFD, OPTFS, NONE,
  COPBC, NONE,  NONE,  NONE, NONE,  NONE,  NONE,  NONE,
  FPU,   FPU,   NONE,  NONE, FPU,   FPU,   NONE,  NONE,
  NONE,  NONE,  NONE,  NONE, NONE,  NONE,  NONE,  NONE
};

static const char *cop1FPUOps[64] = {
  "add", "sub", "mul", "div", "sqrt", "abs", "mov", "neg",
  "round.l", "trunc.l", "ceil.l", "floor.l", "round.w", "trunc.w", "ceil.w", "floor.w",
  "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "",
  "cvt.s", "cvt.d", "", "", "cvt.w", "cvt.l", "", "",
  "", "", "", "", "", "", "", "",
  "c.f", "c.un", "c.eq", "c.ueq", "c.olt", "c.ult", "c.ole", "c.ule",
  "c.sf", "c.ngle", "c.seq", "c.ngl", "c.lt", "c.nge", "c.le", "c.ngt"
};

static const optype cop1FPUOpsType[64] = {
  OPFDFSFT, OPFDFSFT, OPFDFSFT, OPFDFSFT, OPFDFS, OPFDFS, OPFDFS, OPFDFS,
  OPFDFS, OPFDFS, OPFDFS, OPFDFS, OPFDFS, OPFDFS, OPFDFS, OPFDFS,
  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
  OPFDFS, OPFDFS, NONE, NONE, OPFDFS, OPFDFS, NONE, NONE,
  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
  OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT,
  OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT, OPFSFT
};

static const char *cop2Ops[32] = {
  "mfc2", "dmfc2", "cfc2", "", "mtc2", "dmtc2", "ctc2", "",
  "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", ""
};

static const optype cop2OpsType[32] = {
  OPTRD,  OPTRD,  OPTRS,  NONE,   OPTRD,  OPTRD,  OPTRS,  NONE,
  COPBC,  NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,
  VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR,
  VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR
};

/*
static const char *cop2LoadOps[32] = {
  "lbv", "lsv", "llv", "ldv", "lqv", "lrv", "lpv", "luv",
  "lhv", "lfv", "lwv", "ltv", "", "", "", "",
  "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", ""
};

static const char *cop2StoreOps[32] = {
  "sbv", "ssv", "slv", "sdv", "sqv", "srv", "spv", "suv",
  "shv", "sfv", "swv", "stv", "", "", "", "",
  "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", ""
};
*/

static const char *cop2VectorOps[64] = {
  "vmulf", "vmulu", "vrndp", "vmulq", "vmudl", "vmudm", "vmudn", "vmudh",
  "vmacf", "vmacu", "vrndn", "vmacq", "vmadl", "vmadm", "vmadn", "vmadh",
  "vadd", "vsub", "", "vabs", "addc", "vsubc", "", "",
  "", "", "", "", "", "", "", "",
  "vlt", "veq", "vne", "vge", "vcl", "vch", "vcr", "vmrg",
  "vand", "vnand", "vor", "vnor", "vxor", "vnxor", "", "",
  "vrcp", "vrcpl", "vrcph", "vmov", "vrsq", "vrsql", "vrsqh",
  "", "", "", "", "", "", "", ""
};

static const optype cop2VectorOpsType[64] = {
  VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR, VECTOR,
  HELP, HELP, NONE, HELP, HELP, HELP, NONE, NONE,
  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
  HELP, HELP, HELP, HELP, HELP, HELP, HELP, HELP,
  HELP, HELP, HELP, HELP, HELP, HELP, NONE, NONE,
  HELP, HELP, HELP, HELP, HELP, HELP, HELP, HELP,
  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
};

#define getoptype(mipsOp) \
      (mainOpsType[mipsOp.f.op] == SPECIAL) ? specialOpsType[mipsOp.f.func] : \
(mainOpsType[mipsOp.f.op] == REGIMM) ? regimmOpsType[mipsOp.f.rt] : \
      (mainOpsType[mipsOp.f.op] == COP0) ? cop0OpsType[mipsOp.f.rs] : \
      (mainOpsType[mipsOp.f.op] == COP1) ? ((cop1OpsType[mipsOp.f.rs] == FPU) ? cop1FPUOpsType[mipsOp.f.func] : cop1OpsType[mipsOp.f.rs]) : \
      (mainOpsType[mipsOp.f.op] == COP2) ? ((cop2OpsType[mipsOp.f.rs] == VECTOR) ? cop2VectorOpsType[mipsOp.f.func] : cop2OpsType[mipsOp.f.rs]) : \
      mainOpsType[mipsOp.f.op]

#define getopcode(mipsOp) \
    (mainOpsType[mipsOp.f.op] == SPECIAL) ? specialOps[mipsOp.f.func] : \
    (mainOpsType[mipsOp.f.op] == REGIMM) ? regimmOps[mipsOp.f.rt] : \
    (mainOpsType[mipsOp.f.op] == COP0) ? cop0Ops[mipsOp.f.rs] : \
    (mainOpsType[mipsOp.f.op] == COP1) ? ((cop1OpsType[mipsOp.f.rs] == FPU) ? cop1FPUOps[mipsOp.f.func] : cop1Ops[mipsOp.f.rs]) : \
    (mainOpsType[mipsOp.f.op] == COP2) ? ((cop2OpsType[mipsOp.f.rs] == VECTOR) ? cop2VectorOps[mipsOp.f.func] : cop2Ops[mipsOp.f.rs]) : \
    mainOps[mipsOp.f.op]

void mips_show_instruction(char *str, mips_inst *instruction)
{
	Instr inst;
	inst.op.data = instruction->data;

	optype OPCODETYPE = getoptype(inst.op);
	const char *OPCODE = getopcode(inst.op);

	if (inst.op.data == 0) sprintf(str,"nop");
	else {
		switch (OPCODETYPE) {
			case	OP:
					sprintf(str,"%s", OPCODE);
					break;
			case 	OPTGT:
					sprintf(str,"%s 0x%07x", OPCODE, inst.op.t.target*4);
					break;
			case 	OPC:
					sprintf(str,"%s 0x%x", OPCODE, inst.op.c.code);
					break;
			case 	OPS:
					sprintf(str,"%s %s", OPCODE, GPR[inst.op.f.rs]);
					break;
			case 	OPSD:
					sprintf(str,"%s %s %s", OPCODE, GPR[inst.op.f.rs], GPR[inst.op.f.rd]);
					break;
			case 	OPST:
					sprintf(str,"%s %s %s", OPCODE, GPR[inst.op.f.rs], GPR[inst.op.f.rt]);
					break;
			case 	OPSI:
					sprintf(str,"%s %s 0x%x", OPCODE, GPR[inst.op.f.rs], inst.op.i.imm);
					break;
			case 	OPSO:
					sprintf(str,"%s %s $L%d", OPCODE, GPR[inst.op.f.rs],(int)(inst.op.i.imm)*4);
					break;
			case 	OPSTO:
					sprintf(str,"%s %s %s $L%d", OPCODE, GPR[inst.op.f.rs], GPR[inst.op.f.rt],(int)(inst.op.i.imm)*4);
					break;
			case 	OPTI:
					sprintf(str,"%s %s 0x%x", OPCODE, GPR[inst.op.f.rt], inst.op.i.imm);
					break;
			case 	OPTSI:
					sprintf(str,"%s %s %s 0x%x", OPCODE, GPR[inst.op.f.rt], GPR[inst.op.f.rs], inst.op.i.imm);
					break;
			case 	OPTOB:
					sprintf(str,"%s %s 0x%x(%s)", OPCODE, GPR[inst.op.f.rt], inst.op.i.imm, GPR[inst.op.f.rs]);
					break;
			case 	OPTD:
					sprintf(str,"%s %s %s", OPCODE, GPR[inst.op.f.rt], GPR[inst.op.f.rd]);
					break;
			case 	OPD:
					sprintf(str,"%s %s", OPCODE, GPR[inst.op.f.rd]);
					break;
			case 	OPDST:
					sprintf(str,"%s %s %s %s", OPCODE, GPR[inst.op.f.rd], GPR[inst.op.f.rs], GPR[inst.op.f.rt]);
					break;
			case 	OPDTA:
					sprintf(str,"%s %s %s %d", OPCODE, GPR[inst.op.f.rd], GPR[inst.op.f.rt], inst.op.f.sa);
					break;
			case 	OPDTS:
					sprintf(str,"%s %s %s %s", OPCODE, GPR[inst.op.f.rd], GPR[inst.op.f.rt], GPR[inst.op.f.rs]);
					break;
			case 	OPTSD:
					sprintf(str,"%s %s %s", OPCODE, GPR[inst.op.f.rt], SYSR[inst.op.f.rd]);
					break;
			case 	OPTSS:
					sprintf(str,"%s %s %s", OPCODE, GPR[inst.op.f.rt], SYSR[inst.op.f.rs]);
					break;
			case 	OPTFD:
					sprintf(str,"%s %s $f%02d", OPCODE, GPR[inst.op.f.rt], inst.op.f.rd);
					break;
			case	OPTFS:
					sprintf(str,"%s %s $f%02d", OPCODE, GPR[inst.op.f.rt], inst.op.f.rs);
					break;
			case 	OPFTOB:
					sprintf(str,"%s $f%02d 0x%x(%s)", OPCODE, inst.op.f.rt, inst.op.i.imm, GPR[inst.op.f.rs]);
					break;
			case 	OPRTOB:
					sprintf(str,"%s $v%02d 0x%x(%s)", OPCODE, inst.op.f.rt, inst.op.i.imm, GPR[inst.op.f.rs]);
					break;
			case 	OPFSFT:
					sprintf(str,"%s $f%02d $f%02d", OPCODE, inst.op.f.rs, inst.op.f.rt);
					break;
			case 	OPFDFS:
					sprintf(str,"%s $f%02d $f%02d", OPCODE, inst.op.f.rd, inst.op.f.rs);
					break;
			case 	OPFDFSFT:
					sprintf(str,"%s %s $f%02d $f%02d $f%02d", OPCODE, cop1Ops[inst.op.f.rs], inst.op.f.rd, inst.op.f.rs, inst.op.f.rt);
					break;
			case 	OPTRD:
					sprintf(str,"%s %s $v%02d", OPCODE, GPR[inst.op.f.rt], inst.op.f.rd);
					break;
			case 	OPTRS:
					sprintf(str,"%s %s $v%02d", OPCODE, GPR[inst.op.f.rt], inst.op.f.rs);
					break;
			case 	COPBC:
					sprintf(str,"%s%c%s 0x%x", OPCODE,
					(inst.op.bc.tf == 1) ? 't' : 'f',
					(inst.op.bc.nd == 1) ? "l" : "", inst.op.bc.imm);
					break;
			case 	TLB:
					switch (inst.op.f.func) {
						case 1:
							sprintf(str,"tlbr");
							break;
						case 2:
							sprintf(str,"tlbwi");
							break;
						case 6:
							sprintf(str,"tlbwr");
							break;
						case 8:
							sprintf(str,"tlbp");
							break;
						case 24:
							sprintf(str,"eret");
							break;
						default:
							sprintf(str,"Unknown Cop0 TLB Instruction");
							break;
					}
					break;
			case 	VECTOR:
					sprintf(str,"%s $v%02d $v%02d $v%02d[%02d];VECTOR", OPCODE, inst.op.v.dest, inst.op.v.s1, inst.op.v.s2, inst.op.v.el);
					break;
			case 	HELP:
					sprintf(str,"%s[Unknown Format - Help]", OPCODE);
					break;
			case 	SPECIAL:
			case 	REGIMM:
			case 	FPU:
			case 	COP0:
			case 	COP1:
			case 	COP2:
			case 	NONE:
					sprintf(str,"Invalid or Unknown opcode: 0x%08x", inst.op.data);
					break;
		}
	}
}

class MipsDebuggerView;

MipsDebuggerView *view;

class MipsDebuggerView : public BView
{
	sem_id		sem;
	BTextControl *string;
	BTextControl *show;
	BTextControl *breakpoint;
	BButton		*button;
	BButton		*stop;
	BButton		*step;
	BButton		*skip;
	uint32		showbase;
	uint32		count;
	uint32 		breakaddr;
	char 		str[1024];

	public:
				MipsDebuggerView():BView(BRect(0,0,699,369),"",0,B_WILL_DRAW | B_PULSE_NEEDED)
				{
					showbase = 0;
					count = 0;
					breakaddr = 0;

					SetFont(be_fixed_font);
					sem = create_sem(0,"dsadasd");

					SetViewColor(200,200,200);
					SetLowColor(200,200,200);

					string = new BTextControl(
						BRect(0,285+60,100,367+60),
						"RLen",
						"RLen",
						"0",
						new BMessage('____')
					);
					AddChild(string);

					button = new BButton(
						BRect(110,280+60,190,307+60),
						"Run",
						"Run",
						new BMessage('DOSK')
					);
					AddChild(button);

					stop = new BButton(
						BRect(200,280+60,280,307+60),
						"Stop",
						"Stop",
						new BMessage('STOP')
					);
					AddChild(stop);

					step = new BButton(
						BRect(290,280+60,370,307+60),
						"Step",
						"Step",
						new BMessage('STEP')
					);
					AddChild(step);

					skip = new BButton(
						BRect(380,280+60,460,307+60),
						"Skip",
						"Skip",
						new BMessage('SKIP')
					);
					AddChild(skip);

					breakpoint = new BTextControl(
						BRect(490,285+60,580,307+60),
						"BP",
						"BP",
						"0",
						new BMessage('BREK')
					);
					AddChild(breakpoint);

					show = new BTextControl(
						BRect(590,285+60,680,307+60),
						"Show",
						"Show",
						"0",
						new BMessage('SHOW')
					);
					AddChild(show);
				}

				~MipsDebuggerView()
				{
				}

		void	AttachedToWindow()
				{
					MakeFocus();
					SetDrawingMode(B_OP_COPY);

					button->SetTarget(this);
					step->SetTarget(this);
					skip->SetTarget(this);
					stop->SetTarget(this);

					string->SetDivider(30);
					string->SetTarget(this);

					show->SetDivider(30);
					show->SetTarget(this);

					breakpoint->SetDivider(30);
					breakpoint->SetTarget(this);
				}

		void	MouseDown(BPoint where)
				{
					MakeFocus();
				}

		void	MessageReceived(BMessage *msg)
				{
					switch(msg->what)
					{
						case	'SKIP':
								{
									gpr_reg.pc+=4;
									ShowGPR();
								}
								break;
						case	'STEP':
								{
									release_sem(sem);
								}
								break;
						case	'STOP':
								{
									n64_update_screen();
									count = 0;
								}
								break;
						case	'DOSK':
								{
									sscanf(breakpoint->Text(),"%x",(int *)&breakaddr);
									sscanf(string->Text(),"%x",(int *)&count);
									count--;
									release_sem(sem);
								}
								break;
						case	'SHOW':
								{
									sscanf(show->Text(),"%x",(int *)&showbase);
									showbase&=0xFFFFFFFC;
									ShowGPR();
								}
								break;
					}
				}

		void	KeyDown(const char *bytes, int32 numBytes)
				{
					release_sem(sem);
				}

		void	ShowGPRStep()
				{
					if(gpr_reg.pc == breakaddr) {
						count = 0;
					}
					if(count<=0) {
						n64_update_screen();
						ShowGPR();
						acquire_sem(sem);
					}
					else {
						count--;
					}
				}

		void	ShowGPR()
				{
					if(LockLooper())
					{
						MakeFocus();

						SetHighColor(200,200,200);
						FillRect(BRect(0,0,699,369));
						SetHighColor(0,0,0);

						sprintf(str,
						"PC: %016Lx HI: %016Lx LO: %016Lx DPC:%016Lx",
						(int64)gpr_reg.pc,gpr_reg.hi,gpr_reg.lo,(int64)gpr_reg.dl_pc);
						DrawString(str,BPoint(8,12*1));

						for(int32 c=0;c<32;c+=4) {
							sprintf(str,
							"%s:%016Lx %s:%016Lx %s:%016Lx %s:%016Lx",
							GPR[c],gpr_reg.r[c],GPR[c+1],gpr_reg.r[c+1],GPR[c+2],gpr_reg.r[c+2],GPR[c+3],gpr_reg.r[c+3]);
							DrawString(str,BPoint(8,12*((c/4)+2)));
						}

						mips_inst inst;
						for(int32 c=-16;c<=16;c+=4) {
							inst.data = mips_read_uint32(gpr_reg.pc + c);
							mips_show_instruction(str, &inst);
							DrawString(str,BPoint(8,12*(17+(c/4))));
						}

						sprintf(str,
						"PC: %08x DPC:%08x",
						(int32)rsp_reg.pc,(int32)rsp_reg.dl_pc);
						DrawString(str,BPoint(390,12*13));

						for(int32 c=0;c<32;c+=4) {
							sprintf(str,
							"%s:%08x %s:%08x %s:%08x %s:%08x",
							GPR[c],rsp_reg.r[c],GPR[c+1],rsp_reg.r[c+1],GPR[c+2],rsp_reg.r[c+2],GPR[c+3],rsp_reg.r[c+3]);
							DrawString(str,BPoint(390,12*((c/4)+14)));
						}

						for(int32 c=-16;c<=16;c+=4) {
							inst.data = mips_read_uint32(rsp_reg.pc + c);
							mips_show_instruction(str, &inst);
							DrawString(str,BPoint(195,12*(17+(c/4))));
						}

						FillRect(BRect(2,12*16+6,5,12*16+10));

						sprintf(str,
						"(SR IE:%d EXL:%d ERL:%d IM:%02x RE:%d CU:%01x FR:%d BEV:%d)  (CS CD:%d IP:%02x BD:%d)",
							(int)cop_reg[0].cp0.sr.ie,
							(int)cop_reg[0].cp0.sr.exl,
							(int)cop_reg[0].cp0.sr.erl,
							(int)cop_reg[0].cp0.sr.im,
							(int)cop_reg[0].cp0.sr.re,
							(int)cop_reg[0].cp0.sr.cu,
							(int)cop_reg[0].cp0.sr.fr,
							(int)cop_reg[0].cp0.sr.bev,
							(int)cop_reg[0].cp0.cause.exccode,
							(int)cop_reg[0].cp0.cause.ip,
							(int)cop_reg[0].cp0.cause.bd
						);
						DrawString(str,BPoint(8,12*11));

						sprintf(str,
						"count:   %016Lx",(int64)cop_reg[0].cp0.count.value);
						DrawString(str,BPoint(530,12*1));
						sprintf(str,
						"compare: %016Lx",(int64)cop_reg[0].cp0.compare.value);
						DrawString(str,BPoint(530,12*2));
						sprintf(str,
						"cyclec:  %016Lx",(int64)gpr_reg.cc);
						DrawString(str,BPoint(530,12*3));
						sprintf(str,
						"epc:     %016Lx",(int64)cop_reg[0].cp0.epc);
						DrawString(str,BPoint(530,12*4));
						sprintf(str,
						"watchlo: %016Lx",(int64)cop_reg[0].cp0.watchlo);
						DrawString(str,BPoint(530,12*5));
						sprintf(str,
						"watchhi: %016Lx",(int64)cop_reg[0].cp0.watchhi);
						DrawString(str,BPoint(530,12*6));
						sprintf(str,
						"taglo:   %016Lx",(int64)cop_reg[0].cp0.taglo);
						DrawString(str,BPoint(530,12*7));
						sprintf(str,
						"taghi:   %016Lx",(int64)cop_reg[0].cp0.taghi);
						DrawString(str,BPoint(530,12*8));
						sprintf(str,
						"lladdr:  %016Lx",(int64)cop_reg[0].cp0.lladdr);
						DrawString(str,BPoint(530,12*9));

						for(int32 c=0;c<(32*6);c+=32) {
							sprintf(str,
								"%08x: %08x %08x %08x %08x %08x %08x %08x %08x",
								(int)(showbase+c),
								(int)(mips_read_uint32(showbase+c+0)),
								(int)(mips_read_uint32(showbase+c+4)),
								(int)(mips_read_uint32(showbase+c+8)),
								(int)(mips_read_uint32(showbase+c+12)),
								(int)(mips_read_uint32(showbase+c+16)),
								(int)(mips_read_uint32(showbase+c+20)),
								(int)(mips_read_uint32(showbase+c+24)),
								(int)(mips_read_uint32(showbase+c+28))
							);
							DrawString(str,BPoint(8,12*23+(c/32)*12));
						}

						SetHighColor(127,0,0);

						StrokeLine(BPoint(  0,12* 9+8),BPoint(799,12* 9+8));
						StrokeLine(BPoint(  0,12*11+8),BPoint(799,12*11+8));
						StrokeLine(BPoint(  0,12*21+8),BPoint(799,12*21+8));
						StrokeLine(BPoint(190,12*11+8),BPoint(190,12*21+8));
						StrokeLine(BPoint(385,12*11+8),BPoint(385,12*21+8));
						StrokeLine(BPoint(515,      0),BPoint(515,12* 9+8));

						Sync();
						Flush();

						UnlockLooper();
					}
				}

		void	Draw(BRect rect)
				{
					ShowGPR();
				}
};

MipsDebugger::MipsDebugger():BWindow(BRect(8,24,8+699,24+369),"",B_TITLED_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,0)
{
	AddChild(view = new MipsDebuggerView());
	Show();
	SetPulseRate(200000);
}

void MipsDebugger::MipsShow()
{
	view->ShowGPR();
}

void MipsDebugger::MipsStep()
{
	view->ShowGPRStep();
}

bool MipsDebugger::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

static MipsDebugger *mips_debugger;

void mips_init_debugger()
{
	mips_debugger = new MipsDebugger();
}

void mips_step_debugger()
{
	mips_debugger->MipsStep();
}

