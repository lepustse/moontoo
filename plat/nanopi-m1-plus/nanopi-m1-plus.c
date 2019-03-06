#include "all.h"
#include "gic_pl400.h"

#define TIMER0_IRQ 50
#define TIMER1_IRQ 51
#define UART0_IRQ  32

void flush_cache_all(void);

unsigned int readl(volatile void *addr);
void writel(unsigned int v, volatile void *addr);

void timer0_init(void);
unsigned int timer0_get(void);
void timer0_clear_pending(void);

void timer1_init(void);
unsigned int timer1_get(void);
void timer1_clear_pending(void);
void mdelay(unsigned int ms);

int cpu_irq_disable(int disable);

void NS16550_init(const int port);
char _serial_getc(const int port);

void set_irq_target(irq_t irq, int cpu);

extern void dist_ipi_send(irq_t irq, int cpu);
extern void secondy_cpu_start(void);
void cpu_iface_init(void);

#define CPUCFG_CPU_PWR_CLAMP_STATUS_REG(cpu)    ((cpu) * 0x40 + 0x64)
#define CPUCFG_CPU_RST_CTRL_REG(cpu)        (((cpu) + 1) * 0x40)
#define CPUCFG_CPU_CTRL_REG(cpu)        (((cpu) + 1) * 0x40 + 0x04)
#define CPUCFG_CPU_STATUS_REG(cpu)      (((cpu) + 1) * 0x40 + 0x08)
#define CPUCFG_GEN_CTRL_REG         0x184
#define CPUCFG_PRIVATE0_REG         0x1a4
#define CPUCFG_PRIVATE1_REG         0x1a8
#define CPUCFG_DBG_CTL0_REG         0x1e0
#define CPUCFG_DBG_CTL1_REG         0x1e4

#define PRCM_CPU_PWROFF_REG         0x100
#define PRCM_CPU_PWR_CLAMP_REG(cpu)     (((cpu) * 4) + 0x140)

#define BIT(n)                      (1 << n)

static void *cpucfg_membase = (void*)0x01f01c00;
static void *prcm_membase = (void*)0x01f01400;

int sun6i_smp_boot_secondary(unsigned int cpu)
{
    unsigned int reg;
    int i;

    /* Set CPU boot address */
    writel((unsigned int)secondy_cpu_start,
           cpucfg_membase + CPUCFG_PRIVATE0_REG);

    /* Assert the CPU core in reset */
    writel(0, cpucfg_membase + CPUCFG_CPU_RST_CTRL_REG(cpu));

    /* Assert the L1 cache in reset */
    reg = readl(cpucfg_membase + CPUCFG_GEN_CTRL_REG);
    writel(reg & ~BIT(cpu), cpucfg_membase + CPUCFG_GEN_CTRL_REG);

    /* Disable external debug access */
    reg = readl(cpucfg_membase + CPUCFG_DBG_CTL1_REG);
    writel(reg & ~BIT(cpu), cpucfg_membase + CPUCFG_DBG_CTL1_REG);

    /* Power up the CPU */
    for (i = 0; i <= 8; i++)
        writel(0xff >> i, prcm_membase + PRCM_CPU_PWR_CLAMP_REG(cpu));
    mdelay(10);

    /* Clear CPU power-off gating */
    reg = readl(prcm_membase + PRCM_CPU_PWROFF_REG);
    writel(reg & ~BIT(cpu), prcm_membase + PRCM_CPU_PWROFF_REG);
    mdelay(1);

    /* Deassert the CPU core reset */
    writel(3, cpucfg_membase + CPUCFG_CPU_RST_CTRL_REG(cpu));

    /* Enable back the external debug accesses */
    reg = readl(cpucfg_membase + CPUCFG_DBG_CTL1_REG);
    writel(reg | BIT(cpu), cpucfg_membase + CPUCFG_DBG_CTL1_REG);

    __asm__ volatile ("dsb":::"memory");

    return 0;
}

