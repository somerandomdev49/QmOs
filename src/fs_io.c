#include <qmos/disk_io.h>

void insl(int port, void *addr, int count)
{
	asm volatile("cld\n\trepne\n\tinsl"
		: "=D" (addr), "=c" (count)
		: "d" (port), "0" (addr), "1" (count)
		: "memory", "cc");
}

void outsl(int port, const void *addr, int count)
{
	asm volatile("cld\n\trepne\n\toutsl"
		: "=S" (addr), "=c" (count)
		: "d" (port), "0" (addr), "1" (count)
		: "cc");
}
