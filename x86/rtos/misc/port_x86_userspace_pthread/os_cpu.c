#include <ucos_ii.h>

void OSTaskCreateHook(OS_TCB *ptcb){}
void OSTaskDelHook (OS_TCB *ptcb){}
void OSTaskSwHook (void){}
void OSTaskStatHook (void){}
void OSTimeTickHook (void){}
void OSTaskReturnHook(OS_TCB *ptcb){}
void OSInitHookEnd (void){}
void OSSystemReset(void){}

/* OSInit -> OSInitHookBegin */
void OSInitHookBegin (void)
{
	thread_init();
	time_init();
}

/* OS_TaskIdle -> OSTaskIdleHook */
void OSTaskIdleHook (void)
{
	do_tick();
}

