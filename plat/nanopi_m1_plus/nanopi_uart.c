#include <stdint.h>

extern void printf(const char *,...);

#define __REG32(x)				     (*(volatile unsigned int *)(x))

#define REALVIEW_SCTL_BASE              0x10001000  /* System Controller */
#define REALVIEW_REFCLK                 0
#define UART0_HW_BASE                   0x10009000
#define UART1_HW_BASE                   0x1000a000

#define UART0_DR                        __REG32(UART0_HW_BASE + 0x00)
#define UART0_RSR                       __REG32(UART0_HW_BASE + 0x04)
#define UART0_IBRD                      __REG32(UART0_HW_BASE + 0x24)
#define UART0_FBRD                      __REG32(UART0_HW_BASE + 0x28)
#define UART0_LCR_H                     __REG32(UART0_HW_BASE + 0x2C)

#define UART1_DR                        __REG32(UART1_HW_BASE + 0x00)
#define UART1_RSR                       __REG32(UART1_HW_BASE + 0x04)
#define UART1_IBRD                      __REG32(UART1_HW_BASE + 0x24)
#define UART1_FBRD                      __REG32(UART1_HW_BASE + 0x28)
#define UART1_LCR_H                     __REG32(UART1_HW_BASE + 0x2C)

#define UART0_CR                        __REG32(UART0_HW_BASE + 0x30)
#define UART0_CR_ENABLE                 (1 << 0)
#define UART0_CR_TXE                    (1 << 8)
#define UART0_CR_RXE                    (1 << 9)

#define UART1_CR                        __REG32(UART1_HW_BASE + 0x30)
#define UART1_CR_ENABLE                 (1 << 0)
#define UART1_CR_TXE                    (1 << 8)
#define UART1_CR_RXE                    (1 << 9)

#define UART0_RIS                       __REG32(UART0_HW_BASE + 0x3C)
#define UART0_IMSC                      __REG32(UART0_HW_BASE + 0x38)
#define UART0_RX_IM                     (1 << 4)
#define UART0_TX_IM                     (1 << 5)

#define UART1_RIS                       __REG32(UART1_HW_BASE + 0x3C)
#define UART1_IMSC                      __REG32(UART1_HW_BASE + 0x38)
#define UART1_RX_IM                     (1 << 4)
#define UART1_TX_IM                     (1 << 5)

#define UART0_ICR                       __REG32(UART0_HW_BASE + 0x44)
#define UART0_RX_IC                     (1 << 4)
#define UART0_TX_IC                     (1 << 5)

#define UART0_IRQ                       37

#define UART1_ICR                       __REG32(UART1_HW_BASE + 0x44)
#define UART1_RX_IC                     (1 << 4)
#define UART1_TX_IC                     (1 << 5)

#define UART1_IRQ                       38

unsigned int readl(volatile void *addr) {
	return *(volatile unsigned int *)addr;
}

void writel(unsigned int v, volatile void *addr) {
	*(volatile unsigned int *)addr = v;
}

void plat_putc(char byte)
{
	writel(byte, (void*)0x10009000);
}

void uart0_init(void) {
    uint32_t val;

    /* 24MHz setting baud rate */
    UART0_IBRD = 0xd;
    UART0_FBRD = 0x1;
    UART0_LCR_H = 0x40;

    UART0_IMSC |= (UART0_RX_IM);

    val = UART0_CR;
    val &= ~UART0_CR_ENABLE;
    val |= (UART0_CR_TXE | UART0_CR_RXE);

    UART0_CR = val;

    /* enable uart1 */
    UART0_CR |= UART0_CR_ENABLE;

    //printf("uart reg status: 0x%08x\n", UART1_CR);
}

void uart0_clear_tx_pending(void) {
    UART0_ICR |= UART0_TX_IC;
}

void uart0_clear_rx_pending(void) {
    UART0_ICR |= UART0_RX_IC;
}

void uart1_init(void) {
    uint32_t val;

    /* 24MHz setting baud rate */
    UART1_IBRD = 0xd;
    UART1_FBRD = 0x1;
    UART1_LCR_H = 0x40;

    UART1_IMSC |= (UART1_RX_IM);

    val = UART1_CR;
    val &= ~UART1_CR_ENABLE;
    val |= (UART1_CR_TXE | UART1_CR_RXE);

    UART1_CR = val;

    /* enable uart1 */
    UART1_CR |= UART1_CR_ENABLE;

    //printf("uart reg status: 0x%08x\n", UART1_CR);
}

void uart1_clear_tx_pending(void) {
    UART1_ICR |= UART1_TX_IC;
}

void uart1_clear_rx_pending(void) {
    UART1_ICR |= UART1_RX_IC;
}
