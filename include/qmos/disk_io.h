#ifndef QMOS_FS_IO_H
#define QMOS_FS_IO_H

void insl(int port, void *addr, int count);
void outsl(int port, const void *addr, int count);

#endif//QMOS_FS_IO_H
