#include "all.h"

int sj(void *jmpbf[]);
void lj(void *jmpbf[], int param);

void *jmpbf[14];

void main(void) {
	int i;
	int sum = 0;
	int ret;

	ret = sj(jmpbf);
	if (ret == 0) {
		printf("sj exit\n");
	}else {
		printf("lj exit, ret: %d\n", ret);
		while(1);
	}

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

	lj(jmpbf, 100);
}
