#include <io.h>
#include <stddef.h>
#include <stdint.h>

void putchar(char c) {
    volatile char *video_memory = (volatile char *)0xB8000;
    static size_t cursor = 0;
    video_memory[cursor * 2] = c;
    video_memory[cursor * 2 + 1] = 0x07;
    cursor++;
}

char* itoa(int value, char* str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    tmp_value = value;
    do {
        int remainder = tmp_value % base;
        *ptr++ = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'a');
    } while (tmp_value /= base);

    *ptr = '\0';

    ptr1 = str;
    if (*str == '-') ptr1++;
    for (ptr--; ptr1 < ptr; ptr1++, ptr--) {
        tmp_char = *ptr;
        *ptr = *ptr1;
        *ptr1 = tmp_char;
    }

    return str;
}

void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
