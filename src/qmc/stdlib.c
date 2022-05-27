#include <qmos/qmc/stdlib.h>

int atoi(const char *s)
{
	int val = 0;
    while(*s) {
		if(*s < '0' || *s > '9') break;
        val = val * 10 + (*s++ - '0');
    }
    return val;

}
