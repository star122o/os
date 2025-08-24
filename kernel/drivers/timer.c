#include <stdint.h>
#include <io.h>

#define PIT_FREQ 1193182
#define PIT_CMD_PORT 0x43
#define PIT_DATA_PORT 0x40

static volatile uint64_t ticks = 0;

void timer_handler(void) {
    ticks++;
    outb(0x20, 0x20);
}

void timer_init(uint32_t frequency) {
    uint32_t divisor = PIT_FREQ / frequency;
    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_DATA_PORT, divisor & 0xFF);
    outb(PIT_DATA_PORT, (divisor >> 8) & 0xFF);
    extern void set_idt_entry(int num, void (*handler)(), uint16_t selector, uint8_t type_attr);
    set_idt_entry(32, timer_handler, 0x08, 0x8E);
}

uint64_t timer_ticks(void) {
    return ticks;
}

void timer_wait(uint64_t wait_ticks) {
    uint64_t start = ticks;
    while ((ticks - start) < wait_ticks) {
        __asm__ __volatile__("hlt");
    }
}