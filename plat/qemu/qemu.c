#include "all.h"
#include "gic_pl400.h"

extern void timer_init(void);
extern unsigned int timer_get(void);
extern void irq_enable(void);
extern void timer_clear_pending(void);
int cnt;

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
        // irq: 0x80
        // fiq: 0x40
        asm volatile ("mrs %0, cpsr\n\tbic %0,#0x80\t\nmsr cpsr_cxfs, %0":"=r"(cpsr));
    }
    while (1);
}

void irq_handle(void) {
    unsigned int cur;
    interrupt_t cur_irq;

    cur_irq = getActiveIRQ();

    printf("hello irq\n");

    cur = timer_get();

    printf("cur:%d\n", cur);

    if (cur_irq == 35) {
        // !@清除定时器flag
        timer_clear_pending();
        printf("[time init: %d]\n", ++cnt);
    }
    // !@清除gic，不关，新的中断就不会通知
    ackInterrupt(cur_irq);
    //while (1);
}
