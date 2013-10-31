/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _N64_REGISTER_H
#define _N64_REGISTER_H

#ifndef _N64_MEMORY_H
#include "n64_memory.h"
#endif

enum
{
	VI_STATUS_REG		= 0x00, // (R/W)
	VI_ORIGIN_REG		= 0x01, // (R/W)
	VI_WIDTH_REG		= 0x02, // (R/W)
	VI_INTR_REG 		= 0x03, // (R/W)
	VI_CURRENT_REG		= 0x04, // (R/W)
	VI_BURST_REG		= 0x05, // (R/W)
	VI_V_SYNC_REG		= 0x06, // (R/W)
	VI_H_SYNC_REG		= 0x07, // (R/W)
	VI_LEAP_REG			= 0x08, // (R/W)
	VI_H_START_REG		= 0x09, // (R/W)
	VI_V_START_REG		= 0x0A, // (R/W)
	VI_V_BURST_REG		= 0x0B, // (R/W)
	VI_X_SCALE_REG		= 0x0C, // (R/W)
	VI_Y_SCALE_REG		= 0x0D  // (R/W)
};

enum
{
	SPREG_MEM_ADDR_REG	= 0x00, // (R/W)
	SPREG_DRAM_ADDR_REG	= 0x01, // (R/W)
	SPREG_RD_LEN_REG	= 0x02, // (R/W)
	SPREG_WR_LEN_REG	= 0x03, // (R/W)
	SPREG_STATUS_REG	= 0x04, // (R/W)
	SPREG_DMA_FULL_REG	= 0x05, // (R/-)
	SPREG_DMA_BUSY_REG	= 0x06, // (R/-)
	SPREG_SEMAPHORE_REG	= 0x07  // (R/W)
};

enum
{
	SPMEM_PC_REG		= 0x00, // (R/W)
	SPMEM_IBIST_REG		= 0x01  // (R/W)
};

enum
{
	MI_INTR_REG_SP_MASK	= 0x01,
	MI_INTR_REG_SI_MASK	= 0x02,
	MI_INTR_REG_AI_MASK	= 0x04,
	MI_INTR_REG_VI_MASK	= 0x08,
	MI_INTR_REG_PI_MASK	= 0x10,
	MI_INTR_REG_DP_MASK	= 0x20 
};

enum
{
	AI_DRAM_ADDR_REG	= 0x00, // (-/W)
	AI_LEN_REG			= 0x01, // (R/W)
	AI_CONTROL_REG		= 0x02, // (-/W)
	AI_STATUS_REG		= 0x03, // (R/W)
	AI_DACRATE_REG		= 0x04, // (-/W)
	AI_BITRATE_REG		= 0x05  // (-/W)
};

enum
{
	PI_DRAM_ADDR_REG	= 0x00, // (R/W)
	PI_CART_ADDR_REG	= 0x01, // (R/W)
	PI_RD_LEN_REG		= 0x02, // (R/W)
	PI_WR_LEN_REG		= 0x03, // (R/W)
	PI_STATUS_REG		= 0x04, // (R/W)
	PI_BSD_DOM1_LAT_REG	= 0x05, // (R/W)
	PI_BSD_DOM1_PWD_REG	= 0x06, // (R/W)
	PI_BSD_DOM1_PGS_REG	= 0x07, // (R/W)
	PI_BSD_DOM1_RLS_REG	= 0x08, // (R/W)
	PI_BSD_DOM2_LAT_REG	= 0x09, // (R/W)
	PI_BSD_DOM2_PWD_REG	= 0x0A, // (R/W)
	PI_BSD_DOM2_PGS_REG	= 0x0B, // (R/W)
	PI_BSD_DOM2_RLS_REG	= 0x0C  // (R/W)
};

enum
{
	MI_INIT_MODE_REG 	= 0x00, // (R/W)
	MI_VERSION_REG 		= 0x01, // (R/-)
	MI_INTR_REG 		= 0x02, // (R/-)
	MI_INTR_MASK_REG 	= 0x03  // (R/W)
};

enum
{
	RI_MODE_REG			= 0x00, // (R/W)
	RI_CONFIG_REG		= 0x01, // (R/W)
	RI_CURRENT_LOAD_REG	= 0x02, // (-/W)
	RI_SELECT_REG		= 0x03, // (R/W)
	RI_REFRESH_REG		= 0x04, // (R/W)
	RI_LATENCY_REG		= 0x05, // (R/W)
	RI_RERROR_REG		= 0x06, // (R/-)
	RI_WERROR_REG		= 0x07  // (-/W)
};

enum
{
	DPC_START_REG		= 0x00, // (R/W)
	DPC_END_REG			= 0x01, // (R/W)
	DPC_CURRENT_REG		= 0x02, // (R/-)
	DPC_STATUS_REG		= 0x03, // (-/W)
	DPC_CLOCK_REG		= 0x04, // (R/-)
	DPC_BUFBUSY_REG		= 0x05, // (R/-)
	DPC_PIPEBUSY_REG	= 0x06, // (R/-)
	DPC_TMEM_REG		= 0x07  // (R/-)
};

enum
{
	DPS_TBIST_REG		= 0x00, // (-/W)
	DPS_TEST_MODE_REG	= 0x01, // (R/W)
	DPS_BUFTEST_ADDR_REG= 0x02, // (R/W)
	DPS_BUFTEST_DATA_REG= 0x03  // (R/W)
};

enum
{
	SI_DRAM_ADDR_REG	= 0x00, // (R/W)
	SI_PIF_ADDR_RD_REG	= 0x01, // (-/W)
	SI_RESERVED_0		= 0x02, // (R/W)
	SI_RESERVED_1		= 0x03, // (R/W)
	SI_PIF_ADDR_WR_REG	= 0x04, // (-/W)
	SI_RESERVED_2		= 0x05, // (R/W)
	SI_STATUS_REG		= 0x06  // (R/W)
};

bool n64_rdram_read_register(uint32 address, void **addr, int32 size);
bool n64_rdram_write_register(uint32 address, void *value, int32 size);

bool n64_vi_read_register(uint32 address, void **addr, int32 size);
bool n64_vi_write_register(uint32 address, void *value, int32 size);

bool n64_mi_read_register(uint32 address, void **addr, int32 size);
bool n64_mi_write_register(uint32 address, void *value, int32 size);

bool n64_ai_read_register(uint32 address, void **addr, int32 size);
bool n64_ai_write_register(uint32 address, void *value, int32 size);

bool n64_pi_read_register(uint32 address, void **addr, int32 size);
bool n64_pi_write_register(uint32 address, void *value, int32 size);

bool n64_spreg_read_register(uint32 address, void **addr, int32 size);
bool n64_spreg_write_register(uint32 address, void *value, int32 size);

bool n64_spmem_read_register(uint32 address, void **addr, int32 size);
bool n64_spmem_write_register(uint32 address, void *value, int32 size);

bool n64_dpc_read_register(uint32 address, void **addr, int32 size);
bool n64_dpc_write_register(uint32 address, void *value, int32 size);

bool n64_dps_read_register(uint32 address, void **addr, int32 size);
bool n64_dps_write_register(uint32 address, void *value, int32 size);

bool n64_ri_read_register(uint32 address, void **addr, int32 size);
bool n64_ri_write_register(uint32 address, void *value, int32 size);

bool n64_si_read_register(uint32 address, void **addr, int32 size);
bool n64_si_write_register(uint32 address, void *value, int32 size);

bool n64_rd_read_register(uint32 address, void **addr, int32 size);
bool n64_rd_write_register(uint32 address, void *value, int32 size);

#endif
