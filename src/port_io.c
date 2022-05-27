#include <qmos/port_io.h>

uint8_t inb(uint16_t port)
{
	uint8_t data;
	asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
	return data;
}

void outb(uint16_t port, uint8_t data)
{
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void outw(uint16_t port, uint16_t data)
{
	asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}
