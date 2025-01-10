#include <ucos_ii.h>

/* OSStart -> OSStartHighRdy */
void OSStartHighRdy(void)
{
	OSTaskSwHook();
	OSRunning = OS_TRUE;

	/* Wait until all task wrappers have started */
	pthread_mutex_lock (&mutThread);
	if( nNumThreadsCreated != nNumThreadsStarted )
		pthread_cond_wait (&cvThreadWrapper, &mutThread);
	pthread_mutex_unlock (&mutThread);

	/* Get pointer highest priority thread  */
	FuncInfo* pFuncInfo = (FuncInfo*)OSTCBCur->OSTCBStkPtr;

	/* Let highest prio thread go  */
	pthread_mutex_lock( &mutThread );
	pthread_cond_signal( &grcvThread[ pFuncInfo->nThreadIdx ] );
	pthread_mutex_unlock( &mutThread );

	/* Main thread must be kept going otherwise process turns into zombie */
	while(1) sleep(1);

}

/* OSIntEnter / OSIntExit -> OSIntCtxSw */
void OSIntCtxSw(void)
{
	/* Grab task switching mutex */
	pthread_mutex_lock( &mutThread );

	/* Get ptrs to new and old context */
	FuncInfo* pFuncInfoNew = (FuncInfo*)OSTCBHighRdy->OSTCBStkPtr;
	FuncInfo* pFuncInfoOld = (FuncInfo*)OSTCBCur->OSTCBStkPtr;

	/* Set current context to highest priority */
	OSTCBCur = OSTCBHighRdy;
	OSPrioCur = OSPrioHighRdy;

	/* Signal highest priority thread to start by setting its condition variable */
	pthread_cond_signal( &grcvThread[ pFuncInfoNew->nThreadIdx ] );

	/* Wait for this threads condition variable to change ( i.e. the next ctx switch ). */
	pthread_cond_wait( &grcvThread[ pFuncInfoOld->nThreadIdx ], &mutThread );

	/* Variable change automatically locks mutex, so unlock */
	pthread_mutex_unlock( &mutThread );
}

/* OS_Sched -> OS_TASK_SW */
void OS_TASK_SW()
{
	OSIntCtxSw();
}

