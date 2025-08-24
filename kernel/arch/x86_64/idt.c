#include <stdint.h>

#define IDT_ENTRIES 256
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

void default_handler(void) {
    while (1) {
        __asm__ __volatile__("hlt");
    }
}

void set_idt_entry(int num, void (*handler)(), uint16_t selector, uint8_t type_attr) {
    uint64_t handler_addr = (uint64_t)handler;
    idt[num].offset_low = handler_addr & 0xFFFF;
    idt[num].selector = selector;
    idt[num].ist = 0;
    idt[num].type_attr = type_attr;
    idt[num].offset_middle = (handler_addr >> 16) & 0xFFFF;
    idt[num].offset_high = (handler_addr >> 32) & 0xFFFFFFFF;
    idt[num].zero = 0;
}

void idt_init(void) {
    for (int i = 0; i < IDT_ENTRIES; ++i) {
        set_idt_entry(i, default_handler, 0x08, 0x8E);
    }
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;
    __asm__ __volatile__("lidtq (%0)" : : "r"(&idtp));
}
