#include "stdio.h"
#include <stdarg.h>
#include "../kernel/include/io.h"

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = 0;

    for (const char *ptr = format; *ptr != '\0'; ++ptr) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ++ptr;
            switch (*ptr) {
                case 's': {
                    const char *str = va_arg(args, const char *);
                    while (*str) {
                        putchar(*str++);
                        count++;
                    }
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[12];
                    itoa(num, buffer, 10);
                    for (char *bptr = buffer; *bptr != '\0'; ++bptr) {
                        putchar(*bptr);
                        count++;
                    }
                    break;
                }
                default:
                    putchar('%');
                    putchar(*ptr);
                    count += 2;
                    break;
            }
        } else {
            putchar(*ptr);
            count++;
        }
    }

    va_end(args);
    return count;
}

int scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = 0;

    char input_buffer[256];
    char *input_ptr = input_buffer;

    char c;
    while ((c = getchar()) != '\n' && (input_ptr - input_buffer) < 255) {
        *input_ptr++ = c;
    }
    *input_ptr = '\0';

    input_ptr = input_buffer;

    for (const char *ptr = format; *ptr != '\0'; ++ptr) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ++ptr;
            switch (*ptr) {
                case 's': {
                    char *str = va_arg(args, char *);
                    while (*input_ptr != ' ' && *input_ptr != '\0') {
                        *str++ = *input_ptr++;
                    }
                    *str = '\0';
                    count++;
                    break;
                }
                case 'd': {
                    int *num = va_arg(args, int *);
                    *num = 0;
                    while (*input_ptr >= '0' && *input_ptr <= '9') {
                        *num = *num * 10 + (*input_ptr - '0');
                        ++input_ptr;
                    }
                    count++;
                    break;
                }
                default:
                    break;
            }
        }
        while (*input_ptr == ' ') {
            ++input_ptr;
        }
    }

    va_end(args);
    return count;
}

int getchar() {
    char c = inb(0x60);

    while (c == 0) {
        c = inb(0x60);
    }
    
    return (int)c;
}