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
	cmp r0, r1
	blo 1b

	ldr sp, =mon_stack_limit

     ldr r1, =var
     // 读
     ldr r0, [r1]
     mov r0, #3
     // 写
     str r0, [r1]

	bl main

	b .

/*
 * void hanoi_a(int n, char a, char b, char c);
 */
.global hanoi_a
hanoi_a:
    push {lr}

    cmp r0, #1
    beq 1f
    b 2f
1:
    mov r2, r1
    mov r1, r0
    ldr lr, =count_a
    ldr r0, [lr]
    add r0, r0, #1
    str r0, [lr]
    bl move
    pop {pc}
2:
    // 第一步：调用其本身
    push {r0-r3}
    sub r0, r0, #1
    mov lr, r2
    mov r2, r3
    mov r3, lr
    bl hanoi_a
    pop {r0-r3}

    // 第二步：调用move实现移动
    push {r0-r3}
    mov lr, r1
    mov r1, r0
    mov r2, lr

    ldr lr, =count_a
    ldr r0, [lr]
    add r0, r0, #1
    str r0, [lr]

    bl move 
    pop {r0-r3}

    // 第三步：再次调用其本身
    sub r0, r0, #1
    mov lr, r1
    mov r1, r2
    mov r2, lr
    bl hanoi_a

    pop {pc}

/* 
 *  void madai(void (*pfunc)(int x), int i);
 */
.global madai
madai:
    push {lr}
    mov lr, r0
    mov r0, r1
    blx lr
    pop {pc}

.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:
