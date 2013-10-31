/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_memory.h"
#include "n64_register.h"
#include <string.h>

static uint8 rdram_memory_data[0x400000];
static mips_memory_area rdram_memory_area={
	"rd ram",
	0x00000000,
	0x003FFFFF,
	0x1FFFFFFF,
	(void *)rdram_memory_data,
	(void *)rdram_memory_data,
	0,0
};

static uint8 rdreg_memory_data[0x100000];
static mips_memory_area rdreg_memory_area={
	"rd reg",
	0x03F00000,
	0x03FFFFFF,
	0x1FFFFFFF,
	(void *)rdreg_memory_data,
	(void *)rdreg_memory_data,
	n64_rd_write_register,n64_rd_read_register
};

static uint8 spdmem_memory_data[0x001000];
static mips_memory_area spdmem_memory_area={
	"sp dmem",
	0x04000000,
	0x04000FFF,
	0x1FFFFFFF,
	(void *)spdmem_memory_data,
	(void *)spdmem_memory_data,
	0,0
};

static uint8 spimem_memory_data[0x001000];
static mips_memory_area spimem_memory_area={
	"sp imem",
	0x04001000,
	0x04001FFF,
	0x1FFFFFFF,
	(void *)spimem_memory_data,
	(void *)spimem_memory_data,
	0,0
};

static uint8 spreg_memory_data[0x000020] = {
	0x00, 0x00, 0x00, 0x00, // SPREG_MEM_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_DRAM_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_RD_LEN_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_WR_LEN_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_STATUS_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_DMA_FULL_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_DMA_BUSY_REG
	0x00, 0x00, 0x00, 0x00, // SPREG_SEMAPHORE_REG
};
static mips_memory_area spreg_memory_area={
	"sp reg",
	0x04040000,
	0x0404001F,
	0x1FFFFFFF,
	(void *)spreg_memory_data,
	(void *)spreg_memory_data,
	n64_spreg_write_register,n64_spreg_read_register
};

static uint8 spmem_memory_data[0x000008] = {
	0x00, 0x00, 0x00, 0x00, // SPMEM_PC_REG
	0x00, 0x00, 0x00, 0x00, // SPMEM_IBIST_REG
};
static mips_memory_area spmem_memory_area={
	"sp mem",
	0x04080000,
	0x04080007,
	0x1FFFFFFF,
	(void *)spmem_memory_data,
	(void *)spmem_memory_data,
	n64_spmem_write_register,n64_spmem_read_register
};

static uint8 dpcreg_memory_data[0x000020] = {
	0x00, 0x00, 0x00, 0x00, // DPC_START_REG
	0x00, 0x00, 0x00, 0x00, // DPC_END_REG
	0x00, 0x00, 0x00, 0x00, // DPC_CURRENT_REG
	0x00, 0x00, 0x00, 0x00, // DPC_STATUS_REG
	0x00, 0x00, 0x00, 0x00, // DPC_CLOCK_REG
	0x00, 0x00, 0x00, 0x00, // DPC_BUFBUSY_REG
	0x00, 0x00, 0x00, 0x00, // DPC_PIPEBUSY_REG
	0x00, 0x00, 0x00, 0x00, // DPC_TMEM_REG
};
static mips_memory_area dpcreg_memory_area={
	"dpc reg",
	0x04100000,
	0x0410001F,
	0x1FFFFFFF,
	(void *)dpcreg_memory_data,
	(void *)dpcreg_memory_data,
	n64_dpc_write_register,n64_dpc_read_register
};

static uint8 dpsreg_memory_data[0x000010] = {
	0x00, 0x00, 0x00, 0x00, // DPS_TBIST_REG
	0x00, 0x00, 0x00, 0x00, // DPS_TEST_MODE_REG
	0x00, 0x00, 0x00, 0x00, // DPS_BUFTEST_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // DPS_BUFTEST_DATA_REG
};
static mips_memory_area dpsreg_memory_area={
	"dps reg",
	0x04200000,
	0x0420000F,
	0x1FFFFFFF,
	(void *)dpsreg_memory_data,
	(void *)dpsreg_memory_data,
	n64_dps_write_register,n64_dps_read_register
};

static uint8 mireg_memory_data[0x000010] = {
	0x00, 0x00, 0x00, 0x00, // MI_INIT_MODE_REG
	0x01, 0x01, 0x01, 0x01, // MI_VERSION_REG
	0x00, 0x00, 0x00, 0x00, // MI_INTR_REG
	0x00, 0x00, 0x00, 0x00, // MI_INTR_MASK_REG
};
static mips_memory_area mireg_memory_area={
	"mi reg",
	0x04300000,
	0x0430000F,
	0x1FFFFFFF,
	(void *)mireg_memory_data,
	(void *)mireg_memory_data,
	n64_mi_write_register,n64_mi_read_register
};

static uint8 vireg_memory_data[0x000038] = {
	0x00, 0x00, 0x00, 0x00, // VI_STATUS_REG
	0x00, 0x00, 0x00, 0x00, // VI_ORIGIN_REG
	0x00, 0x00, 0x00, 0x00, // VI_WIDTH_REG
	0x00, 0x00, 0x00, 0x00, // VI_INTR_REG
	0x00, 0x00, 0x00, 0x00, // VI_CURRENT_REG
	0x00, 0x00, 0x00, 0x00, // VI_BURST_REG
	0x00, 0x00, 0x00, 0x00, // VI_V_SYNC_REG
	0x00, 0x00, 0x00, 0x00, // VI_H_SYNC_REG
	0x00, 0x00, 0x00, 0x00, // VI_LEAP_REG
	0x00, 0x00, 0x00, 0x00, // VI_H_START_REG
	0x00, 0x00, 0x00, 0x00, // VI_V_START_REG
	0x00, 0x00, 0x00, 0x00, // VI_V_BURST_REG
	0x00, 0x00, 0x00, 0x00, // VI_X_SCALE_REG
	0x00, 0x00, 0x00, 0x00, // VI_Y_SCALE_REG
};
static mips_memory_area vireg_memory_area={
	"vi reg",
	0x04400000,
	0x04400037,
	0x1FFFFFFF,
	(void *)vireg_memory_data,
	(void *)vireg_memory_data,
	n64_vi_write_register,n64_vi_read_register
};

static uint8 aireg_memory_data[0x000018] = {
	0x00, 0x00, 0x00, 0x00, // AI_DRAM_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // AI_LEN_REG
	0x00, 0x00, 0x00, 0x00, // AI_CONTROL_REG
	0x00, 0x00, 0x00, 0x00, // AI_STATUS_REG
	0x00, 0x00, 0x00, 0x00, // AI_DACRATE_REG
	0x00, 0x00, 0x00, 0x00, // AI_BITRATE_REG
};
static mips_memory_area aireg_memory_area={
	"ai reg",
	0x04500000,
	0x04500017,
	0x1FFFFFFF,
	(void *)aireg_memory_data,
	(void *)aireg_memory_data,
	n64_ai_write_register,n64_ai_read_register
};

static uint8 pireg_memory_data[0x000034] = {
	0x00, 0x00, 0x00, 0x00, // PI_DRAM_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // PI_CART_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // PI_RD_LEN_REG
	0x00, 0x00, 0x00, 0x00, // PI_WR_LEN_REG
	0x00, 0x00, 0x00, 0x00, // PI_STATUS_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM1_LAT_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM1_PWD_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM1_PGS_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM1_RLS_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM2_LAT_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM2_PWD_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM2_PGS_REG
	0x00, 0x00, 0x00, 0x00, // PI_BSD_DOM2_RLS_REG
};
static mips_memory_area pireg_memory_area={
	"pi reg",
	0x04600000,
	0x04600033,
	0x1FFFFFFF,
	(void *)pireg_memory_data,
	(void *)pireg_memory_data,
	n64_pi_write_register,n64_pi_read_register
};

