#include <stddef.h>
#include <stdint.h>

#include <vga.h>
#include <keyboard.h>
#include <timer.h>

extern void idt_init(void);

void kernel_main(void) {
    timer_init(100);

    const char* msg = "starkrnl\n";
    vga_print(msg);
    
    idt_init();
    keyboard_init();
    
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
