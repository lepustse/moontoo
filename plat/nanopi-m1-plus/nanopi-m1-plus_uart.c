#define SUNXI_UART0_BASE        0x01c28000
#define SUNXI_UART1_BASE        0x01c28400
#define SUNXI_UART2_BASE        0x01c28800
#define SUNXI_UART3_BASE        0x01c28c00

#define UART_REG(x) unsigned int x

struct NS16550 {
    UART_REG(rbr);      /* 0 */
    UART_REG(ier);      /* 1 */ // interrupt enable
    UART_REG(fcr);      /* 2 */
    UART_REG(lcr);      /* 3 */
    UART_REG(mcr);      /* 4 */
    UART_REG(lsr);      /* 5 */
    UART_REG(msr);      /* 6 */
    UART_REG(spr);      /* 7 */
};

#define thr rbr
#define iir fcr
#define dll rbr
#define dlm ier

#define UART_LSR_TEMT   0x40    /* Transmitter empty */
#define UART_LSR_THRE   0x20    /* Transmit-hold-register empty */
#define UART_LSR_BI 0x10    /* Break interrupt indicator */
#define UART_LSR_FE 0x08    /* Frame error indicator */
#define UART_LSR_PE 0x04    /* Parity error indicator */
#define UART_LSR_OE 0x02    /* Overrun error indicator */
#define UART_LSR_DR 0x01    /* Receiver data ready */

#define UART_IER_PTIME 0x80
#define UART_IER_ELSI  0x4
#define UART_IER_ETBEI 0x2
#define UART_IER_ERBFI 0x1 // enable received data available interrupt

typedef struct NS16550 *NS16550_t;

unsigned int readl(volatile void *addr) {
	return *(volatile unsigned int *)addr;
}

void writel(unsigned int v, volatile void *addr) {
	*(volatile unsigned int *)addr = v;
}

void NS16550_putc(NS16550_t com_port, char c)
{
	while ((readl(&com_port->lsr) & UART_LSR_THRE) == 0)
		;
	writel(c, &com_port->thr);
}

char NS16550_getc(NS16550_t com_port)
{
	while ((readl(&com_port->lsr) & UART_LSR_DR) == 0)
		;
	return readl(&com_port->thr);
}

NS16550_t serial_ports[4] = {
	(NS16550_t)SUNXI_UART0_BASE,
	(NS16550_t)SUNXI_UART1_BASE,
	(NS16550_t)SUNXI_UART2_BASE,
	(NS16550_t)SUNXI_UART3_BASE,
};

static void _serial_putc(const char c, const int port)
{
	if (c == '\n')
		NS16550_putc(serial_ports[port], '\r');

	NS16550_putc(serial_ports[port], c);
}

char _serial_getc(const int port)
{
	return NS16550_getc(serial_ports[port]);
}

void plat_putc(char byte)
{
	_serial_putc(byte, 0);
}

void plat_getc(char byte)
{
	_serial_getc(0);
}

void NS16550_init(const int port) {
	serial_ports[port]->ier |= UART_IER_ERBFI;
}
