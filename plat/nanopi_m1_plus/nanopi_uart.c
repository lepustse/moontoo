#include <stdint.h>

extern void printf(const char *,...);

#define __REG32(x)				     (*(volatile unsigned int *)(x))

//#define REALVIEW_SCTL_BASE              0x10001000  /* System Controller */
#define REALVIEW_SCTL_BASE              0x01C00000  /* System Controller */
#define REALVIEW_REFCLK                 0
#define UART0_HW_BASE                   0x01C28000
#define UART1_HW_BASE                   0x01C28400

/* nanopi_m1_plus uart0 reg */
#define UART0_RBR                       __REG32(UART0_HW_BASE + 0x00) // receive buf; only if DR in LSR is set, data is valid
#define UART0_THR                       __REG32(UART0_HW_BASE + 0x00) // transmit holding;
#define UART0_DLL                       __REG32(UART0_HW_BASE + 0x00) // dll
#define UART0_DLH                       __REG32(UART0_HW_BASE + 0x04) // dlh
#define UART0_IER                       __REG32(UART0_HW_BASE + 0x04) // irp en
#define UART0_PTIME                     (1 << 7)
#define UART0_IIR                       __REG32(UART0_HW_BASE + 0x08) // interrupt id
#define UART0_IIR_FEFLAG                ((1 << 0) | (1 << 1))         // fifos en flag
#define UART0_FCR                       __REG32(UART0_HW_BASE + 0x08) // fifo ctrl
#define UART0_FCR_DMAM                  (1 << 3)                      // dma mode

#define UART0_LCR                       __REG32(UART0_HW_BASE + 0x0C) // line ctrl

#define UART0_MCR                       __REG32(UART0_HW_BASE + 0x10) // modem ctrl

#define UART0_LSR                       __REG32(UART0_HW_BASE + 0x14) // line status read only

#define UART0_MSR                       __REG32(UART0_HW_BASE + 0x18) // modem status
#define UART0_SCH                       __REG32(UART0_HW_BASE + 0x1C) //

#define UART0_USR                       __REG32(UART0_HW_BASE + 0x7C) // status
#define UART0_USR_RFF                   (1 << 4)                      // ready fifo full
#define UART0_USR_RFNE                  (1 << 3)                      // receive fifo not empty
#define UART0_USR_TFE                   (1 << 2)                      // transmit fifo empty
#define UART0_USR_TFNF                  (1 << 1)                      // transmit fifo not full
#define UART0_USR_BUSY                  (1 << 0)                      // uart busy

#define UART0_TFL                       __REG32(UART0_HW_BASE + 0x80) // transmit fifo level
#define UART0_RFL                       __REG32(UART0_HW_BASE + 0x84) // receive fifo level
#define UART0_HALT                      __REG32(UART0_HW_BASE + 0xA4) // halt tx
/* nanopi_m1_plus uart0 reg */

/* qemu uart0 reg */
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
	writel(byte, (void*)0x01C28000); // DR寄存器
}

void uart0_init(void) {
    uint32_t val;

    /* 读取usr[0] & 置位lcr[7] */

    /* 设置dll & dlh */

    /* 读取usr[0] & 清楚lcr[7] */

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