static uint8 rireg_memory_data[0x000020] = {
	0x00, 0x00, 0x00, 0x00, // RI_MODE_REG
	0x00, 0x00, 0x00, 0x00, // RI_CONFIG_REG
	0x00, 0x00, 0x00, 0x00, // RI_CURRENT_LOAD_REG
	0x00, 0x00, 0x00, 0x00, // RI_SELECT_REG
	0x00, 0x00, 0x00, 0x00, // RI_REFRESH_REG
	0x00, 0x00, 0x00, 0x00, // RI_REFRESH_REG
	0x00, 0x00, 0x00, 0x00, // RI_RERROR_REG
	0x00, 0x00, 0x00, 0x00, // RI_WERROR_REG
};
static mips_memory_area rireg_memory_area={
	"ri reg",
	0x04700000,
	0x0470001F,
	0x1FFFFFFF,
	(void *)rireg_memory_data,
	(void *)rireg_memory_data,
	n64_ri_write_register,n64_ri_read_register
};

static uint8 sireg_memory_data[0x00001C] = {
	0x00, 0x00, 0x00, 0x00, // SI_DRAM_ADDR_REG
	0x00, 0x00, 0x00, 0x00, // SI_PIF_ADDR_RD_REG
	0x00, 0x00, 0x00, 0x00, // SI_RESERVED_0
	0x00, 0x00, 0x00, 0x00, // SI_RESERVED_1
	0x00, 0x00, 0x00, 0x00, // SI_PIF_ADDR_WR_REG
	0x00, 0x00, 0x00, 0x00, // SI_RESERVED_2
	0x00, 0x00, 0x00, 0x00, // SI_STATUS_REG
};
static mips_memory_area sireg_memory_area={
	"si reg",
	0x04800000,
	0x0480001B,
	0x1FFFFFFF,
	(void *)sireg_memory_data,
	(void *)sireg_memory_data,
	n64_si_write_register,n64_si_read_register
};

n64_memory_areas n64_memory=
{
	rdram_memory_data,
	rdreg_memory_data,
	spdmem_memory_data,
	spimem_memory_data,
	spreg_memory_data,
	spmem_memory_data,
	dpcreg_memory_data,
	dpsreg_memory_data,
	mireg_memory_data,
	vireg_memory_data,
	aireg_memory_data,
	pireg_memory_data,
	rireg_memory_data,
	sireg_memory_data,
};

void n64_get_rdram_ptr(uint32 addr, uint8 **ptr, int32 *len) 
{
	if((addr >= rdram_memory_area.start) &&  (addr <= rdram_memory_area.end))  {
		*ptr = rdram_memory_data + addr - rdram_memory_area.start;
		*len = rdram_memory_area.end - addr;
	}
}

void n64_init_memory()
{
	memset(rdram_memory_data,0,sizeof(rdram_memory_data));
	memset(rdreg_memory_data,0,sizeof(rdreg_memory_data));
	memset(spdmem_memory_data,0,sizeof(spdmem_memory_data));
	memset(spimem_memory_data,0,sizeof(spimem_memory_data));

	mips_add_memory_area(&rdram_memory_area);
	mips_add_memory_area(&rdreg_memory_area);
	mips_add_memory_area(&spdmem_memory_area);
	mips_add_memory_area(&spimem_memory_area);
	mips_add_memory_area(&spreg_memory_area);
	mips_add_memory_area(&spmem_memory_area);
	mips_add_memory_area(&dpcreg_memory_area);
	mips_add_memory_area(&dpsreg_memory_area);
	mips_add_memory_area(&mireg_memory_area);
	mips_add_memory_area(&vireg_memory_area);
	mips_add_memory_area(&aireg_memory_area);
	mips_add_memory_area(&pireg_memory_area);
	mips_add_memory_area(&rireg_memory_area);
	mips_add_memory_area(&sireg_memory_area);
}

void n64_exit_memory()
{
}
