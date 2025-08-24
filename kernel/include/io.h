#ifndef IO_H
#define IO_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void putchar(char c);
char* itoa(int value, char* str, int base);

#ifdef __cplusplus
}
#endif

#endif
