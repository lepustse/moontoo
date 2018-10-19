/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <stdarg.h>
#include <common.h>

#define u8 unsigned char
#define size_t unsigned int
#define ptrdiff_t int

# define NUM_TYPE long
#define do_div(n, base) ({ \
	unsigned int __res; \
	__res = ((unsigned NUM_TYPE) n) % base; \
	n = ((unsigned NUM_TYPE) n) / base; \
	__res; \
})

#define noinline __attribute__((noinline))

#  define _ISbit(bit)	((bit) < 8 ? ((1 << (bit)) << 8) : ((1 << (bit)) >> 8))

#if !defined __NO_CTYPE && !defined __cplusplus
# define isalnum(c)	__isctype((c), _ISalnum)
# define isalpha(c)	__isctype((c), _ISalpha)
# define iscntrl(c)	__isctype((c), _IScntrl)
# define isdigit(c)	__isctype((c), _ISdigit)
# define islower(c)	__isctype((c), _ISlower)
# define isgraph(c)	__isctype((c), _ISgraph)
# define isprint(c)	__isctype((c), _ISprint)
# define ispunct(c)	__isctype((c), _ISpunct)
# define isspace(c)	__isctype((c), _ISspace)
# define isupper(c)	__isctype((c), _ISupper)
# define isxdigit(c)	__isctype((c), _ISxdigit)

# ifdef __USE_ISOC99
#  define isblank(c)	__isctype((c), _ISblank)
# endif

# if __GNUC__ >= 2 && defined __OPTIMIZE__ && !defined __cplusplus
#  define tolower(c)	__tobody (c, tolower, *__ctype_tolower_loc (), (c))
#  define toupper(c)	__tobody (c, toupper, *__ctype_toupper_loc (), (c))
# endif /* Optimizing gcc */

# if defined __USE_SVID || defined __USE_MISC || defined __USE_XOPEN
#  define isascii(c)	__isascii (c)
#  define toascii(c)	__toascii (c)

#  define _tolower(c)	((int) (*__ctype_tolower_loc ())[(int) (c)])
#  define _toupper(c)	((int) (*__ctype_toupper_loc ())[(int) (c)])
# endif

#endif /* Not __NO_CTYPE.  */


enum
{
  _ISupper = _ISbit (0),	/* UPPERCASE.  */
  _ISlower = _ISbit (1),	/* lowercase.  */
  _ISalpha = _ISbit (2),	/* Alphabetic.  */
  _ISdigit = _ISbit (3),	/* Numeric.  */
  _ISxdigit = _ISbit (4),	/* Hexadecimal numeric.  */
  _ISspace = _ISbit (5),	/* Whitespace.  */
  _ISprint = _ISbit (6),	/* Printing.  */
  _ISgraph = _ISbit (7),	/* Graphical.  */
  _ISblank = _ISbit (8),	/* Blank (usually SPC and TAB).  */
  _IScntrl = _ISbit (9),	/* Control character.  */
  _ISpunct = _ISbit (10),	/* Punctuation.  */
  _ISalnum = _ISbit (11)	/* Alphanumeric.  */
};

#define __isctype(c, type) \
  ((*__ctype_b_loc ())[(int) (c)] & (unsigned short int) type)

#define	__isascii(c)	(((c) & ~0x7f) == 0)	/* If C is a 7 bit value.  */
#define	__toascii(c)	((c) & 0x7f)		/* Mask off high bits.  */


const char hex_asc[] = "0123456789abcdef";
#define hex_asc_lo(x)   hex_asc[((x) & 0x0f)]
#define hex_asc_hi(x)   hex_asc[((x) & 0xf0) >> 4]

#if 0
unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,			/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,			/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,			/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,			/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,			/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,	/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,			/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,			/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */
#endif

static inline char *pack_hex_byte(char *buf, u8 byte)
{
	*buf++ = hex_asc_hi(byte);
	*buf++ = hex_asc_lo(byte);
	return buf;
}

size_t strnlen(const char *s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

/* Decimal conversion is by far the most typical, and is used
 * for /proc and /sys data. This directly impacts e.g. top performance
 * with many processes running. We optimize it for speed
 * using code from
 * http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 * (with permission from the author, Douglas W. Jones). */

/* Formats correctly any integer in [0,99999].
 * Outputs from one to five digits depending on input.
 * On i386 gcc 4.1.2 -O2: ~250 bytes of code. */
static char* put_dec_trunc(char *buf, unsigned q)
{
	unsigned d3, d2, d1, d0;
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0'; /* least significant digit */
	d1 = q + 9*d3 + 5*d2 + d1;
	if (d1 != 0) {
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0'; /* next digit */

		d2 = q + 2*d2;
		if ((d2 != 0) || (d3 != 0)) {
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0'; /* next digit */

			d3 = q + 4*d3;
			if (d3 != 0) {
				q = (d3 * 0xcd) >> 11;
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';  /* next digit */
				if (q != 0)
					*buf++ = q + '0';  /* most sign. digit */
			}
		}
	}
	return buf;
}
/* Same with if's removed. Always emits five digits */
static char* put_dec_full(char *buf, unsigned q)
{
	/* BTW, if q is in [0,9999], 8-bit ints will be enough, */
	/* but anyway, gcc produces better code with full-sized ints */
	unsigned d3, d2, d1, d0;
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	/*
	 * Possible ways to approx. divide by 10
	 * gcc -O2 replaces multiply with shifts and adds
	 * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
	 * (x * 0x67) >> 10:  1100111
	 * (x * 0x34) >> 9:    110100 - same
	 * (x * 0x1a) >> 8:     11010 - same
	 * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
	 */

	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0';
	d1 = q + 9*d3 + 5*d2 + d1;
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0';

		d2 = q + 2*d2;
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0';

			d3 = q + 4*d3;
				q = (d3 * 0xcd) >> 11; /* - shorter code */
				/* q = (d3 * 0x67) >> 10; - would also work */
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';
					*buf++ = q + '0';
	return buf;
}
/* No inlining helps gcc to use registers better */
static noinline char* put_dec(char *buf, unsigned NUM_TYPE num)
{
	while (1) {
		unsigned rem;
		if (num < 100000)
			return put_dec_trunc(buf, num);
		rem = do_div(num, 100000);
		buf = put_dec_full(buf, rem);
	}
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SMALL	32		/* Must be 32 == 0x20 */
#define SPECIAL	64		/* 0x */

static char *number(char *buf, unsigned NUM_TYPE num, int base, int size, int precision, int type)
{
	/* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
	static const char digits[16] = "0123456789ABCDEF"; /* "GHIJKLMNOPQRSTUVWXYZ"; */

	char tmp[66];
	char sign;
	char locase;
	int need_pfx = ((type & SPECIAL) && base != 10);
	int i;

	/* locase = 0 or 0x20. ORing digits or letters with 'locase'
	 * produces same digits or (maybe lowercased) letters */
	locase = (type & SMALL);
	if (type & LEFT)
		type &= ~ZEROPAD;
	sign = 0;
	if (type & SIGN) {
		if ((signed NUM_TYPE) num < 0) {
			sign = '-';
			num = - (signed NUM_TYPE) num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (need_pfx) {
		size--;
		if (base == 16)
			size--;
	}

	/* generate full string in tmp[], in reverse order */
	i = 0;
	if (num == 0)
		tmp[i++] = '0';
	/* Generic code, for any base:
	else do {
		tmp[i++] = (digits[do_div(num,base)] | locase);
	} while (num != 0);
	*/
	else if (base != 10) { /* 8 or 16 */
		int mask = base - 1;
		int shift = 3;
		if (base == 16) shift = 4;
		do {
			tmp[i++] = (digits[((unsigned char)num) & mask] | locase);
			num >>= shift;
		} while (num);
	} else { /* base 10 */
		i = put_dec(tmp, num) - tmp;
	}

	/* printing 100 using %2d gives "100", not "00" */
	if (i > precision)
		precision = i;
	/* leading space padding */
	size -= precision;
	if (!(type & (ZEROPAD+LEFT)))
		while(--size >= 0)
			*buf++ = ' ';
	/* sign */
	if (sign)
		*buf++ = sign;
	/* "0x" / "0" prefix */
	if (need_pfx) {
		*buf++ = '0';
		if (base == 16)
			*buf++ = ('X' | locase);
	}
	/* zero or space padding */
	if (!(type & LEFT)) {
		char c = (type & ZEROPAD) ? '0' : ' ';
		while (--size >= 0)
			*buf++ = c;
	}
	/* hmm even more zero padding? */
	while (i <= --precision)
		*buf++ = '0';
	/* actual digits of result */
	while (--i >= 0)
		*buf++ = tmp[i];
	/* trailing space padding */
	while (--size >= 0)
		*buf++ = ' ';
	return buf;
}

static char *string(char *buf, char *s, int field_width, int precision, int flags)
{
	int len, i;

	if (s == 0)
		s = "<NULL>";

	len = strnlen(s, precision);

	if (!(flags & LEFT))
		while (len < field_width--)
			*buf++ = ' ';
	for (i = 0; i < len; ++i)
		*buf++ = *s++;
	while (len < field_width--)
		*buf++ = ' ';
	return buf;
}

/*
 * Show a '%p' thing.  A kernel extension is that the '%p' is followed
 * by an extra set of alphanumeric characters that are extended format
 * specifiers.
 *
 * Right now we handle:
 *
 * - 'M' For a 6-byte MAC address, it prints the address in the
 *       usual colon-separated hex notation
 * - 'I' [46] for IPv4/IPv6 addresses printed in the usual way (dot-separated
 *       decimal for v4 and colon separated network-order 16 bit hex for v6)
 * - 'i' [46] for 'raw' IPv4/IPv6 addresses, IPv6 omits the colons, IPv4 is
 *       currently the same
 *
 * Note: The difference between 'S' and 'F' is that on ia64 and ppc64
 * function pointers are really function descriptors, which contain a
 * pointer to the real address.
 */
#if 0
static char *pointer(const char *fmt, char *buf, void *ptr, int field_width, int precision, int flags)
{
	if (!ptr)
		return string(buf, "(null)", field_width, precision, flags);

	flags |= SMALL;
	if (field_width == -1) {
		field_width = 2*sizeof(void *);
		flags |= ZEROPAD;
	}
	return number(buf, (unsigned long) ptr, 16, field_width, precision, flags);
}
#endif

/**
 * vsprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * This function follows C99 vsprintf, but has some extensions:
 * %pS output the name of a text symbol
 * %pF output the name of a function pointer
 * %pR output the address range in a struct resource
 *
 * The function returns the number of characters written
 * into @buf.
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want sprintf() instead.
 */
int vsprintf(char *buf, const char *fmt, va_list args)
{
	unsigned NUM_TYPE num;
	int base;
	char *str;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
				/* 'z' support added 23/7/1999 S.H.    */
				/* 'z' changed to 'Z' --davidm 1/25/99 */
				/* 't' added for ptrdiff_t */

	str = buf;

	for (; *fmt ; ++fmt) {

		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}

		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
			}

		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
		    *fmt == 'Z' || *fmt == 'z' || *fmt == 't') {
			qualifier = *fmt;
			++fmt;
			if (qualifier == 'l' && *fmt == 'l') {
				qualifier = 'L';
				++fmt;
			}
		}

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			continue;

		case 's':
			str = string(str, va_arg(args, char *), field_width, precision, flags);
			continue;

		case '%':
			*str++ = '%';
			continue;

			/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}
		if (qualifier == 'l') {
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z' || qualifier == 'z') {
			num = va_arg(args, size_t);
		} else if (qualifier == 't') {
			num = va_arg(args, ptrdiff_t);
		} else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, num, base, field_width, precision, flags);
	}
	*str = '\0';
	return str-buf;
}

/**
 * sprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @...: Arguments for the format string
 *
 * The function returns the number of characters written
 * into @buf.
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}