typedef struct {
    union {
        unsigned long slock;
        struct __raw_tickets {
            unsigned short owner;
            unsigned short next;
        } tickets;
    };
} raw_spinlock_t;

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	unsigned long tmp;
	unsigned long newval;
	raw_spinlock_t lockval;

	__asm__ __volatile__(
		"pld [%0]"
		::"r"(&lock->slock)
	);

	__asm__ __volatile__(
			"1: ldrex   %0, [%3]\n"
			"   add %1, %0, %4\n"
			"   strex   %2, %1, [%3]\n"
			"   teq %2, #0\n"
			"   bne 1b"
			: "=&r" (lockval), "=&r" (newval), "=&r" (tmp)
			: "r" (&lock->slock), "I" (1 << 16)
			: "cc");

	while (lockval.tickets.next != lockval.tickets.owner) {
		__asm__ __volatile__("wfe":::"memory");
		lockval.tickets.owner = *(volatile unsigned short *)(&lock->tickets.owner);
	}

	__asm__ volatile ("dmb":::"memory");
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	__asm__ volatile ("dmb":::"memory");
	lock->tickets.owner++;
	__asm__ volatile ("dsb ishst\nsev":::"memory");
}

raw_spinlock_t gic_lock = {.slock = 0};
raw_spinlock_t share_lock = {.slock = 0};

volatile unsigned int share_var = 0;
unsigned int cnt_cpu0 = 0;
unsigned int cnt_cpu1 = 0;

void main(void) {
	int i;
	int sum = 0;

	printf("hello world!\n");

	flush_cache_all();

	__raw_spin_lock(&gic_lock);
	initIRQController();
	__raw_spin_unlock(&gic_lock);

	timer0_init();
	__raw_spin_lock(&gic_lock);
	maskInterrupt(0, TIMER0_IRQ);
	__raw_spin_unlock(&gic_lock);

	sun6i_smp_boot_secondary(1);

	NS16550_init(0);
	__raw_spin_lock(&gic_lock);
	maskInterrupt(0, UART0_IRQ);
	__raw_spin_unlock(&gic_lock);

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);

	cpu_irq_disable(0); //0 enable irq, ret pre value

	while (1) {
#if 0
		asm volatile ("wfe");
#else
		int i;
		for (i = 0; i < 1000000; i++) {
			__raw_spin_lock(&share_lock);
			share_var++;
			cnt_cpu0++;
			__raw_spin_unlock(&share_lock);
		}
		__raw_spin_lock(&share_lock);
		printf("cpu0: share_var is %d, cnt_cpu0[%d] + cnt_cpu1[%d] = [%d]\n", share_var, cnt_cpu0, cnt_cpu1, cnt_cpu0 + cnt_cpu1);
		__raw_spin_unlock(&share_lock);
#endif
	}
}

unsigned int cnt = 0;
void irq_handler(void)
{
    interrupt_t cur_irq;
	unsigned int cpu_id;

	asm volatile ("clrex");
    cur_irq = getActiveIRQ();

    __asm__ volatile (
		"mrc p15, 0, %0, c0, c0, 5"
		:"=r"(cpu_id)
		);
	cpu_id &= 0xf;

    if (cur_irq < 16) {
		printf("IPI: %d cpu: %d\n", cur_irq, cpu_id);
    } else if (cur_irq == TIMER0_IRQ) {
		timer0_clear_pending();
		printf("cpu%d [timer0 int %d]\n", cpu_id, ++cnt);
		dist_ipi_send(10, 1);
    } else if (cur_irq == TIMER1_IRQ) {
		timer1_clear_pending();
		printf("cpu%d [timer1 int]\n", cpu_id);
	} else if (cur_irq == UART0_IRQ) {
		char c = _serial_getc(0);
		printf("%c", c);
    } else {
        printf("cpu%d unkown irq no %d\n", cpu_id, cur_irq);
    }
    ackInterrupt(cur_irq);
}

void second_cpu_c_start(void)
{
    printf("cpu1 booting...\n");

	timer1_init();

	__raw_spin_lock(&gic_lock);
	set_irq_target(TIMER1_IRQ, 1);
	maskInterrupt(0, TIMER1_IRQ);
	__raw_spin_unlock(&gic_lock);

	cpu_irq_disable(0); //0 enable irq, ret pre value

    while (1) {
#if 0
		asm volatile ("wfe");
#else
		int i;
		for (i = 0; i < 1000000; i++) {
			__raw_spin_lock(&share_lock);
			share_var++;
			cnt_cpu1++;
			__raw_spin_unlock(&share_lock);
		}
		__raw_spin_lock(&share_lock);
		printf("cpu1: share_var is %d, cnt_cpu0[%d] + cnt_cpu1[%d] = [%d]\n", share_var, cnt_cpu0, cnt_cpu1, cnt_cpu0 + cnt_cpu1);
		__raw_spin_unlock(&share_lock);
#endif
    }
}

