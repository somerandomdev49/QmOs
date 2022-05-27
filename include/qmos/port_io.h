#ifndef QMOS_PORT_IO
#define QMOS_PORT_IO
#include <qmos/kernel.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);
// extern uint8_t inb(uint16_t port);
// extern void outb(uint16_t port, uint8_t data);

#endif
