/* The bootloader will look at this image and start execution at the symbol
    designated as the entry point. */
ENTRY(start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS {

    /* Begin putting sections at 1 MiB, a conventional place for kernels to 
       loaded at by the bootloader */
    . = 1M;

    .rodata :
    {
        /* ensure that the multiboot header is at the beginning */
        KEEP(*(.multiboot_header))
        *(.rodata .rodata.*)
        . = ALIGN(4K);
    }

    .text : 
    {
        *(.text .text.*)
        . = ALIGN(4K);
    }

    /* Read-write data (initialized) */
    .data :
    {
        *(.data .data.*)
        . = ALIGN(4K);
    }

    /* Read-write data (uninitialized) and stack */
    .bss :
    {
        *(.bss .bss.*)
        . = ALIGN(4K);
    }

    /* The compiler may produce other sections, by default it will put them in
       a segment with the same name. Simply add stuff here as needed. */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}
