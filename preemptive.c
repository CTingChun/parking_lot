#include <8051.h>
#include "preemptive.h"

#define SAVESTATE \
{ __asm \
push ACC \
push B \
push DPL \
push DPH \
push PSW \
clr RS1 \
clr RS0 \
__endasm; \
savedSP[ID] = SP;\
}

#define RESTORESTATE \
{ SP = savedSP[ID];\
__asm \
pop PSW \
pop DPH \
pop DPL \
pop B \
pop ACC \
__endasm; \
}

extern void main( void );

void threadManager (void) {
    for (;;) {
        if (ID == 4) ID = 0;
        else ID ++;
        if (threadBitmap[ID] > 0) break;
    }
    RESTORESTATE;
    __asm
        reti
    __endasm;
}

void myTimer0Handler (void) {
    if (cnt == 10) cnt = 0;
    else cnt ++;
    if (cnt == 0) time ++;
    SAVESTATE;
    currentID = cnt;
    tmp = ID;
    ID = managerID;
    RESTORESTATE;
    ID = tmp;
    cnt = currentID;
}

void Bootstrap( void ) {
    threadBitmap[0] = threadBitmap[1] = threadBitmap[2] = threadBitmap[3] = threadBitmap[4] = 0;
    carTime [0] = carTime [1] = carTime [2] = carTime [3] = carTime [4] = 0;
    cnt = time = 0;
    TMOD = 0;
    IE = 0x82;
    TR0 = 1;
    managerID = ThreadCreate(&threadManager);
    ID = ThreadCreate(&main);
    RESTORESTATE;
}

ThreadID ThreadCreate(FunctionPtr fp) {
    EA = 0;
    for (i=0; i<MAXTHREADS; i++) if (!threadBitmap[i]) break;
    if (i == MAXTHREADS) return -1;
    threadBitmap[i] = 1;
    tmp = SP;
    SP = 0x3F + i*0x10;
    __asm
        mov a,DPL
        mov b,DPH
        mov dptr, #_ThreadExit
        push DPL
        push DPH
        push a
        push b
        mov a, #0x00
        push a
        push a
        push a
        push a
    __endasm;
    currentID = i<<3;
    __asm
        push _currentID
    __endasm;
    savedSP[i] = SP;
    SP = tmp;
    EA = 1;
    return i;
}

void ThreadYield( void ) {
    EA = 0;
    SAVESTATE ;
    for (;;) {
        if (ID == MAXTHREADS - 1) ID = 0;
        else ID ++;
        if (threadBitmap[ID] > 0) break;
    }
    RESTORESTATE ;
    EA = 1;
}

void ThreadExit( void ) {
    EA = 0;
    threadBitmap[ID] = 0;
    __asm
        clr RS1
        clr RS0
    __endasm;
    for (;;) {
        if (ID == MAXTHREADS - 1) ID = 0;
        else ID++;
        if (threadBitmap[ID] > 0) break;
    }
    RESTORESTATE;
    EA = 1;
}

void delay (unsigned char n) {
    carTime[ID] = n + now();
    while (carTime[ID] != time) {};
}

unsigned char now (void) { return time; }