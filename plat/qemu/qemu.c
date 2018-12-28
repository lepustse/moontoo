#include "all.h"

void move(int count, int disks, char n, char m); 
void hanoi(int n, char a, char b, char c);

void move(int count, int disks, char n, char m) {
    printf("第%d次移动：把%d号圆盘从%c移到%c\n", count, disks, n, m);
}

int count = 0;

void hanoi(int n, char a, char b, char c) {

    if (n == 1) {
        move(++count, 1, a, c);
    } else {
        hanoi(n-1, a, c, b);
        move(++count, n, a, c);
        hanoi(n-1, b, a, c);
    }
}

void main(int argc, char *argv[]) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

    hanoi(3, 'A', 'B', 'C');
}
