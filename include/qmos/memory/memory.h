#ifndef QMOS_MEMORY_H
#define QMOS_MEMORY_H
#include <stdint.h>

void *qmos_alloc_aligned(uint32_t size);
void *qmos_alloc_phys(uint32_t size, uint32_t *phys);
void *qmos_alloc_aligned_phys(uint32_t size, uint32_t *phys);
void *qmos_alloc(uint32_t size);

#endif
