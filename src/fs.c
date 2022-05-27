#include <qmos/disk.h>
#include <qmos/port_io.h>
#include <qmos/pci.h>
#include <qmos/qmc/stdio.h>
#include <qmos/disk_io.h>

#define IDE_BSY 0x80
#define IDE_DRDY 0x40
#define IDE_DF 0x20
#define IDE_ERR 0x01

static int disk_id = 1;
static uint32_t io_base;

uint32_t qmos_ide_get_io_base()
{
	return 0x1f0;
}

void qmos_init_fs()
{
	io_base = qmos_ide_get_io_base();
	if(io_base < 2)
	{
		printf("No disk found! PCI io base: 0x%x\n", io_base);
	}
}

static bool ide_wait_ready(bool check_error)
{
	int r;

	while(1)
	{
		r = inb(io_base + 7);

		if(r == 0xff) { printf("Floating bus (no IDE drives)\n"); return -1; }

 		if((r & (IDE_BSY | IDE_DRDY)) == IDE_DRDY) break;
	}

	if(check_error && (r & (IDE_DF | IDE_ERR)) != 0) return false;
	return true;
}

bool qmos_fs_write(uint32_t index, uint8_t section_count, const void *source)
{
	ide_wait_ready(false);
	outb(io_base + 2, section_count);
	outb(io_base + 3, index & 0xff);
	outb(io_base + 4, (index >> 8) & 0xff);
	outb(io_base + 5, (index >> 16) & 0xff);
	outb(io_base + 6, 0xE0 | ((disk_id & 1) << 4) | ((index >> 24) & 0x0f));
	outb(io_base + 7, 0x30);

	for(; section_count > 0; section_count--, source += SECTOR_SIZE) {
		if(!ide_wait_ready(true)) return false;
		outsl(io_base, source, SECTOR_SIZE / 4);
	}

	// https://github.com/tehybel/osdev/blob/hardware/fs/ide.c
	return true;
}

bool qmos_fs_read(uint32_t index, uint8_t section_count, void *dest)
{
	ide_wait_ready(false);
	outb(io_base + 2, section_count);
	outb(io_base + 3, index & 0xff);
	outb(io_base + 4, (index >> 8) & 0xff);
	outb(io_base + 5, (index >> 16) & 0xff);
	outb(io_base + 6, 0xE0 | ((disk_id & 1) << 4) | ((index >> 24) & 0x0f));
	outb(io_base + 7, 0x20);

	for(; section_count > 0; section_count--, dest += SECTOR_SIZE) {
		if(!ide_wait_ready(true)) return false;
		insl(io_base, dest, SECTOR_SIZE / 4);
	}

	return true;
}


