#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char *format, ...);
int scanf(const char *format, ...);
int getchar();

#ifdef __cplusplus
}
#endif

#endif