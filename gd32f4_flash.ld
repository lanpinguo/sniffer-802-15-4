ENTRY(Reset_Handler)

/* Stack is placed in the 64KB CCM memory */
/* NB. CCM memory cannot be accessed by peripherals or DMA! */
/* Always use static/global variables with peripherals/DMA. */
_estack = 0x10000000 + 64K;
__heap_size = 0x200;
__stack_size = 0x400;


MEMORY
{
	FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 896K
	FRAME_FLASH (rx) : ORIGIN = 0x080E0000, LENGTH = 128K
	RAM (xrw)       : ORIGIN = 0x10000000, LENGTH = 64K
	FRAME_RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	/* Reset Vector */
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} >FLASH

    /* use one sector */
	.param :
	{
        . = ALIGN(0x4000);
        param_start = .;
		KEEP(*(.param))
		. = ALIGN(0x8000);
	} >FLASH
    param_end = .;

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata*)
		*(.glue_7)         /* glue arm to thumb code */
		*(.glue_7t)        /* glue thumb to arm code */
		*(.eh_frame)

		KEEP (*(.init))
		KEEP (*(.fini))

		. = ALIGN(4);
		_etext = .;
		_exit = .;
	} >FLASH

	.ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
	.ARM : {
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} >FLASH

	.preinit_array :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} >FLASH

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	} >FLASH

	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array*))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >FLASH

	_sidata = .;

	.data : AT ( _sidata )
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_edata = .;
	} >RAM

	. = ALIGN(4);
	.bss :
	{
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} >RAM

    /***********************************************************************************************/
    /* USER HEAP                                                                                   */
    /***********************************************************************************************/
    .heap (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        PROVIDE(end = .);
        __heap_start = .;
        __heap_base__ = .;
        . = . + __heap_size;
        __heap_end = .;
        __HeapLimit = .;
    } > RAM

    /***********************************************************************************************/
    /* USER STACK                                                                                  */
    /***********************************************************************************************/
    .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size (NOLOAD) :
    {
        __stack_start = .;
        __stack = .;
        *(.stack)
        . = . + __stack_size;
        __stack_end = .;
        __StackLimit = .;
    } > RAM

	/* Remove information from the standard libraries */
	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	.frame_buffer :
	{
		*(.frame_data)
	} >FRAME_RAM

	.frame_flash :
	{
		*(.background)
	} >FRAME_FLASH

	.ARM.attributes 0 : { *(.ARM.attributes) }
}
