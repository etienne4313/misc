/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                    Copyright 1992-2021 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*
*
* Filename : os_cpu.h
* Version  : V2.93.01
*********************************************************************************************************
*/

#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>


#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

extern int nNumThreadsCreated;
extern int nNumThreadsStarted;
extern pthread_cond_t	cvThreadWrapper;
extern pthread_cond_t	grcvThread[ OS_LOWEST_PRIO ];
extern pthread_t	threadTask[ OS_LOWEST_PRIO ];
extern pthread_mutex_t 	mutThread;
extern void thread_init(void);
extern void time_init(void);
extern void do_tick(void);

typedef struct FuncInfoType
{
	void*	pFunc;
	void*	pArgs;
	int	nThreadIdx;
} FuncInfo;

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                            */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                            */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                            */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                            */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                            */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                            */
typedef float          FP32;                     /* Single precision floating point                     */

typedef unsigned long  OS_STK;                   /* Each stack entry is 32-bit wide                     */
#define	OS_STK_GROWTH  1u                        /* Stack grows from HIGH to LOW                        */

#define  OS_CRITICAL_METHOD    3 /* Save IRQ flag in cpu_sr */
typedef unsigned long OS_CPU_SR;
#define	OS_ENTER_CRITICAL() { cpu_sr = cpu_sr ; }
#define	OS_EXIT_CRITICAL() { cpu_sr = cpu_sr ; }

#define MSEC_PER_SEC 1000u
#define OS_TICKS_PERIOD         (MSEC_PER_SEC/OS_TICKS_PER_SEC)

#define PRINT printf
#define DELAY usleep
#define STACK_SIZE 128

/********************************************************************************************************
 *                                START HIGHEST PRIORITY TASK READY-TO-RUN
 *
 * Description : This function is called by OSStart() to start the highest priority task that was created
 * by your application before calling OSStart().
 */
void OSStartHighRdy(void);

/********************************************************************************************************
 *                                       TASK LEVEL CONTEXT SWITCH
 *
 * Description : This function is called when a task makes a higher priority task ready-to-run.
 *
 * Note(s)     : 1) Upon entry,
 *                  OSTCBCur     points to the OS_TCB of the task to suspend
 *                  OSTCBHighRdy points to the OS_TCB of the task to resume
 */
void OS_TASK_SW(void);

/*********************************************************************************************************
 *                                INTERRUPT LEVEL CONTEXT SWITCH
 *
 * Description : This function is called by OSIntExit() to perform a context switch to a task that has
 *               been made ready-to-run by an ISR.
 *
 * Note(s)     : 1) Upon entry,
 *                  OSTCBCur     points to the OS_TCB of the task to suspend
 *                  OSTCBHighRdy points to the OS_TCB of the task to resume
 */
void OSIntCtxSw(void);


