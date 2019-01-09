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

	bl main

	b .

/* void caller_printf_a(void); */
.global caller_printf_a
caller_printf_a:
    push {lr}
    // str 地址
    ldr r0, =v4
    ldr r0, [r0]
    ldr r1, =v5
    ldr r1, [r1]
    stmfd sp!, {r0-r1}
    
    ldr r0, =str
    ldr r1, =v1
    ldr r1, [r1]
    ldr r2, =v2
    ldr r2, [r2]
    ldr r3, =v3
    ldr r3, [r3]

    bl printf
    add sp, sp, #8
    pop {pc}
    
str:
    .ascii "caller by asm: %d, %d, %d, %d, %d\n"
.data
// 何时需要对齐？
.align
v1:
    .word 1
v2:
    .word 2
v3:
    .word 3
v4:
    .word 4
v5:
    .word 5

/* uint32_t add_a(uint32_t i, ...); */
.global add_a
add_a:
    push {r1-r3}
    mov r2, #0
    mov r3, sp
1:
    cmp r0, #0
    beq 2f
    sub r0, r0, #1
    ldr r1, [r3], #4
    add r2, r1, r2
    b 1b
2:
    mov r0, r2
    pop {r1-r3}
    mov pc, lr

/* void caller_add_a(void); */
.global caller_add_a
caller_add_a:
    push {lr}
    
    ldr r0, =a4
    ldr r0, [r0]

    ldr r1, =a5
    ldr r1, [r1]

    ldr r2, =a6
    ldr r2, [r2]
    push {r0-r2}

    mov r0, #6
    ldr r1, =a1
    ldr r1, [r1]
    ldr r2, =a2
    ldr r2, [r2]
    ldr r3, =a3
    ldr r3, [r3]

    bl add_a

    mov r1, r0
    ldr r0, =xxx

    bl printf

    pop {r0-r2}
    pop {pc}
xxx:
    .ascii "caller add by asm: %d\n"
.data
.align
a1:
    .word 1
a2:
    .word 2
a3:
    .word 3
a4:
    .word 4
a5:
    .word 5
a6:
    .word 6


.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:
