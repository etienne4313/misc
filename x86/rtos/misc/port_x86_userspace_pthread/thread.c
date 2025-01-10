#include <ucos_ii.h>

int nNumThreadsCreated=0;
int nNumThreadsStarted=0;

pthread_cond_t	cvThreadWrapper;
pthread_cond_t	grcvThread[ OS_LOWEST_PRIO ];
pthread_t	threadTask[ OS_LOWEST_PRIO ];
pthread_mutex_t 	mutThread;

void (*pF)(char*);

static void ThreadWrapper(void* pTaskInfo)
{
	/* Grab thread index */
	int nThreadIdx = (( FuncInfo* )( pTaskInfo ))->nThreadIdx;

	/* Wait until task switching mutex is available */
	pthread_mutex_lock( &mutThread );

	/* Increment the number of task wrappers that have started */
	nNumThreadsStarted++;

	/* Signal if all wrappers have started */
	if( nNumThreadsCreated == nNumThreadsStarted )
		pthread_cond_signal( &cvThreadWrapper );

	/* Wait on this threads condition variable and unlock task switching mutex */
	pthread_cond_wait( &grcvThread[  nThreadIdx  ], &mutThread );
	pthread_mutex_unlock( &mutThread );

	pF = (( FuncInfo* )( pTaskInfo ))->pFunc;
	( *pF )( (( FuncInfo* )( pTaskInfo ))->pArgs );
}

/* OSTaskCreate -> OSTaskStkInit */
OS_STK* OSTaskStkInit (void (*task)(void* pd), void* pdata, OS_STK* ptos, INT16U opt)
{
	FuncInfo* pFuncInfo = (FuncInfo*)((char *)ptos - STACK_SIZE + 1);
	pFuncInfo->pFunc = task;
	pFuncInfo->pArgs = pdata;
	return ((OS_STK*)pFuncInfo);
}

/* OSTaskCreate -> OS_TCBInit -> OSTCBInitHook */
void OSTCBInitHook(OS_TCB *ptcb)
{
	/* Grab task info that was previously stored on task stack */
	FuncInfo* pFuncInfo = (FuncInfo*)ptcb->OSTCBStkPtr;

	/* Store index of thread and condition variable on stk for use in ctx switching */
	pFuncInfo->nThreadIdx = nNumThreadsCreated;
	nNumThreadsCreated++;

	/* Initialize thread condition variable and create thread */
	pthread_cond_init(&grcvThread[ pFuncInfo->nThreadIdx ], NULL);
	pthread_create(&threadTask[ pFuncInfo->nThreadIdx ], NULL, (void*)&ThreadWrapper, (void*)(pFuncInfo) );
}

void thread_init(void)
{
	pthread_mutex_init(&mutThread, NULL);
	pthread_cond_init (&cvThreadWrapper, NULL);
}
