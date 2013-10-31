/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include "n64_screen.h"
#include "n64_memory.h"
#include "n64_register.h"
#include <View.h>
#include <Window.h>
#include <Bitmap.h>

class N64View;
class N64Window;

static BBitmap *n64_bitmap=0;
static N64Window *n64_window=0;
static N64View *n64_view=0;

class N64View : public BView
{
	public:
	
				N64View():BView(BRect(0,0,319,511),"N64View",0,B_WILL_DRAW)
				{
				}
				
		void	ReDraw()
				{
					if(LockLooper())
					{
						Draw(Bounds());
						UnlockLooper();
					}
				}
				
		void	Draw(BRect rect)
				{
					DrawBitmap(n64_bitmap,BPoint(0,0));
				} 
};

class N64Window : public BWindow
{

	public:
				N64Window():BWindow(BRect(680,24,680+319,24+239),"N64Screen",B_TITLED_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,0)
				{
					n64_bitmap = new BBitmap(BRect(0,0,319,239),B_RGB15_BIG);
					n64_view = new N64View();
					AddChild(n64_view);
					Show();
				}
};

void n64_init_screen()
{
	n64_window = new N64Window();
}

void n64_update_screen()
{	
	uint32 *vireg = (uint32 *)n64_memory.vireg;
	
	if(mips_host_little_endian()) {
		vireg[VI_ORIGIN_REG] = swap_uint32(vireg[VI_ORIGIN_REG]);
		vireg[VI_WIDTH_REG] = swap_uint32(vireg[VI_WIDTH_REG]);
	}

	uint8 *scr_ptr=0;
	int32 scr_len=0;
	n64_get_rdram_ptr(vireg[VI_ORIGIN_REG],&scr_ptr,&scr_len);

	if(scr_ptr && scr_len) {
		memcpy(n64_bitmap->Bits(),scr_ptr,320*240*2);
		n64_view->ReDraw();
	}

	if(mips_host_little_endian()) {
		vireg[VI_ORIGIN_REG] = swap_uint32(vireg[VI_ORIGIN_REG]);
		vireg[VI_WIDTH_REG] = swap_uint32(vireg[VI_WIDTH_REG]);
	}
}
