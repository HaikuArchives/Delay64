/*
	Delay64 V0.00 - The slow N64 emulator
	Copyright (C) 1999 Tinic Uro <turo@beatware.com>
*/
#include <AppKit.h>
#include <stdio.h>
#include "mips_opcode.h"
#include "mips_gpr.h"
#include "mips_cop.h"
#include "mips_debugger.h"
#include "mips_step.h"
#include "n64_step_rsp.h"
#include "n64_rsp.h"
#include "n64_memory.h"
#include "n64_cart.h"
#include "n64_interrupt.h"
#include "n64_screen.h"

extern void (*testasm_start)(void);
extern void (*testasm_end)(void);

void testasm()
{
asm(
	"testasm_start:\n"
	"\tpusha\n\t"
);
	uint64 ret = 24;
	uint32 rerew = 45;
	ret = 24*rerew;
	ret*=2;
asm(
	"\tpopa\n"
	"testasm_end:\n"
);
}

class MyApp : public BApplication
{

		   void Thread()
		   		{
					for(;;) {
						if(!gpr_reg.hlt) {
							mips_step();
						}
						if(!rsp_reg.hlt) {
							n64_step_rsp();
						}
					}
		   		}

	static void ThreadCaller(MyApp *app)
				{
					app->Thread();
				}
				
	thread_id	thread;


	public:
				MyApp():BApplication("application/x-vnd.Nin64")
				{
				}

		
		void 	ReadyToRun()
				{
					mips_init_memory();
					mips_init_gpr_regs();
					mips_init_cop_regs();
					mips_init_interrupts();
					
					n64_init_memory();
					n64_init_interrupts();
					n64_init_rsp_regs();
					n64_init_screen();
					

					printf("!!! %p\n",&testasm_end-&testasm_start);
					
					if(n64_load_cardtrige("SPICEROT.V64")) {
						mips_init_debugger();
						resume_thread(thread = spawn_thread((thread_entry)ThreadCaller,"Thread",B_NORMAL_PRIORITY,this));
					}
					else {
						PostMessage(B_QUIT_REQUESTED);
					}
				}
				
		bool	QuitRequested()
				{
					kill_thread(thread);
					mips_exit_memory();
					return true;
				}
				
};

int main(int argc, char **argv)
{
	MyApp().Run();
	return 0;
}
