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

.align 5          //align to 2~5=32
arm_vector_table:
//  the order is fixed and cannot to modify
    ldr pc, =arm_reset_exception
    ldr pc, =arm_undefined_inst_exception
// !@ step 5: hardware auto
    ldr pc, =arm_swi_syscall
    ldr pc, =arm_prefetch_abort_exception
    ldr pc, =arm_data_abort_exception
    ldr pc, =arm_reset_exception
    ldr pc, =arm_irq_exception
    ldr pc, =arm_fiq_exception

arm_irq_exception:

.global mon_stack_limit
.bss
.align 2   //align to  2~2=4
mon_stack:
	.space (1 << 16) //64k stack
mon_stack_limit:
