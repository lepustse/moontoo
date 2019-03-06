OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(tz_init)

SECTIONS
{
	. = 0x40000000;
	. = ALIGN(4);
	_SYS_CODE_START = .;
	.text : {
		*(.text.init)
		*(.text*)
	}

	. = ALIGN(4);
	.data : { 
		*(.data)
	}
	__bss_start = .;
	.bss : { 
		*(.bss) 
	__bss_end = .;
    }
}
