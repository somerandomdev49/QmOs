#ifndef LIB_QMC_STRING_H
#define LIB_QMC_STRING_H
#include <stddef.h>
#include <stdint.h>

int strcmp(const char *s1, const char *s2);
size_t strlen(const char *s);
void *memset(void *s, unsigned char c, size_t n);

const char *strchr(const char *s, int c);
const char *strrchr(const char *s, int c);
char *strncpy(char *dest, const char *src, size_t n);

#endif
