#include "all.h"

void move(int count_c, int n, char src, char dst);
void hanoi_c(int n, char src, char mid, char dst);
extern void hanoi_a(int n, char src, char mid, char dst);
extern void madai(void (*pfunc)(int x), int i);

void move(int count_c, int n, char src, char dst) {
    printf("第%d次移动：将%d号圆盘从%c移到%c\n", count_c, n, src, dst);
}

int count_c = 0;
int count_a = 0;

void hanoi_c(int n, char src, char mid, char dst) {
    if (n == 1) {
        move(++count_c, 1, src, dst);
    } else {
        hanoi_c(n-1, src, dst, mid);
        move(++count_c, n, src, dst); // 第ｎ号盘，Ａ->Ｃ
        hanoi_c(n-1, mid, src, dst);
    }
}

void func(int i) {
    printf("麻袋试验%d\n", i);
}

void main(int argc, char *argv[]) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    printf("以下是Ｃ函数：\n");
    hanoi_c(3, 'A', 'B', 'C');
    printf("以下是汇编函数：\n");
    hanoi_a(3, 'A', 'B', 'C');

    madai(func, 8);
}
