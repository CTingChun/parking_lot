#include <8051.h>
#include "preemptive.h"

void Producer1 (void) {
    buffer[0] = time;
    delay(1);
    buffer[1] = time;
    total --;
    ThreadExit();
}
void Producer2 (void) {
    buffer[2] = time;
    delay(1);
    buffer[3] = time;
    total --;
    ThreadExit();
}
void Producer3 (void) {
    buffer[4] = time;
    delay(1);
    buffer[5] = time;
    total --;
    ThreadExit();
}
void Producer4 (void) {
    buffer[6] = time;
    delay(1);
    buffer[7] = time;
    total --;
    ThreadExit();
}
void Producer5 (void) {
    buffer[8] = time;
    delay(1);
    buffer[9] = time;
    total --;
    ThreadExit();
}
void Consumer( void ) {
    TMOD |= 0x20;
    TH1 = (char)-6;
    SCON = 0x50;
    TR1 = 1;
    for (j=0; j<10; j++) {
        delay(1);
        SBUF = 'c';
        while (!TI) {}
        TI = 0;
        SBUF = 'a';
        while (!TI) {}
        TI = 0;
        SBUF = 'r';
        while (!TI) {}
        TI = 0;
        SBUF = '1' + j/2;
        while (!TI) {}
        TI = 0;
        SBUF = ' ';
        while (!TI) {}
        TI = 0;
        if (j % 2 == 1) {
            SBUF = 'l';
            while (!TI) {}
            TI = 0;
            SBUF = 'e';
            while (!TI) {}
            TI = 0;
            SBUF = 'f';
            while (!TI) {}
            TI = 0;
            SBUF = 't';
            while (!TI) {}
            TI = 0;
        }
        else {
            SBUF = 'g';
            while (!TI) {}
            TI = 0;
            SBUF = 'e';
            while (!TI) {}
            TI = 0;
            SBUF = 't';
            while (!TI) {}
            TI = 0;
            SBUF = ' ';
            while (!TI) {}
            TI = 0;
            SBUF = 's';
            while (!TI) {}
            TI = 0;
            SBUF = 'p';
            while (!TI) {}
            TI = 0;
            SBUF = 'o';
            while (!TI) {}
            TI = 0;
            SBUF = 't';
            while (!TI) {}
            TI = 0;
        }
        SBUF = ' ';
        while (!TI) {}
        TI = 0;
        SBUF = ':';
        while (!TI) {}
        TI = 0;
        SBUF = ' ';
        while (!TI) {}
        TI = 0;
        SBUF = 't';
        while (!TI) {}
        TI = 0;
        SBUF = 'i';
        while (!TI) {}
        TI = 0;
        SBUF = 'm';
        while (!TI) {}
        TI = 0;
        SBUF = 'e';
        while (!TI) {}
        TI = 0;
        SBUF = ' ';
        while (!TI) {}
        TI = 0;
        SBUF = '0' + (buffer[j]);
        while (!TI) {}
        TI = 0;
        SBUF = '\n';
        while (!TI) {}
        TI = 0;
    }
}
void main( void ) {
    create = 6;
    tag = total = 0;
    while (create > 0) {
        while (total == 2) {}
        if(tag == 0) ThreadCreate(&Producer1);
        else if(tag == 1) ThreadCreate(&Producer2);
        else if(tag == 2) ThreadCreate(&Producer3);
        else if(tag == 3) ThreadCreate(&Producer4);
        else if(tag == 4) ThreadCreate(&Producer5);
        else if(tag == 5) ThreadCreate(&Consumer);
        create --;
        tag ++;
        total ++;
        ThreadYield();
    }
}

void _sdcc_gsinit_startup( void ) {
    __asm
    ljmp _Bootstrap
    __endasm ;
}
void _mcs51_genRAMCLEAR( void ) {}
void _mcs51_genXINIT( void ) {}
void _mcs51_genXRAMCLEAR( void ) {}
void timer0_ISR(void) __interrupt(1) {
    __asm
    ljmp _myTimer0Handler
    __endasm;
}