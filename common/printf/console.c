/*
 * (C) Copyright 2000
 * Paolo Scaffardi, AIRVENT SAM s.p.a - RIMINI(ITALY), arsenio@tin.it
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <stdarg.h>
void serial_puts (const char *s);
void serial_putc (const char c);
void plat_putc(char c);
int vsprintf(char *buf, const char *fmt, va_list args);

#define uint unsigned long 

static char printbuffer[1024];

void printf(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	vsprintf(printbuffer, fmt, args);
	va_end(args);

	serial_puts(printbuffer);
}
/** U-Boot INITIAL CONSOLE-COMPATIBLE FUNCTION *****************************/

void puts(const char *s)
{
	serial_puts(s);
}

void serial_puts(const char *str) {
 while (*str)
        serial_putc((char)*str++);
}

void serial_putc(char ch)
{
	plat_putc(ch);
	if (ch == '\n')
		serial_putc('\r');
}

