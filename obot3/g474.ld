/* linker.ld */
ENTRY(Reset_Handler)
/* Define the memory map for your specific MCU */
/* Check your MCU datasheet for exact ORIGIN and LENGTH */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 96K
}

/* Define the top of the stack (End of RAM) */
_estack = ORIGIN(RAM) + LENGTH(RAM);
__stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* The vector table goes first into Flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* KEEP prevents the linker from discarding it */
        . = ALIGN(4);
    } > FLASH

    /* The actual code goes after the vector table */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.init)
        *(.fini)
        . = ALIGN(4);
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* Start of .data in RAM */

        *(.data)           /* Initialized variables */
        *(.data.*)

        . = ALIGN(4);
        _edata = .;        /* End of .data in RAM */
    } > RAM AT> FLASH      /* <--- The crucial part */

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .; /* crt0 expects this exact name */

        *(.bss)
        *(.bss.*)
        *(COMMON)

        . = ALIGN(4);
        __bss_end__ = .;   /* crt0 expects this exact name */

        /* If you have ST-style symbols like _sbss and _ebss already here,
           you can leave them alone. Just add the two lines above. */
    } > RAM

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } > FLASH

    /* Required for C++ Exception Unwinding */
    .ARM.extab : 
    { 
        *(.ARM.extab* .gnu.linkonce.armextab.*) 
    } > FLASH
    
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH
}