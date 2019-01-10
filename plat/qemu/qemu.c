#include "all.h"
#include "gic_pl400.h"

extern void timer_init(void);
extern unsigned int timer_get(void);
extern void irq_enable(void);

void main(void) {
    int i;
    int sum = 0;

    for (i = 1; i <= 100; i++) {
        sum += i;
    }
    printf("1 + 2 + ... + 100 = %d\n", sum);
    printf("hello world!\n");

    initIRQController();
    timer_init();
    maskInterrupt(0, 35);
    //irq_enable();
    { 
        uint32_t cpsr;
        asm volatile ("mrs %0, cpsr\n\tbic %0,#0x80\t\nmsr cpsr_cxfs, %0":"=r"(cpsr));
    }
    while (1);
}

void irq_handle(void) {
    unsigned int cur;

    printf("hello irq\n");

    cur = timer_get();

    printf("cur:%d\n", cur);
}
