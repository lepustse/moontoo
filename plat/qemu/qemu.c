#include "all.h"

extern void mt_memcpy(void *dst, void *src, unsigned int size);
extern void mt_memset(void *d, int c, unsigned int size);

char src[7] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
char dst[7];
unsigned int tuz[7];

char s[4] = "tuz\0";

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    mt_memcpy(dst, src, 7);
    for (i = 0; i < 7; i++) {
        printf("dst[%d]：0x%08x\n", i, dst[i]);
    }

    printf("\n");

    mt_memset(dst, 3, 4);
    for (i = 0; i < 7; i++) {
        printf("dst[%d]：0x%x\n", i, dst[i]);
    }
    printf("end\n");
}
