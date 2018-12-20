#include "all.h"

int task1_stack[15];
int task2_stack[15];

void mt_task_init(void *task, int *task_stack, int stack_size);
void mt_schedule_start(void);
void mt_schedule(void);

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

    mt_task_init(task1, task1_stack, sizeof(task1_stack));
    mt_task_init(task2, task2_stack, sizeof(task2_stack));

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
