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

// int sj(void *jmpbf[])
// 返回0 或 非0
// 如果lj的参数是0，要强制变成1
.global sj
sj:
	// 保存reg
	stm r0, {r1-lr}
	// 返回0
	mov r0, #0
	mov pc, lr

// void lj(void *jmpbf[], int param)
// param: 按照此值返回，如果是0，强制变成1
.global lj
lj:
	// 调换r0 r1的值
	// r0: buff
	// r1: param
	// todo: 如果param为0，需要强制变成1
	eor r0, r0, r1
	eor r1, r0, r1
	eor r0, r0, r1

	ldm r1, {r1-r13, lr}
	mov pc, lr

.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:
