#include <stdint.h>
#define TIMER_BASE 0x01c20c00
void printf(const char *,...);
#define __REG32(x)						(*(volatile unsigned int *)(x))

#define REALVIEW_SCTL_BASE         		0x10001000  /* System Controller */
#define REALVIEW_REFCLK                 0
#define TIMER_HW_BASE                   0x10012000

#define TIMER_LOAD(hw_base)             __REG32(hw_base + 0x00)
#define TIMER_VALUE(hw_base)            __REG32(hw_base + 0x04)
#define TIMER_CTRL(hw_base)             __REG32(hw_base + 0x08)
#define TIMER_CTRL_ONESHOT              (1 << 0)
#define TIMER_CTRL_32BIT                (1 << 1)
#define TIMER_CTRL_DIV1                 (0 << 2)
#define TIMER_CTRL_DIV16                (1 << 2)
#define TIMER_CTRL_DIV256               (2 << 2)
#define TIMER_CTRL_IE                   (1 << 5)        /* Interrupt Enable (versatile only) */
#define TIMER_CTRL_PERIODIC             (1 << 6)
#define TIMER_CTRL_ENABLE               (1 << 7)

#define TIMER_INTCLR(hw_base)           __REG32(hw_base + 0x0c)
#define TIMER_RIS(hw_base)              __REG32(hw_base + 0x10)
#define TIMER_MIS(hw_base)              __REG32(hw_base + 0x14)
#define TIMER_BGLOAD(hw_base)           __REG32(hw_base + 0x18)

#define SYS_CTRL                        __REG32(REALVIEW_SCTL_BASE)

#define IRQ_PBA8_TIMER2_3               35

void timer_init(void) {
	uint32_t val;

	SYS_CTRL |= REALVIEW_REFCLK;

	/* Setup Timer0 for generating irq */
	val = TIMER_CTRL(TIMER_HW_BASE);
	val &= ~TIMER_CTRL_ENABLE;
	val |= (TIMER_CTRL_32BIT | TIMER_CTRL_PERIODIC | TIMER_CTRL_IE);
	//val |= (TIMER_CTRL_32BIT | TIMER_CTRL_PERIODIC);
	TIMER_CTRL(TIMER_HW_BASE) = val;

	TIMER_LOAD(TIMER_HW_BASE) = 1000000;

	/* enable timer */
	TIMER_CTRL(TIMER_HW_BASE) |= TIMER_CTRL_ENABLE;

	//rt_hw_interrupt_umask(IRQ_PBA8_TIMER2_3);
}

unsigned int timer_get(void) {
	return TIMER_VALUE(TIMER_HW_BASE);
}

void timer_clear_pending(void) {
	TIMER_INTCLR(TIMER_HW_BASE) = 0x01;
}
