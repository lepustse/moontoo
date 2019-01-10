#include "all.h"

void main(void) {
	int i;
	int sum = 0;
    unsigned int cur;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    timer_init();

    while (1) {
        cur = timer_get();

        printf("cur:%d\n", cur);
    }
}


