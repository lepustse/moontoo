#include "all.h"

int sj(void *jmpbf[]);
void lj(void *jmpbf[], int param);

void *jmpbf[10];

void main(void) {
	int i;
	int sum = 0;
	int ret;

    //printf("jmpbf addr: 0x%x\n", *jmpbf);

	ret = sj(jmpbf);
	if (ret == 0) {
		printf("sj exit\n");
	} else {
		printf("lj exit, ret: %d\n", ret);
		while(1);
	}

    printf("jmpbf addr: 0x%x\n", *jmpbf);
    void **p = jmpbf;
    int k = 4;
    for (int j = 0; j < 10; j++) {
        printf("jmpbf%d: 0x%x\n", k, *p);
        k++; 
        p++;
    }

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

	lj(jmpbf, 100);
}
