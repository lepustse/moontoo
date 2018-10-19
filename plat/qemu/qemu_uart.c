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

