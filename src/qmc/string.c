#include <qmos/qmc/string.h>

size_t strlen(const char *s) 
{
	size_t len = 0;
	while(*s) ++s, ++len;
	return len;
}

// ! noone said we need to fill from left to right!
void *memset(void *s, unsigned char c, size_t n)
{
	while(n) ((unsigned char*)s)[--n] = c;
	return s;
}

const char *strchr(const char *s, int c)
{
	while(*s)
		if(*s == c) return s;
		else ++s;
	return NULL;
}

const char *strrchr(const char *s, int c)
{
	size_t l = strlen(s);
	while(s[l])
		if(s[l] == c) return s;
		else --l;
	return NULL;
}

// https://stackoverflow.com/a/45893697/9110517
// Should be 10 opcodes for gcc on x86-64 (we have x86 but shouldn't be a problem)
int strcmp(const char *s1, const char *s2)
{
    while(1)
    {
        int res = ((*s1 == 0) || (*s1 != *s2));
        if (__builtin_expect((res),0)) break;
        ++s1;
        ++s2;
    }
    return (*s1 - *s2);
}

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;
	for(i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for(; i < n; i++)
		dest[i] = '\0';

	return dest;
}
