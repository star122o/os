MULTIBOOT2_MAGIC    equ 0xE85250D6
MULTIBOOT2_FLAGS    equ 0x0
MULTIBOOT2_CHECKSUM equ -(MULTIBOOT2_MAGIC + MULTIBOOT2_FLAGS)

section .multiboot
align 8
multiboot_header:
    dd MULTIBOOT2_MAGIC
    dd MULTIBOOT2_FLAGS
    dd MULTIBOOT2_CHECKSUM
    dd 0
    dd 0

section .text
[BITS 32]
[GLOBAL start]
[EXTERN kernel_main]

start:
    cli

    lgdt [gdt_descriptor]

    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x100
    wrmsr

    mov eax, cr4
    or eax, 0x20
    mov cr4, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    jmp 0x08:long_mode_start

align 16
[BITS 64]
long_mode_start:
    mov rsp, stack_top
    call kernel_main

hang:
    hlt
    jmp hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .data
gdt:
    dq 0x0000000000000000
    dq 0x00AF9A000000FFFF
    dq 0x00AF92000000FFFF

gdt_descriptor:
    dw gdt_end - gdt - 1
    dq gdt

gdt_end:
