/* linker.ld */

/* Define the memory map for your specific MCU */
/* Check your MCU datasheet for exact ORIGIN and LENGTH */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 96K
}

/* Define the top of the stack (End of RAM) */
_estack = ORIGIN(RAM) + LENGTH(RAM);

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
}