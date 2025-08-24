ENTRY(start)

SECTIONS {
    . = 1M;

    .multiboot ALIGN(4K) : AT(ADDR(.multiboot)) {
        *(.multiboot)
    }

    .text ALIGN(4K) : AT(ADDR(.text)) {
        *(.text*)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata)) {
        *(.rodata*)
    }

    .data ALIGN(4K) : AT(ADDR(.data)) {
        *(.data*)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss)) {
        *(.bss*)
        *(COMMON)
    }
}
