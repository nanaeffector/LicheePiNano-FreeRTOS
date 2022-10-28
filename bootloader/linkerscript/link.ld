OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

STACK_UND_SIZE = 0x400;
STACK_ABT_SIZE = 0x400;
STACK_IRQ_SIZE = 0x1000;
STACK_FIQ_SIZE = 0x1000;
STACK_SRV_SIZE = 0x1000;
STACK_SYS_SIZE = 0x1000;
STACK_SIZE = STACK_UND_SIZE + STACK_ABT_SIZE + STACK_IRQ_SIZE + STACK_FIQ_SIZE + STACK_SRV_SIZE + STACK_SYS_SIZE;
FLASH_SIZE = 16M;
DRAM_SIZE = 32M;
HEAP_SIZE = DRAM_SIZE - FLASH_SIZE - STACK_SIZE;
FLASH_BLOCK_SIZE = 64K;

MEMORY {
	/* cpu internal sram area. */
    BROM (r):			ORIGIN = 0x00000000, LENGTH = 32K															/* cpu内蔵 boot-rom(BROM) 領域. */
    SRAM (xw):			ORIGIN = 0x00000000, LENGTH = 32K															/* sram 領域. */
    
    /* cpu external dram area. */
    DRAM_BOOT 	(xw):	ORIGIN = 0x80000000,							LENGTH = 16K								/* bootloader(boot1) 領域. */
    DRAM_MAIN 	(xw):	ORIGIN = 0x80004000,							LENGTH = FLASH_SIZE							/* main code 領域. */
    DRAM_STACK	(xw):	ORIGIN = 0x80004000 + FLASH_SIZE,				LENGTH = STACK_SIZE							/* stack 領域. */
    DRAM_HEAP	(xw):	ORIGIN = 0x80004000 + FLASH_SIZE + STACK_SIZE,	LENGTH = HEAP_SIZE							/* heap 領域. */
    
    /* spi flash area. */
    FLASH_BOOT0	(r):	ORIGIN = 0x00000000, LENGTH = LENGTH(SRAM)													/* cpu external flash area(boot-code). 実利用領域は32kbyteだが、64kb(=1block分)確保する. (*BROMにより、SRAMに展開される. よってSRAMの領域までが限度).*/
    FLASH_BOOT1	(r):	ORIGIN = 0x00010000, LENGTH = FLASH_BLOCK_SIZE												/* cpu external flash area(main-code). 16384kb - 64kb. (*ここをDRAMに展開する.)*/
    FLASH_MAIN	(r):	ORIGIN = 0x00020000, LENGTH = FLASH_SIZE - LENGTH(FLASH_BOOT0) - LENGTH(FLASH_BOOT1)		/* cpu external flash area(main-code). 16384kb - 64kb - 64kb. (*ここをDRAMに展開する.)*/
}

SECTIONS
{
	.text :
	{
		. = ALIGN(4);
		PROVIDE(__text_start = .);
		PROVIDE(__spl_start = .);
		build/start.o (.text*)
		*(.text*)
		*(.init.text)
		*(.exit.text)
		*(.glue*)
		*(.note.gnu.build-id)
		PROVIDE(__text_end = .);
	} > SRAM AT > FLASH_BOOT0

	.rodata :
	{
		. = ALIGN(4);
		PROVIDE(__rodata_start = .);
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		PROVIDE(__rodata_end = .);
	} > SRAM AT > FLASH_BOOT0

	__sidata = LOADADDR(.data);
	.data :
	{
		. = ALIGN(4);
		__sdata  = .;
		*(.data*)
		__edata  = .;
		PROVIDE(__spl_end = .);
	} > SRAM AT > FLASH_BOOT0
	PROVIDE(__spl_size = __spl_end - __spl_start);

	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		__sbss = .;
		__bss_start__ = __sbss;
		*(.bss*)
		*(COMMON)
		__ebss = .;
		__bss_end__ = __ebss;
	} > SRAM 
  	PROVIDE (end = .);

	/* Mainプログラム用のsection. */
	PROVIDE(__flash_load_addr = ORIGIN(FLASH_BOOT1));
	PROVIDE(__vect_undefined = ORIGIN(DRAM_BOOT) + 0x04);
	PROVIDE(__vect_software = ORIGIN(DRAM_BOOT) + 0x08);
	PROVIDE(__vect_prefetch_abort = ORIGIN(DRAM_BOOT) + 0x0C);
	PROVIDE(__vect_data_abort = ORIGIN(DRAM_BOOT) + 0x10);
	PROVIDE(__vect_irq = ORIGIN(DRAM_BOOT) + 0x18);
	PROVIDE(__vect_fiq = ORIGIN(DRAM_BOOT) + 0x1C);
	PROVIDE(__image_start = ORIGIN(DRAM_BOOT));
	PROVIDE(__image_end = ORIGIN(DRAM_BOOT) + LENGTH(DRAM_BOOT));
	
	.stack (NOLOAD) :
	{
        . = ALIGN(8);
		PROVIDE(__stack_start = .);
		PROVIDE(__stack_und_start = .);
		. += STACK_UND_SIZE;
		PROVIDE(__stack_und_end = .);
		. = ALIGN(8);
		PROVIDE(__stack_abt_start = .);
		. += STACK_ABT_SIZE;
		PROVIDE(__stack_abt_end = .);
		. = ALIGN(8);
		PROVIDE(__stack_irq_start = .);
		. += STACK_IRQ_SIZE;
		PROVIDE(__stack_irq_end = .);
		. = ALIGN(8);
		PROVIDE(__stack_fiq_start = .);
		. += STACK_FIQ_SIZE;
		PROVIDE(__stack_fiq_end = .);
		. = ALIGN(8);
		PROVIDE(__stack_srv_start = .);
		. += STACK_SRV_SIZE;
		PROVIDE(__stack_srv_end = .);
		. = ALIGN(8);
		PROVIDE(__stack_sys_start = .);
		. += STACK_SYS_SIZE;
		PROVIDE(__stack_sys_end = .);
		. = ALIGN(8);
		PROVIDE(__stack_end = .);
	} > DRAM_STACK

	.heap ALIGN(8) (NOLOAD) :
	{
		PROVIDE(__heap_start = ORIGIN(DRAM_HEAP));
		*(.heap)
		PROVIDE(__heap_end = ORIGIN(DRAM_HEAP) + LENGTH(DRAM_HEAP));
	} > DRAM_HEAP
	
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_info 0 : { *(.debug_info) }
	.debug_line 0 : { *(.debug_line) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_aranges 0 : { *(.debug_aranges) }
}