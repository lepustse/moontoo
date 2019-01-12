#define  Mode_USR       0x10
#define  Mode_FIQ       0x11
#define  Mode_IRQ       0x12
#define  Mode_SVC       0x13
#define  Mode_MON       0x16
#define  Mode_ABT       0x17
#define  Mode_UDF       0x1B
#define  Mode_SYS       0x1F

#define A_Bit        0x100 
#define I_Bit        0x80 @; when I bit is set, IRQ is disabled
#define F_Bit        0x40 @; when F bit is set, FIQ is disabled
#define T_Bit        0x20

.global tz_init
.section ".text.init", "x"
tz_init:
	//运行到这里的时候，应该是s_svc
	//先把bss清空
	ldr r0, =__bss_start
	ldr r1, =__bss_end
	mov r2, #0
1:
	str r2, [r0], #4
	cmp r0, r1 // r0 == r1
	blo 1b // 小于

	ldr sp, =mon_stack_limit

	bl main

	b .

/*
 * void mt_memcpy(void *dst, const void *src, unsigned int size);
 */
.global mt_memcpy
mt_memcpy:
    cmp r2, #0
    beq 2f
1:
    ldrb r3, [r1], #1
    strb r3, [r0], #1
    subs r2, r2, #1
    bne 1b
2:
    mov pc, lr

/*
 * void mt_memset(void *d, int c, unsigned int size);
 */
.global mt_memset
mt_memset:
    cmp r2, #0
    beq 2f
1:
    strb r1, [r0], #1
    subs r2, r2, #1
    bne 1b
2:
    mov pc, lr

/*
 * char *mt_strcpy(char *dst, const char *src);
 */

/* unsigned int mt_strlen(const char *s); */
.global mt_strlen
mt_strlen:
    mov r1, r0
    mov r3, r0
1:
    ldrb r2, [r1], #1
    cmp r2, #0
    beq 2f
    b 1b
2:
    sub r0, r1, r3
    sub r0, #1
    mov pc, lr

.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:
