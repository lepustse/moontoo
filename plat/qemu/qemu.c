#include "all.h"

char *get_mem(void) {
    char p[] = "hello world";
    return p;
}

void test(void) {
    char *str;

    str = get_mem();
    printf("%x\n", str);
    printf("%s\n", str);
}

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);

    test();
}
