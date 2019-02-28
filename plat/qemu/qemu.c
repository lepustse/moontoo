#include "all.h"
#include "stdint.h"

void pointer_1(void) {
    int32_t a[5] = {1, 2, 3, 4, 5};
    int32_t *ptr1 = (int32_t *)(&a + 1);
    int32_t *ptr2 = (int32_t *)((int32_t)&a[0] + 1); // 这里不懂

    printf("指针题一：%x, %x\n", ptr1[-1], *ptr2);
}

uint32_t mt_strlen(char *src) {
    if (*src == '\0')
        return 0;
    else
        return mt_strlen(src + 1) + 1;
}

#if defined(REPEAT_FIR)

#elif defined(REPEAT_SEC)

#endif
void repeat(void) {
    uint8_t buf[256];
    uint32_t val;

    for (int i = 0; i < 256; i++) {
        buf[i] = i;
        val = 1 << buf[i] / 100 | 1 << buf[i] % 100 /10 | 1 << buf[i] % 100 % 10 / 1;

        if (val == 0x6)
            printf("呆括：%d\n", buf[i]);
    }
}

void hex_2_bin(uint32_t i) {
    for (int j = 0; j < 32; j++) {
        if (i & 0x80000000)
            printf("1");
        else
            printf("0");
        i <<= 1;
    }
}

void repeat_sec(void) {
    uint8_t i = 0;

    for (int j = 0; j < 6; j++) {
        hex_2_bin(++i);
        printf("\n");
    }
}

uint8_t a[4] = {1, 2, 3, 4};
void func(uint8_t *buf, uint8_t len) {
    if (len < 1) {
        printf("\n");
        return;
    }
    else {
        printf("%d", *buf);
        return func(buf + 1, len - 1);
    }
}

void hanoi(uint8_t n, char src, char mid, char dst) {
    if (n <= 1) {
        printf("将%d号圆盘从%c盘移到%c盘\n", n, src, dst);
        return;
    } else {
        hanoi(n - 1, src, dst, mid);
        printf("将%d号圆盘从%c盘移到%c盘\n", n, src, dst);
        hanoi(n - 1, mid, src, dst);
    }
}

void main(void) {
    int32_t i;
    int32_t sum = 0;

    for (i = 1; i <= 100; i++) {
        sum += i;
    }
    printf("1 + 2 + ... + 100 = %d\n", sum);

    pointer_1();
    repeat();
    repeat_sec();
    func(a, 4);
    hanoi(3, 'A', 'B', 'C');
}
