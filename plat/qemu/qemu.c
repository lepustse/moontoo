#include "all.h"

void *task1_stack[1024];
void *task2_stack[1024];

void *task1_sp;
void *task2_sp;
int current_id;

void mt_task_init(int task_id, void (*func)(void), void **task_stack, int stack_size);
void mt_task_start(void);
extern void __sch_start(void **sp);

extern void __mt_schedule(void **dst, void **src);
void mt_schedule(void);

void task1(void);
void task2(void);

void mt_task_init(int task_id, void (*func)(void), void **task_stack, int stack_size) {
    void **p = task_stack + stack_size / 4;

    // 满减操作
    *--p = (void *)func;
    *--p = (void *)0xe; // lr
    *--p = (void *)0xc; // r12
    *--p = (void *)0xb; // r11
    *--p = (void *)0xa; // r10
    *--p = (void *)9; // r9
    *--p = (void *)8; // r8
    *--p = (void *)7; // r7
    *--p = (void *)6; // r6
    *--p = (void *)5; // r5
    *--p = (void *)4; // r4
    *--p = (void *)3; // r3
    *--p = (void *)2; // r2
    *--p = (void *)1; // r1
    *--p = (void *)0; // r0

    if (task_id == 1) {
        // 满减栈
        task1_sp = p;
    } else if (task_id == 2) {
        task2_sp = p;
    } else {
        return;
    }
}

// 切换到task1，不用回来的
void mt_task_start(void) {
    __sch_start(&task1_sp);
}

void mt_schedule(void) {
    int current_id = 1;

    if (current_id == 1) {
        __mt_schedule(&task1_sp, &task2_sp);
    } else {
        __mt_schedule(&task2_sp, &task1_sp);
    }
}

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    mt_task_init(1, task1, task1_stack, sizeof(task1_stack));
    mt_task_init(2, task2, task2_stack, sizeof(task2_stack));

    mt_task_start();
}

void task1(void) {
    int loop = 0;

    printf("task1 start\n");

    while (1) {
        printf("now in task1:%d\n", ++loop);
        // dst, src
        // 1 -> 2
        __mt_schedule(&task2_sp, &task1_sp);
    }
}

void task2(void) {
    int loop = 0;

    printf("task2 start\n");

    while (1) {
        printf("now in task2:%d\n", ++loop);
        // dst, src
        // 2 -> 1
        __mt_schedule(&task1_sp, &task2_sp);
    }
}

