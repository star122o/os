#include <stdint.h>
#include <io.h>

static const char scancode_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0
};

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);
    if (scancode < 128) {
        char c = scancode_ascii[scancode];
        if (c) {
        }
    }
    outb(0x20, 0x20);
}

void keyboard_init(void) {
    extern void set_idt_entry(int num, void (*handler)(), uint16_t selector, uint8_t type_attr);
    set_idt_entry(33, keyboard_handler, 0x08, 0x8E);
}
