/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#ifndef _N64_CART_H
#define _N64_CART_H

#ifndef _MIPS_MEMORY_H
#include "mips_memory.h"
#endif

struct n64_cartridge_header
{
	uint16	validation;
	uint8	compression;
	uint8	reserved0;
	uint32	clockrate;
	uint32	programcounter;
	uint32	release;
	uint32	crc1;
	uint32	crc2;
	uint64	reserved1;
	char	name[20];
	uint8	reserved2[7];
	uint8	manufactor;
	uint16	cartridge;
	char	countrycode;
	uint8	reserved3;
};

struct n64_cartridge
{
	uint8					*image;
	n64_cartridge_header	*header;
	uint8					*bootcode;
	uint8					*gamecode;
	uint32					 length;
};

extern n64_cartridge n64_cart;

bool n64_load_cardtrige(const char *filename);
void n64_get_cardtrige_ptr(uint32 addr, uint8 **ptr, int32 *maxlen);

#endif
