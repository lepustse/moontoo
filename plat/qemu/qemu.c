#include "all.h"

extern void mt_memcpy(void *dst, void *src, unsigned int size);
extern void mt_memset(void *d, int c, unsigned int size);
unsigned int _strlen(const char *s);

char src[7] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
char dst[7];
unsigned int tuz[7];

char s[4] = "tuz\0";
char *str = "never\0";

unsigned int _strlen(const char *s) {
    char *p = s;

    while (*p++);

    return (p - s - 1);
}
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

    //i = mt_strlen(str);
    i = mt_strlen("");
    printf("字符串长度：%d\n", i);

    i = _strlen(str);
    printf("end\n");
}
