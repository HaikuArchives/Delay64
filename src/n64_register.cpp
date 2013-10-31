/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_memory.h"
#include "mips_cop.h"
#include "n64_rsp.h"
#include "n64_register.h"
#include "n64_memory.h"
#include "n64_cart.h"
#include <stdio.h>
#include <string.h>

#define MIN(a,b) ((a < b) ? a : b)

#define DEBUG_READ(name) //printf( name " Read  pc=%08x a=0x%08x s=%02x\n",(int)gpr_reg.pc,(int)address,(int)size)
#define DEBUG_WRITE(name) //printf( name " Write pc=%08x a=0x%08x s=%02x d=%08x\n",(int)gpr_reg.pc,(int)address,(int)size,(int)*((uint32 *)v))

bool n64_vi_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("VI");
	
	return true;
}

bool n64_vi_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("VI");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	VI_CURRENT_REG:
					{
						if(mips_host_little_endian()) {
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}

						mireg[MI_INTR_REG] &= ~MI_INTR_REG_VI_MASK;
		
						if(mips_host_little_endian()) {
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}
					}
					return false;
		}
	}
	return true;
}

bool n64_mi_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("MI");

	return true;
}

bool n64_mi_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("MI");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;
	uint32  value = *(uint32 *)v;

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	MI_INIT_MODE_REG:
					{
						if(mips_host_little_endian()) {
							mireg[MI_INIT_MODE_REG] = swap_uint32(mireg[MI_INIT_MODE_REG]);
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}

						mireg[MI_INIT_MODE_REG] = // Init length
							(mireg[MI_INIT_MODE_REG] & 0xFFFFFF80) |
							(value & 0x007F);

						if(value & 0x0080) // Clear init mode
							mireg[MI_INIT_MODE_REG] &= ~0x0080;
						
						if(value & 0x0100) // Set init mode
							mireg[MI_INIT_MODE_REG] |=  0x0080;
						
						if(value & 0x0200) // Clear ebus test mode
							mireg[MI_INIT_MODE_REG] &= ~0x0100;
						
						if(value & 0x0400) // Set ebus test mode
							mireg[MI_INIT_MODE_REG] |=  0x0100;
						
						if(value & 0x0800) // Clear DP interrupt
							mireg[MI_INTR_REG]      &= ~MI_INTR_REG_DP_MASK;
						
						if(value & 0x1000) // Clear RDRAM reg mode
							mireg[MI_INIT_MODE_REG] &= ~0x0200;
						
						if(value & 0x2000) // Set RDRAM reg mode
							mireg[MI_INIT_MODE_REG] |=  0x0200;

						if(mips_host_little_endian()) {
							mireg[MI_INIT_MODE_REG] = swap_uint32(mireg[MI_INIT_MODE_REG]);
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}
					}
					return false;
			case	MI_INTR_MASK_REG:
					{
						if(mips_host_little_endian()) {
							mireg[MI_INTR_MASK_REG] = swap_uint32(mireg[MI_INTR_MASK_REG]);
						}

						if(value & 0x0001) // Clear
							mireg[MI_INTR_MASK_REG] &= ~MI_INTR_REG_SP_MASK;
						if(value & 0x0002) // Set
							mireg[MI_INTR_MASK_REG] |=  MI_INTR_REG_SP_MASK;

						if(value & 0x0004) // Clear
							mireg[MI_INTR_MASK_REG] &= ~MI_INTR_REG_SI_MASK;
						if(value & 0x0008) // Set
							mireg[MI_INTR_MASK_REG] |=  MI_INTR_REG_SI_MASK;

						if(value & 0x0010) // Clear
							mireg[MI_INTR_MASK_REG] &= ~MI_INTR_REG_AI_MASK;
						if(value & 0x0020) // Set
							mireg[MI_INTR_MASK_REG] |=  MI_INTR_REG_AI_MASK;

						if(value & 0x0040) // Clear
							mireg[MI_INTR_MASK_REG] &= ~MI_INTR_REG_VI_MASK;
						if(value & 0x0080) // Set
							mireg[MI_INTR_MASK_REG] |=  MI_INTR_REG_VI_MASK;

						if(value & 0x0100) // Clear
							mireg[MI_INTR_MASK_REG] &= ~MI_INTR_REG_PI_MASK;
						if(value & 0x0200) // Set
							mireg[MI_INTR_MASK_REG] |=  MI_INTR_REG_PI_MASK;

						if(value & 0x0400) // Clear
							mireg[MI_INTR_MASK_REG] &= ~MI_INTR_REG_DP_MASK;
						if(value & 0x0800) // Set
							mireg[MI_INTR_MASK_REG] |=  MI_INTR_REG_DP_MASK;

						// A write will also clear the MI interrupt??
						cop_reg[0].cp0.cause.ip &= ~0x4;

						if(mips_host_little_endian()) {
							mireg[MI_INTR_MASK_REG] = swap_uint32(mireg[MI_INTR_MASK_REG]);
						}
					}
					return false;
			case	MI_VERSION_REG:	// read only
			case	MI_INTR_REG:	// read only
			default:
					return false;
		}
	}
	else {
		printf("Wrong MI register access\n");
	}
	return false;
}

bool n64_ai_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("AI");

	return true;
}

bool n64_ai_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("AI");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	SI_STATUS_REG:
					{
						if(mips_host_little_endian()) {
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}
						
						mireg[MI_INTR_REG] &= ~MI_INTR_REG_AI_MASK;
						
						if(mips_host_little_endian()) {
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}
					}
					return false;
		}
	}

	return true;
}

bool n64_pi_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("PI");

	return true;
}

bool n64_pi_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("PI");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;
	uint32 *pireg =  (uint32 *)n64_memory.pireg;
	uint32  value = *(uint32 *)v;

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	PI_STATUS_REG:
					{	
						if(mips_host_little_endian()) {
							mireg[MI_INIT_MODE_REG] = swap_uint32(mireg[MI_INIT_MODE_REG]);
						}

						if(value&1);// reset controller (and abort current op) 
							// ????
						if(value&2) // Clear PI interrupt
							mireg[MI_INTR_REG] &= ~MI_INTR_REG_PI_MASK;

						if(mips_host_little_endian()) {
							mireg[MI_INIT_MODE_REG] = swap_uint32(mireg[MI_INIT_MODE_REG]);
						}
					}
					return false;
			case	PI_WR_LEN_REG:
					{
						if(mips_host_little_endian()) {
							pireg[PI_CART_ADDR_REG] = swap_uint32(pireg[PI_CART_ADDR_REG]);
							pireg[PI_DRAM_ADDR_REG] = swap_uint32(pireg[PI_DRAM_ADDR_REG]);
						}

						uint8 *src_ptr=0;
						int32 src_len=0;
						n64_get_cardtrige_ptr(pireg[PI_CART_ADDR_REG],&src_ptr,&src_len);
						src_len = MIN(src_len,(int32)(value&0xFFFFFF)+1);

						uint8 *dst_ptr=0;
						int32 dst_len=0;
						n64_get_rdram_ptr(pireg[PI_DRAM_ADDR_REG]&0xFFFFFF,&dst_ptr,&dst_len);
						dst_len = MIN(dst_len,(int32)(value&0xFFFFFF)+1);
						
						if(src_ptr && dst_ptr) {
							memcpy(dst_ptr,src_ptr,MIN(src_len,dst_len));
						}
						
						if(mips_host_little_endian()) {
							pireg[PI_CART_ADDR_REG] = swap_uint32(pireg[PI_CART_ADDR_REG]);
							pireg[PI_DRAM_ADDR_REG] = swap_uint32(pireg[PI_DRAM_ADDR_REG]);
						}
						
						// No need to set DMA busy or error ...??
						// How much cycles does take a real DMA process?
					}
					return true;
			default:	
					return true;
		}
	}
	else {
		printf("Wrong PI register access\n");
	}
	return false;
}

bool n64_spreg_read_register(uint32 address, void **addr, int32 size)
{
	uint32 *spreg =  (uint32 *)n64_memory.spreg;

	DEBUG_READ("SPREG");

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	SPREG_STATUS_REG:
					{
						if(mips_host_little_endian()) {
							spreg[SPREG_SEMAPHORE_REG] = swap_uint32(spreg[SPREG_SEMAPHORE_REG]);
						}
						if(!spreg[SPREG_SEMAPHORE_REG]) {
							spreg[SPREG_SEMAPHORE_REG] = 1;
							if(mips_host_little_endian()) {
								spreg[SPREG_SEMAPHORE_REG] = swap_uint32(spreg[SPREG_SEMAPHORE_REG]);
							}
							return false;
						}
						if(mips_host_little_endian()) {
							spreg[SPREG_SEMAPHORE_REG] = swap_uint32(spreg[SPREG_SEMAPHORE_REG]);
						}
					}
					break;
		}
	}
	else {
		printf("Wrong SPREG register access\n");
	}

	return true;
}

bool n64_spreg_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("SPREG");

	uint32 *spreg =  (uint32 *)n64_memory.spreg;
	uint32 *mireg =  (uint32 *)n64_memory.mireg;
	uint32  value = *(uint32 *)v;

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	SPREG_STATUS_REG:
					{
						if(mips_host_little_endian()) {
							spreg[SPREG_STATUS_REG] = swap_uint32(spreg[SPREG_STATUS_REG]);
							mireg[MI_INIT_MODE_REG] = swap_uint32(mireg[MI_INIT_MODE_REG]);
						}

						if(value & 0x00000001) { // Clear halt 
							spreg[SPREG_STATUS_REG] &= ~0x00000001;
							rsp_reg.hlt = 0;
						}
						if(value & 0x00000002) { // Set halt
							spreg[SPREG_STATUS_REG] |=  0x00000001;
							rsp_reg.hlt = 1;
						}
						if(value & 0x00000004) // Clear broke
							spreg[SPREG_STATUS_REG] &= ~0x00000002;

						if(value & 0x00000008) // Clear intr
							mireg[MI_INTR_REG] &= ~MI_INTR_REG_SP_MASK;
						if(value & 0x00000010) // Set intr
							mireg[MI_INTR_REG] |=  MI_INTR_REG_SP_MASK;

						if(value & 0x00000020) // Clear sstep
							spreg[SPREG_STATUS_REG] &= ~0x00000020;
						if(value & 0x00000040) // Set sstep
							spreg[SPREG_STATUS_REG] |=  0x00000020;

						if(value & 0x00000080) // Clear intr on break
							spreg[SPREG_STATUS_REG] &= ~0x00000040;
						if(value & 0x00000100) // Set intr on break
							spreg[SPREG_STATUS_REG] |=  0x00000040;

						if(value & 0x00000200) // Clear signal 0
							spreg[SPREG_STATUS_REG] &= ~0x00000080;
						if(value & 0x00000400) // Set signal 0
							spreg[SPREG_STATUS_REG] |=  0x00000080;

						if(value & 0x00000800) // Clear signal 1
							spreg[SPREG_STATUS_REG] &= ~0x00000100;
						if(value & 0x00001000) // Set signal 1
							spreg[SPREG_STATUS_REG] |=  0x00000100;

						if(value & 0x00002000) // Clear signal 2
							spreg[SPREG_STATUS_REG] &= ~0x00000200;
						if(value & 0x00004000) // Set signal 2
							spreg[SPREG_STATUS_REG] |=  0x00000200;

						if(value & 0x00008000) // Clear signal 3
							spreg[SPREG_STATUS_REG] &= ~0x00000400;
						if(value & 0x00010000) // Set signal 3
							spreg[SPREG_STATUS_REG] |=  0x00000400;

						if(value & 0x00020000) // Clear signal 4
							spreg[SPREG_STATUS_REG] &= ~0x00000800;
						if(value & 0x00040000) // Set signal 4
							spreg[SPREG_STATUS_REG] |=  0x00000800;

						if(value & 0x00080000) // Clear signal 5
							spreg[SPREG_STATUS_REG] &= ~0x00001000;
						if(value & 0x00100000) // Set signal 5
							spreg[SPREG_STATUS_REG] |=  0x00001000;

						if(value & 0x00200000) // Clear signal 6
							spreg[SPREG_STATUS_REG] &= ~0x00002000;
						if(value & 0x00400000) // Set signal 6
							spreg[SPREG_STATUS_REG] |=  0x00002000;

						if(value & 0x00800000) // Clear signal 7
							spreg[SPREG_STATUS_REG] &= ~0x00004000;
						if(value & 0x01000000) // Set signal 7
							spreg[SPREG_STATUS_REG] |=  0x00004000;

						if(mips_host_little_endian()) {
							spreg[SPREG_STATUS_REG] = swap_uint32(spreg[SPREG_STATUS_REG]);
							mireg[MI_INIT_MODE_REG] = swap_uint32(mireg[MI_INIT_MODE_REG]);
						}
					}
					return false;
			case	SPREG_DMA_FULL_REG:
			case	SPREG_DMA_BUSY_REG:
					return false;
			case	SPREG_SEMAPHORE_REG:
					{
						spreg[SPREG_SEMAPHORE_REG] = 0;
					}
					return true;
			default:
					return true;
		}
	}
	else {
		printf("Wrong SPREG register access\n");
	}

	return true;
}

bool n64_spmem_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("SPMEM");

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	SPMEM_PC_REG:
					{
						*addr = (void *)&rsp_reg.pc;
					}
					return false;
		}
	}
		
	return true;
}

bool n64_spmem_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("SPMEM");

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	SPMEM_PC_REG:
					{
						rsp_reg.pc = (*((uint32 *)v))&0xFFF;
					}
					return false;
		}
	}

	return true;
}

bool n64_dpc_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("DPC");

	return true;
}

bool n64_dpc_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("DPC");

	return true;
}

bool n64_dps_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("DPS");

	return true;
}

bool n64_dps_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("DPS");

	return true;
}

bool n64_ri_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("RI");

	return true;
}

bool n64_ri_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("RI");

	return true;
}

bool n64_si_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("SI");

	return true;
}

bool n64_si_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("SI");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;

	if((size==4) && ((address&3)==0)) {
		address &= 0xFF;
		switch(address/4) {
			case	SI_STATUS_REG:
					{
						if(mips_host_little_endian()) {
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}
						
						mireg[MI_INTR_REG] &= ~MI_INTR_REG_SI_MASK;
						
						if(mips_host_little_endian()) {
							mireg[MI_INTR_REG] = swap_uint32(mireg[MI_INTR_REG]);
						}
					}
					return false;
		}
	}
	return true;
}

bool n64_rd_read_register(uint32 address, void **addr, int32 size)
{
	DEBUG_READ("RD");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;
	
	if(mireg[MI_INIT_MODE_REG]&0x200) {
		return true;
	}
	else {
		return false;
	}
}

bool n64_rd_write_register(uint32 address, void *v, int32 size)
{
	DEBUG_WRITE("RD");

	uint32 *mireg =  (uint32 *)n64_memory.mireg;

	if(mireg[MI_INIT_MODE_REG]&0x200) {
		return true;
	}
	else {
		return false;
	}
}
