#include <qmos/memory/memory.h>

typedef struct
{
	uint32_t magic;
	uint8_t is_hole;
	uint32_t size;
} header_t;

typedef struct
{
	uint32_t magic; // same as in header_t
	header_t *hd;
} footer_t;


extern uint8_t __addr_end;
uint32_t qmos_alloc_addr = (uint32_t)&__addr_end;
static void *qmos_alloc_int(uint32_t sz, int align, uint32_t *phys)
{
	if(align && (qmos_alloc_addr & 0xFFFFF000))
	{
		qmos_alloc_addr &= 0xFFFFF000;
		qmos_alloc_addr += 0x1000;
	}

	if(phys) *phys = qmos_alloc_addr;
	uint32_t tmp = qmos_alloc_addr;
	qmos_alloc_addr += sz;
	return (uint32_t)tmp;
}

void *qmos_alloc_aligned(uint32_t size)
{
	return qmos_alloc_int(size, 1, 0);
}

void *qmos_alloc_phys(uint32_t size, uint32_t *phys)
{
	return qmos_alloc_int(size, 0, phys);
}

void *qmos_alloc_aligned_phys(uint32_t size, uint32_t *phys)
{
	return qmos_alloc_int(size, 1, phys);
}

void *qmos_alloc(uint32_t size)
{
	return qmos_alloc_int(size, 0, 0);
}

