#include "all.h"

void* task1_stack[1024];
void* task2_stack[1024];

void* task_1_sp;
void* task_2_sp;
int   current_task_id;

void mt_task_init(int task_id, void (*task)(void), void *task_stack, int stack_size)
{
    void **stk = task_stack + stack_size;
    *--stk = (void*)task;
    *--stk = (void*)0xe; //lr
    *--stk = (void*)0xc; //r12
    *--stk = (void*)0xb; //r11
    *--stk = (void*)0xa; //r10
    *--stk = (void*)0x9; //r9
    *--stk = (void*)0x8; //r8
    *--stk = (void*)0x7; //r7
    *--stk = (void*)0x6; //r6
    *--stk = (void*)0x5; //r5
    *--stk = (void*)0x4; //r4
    *--stk = (void*)0x3; //r3
    *--stk = (void*)0x2; //r2
    *--stk = (void*)0x1; //r1
    *--stk = (void*)0x0; //r0
    if (task_id == 1) {
        task_1_sp = stk;
    } else if (task_id == 2) {
        task_2_sp = stk;
    } else {
        return;
    }
}

extern void switch_to(void** p_sp);

void mt_schedule_start(void)
{
    current_task_id = 1;
    switch_to(&task_1_sp);
}

extern void _schedule(void** p_sp_from, void** p_sp_to);

void mt_schedule(void)
{
    if (current_task_id == 1) {
        current_task_id = 2;
        _schedule(&task_1_sp, &task_2_sp);
    } else {
        current_task_id = 1;
        _schedule(&task_2_sp, &task_1_sp);
    }
}

void task1(void);
void task2(void);

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    mt_task_init(1, task1, task1_stack, sizeof(task1_stack)/sizeof(void*));
    mt_task_init(2, task2, task2_stack, sizeof(task2_stack)/sizeof(void*));

    mt_schedule_start();
}

void task1(void) {
    int loop = 0;

    printf("task1 start\n");
    while (1) {
        printf("now in task1\n");
        printf("task1 loop %d\n", ++loop);
        mt_schedule();
    }
}

void task2(void) {
    int loop = 0;

    printf("task2 start\n");
    while (1) {
        printf("now in task2\n");
        printf("task2 loop %d\n", ++loop);
        mt_schedule();
    }
}
