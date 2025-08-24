#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000
#define VGA_DEFAULT_COLOR 0x07

static volatile uint16_t* const vga_buffer = (uint16_t*)VGA_ADDRESS;

void vga_print(const char* str) {
    size_t i = 0;
    while (str[i] && i < VGA_WIDTH) {
        vga_buffer[i] = ((uint16_t)str[i]) | (VGA_DEFAULT_COLOR << 8);
        i++;
    }
}