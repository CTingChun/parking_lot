/*
 * file: preemptive.h
 *
 * this is the include file for the preemptive multithreading
 * package.  It is to be compiled by SDCC and targets the EdSim51 as
 * the target architecture.
 *
 * CS 3423 Fall 2018
 */

#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define CNAME(s) _ ## s
#define LABNAME(lab) lab ## $

#define MAXTHREADS 5
__idata __at (0x10) char buffer[10]; // 10-19
__idata __at (0x1E) char savedSP[MAXTHREADS];
__idata __at (0x23) char threadBitmap[MAXTHREADS]; // 23-27
__idata __at (0x28) char ID;
__idata __at (0x29) char tmp;
__idata __at (0x2A) char i;
__idata __at (0x2B) char currentID;
__idata __at (0x2C) char managerID;
__idata __at (0x35) char tag;
__idata __at (0x36) char j;
//__idata __at (0x37) char produce;
//__idata __at (0x38) char consume;
__idata __at (0x39) char total;
__idata __at (0x3A) char create;
__idata __at (0x3B) char cnt;
__idata __at (0x3C) char time;
__idata __at (0x49) char carTime[MAXTHREADS];

void delay(unsigned char n); // delays the thread by n time units
// returns the “current time” 
// i.e., number of time units that has elapsed since the system reboots
unsigned char now(void);
typedef char ThreadID;
typedef void (*FunctionPtr)(void);

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);

#define SemaphoreWaitBody(S, lab) \
{ __asm \
    LABNAME(label): \
    mov ACC, CNAME(S)\
    JB ACC.7, LABNAME(lab)\
    dec  CNAME(S) \
__endasm; }
#define SemaphoreCreate(s, n) s = n;
// create a counting semaphore s initialized to n
#define SemaphoreWait(s) SemaphoreWaitBody(s, __COUNTER__);
// do busy-wait on semaphore s
#define SemaphoreSignal(s) {__asm INC CNAME(s) __endasm;}
// signal a semaphore s
#endif // __COOPERATIVE_H__

