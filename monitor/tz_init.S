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
	mov r1, #1
	mov r2, #0
2:
	add r2, r2, r1
	add r1, r1, #1
	cmp r1, #100
	ble 2b
	mov r0, r2
	bl show_num

//	cps #Mode_IRQ
//	ldr sp, =moontu_irq
//	mrs r0, cpsr
//	and r0, #0x1f
//	bl show_num
//
//	cps #Mode_SVC
//	mrs r0, cpsr
//	and r0, #0x1f
//	bl show_num
	mov r0, #1
	mov r1, #2
	add r0, r0, r1
	push {r0}

	mov r0, #3
	mov r1, #4
	add r0, r0, r1
	pop {r1}
	add r0, r0, r1
	bl show_num

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

.global moontn_usr
.bss
.align 2   //align to  2~2=4
usr_stack:
	.space (1 << 16) //64k stack
moontn_usr:
