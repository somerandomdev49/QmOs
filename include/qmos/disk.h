#ifndef QMOS_FS_H
#define QMOS_FS_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SECTOR_SIZE 512

void qmos_init_fs();
bool qmos_fs_write(uint32_t index, uint8_t section_count, const void *source);
bool qmos_fs_read(uint32_t index, uint8_t section_count, void *dest);

#endif//QMOS_FS_H
