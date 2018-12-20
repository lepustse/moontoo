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

/* ----------------------- mt_schedule ----------------------- */

/*
 * void mt_task_init(void *task, int *task_stack, int stack_size);
 */
.global mt_task_init
mt_task_init:
    // 设置栈指针
    add sp, r1, r2
    // 设置lr指针，有bug
    //mov lr, r0

    // 保存到栈里
    stmfd sp, {r0-r14}
    sub sp, sp, #4
    str r0, [sp]
    sub sp, sp, #52
    mov pc, lr

/*
 * void mt_schedule_start(void);
 */
.global mt_schedule_start
mt_schedule_start:
    ldr r0, =task1_stack
    ldm r0, {r0-r14}
    mov pc, r1

    mov pc, lr

/* 
 * void mt_schedule(void);
 */
.global mt_schedule
mt_schedule:
    // 保存lr

    // 恢复task2状态，恢复对方的，目前在task1
    //ldr r0, =task2_stack
    //ldm r0, {r0-r14}
    //mov pc, lr

    ldmfd sp, {r0-r14}
    add sp, sp, #56
    mov pc, lr


/* ----------------------- mt_schedule ----------------------- */

.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:
