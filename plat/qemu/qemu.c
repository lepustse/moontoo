#include "all.h"

extern void mt_memcpy(void *dst, void *src, unsigned int size);

char src[7] = {1, 2, 3, 4, 5, 6, 7};
char dst[7];

char s[4] = "tuz\0";

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    mt_memcpy(dst, src, 0);
    for (i = 0; i < 7; i++) {
        printf("dst[%d]:%d\n", i, dst[i]);
    }
    printf("end\n");
}
