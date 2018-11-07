#include "all.h"

unsigned int asm_sum(unsigned int x, unsigned y);

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

	sum = asm_sum(3, 2);
	printf("3 + 2 = %u\n", sum);
}

// r0
void show_num(int num)
{
	printf("num: %d\n", num);
}
