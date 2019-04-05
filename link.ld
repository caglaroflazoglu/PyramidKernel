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
    }
}
