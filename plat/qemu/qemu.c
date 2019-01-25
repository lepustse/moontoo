#include "all.h"

void *task_1_stask[1024];
void *task_2_stask[1024];

void *task_1_sp;
void *task_2_sp;
int task_cur_id;

void mt_task_init(int task_id, void (*func)(void), void **task_stack, int stack_size);
void mt_task_start(void);
extern void __sch_start(void **sp);

extern void __mt_schedule(void **dst, void **src);
void mt_schedule(void);

void task1(void);
void task2(void);
void abc(void);

void mt_task_init(int task_id, void (*func)(void), void **task_stack, int stack_size) {
    void **p = task_stack + stack_size / 4;

    // 满减操作
    *--p = (void *)func;// pc r15
    *--p = (void *)0xe; // lr r14
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
        task_1_sp = p;
    } else if (task_id == 2) {
        task_2_sp = p;
    } else {
        return;
    }
}

void mt_task_start(void) {
    task_cur_id = 1;
    __sch_start(&task_1_sp);
}

void mt_schedule(void) {

    if (task_cur_id == 1) {
        task_cur_id = 2;
        __mt_schedule(&task_2_sp, &task_1_sp);
    } else if (task_cur_id == 2) {
        task_cur_id = 1;
        __mt_schedule(&task_1_sp, &task_2_sp);
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

    mt_task_init(1, task1, task_1_stask, sizeof(task_1_stask));
    mt_task_init(2, task2, task_2_stask, sizeof(task_2_stask));

    mt_task_start();
}

void task1(void) {
    int loop = 0;

    printf("task1 start\n");

    while (1) {
        printf("now in task1:%d\n", ++loop);
        mt_schedule();
    }
}

void abc(void) {
    printf("now in abc\n");
    printf("sch之前的\n");
    mt_schedule();
    printf("sch之后的\n");
}

void task2(void) {
    int loop = 0;

    printf("task2 start\n");

    while (1) {
        printf("now in task2:%d\n", ++loop);
        //mt_schedule();
        abc();
    }
}

