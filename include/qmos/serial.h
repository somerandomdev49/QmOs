#ifndef QMOS_SERIAL_H
#define QMOS_SERIAL_H
#include <qmos/port_io.h>

int init_serial();
void write_serial(char c);
void writes_serial(const char *s);
void writes_serial_nonl(const char *s);

#endif
