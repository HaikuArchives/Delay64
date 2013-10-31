/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "mips_memory.h"
#include "mips_exception.h"
#include "mips_cop.h"
#include <stdio.h>

mips_memory_area *memory_areas[MIPS_MAX_MEMORY_AREAS];

int64 	(*mips_read_int64)	(uint32 address);
int32 	(*mips_read_int32)	(uint32 address);
int16 	(*mips_read_int16)	(uint32 address);
int8 	(*mips_read_int8)	(uint32 address);
uint64 	(*mips_read_uint64)	(uint32 address);
uint32 	(*mips_read_uint32)	(uint32 address);
uint16 	(*mips_read_uint16)	(uint32 address);
uint8 	(*mips_read_uint8)	(uint32 address);

void 	(*mips_write_int64)	(uint32 address, int64 value);
void 	(*mips_write_int32)	(uint32 address, int32 value);
void 	(*mips_write_int16)	(uint32 address, int16 value);
void 	(*mips_write_int8)	(uint32 address, int8 value);
void 	(*mips_write_uint64)(uint32 address, uint64 value);
void 	(*mips_write_uint32)(uint32 address, uint32 value);
void 	(*mips_write_uint16)(uint32 address, uint16 value);
void 	(*mips_write_uint8)	(uint32 address, uint8 value);

static uint64 null=0;

#define MIPS_READ_PHYS_ADDR(address,type,swapfunc,aligncheck) \
	void *addr=(void *)&null; \
	if((aligncheck)==0) { \
		for(int32 c=0;c<MIPS_MAX_MEMORY_AREAS;c++) { \
			mips_memory_area *area; \
			if((area = memory_areas[c])!=0) { \
				if(((address&area->mask)>=area->start) && ((address&area->mask)<=area->end)) { \
					if(area->read_notify) { \
						if(area->read_notify(address,&addr,sizeof(type))) { \
							addr = (void *)(((uint8 *)area->readdata)+((address&area->mask)-area->start)); \
						} \
						else { \
							return *((type *)addr); \
						} \
					} \
					else { \
						addr = (void *)(((uint8 *)area->readdata)+((address&area->mask)-area->start)); \
					} \
					break; \
				} \
			} \
			else { \
				break; \
			} \
		} \
	} \
	else { \
		mips_do_exception(MIPS_EXCEPTION_ADEL,0); \
	} \
	return swapfunc(*((type *)addr));

static int64 mips_read_int64_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int64,,address&7);
}

static int32 mips_read_int32_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int32,,address&3);
}

static int16 mips_read_int16_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int16,,address&1);
}

static int8 mips_read_int8_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int8,,0);
}

static uint64 mips_read_uint64_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint64,,address&7);
}

static uint32 mips_read_uint32_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint32,,address&3);
}

static uint16 mips_read_uint16_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint16,,address&1);
}

static uint8 mips_read_uint8_noswap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint8,,0);
}

static int64 mips_read_int64_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int64,swap_int64,address&7);
}

static int32 mips_read_int32_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int32,swap_int32,address&3);
}

static int16 mips_read_int16_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int16,swap_int16,address&1);
}

static int8 mips_read_int8_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,int8,,0);
}

static uint64 mips_read_uint64_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint64,swap_uint64,address&7);
}

static uint32 mips_read_uint32_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint32,swap_uint32,address&3);
}

static uint16 mips_read_uint16_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint16,swap_uint16,address&1);
}

static uint8 mips_read_uint8_swap(uint32 address)
{
	MIPS_READ_PHYS_ADDR(address,uint8,,0);
}

#define MIPS_WRITE_PHYS_ADDR(address,value,type,swapfunc,aligncheck) \
	if((aligncheck)==0) { \
		for(int32 c=0;c<MIPS_MAX_MEMORY_AREAS;c++) { \
			mips_memory_area *area; \
			if((area = memory_areas[c])!=0) { \
				if(((address&area->mask)>=area->start) && ((address&area->mask)<=area->end)) { \
					void *addr = (void *)(((uint8 *)area->writedata)+((address&area->mask)-area->start)); \
					if(area->write_notify) { \
						if(area->write_notify(address,(void *)&value,sizeof(type))) { \
							*((type *)addr)=swapfunc(value); \
						} \
					} \
					else { \
						*((type *)addr)=swapfunc(value); \
					}\
					return; \
				} \
			} \
			else { \
				return; \
			} \
		} \
	} \
	mips_do_exception(MIPS_EXCEPTION_ADES,0);

static void mips_write_int64_noswap(uint32 address, int64 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int64,,address&7);
}

static void mips_write_int32_noswap(uint32 address, int32 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int32,,address&3);
}

static void mips_write_int16_noswap(uint32 address, int16 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int16,,address&1);
}

static void mips_write_int8_noswap(uint32 address, int8 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int8,,0);
}

static void mips_write_uint64_noswap(uint32 address, uint64 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint64,,address&7);
}

static void mips_write_uint32_noswap(uint32 address, uint32 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint32,,address&3);
}

static void mips_write_uint16_noswap(uint32 address, uint16 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint16,,address&1);
}

static void mips_write_uint8_noswap(uint32 address, uint8 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint8,,0);
}

static void mips_write_int64_swap(uint32 address, int64 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int64,swap_int64,address&7);
}

static void mips_write_int32_swap(uint32 address, int32 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int32,swap_int32,address&3);
}

static void mips_write_int16_swap(uint32 address, int16 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int16,swap_int16,address&1);
}

static void mips_write_int8_swap(uint32 address, int8 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,int8,,0);
}

static void mips_write_uint64_swap(uint32 address, uint64 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint64,swap_uint64,address&7);
}

static void mips_write_uint32_swap(uint32 address, uint32 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint32,swap_uint32,address&3);
}

static void mips_write_uint16_swap(uint32 address, uint16 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint16,swap_uint16,address&1);
}

static void mips_write_uint8_swap(uint32 address, uint8 value)
{
	MIPS_WRITE_PHYS_ADDR(address,value,uint8,,0);
}

void mips_add_memory_area(mips_memory_area *area)
{
	for(int32 c=0;c<MIPS_MAX_MEMORY_AREAS;c++) {
		if(memory_areas[c]==0) {
			memory_areas[c] = area;
			return;
		}
	}
}

static bool mips_is_little_endian=false;
static bool mips_host_is_little_endian=false;

bool mips_host_little_endian()
{
	return mips_host_is_little_endian;
}

void mips_set_little_endian()
{
	mips_is_little_endian = true;
	if(mips_host_is_little_endian) {
		mips_read_int64=mips_read_int64_noswap;
		mips_read_int32=mips_read_int32_noswap;
		mips_read_int16=mips_read_int16_noswap;
		mips_read_int8=mips_read_int8_noswap;
		mips_read_uint64=mips_read_uint64_noswap;
		mips_read_uint32=mips_read_uint32_noswap;
		mips_read_uint16=mips_read_uint16_noswap;
		mips_read_uint8=mips_read_uint8_noswap;
		mips_write_int64=mips_write_int64_noswap;
		mips_write_int32=mips_write_int32_noswap;
		mips_write_int16=mips_write_int16_noswap;
		mips_write_int8=mips_write_int8_noswap;
		mips_write_uint64=mips_write_uint64_noswap;
		mips_write_uint32=mips_write_uint32_noswap;
		mips_write_uint16=mips_write_uint16_noswap;
		mips_write_uint8=mips_write_uint8_noswap;
	}
	else {
		mips_read_int64=mips_read_int64_swap;
		mips_read_int32=mips_read_int32_swap; 
		mips_read_int16=mips_read_int16_swap;
		mips_read_int8=mips_read_int8_swap;
		mips_read_uint64=mips_read_uint64_swap;
		mips_read_uint32=mips_read_uint32_swap;
		mips_read_uint16=mips_read_uint16_swap;
		mips_read_uint8=mips_read_uint8_swap;
		mips_write_int64=mips_write_int64_swap;
		mips_write_int32=mips_write_int32_swap;
		mips_write_int16=mips_write_int16_swap;
		mips_write_int8=mips_write_int8_swap;
		mips_write_uint64=mips_write_uint64_swap;
		mips_write_uint32=mips_write_uint32_swap;
		mips_write_uint16=mips_write_uint16_swap;
		mips_write_uint8=mips_write_uint8_swap;
	}
}

bool mips_little_endian()
{
	return mips_is_little_endian ? true : false;
}

void mips_set_big_endian()
{
	mips_is_little_endian = false;
	if(!mips_host_is_little_endian) {
		mips_read_int64=mips_read_int64_noswap;
		mips_read_int32=mips_read_int32_noswap;
		mips_read_int16=mips_read_int16_noswap;
		mips_read_int8=mips_read_int8_noswap;
		mips_read_uint64=mips_read_uint64_noswap;
		mips_read_uint32=mips_read_uint32_noswap;
		mips_read_uint16=mips_read_uint16_noswap;
		mips_read_uint8=mips_read_uint8_noswap;
		mips_write_int64=mips_write_int64_noswap;
		mips_write_int32=mips_write_int32_noswap;
		mips_write_int16=mips_write_int16_noswap;
		mips_write_int8=mips_write_int8_noswap;
		mips_write_uint64=mips_write_uint64_noswap;
		mips_write_uint32=mips_write_uint32_noswap;
		mips_write_uint16=mips_write_uint16_noswap;
		mips_write_uint8=mips_write_uint8_noswap;
	}
	else {
		mips_read_int64=mips_read_int64_swap;
		mips_read_int32=mips_read_int32_swap; 
		mips_read_int16=mips_read_int16_swap;
		mips_read_int8=mips_read_int8_swap;
		mips_read_uint64=mips_read_uint64_swap;
		mips_read_uint32=mips_read_uint32_swap;
		mips_read_uint16=mips_read_uint16_swap;
		mips_read_uint8=mips_read_uint8_swap;
		mips_write_int64=mips_write_int64_swap;
		mips_write_int32=mips_write_int32_swap;
		mips_write_int16=mips_write_int16_swap;
		mips_write_int8=mips_write_int8_swap;
		mips_write_uint64=mips_write_uint64_swap;
		mips_write_uint32=mips_write_uint32_swap;
		mips_write_uint16=mips_write_uint16_swap;
		mips_write_uint8=mips_write_uint8_swap;
	}
}

bool mips_big_endian()
{
	return mips_is_little_endian ? false : true;
}

void mips_init_memory()
{
	for(int32 c=0;c<MIPS_MAX_MEMORY_AREAS;c++) memory_areas[c]=0;

	uint32 endianl = 0x1234567;
	uint8 *endianc = (uint8 *)&endianl;

	if(endianc[0]==0x67) {
		mips_host_is_little_endian = true;
	}
	else {
		mips_host_is_little_endian = false;
	}
	
	if(cop_reg[0].cp0.sr.re) {
		mips_set_little_endian();
	}
	else {
		mips_set_big_endian();
	}
}

void mips_exit_memory()
{
}
