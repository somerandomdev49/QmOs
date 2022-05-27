#include <stdint.h>

void qmos_ata_read(uint32_t block_addr, uint16_t sector_num, void *out_data)
{
	(void)block_addr;
	(void)sector_num;
	(void)out_data;
}

int main(int argc, char **argv)
{
	qmos_ata_read(10, 231, (void*)69420);
}
