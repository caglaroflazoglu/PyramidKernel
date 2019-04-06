<<<<<<< HEAD
ENTRY(start)    /* entry point after loading the kernel */

SECTIONS {
    . = 1M;     /* the load address of 1st section */

    .boot :
    {
        *(.multiboot_header)    /* must at the beginning */
    }

    .text :
    {
        *(.text)
=======
ENTRY(start)
SECTIONS {

    .text BLOCK(1K) : ALIGN(1K)
    {
        *(main_section)
        *(.text)
    }

    .rodata BLOCK(1K) : ALIGN(1K)
    {
        *(.rodata)
    }

    .data BLOCK(1K) : ALIGN(1K)
    {
        *(.data)
    }

    .bss BLOCK(1K) : ALIGN(1K)
    {
        *(.bss)
>>>>>>> 7c0fe8e59b852aeb5e6f47a386e6b7ad8764fad1
    }
}
