#include "all.h"
#include "stdint.h"
#include "stdarg.h"

void caller_printf_c(void);
extern void caller_printf_a(void);

void caller_printf_add_c(void);
extern void caller_printf_add_a(void);

uint32_t add_c(uint32_t i, ...);
void caller_add_c(void);

extern uint32_t add_a(uint32_t i, ...);
extern void caller_add_a(void);

// 求和
uint32_t add_c(uint32_t i, ...) {

}

// 编译器自动
void caller_printf_c(void) {
    int buf[5];
    for (int i = 0; i < 5; i++) {
        buf[i] = i;
    }
    printf("caller by c: %d, %d, %d, %d, %d\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
}

void fun(void) {
    add_a(6, 1, 1, 1, 1, 1, 0);
}
void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");
     caller_printf_a();
     caller_printf_c();

     i = add_a(6, 1, 2, 3, 4, 5, 6);
     printf("add求和结果：%d\n", i);
     caller_add_a();
}
