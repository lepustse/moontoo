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
	cps #Mode_SVC

	//nanopi的u-boot到这里时是开着mmu的
	//disable mmu
    mrc p15, 0, r0, c1, c0, 0
    bic r0, #(1 | 4)
    bic r0, #(1 << 12)
    mcr p15, 0, r0, c1, c0, 0
    dsb
	isb

    mrc p15, 0, r1, c1, c0, 1
    mov r0, #(1<<6)
    orr r1, r0
    mcr p15, 0, r1, c1, c0, 1 //enable smp

	ldr lr, =after_enable_mmu
    ldr r0, =mtbl_s1
    b enable_mmu

after_enable_mmu:
    //运行到这里的时候，应该是s_svc
    //先把bss清空
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
1:
    str r2, [r0], #4
    cmp r0, r1
    blo 1b

    cps #Mode_IRQ
    ldr sp, =irq_stack_limit

    cps #Mode_FIQ
    ldr sp, =irq_stack_limit

    cps #Mode_SVC
    ldr sp, =svc_stack_limit

    ldr r0, =arm_vector_table
    mcr p15, 0, r0, c12, c0, 0

	bl main
2:
	wfi
	b 2b

.global secondy_cpu_start
secondy_cpu_start:
	//check is cpu1
	mrc p15, 0, r1, c0, c0, 5
	and r1, #0xf
	cmp r1, #1
	bne not_1_cpu

    mrc p15, 0, r1, c1, c0, 1
    mov r0, #(1<<6)
    orr r1, r0
    mcr p15, 0, r1, c1, c0, 1 //enable smp

    ldr r0, =mtbl_s1
    ldr lr, =1f

    b enable_mmu
1:
    //clear sctlr v bit
    mrc p15, 0, r0, c1, c0, 0
	bic r0, #(1 << 13)
    mcr p15, 0, r0, c1, c0, 0

    ldr r0, =arm_vector_table
    mcr p15, 0, r0, c12, c0, 0

    cps #Mode_IRQ
    ldr sp, =irq_stack_2_limit

    cps #Mode_FIQ
    ldr sp, =irq_stack_2_limit

    cps #Mode_SVC
    ldr sp, =svc_stack_2_limit

    bl cpu_iface_init

    b second_cpu_c_start

not_1_cpu:
	wfe
	b not_1_cpu

.global cpu_irq_disable
cpu_irq_disable:
	and r0, #1
	lsl r0, #7
	mrs r1, cpsr
	mov r2, r1
	bic r2, #(1 << 7)
	orr r2, r0
	msr cpsr_cxsf, r2
	lsr r1, #7
	and r0, r1, #1
	mov pc, lr

.align 5          //align to 2~5=32
arm_vector_table:
    ldr pc, =arm_reset_exception
    ldr pc, =arm_undefined_inst_exception
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
	b .
arm_prefetch_abort_exception:
	b .
arm_data_abort_exception:
	b .
arm_irq_exception:
arm_fiq_exception:
	push {r0-r12, lr}
	bl irq_handler
	pop {r0-r12, lr}
	subs pc, lr, #4

.align 2
.global enable_mmu
enable_mmu:
    orr r0, #0x18
    mcr p15, 0, r0, c2, c0, 0 //ttbr0

    mov r0, #(1 << 5)         //PD1=1
    mcr p15, 0, r0, c2, c0, 2 //ttbcr

    mov r0, #1
    mcr p15, 0, r0, c3, c0, 0 //dacr

    //开mmu之前,先清除tlb缓存
    mov r0, #0

    mcr p15, 0, r0, c8, c7, 0
    mcr p15, 0, r0, c7, c5, 0   ;//iciallu
    mcr p15, 0, r0, c7, c5, 6   ;//bpiall

    mrc p15, 0, r0, c1, c0, 0
    orr r0, #(1 | 4)
    orr r0, #(1 << 12)
    mcr p15, 0, r0, c1, c0, 0
    dsb
    isb
    mov pc, lr

.global switch_mmu
switch_mmu:
    orr r0, #0x18
    mcr p15, 0, r0, c2, c0, 0 //ttbr0

    //清除tlb缓存
    mov r0, #0
    mcr p15, 0, r0, c8, c7, 0
    mcr p15, 0, r0, c7, c5, 0   ;//iciallu
    mcr p15, 0, r0, c7, c5, 6   ;//bpiall

    dsb
    isb
    mov pc, lr

.global flush_cache_all
flush_cache_all:
    stmfd   sp!, {r0-r12, lr}
    bl  v7_flush_dcache_all
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0       @ I+BTB cache invalidate
    dsb
    isb
    ldmfd   sp!, {r0-r12, lr}
    mov pc, lr

v7_flush_dcache_all:
    dmb                 @ ensure ordering with previous memory accesses
    mrc p15, 1, r0, c0, c0, 1       @ read clidr
    ands    r3, r0, #0x7000000      @ extract loc from clidr
    mov r3, r3, lsr #23         @ left align loc bit field
    beq finished            @ if loc is 0, then no need to clean
    mov r10, #0             @ start clean at cache level 0
loop1:
    add r2, r10, r10, lsr #1        @ work out 3x current cache level
    mov r1, r0, lsr r2          @ extract cache type bits from clidr
    and r1, r1, #7          @ mask of the bits for current cache only
    cmp r1, #2              @ see what cache we have at this level
    blt skip                @ skip if no cache, or just i-cache
    mcr p15, 2, r10, c0, c0, 0      @ select current cache level in cssr
    isb                 @ isb to sych the new cssr&csidr
    mrc p15, 1, r1, c0, c0, 0       @ read the new csidr
    and r2, r1, #7          @ extract the length of the cache lines
    add r2, r2, #4          @ add 4 (line length offset)
    ldr r4, =0x3ff
    ands    r4, r4, r1, lsr #3      @ find maximum number on the way size
    clz r5, r4              @ find bit position of way size increment
    ldr r7, =0x7fff
    ands    r7, r7, r1, lsr #13     @ extract max number of the index size
loop2:
    mov r9, r4              @ create working copy of max way size
loop3:
    orr r11, r10, r9, lsl r5        @ factor way and cache number into r11
    orr r11, r11, r7, lsl r2        @ factor index number into r11
    mcr p15, 0, r11, c7, c14, 2     @ clean & invalidate by set/way
    subs    r9, r9, #1          @ decrement the way
    bge loop3
    subs    r7, r7, #1          @ decrement the index
    bge loop2
skip:
    add r10, r10, #2            @ increment cache number
    cmp r3, r10
    bgt loop1
finished:
    mov r10, #0             @ swith back to cache level 0
    mcr p15, 2, r10, c0, c0, 0      @ select current cache level in cssr
    dsb
    isb
    mov pc, lr

.bss
.align 2   //align to  2~2=4
svc_stack:
	.space (1 << 16) //64k stack
svc_stack_limit:

irq_stack:
	.space (1 << 16) //64k stack
irq_stack_limit:


svc_stack_2:
    .space (1 << 16) //64k stack
svc_stack_2_limit:

irq_stack_2:
    .space (1 << 16) //64k stack
irq_stack_2_limit:

.data
#define DEVICE_MEM  0x10406
#define NORMAL_MEM  0x1140e
.align 14
mtbl_s1:

	//vaddr: 0x00000000
	.equ mmu_tbl_map_paddr, 0x00000000
	.rept 0x020
	.word mmu_tbl_map_paddr | DEVICE_MEM
	.equ mmu_tbl_map_paddr, mmu_tbl_map_paddr + 0x100000
	.endr

	//vaddr: 0x02000000
	.rept 0x3e0
	.word 0x0
	.endr

	//vaddr: 0x40000000
	.equ mmu_tbl_map_paddr, 0x40000000
	.rept 0x400
	.word mmu_tbl_map_paddr | NORMAL_MEM
	.equ mmu_tbl_map_paddr, mmu_tbl_map_paddr + 0x100000
	.endr

	//vaddr: 0x80000000
	.rept 0x800
	.word 0x0
	.endr
