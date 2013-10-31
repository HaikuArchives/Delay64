/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _MIPS_MEMORY_H
#define _MIPS_MEMORY_H

#define MIPS_MAX_MEMORY_AREAS 64

#ifndef _MIPS_TYPES_H
#include "mips_types.h"
#endif

// fill addr with an alternative pointer if neccesary, return false to force zero return
typedef bool (*mips_memory_read_notify_func)	(uint32, void **addr, int32 size);
// return true if write operation shoulde be done
typedef bool (*mips_memory_write_notify_func)	(uint32, void *value, int32 size);

extern int64 	(*mips_read_int64)		(uint32 address);
extern int32 	(*mips_read_int32)		(uint32 address);
extern int16 	(*mips_read_int16)		(uint32 address);
extern int8 	(*mips_read_int8)		(uint32 address);
extern uint64 	(*mips_read_uint64)		(uint32 address);
extern uint32 	(*mips_read_uint32)		(uint32 address);
extern uint16 	(*mips_read_uint16)		(uint32 address);
extern uint8 	(*mips_read_uint8)		(uint32 address);

extern void 	(*mips_write_int64)		(uint32 address, int64 value);
extern void 	(*mips_write_int32)		(uint32 address, int32 value);
extern void 	(*mips_write_int16)		(uint32 address, int16 value);
extern void 	(*mips_write_int8)		(uint32 address, int8 value);
extern void 	(*mips_write_uint64)	(uint32 address, uint64 value);
extern void 	(*mips_write_uint32)	(uint32 address, uint32 value);
extern void 	(*mips_write_uint16)	(uint32 address, uint16 value);
extern void 	(*mips_write_uint8)		(uint32 address, uint8 value);

struct mips_memory_area
{
	const char						*name;			// For debug
	uint32	 						 start;			// Start of area
	uint32	 						 end;			// End of area
	uint32							 mask;			// address mask
	void		   					*readdata;		// The pointer to read buffer
	void		   					*writedata;		// The pointer to write buffer
	mips_memory_write_notify_func	 write_notify;	// Called before a write if != 0
	mips_memory_read_notify_func	 read_notify;	// Called before a read if != 0
};

void mips_add_memory_area(mips_memory_area *area);	// Add a new memory area

bool mips_little_endian();							// mips machine is little endian
bool mips_big_endian();								// mips machine is big endian
void mips_set_little_endian();						// set mips machine to little endian
void mips_set_big_endian();							// set mips machine to big endian

bool mips_host_little_endian();						// the emulation platform is little endian

void mips_init_memory();
void mips_exit_memory();

extern mips_memory_area *memory_areas[MIPS_MAX_MEMORY_AREAS];

inline uint16 swap_uint16(uint16 value) {
	return (value>>8) | (value<<8);
}

inline uint32 swap_uint32(uint32 value) {
	return (value>>24) | ((value>>8)&0x0000FF00) | ((value<<8)&0x00FF0000) | (value<<24);
}

inline uint64 swap_uint64(uint64 value) {
	return ((uint64)swap_uint32(value))<<32 | ((uint64)swap_uint32(value>>32));
}

inline int16 swap_int16(int16 value) {
	return swap_uint16(value);
}

inline int32 swap_int32(int32 value) {
	return swap_uint32(value);
}

inline int64 swap_int64(int64 value) {
	return swap_uint64(value);
}

#endif
