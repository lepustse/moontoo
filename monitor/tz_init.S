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

	mrs r0, cpsr
	and r0, #0x1f
	bl show_num
	
	ldr r0, =arm_vector_table
	mcr p15, 0, r0, c12, c0, 0

	cps #Mode_USR
	ldr sp, =moontu_usr
	mrs r0, cpsr
	and r0, #0x1f
	bl show_num

	// call svc
	// SWI: the addr of the following instraction will transfer svc lr
	//swi #0
	//bx lr
	bl swi_func

	bl show_status
	mrs r0, cpsr
	and r0, #0x1f
	bl show_num

	b .

swi_func:
	swi #0
	bx lr

.align 5          //align to 2~5=32
arm_vector_table:
//	the order is fixed and cannot to modify
    ldr pc, =arm_reset_exception
	ldr pc, =arm_undefined_inst_exception
	// !@ step 5: hardware auto
	ldr pc, =arm_swi_syscall
	ldr pc, =arm_prefetch_abort_exception
	ldr pc, =arm_data_abort_exception
	ldr pc, =arm_reset_exception
	ldr pc, =arm_irq_exception
	ldr pc, =arm_fiq_exception

arm_reset_exception:
	b .
arm_undefined_inst_exception:
	b .
arm_swi_syscall:
	mov r0, lr
	
	// SYS 模式，不需要设置栈，SYS与USR只是权限不同而已
	cps #Mode_SYS
	push {r0, lr}
	mrs r0, cpsr
	and r0, #0x1f
	bl show_num

	bl svc_handle

	pop {r0, lr}
	// s: change cpsr
	// spsr -> cpsr
	// not change cpsr, so it cannot change mode
	bx r0

arm_prefetch_abort_exception:
	b .
arm_data_abort_exception:
	b .
arm_irq_exception:
	b .
arm_fiq_exception:
    b .

.global asm_sum
asm_sum:
	add r0, r0, r1
	mov pc, lr

.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:

.global moontu_irq
.bss
.align 2   //align to  2~2=4
irq_stack:
	.space (1 << 16) //64k stack
moontu_irq:

.global moontu_usr
.bss
.align 2   //align to  2~2=4
usr_stack:
	.space (1 << 16) //64k stack
moontu_usr:

