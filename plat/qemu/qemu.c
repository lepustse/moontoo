#include "all.h"

void funcA(void);
void funcB(void);
void funcC(void);

void main(void) {
	int i;
	int sum = 0;

	//for (i = 1; i <= 100; i++) {
	//	sum += i;
	//}
	//printf("1 + 2 + ... + 100 = %d\n", sum);
	//printf("hello world!\n");

    funcA();
}

void funcA(void) {
    funcB();
    //printf("now in funcA, funcB's addr: 0x%x\n", funcB);
}

void funcB(void) {
    int j = 1;

    printf("now in funcB, funcA's addr: 0x%x\n", funcA);
    j++;
    funcC();
}

void funcC(void) {
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
    asm volatile ("dmb":::"memory");
}
