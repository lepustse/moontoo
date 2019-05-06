OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(tz_init)

SECTIONS
{
	. = 0x60008000;
	. = ALIGN(4);
	_SYS_CODE_START = .;
	.text : {
		*(.text.init)
		*(.text*)
	}

	. = ALIGN(4);
	.data : { 
		*(.data)
		. = ALIGN(4);
		tuzi_export_start = .;
		KEEP(*(tuzi_export))
		tuzi_export_end = .;
	}
	__bss_start = .;
	.bss : { 
		*(.bss) 
	__bss_end = .;
    }
}
