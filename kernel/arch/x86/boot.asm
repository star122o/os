MULTIBOOT_MAGIC    equ 0x1BADB002
MULTIBOOT_FLAGS    equ 0x0
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

section .multiboot
align 4
multiboot_header:
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .text
[BITS 32]
[GLOBAL start]
[EXTERN kernel_main]

start:
    cli
    
    ; cmp eax, MULTIBOOT_MAGIC
    ; jne hang    

    mov esp, stack_top
    call kernel_main
    
hang:
    hlt
    jmp hang

section .bss
align 16
stack_bottom:
    resb 16384 
stack_top:
