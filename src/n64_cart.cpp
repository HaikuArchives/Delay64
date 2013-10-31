/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_cart.h"
#include "n64_memory.h"
#include <stdio.h>

#define MIN(a,b) ((a < b) ? a : b)

n64_cartridge n64_cart;

static mips_memory_area cart_memory_area_0={
	"cartridge",
	0x1fd00000,
	0x7FFFFFFF,
	0x1FFFFFFF,
	0,
	0,
	0,0
};

static mips_memory_area cart_memory_area_1={
	"cartridge",
	0x10000000,
	0x1FBFFFFF,
	0x1FFFFFFF,
	0,
	0,
	0,0
};

static mips_memory_area cart_memory_area_2={
	"cartridge",
	0x08000000,
	0x0FFFFFFF,
	0x1FFFFFFF,
	0,
	0,
	0,0
};

static mips_memory_area cart_memory_area_3={
	"cartridge",
	0x06000000,
	0x07FFFFFF,
	0x1FFFFFFF,
	0,
	0,
	0,0
};

static mips_memory_area cart_memory_area_4={
	"cartridge",
	0x05000000,
	0x05FFFFFF,
	0x1FFFFFFF,
	0,
	0,
	0,0
};

static bool n64_check_cardtridge()
{
	if(n64_cart.length>=(0x100000L+0x1000)) 
	{
		uint32 c1, k1, k2;
		uint32 t1, t2, t3, t4;
		uint32 t5, t6;
		uint32 sum1, sum2;

		t1 = 0xf8ca4ddc;
		t2 = 0xf8ca4ddc;
		t3 = 0xf8ca4ddc;
		t4 = 0xf8ca4ddc;
		t5 = 0xf8ca4ddc;
		t6 = 0xf8ca4ddc;

		for(uint32 c=0x1000;c<(0x100000L+0x1000);c+=4) 
		{
			c1 = swap_uint32(*((uint32 *)&n64_cart.image[c]));
			k1 = t6 + c1;
			if( k1 < t6 ) t4++;
			t6 = k1;
			t3 ^= c1;
			k2 = c1 & 0x1f;
			k1 = ((c1<<k2) | (c1>>(32-k2)));
			t5 += k1;
			if( c1 < t2 )
				t2 ^= k1;
			else
				t2 ^= t6 ^ c1;
			t1 += c1 ^ t5;
		}
		
		sum1 = t6 ^ t4 ^ t3;
		sum2 = t5 ^ t2 ^ t1;
		
		if(
			(swap_uint32(n64_cart.header->crc1) == sum1)||
			(swap_uint32(n64_cart.header->crc2) == sum2))
		{
			printf("Checksum ok.\n");
		    return true;
		}
	}
	printf("Wrong checksum or file too short.\n");
	return false;
}

void n64_get_cardtrige_ptr(uint32 addr, uint8 **ptr, int32 *len) 
{
	     if((addr >= cart_memory_area_0.start) &&  (addr <= cart_memory_area_0.end))  {
		*ptr = n64_cart.image + addr - cart_memory_area_0.start;
		*len = cart_memory_area_0.end - addr;
	}
	else if((addr >= cart_memory_area_1.start) &&  (addr <= cart_memory_area_1.end)) {
		*ptr = n64_cart.image + addr - cart_memory_area_1.start;
		*len = cart_memory_area_1.end - addr;
	}
	else if((addr >= cart_memory_area_2.start) &&  (addr <= cart_memory_area_2.end)) {
		*ptr = n64_cart.image + addr - cart_memory_area_2.start;
		*len = cart_memory_area_2.end - addr;
	}
	else if((addr >= cart_memory_area_3.start) &&  (addr <= cart_memory_area_3.end)) {
		*ptr = n64_cart.image + addr - cart_memory_area_3.start;
		*len = cart_memory_area_3.end - addr;
	}
	else if((addr >= cart_memory_area_4.start) &&  (addr <= cart_memory_area_4.end)) {
		*ptr = n64_cart.image + addr - cart_memory_area_4.start;
		*len = cart_memory_area_4.end - addr;
	}
}

bool n64_load_cardtrige(const char *filename)
{
	FILE *fp;
	if((fp=fopen(filename,"r"))!=0)
	{
        fseek(fp,0,SEEK_END);
        if((n64_cart.length = ftell(fp))>0)
        {
	        if((n64_cart.image = new uint8[n64_cart.length])!=0)
	        {
				fseek(fp,0,SEEK_SET);
				fread(n64_cart.image,1,n64_cart.length,fp);

	     		n64_cart.header = (n64_cartridge_header *)n64_cart.image;
				n64_cart.bootcode = n64_cart.image + 0x40;
				n64_cart.gamecode = n64_cart.image + 0x1000;

				if((n64_cart.image[0]==0x37) && (n64_cart.image[1]==0x80))
				{
					for(uint32 i=0;i<n64_cart.length;i+=4)
					{
						uint8 a = n64_cart.image[i+0];
						uint8 b = n64_cart.image[i+1];
						uint8 c = n64_cart.image[i+2];
						uint8 d = n64_cart.image[i+3];
						n64_cart.image[i+0] = b;
						n64_cart.image[i+1] = a;
						n64_cart.image[i+2] = d;
						n64_cart.image[i+3] = c;
					}
				}
				if(n64_check_cardtridge())
				{
					printf(
							"--------------------------------------------------------------\n"
							"CART:\n"
							"--------------------------------------------------------------\n"
							"Cartridge Name: %s\n"
							"Compressed:  %08x ClockRate:  %08x PC:      %08x\n"
							"Release:     %08x CRC1:       %08x CRC2:    %08x\n"
							"CartridgeID: %08x ManufactID: %c        Country: %c\n"
							"--------------------------------------------------------------\n",
							n64_cart.header->name,
							(int)n64_cart.header->compression,
							(int)swap_uint32(n64_cart.header->clockrate),
							(int)swap_uint32(n64_cart.header->programcounter),
							(int)swap_uint32(n64_cart.header->release),
							(int)swap_uint32(n64_cart.header->crc1),
							(int)swap_uint32(n64_cart.header->crc2),
							(int)swap_uint16(n64_cart.header->cartridge),
							n64_cart.header->manufactor,
							n64_cart.header->countrycode
	
					);

					cart_memory_area_0.end 		 = MIN(cart_memory_area_0.end,cart_memory_area_0.start+n64_cart.length-1);
					cart_memory_area_0.readdata  = n64_cart.image;
					cart_memory_area_0.writedata = n64_cart.image;
					mips_add_memory_area(&cart_memory_area_0);

					cart_memory_area_1.end 		 = MIN(cart_memory_area_1.end,cart_memory_area_1.start+n64_cart.length-1);
					cart_memory_area_1.readdata  = n64_cart.image;
					cart_memory_area_1.writedata = n64_cart.image;
					mips_add_memory_area(&cart_memory_area_1);

					cart_memory_area_2.end 		 = MIN(cart_memory_area_2.end,cart_memory_area_2.start+n64_cart.length-1);
					cart_memory_area_2.readdata  = n64_cart.image;
					cart_memory_area_2.writedata = n64_cart.image;
					mips_add_memory_area(&cart_memory_area_2);

					cart_memory_area_3.end 		 = MIN(cart_memory_area_3.end,cart_memory_area_3.start+n64_cart.length-1);
					cart_memory_area_3.readdata  = n64_cart.image;
					cart_memory_area_3.writedata = n64_cart.image;
					mips_add_memory_area(&cart_memory_area_2);

					cart_memory_area_4.end 		 = MIN(cart_memory_area_4.end,cart_memory_area_4.start+n64_cart.length-1);
					cart_memory_area_4.readdata  = n64_cart.image;
					cart_memory_area_4.writedata = n64_cart.image;
					mips_add_memory_area(&cart_memory_area_2);
					
					for(int32 c=0;c<0x1000;c++) {
						n64_memory.spdmem[c] = n64_cart.image[c];
					}
	
					fclose(fp);
					return true;
				}
			}
		}
		fclose(fp);
	}
	return false;
}
