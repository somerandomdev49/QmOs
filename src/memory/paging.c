#include <qmos/memory/paging.h>
#include <qmos/memory/memory.h>
#include <qmos/cpu/irq.h>
#include <qmos/serial.h>
#include <qmos/qmc/string.h>

uint32_t *frames;
uint32_t frame_count;

extern uint32_t qmos_alloc_addr;

#define IDX_FROM_BIT(B) (B/32)
#define OFF_FROM_BIT(B) (B%32)

static void set_frame(uint32_t addr)
{
	uint32_t frame = addr / 0x1000;
	frames[IDX_FROM_BIT(frame)] |= (1 << OFF_FROM_BIT(frame));
}

static void clear_frame(uint32_t addr)
{
	uint32_t frame = addr / 0x1000;
	frames[IDX_FROM_BIT(frame)] &= ~(1 << OFF_FROM_BIT(frame));
}

static uint32_t test_frame(uint32_t addr)
{
	uint32_t frame = addr / 0x1000;
	return frames[IDX_FROM_BIT(frame)] & (1 << OFF_FROM_BIT(frame));
}

static uint32_t first_free_frame()
{
	for(uint32_t i = 0; i < IDX_FROM_BIT(frame_count); i++)
	{
		if(frames[i] == 0xFFFFFFFF) continue;
		for(uint32_t j = 0; j < 32; j++)
		{
			if(!(frames[i] & (1 << j))) return i * 32 + j;
		}
	}
	return (uint32_t)-1;
}

void alloc_frame(qmos_page_t *page, int kern, int write)
{
	if(page->frame != 0) return;
	uint32_t idx = first_free_frame();
	if(idx == (uint32_t)-1) { writes_serial("No free frames!"); while(1); }
	set_frame(idx * 0x1000);
	page->present = 1;
	page->rw = write ? 1 : 0;
	page->user = kern ? 0 : 1;
	page->frame = idx;
}

void free_frame(qmos_page_t *page)
{
	uint32_t frame = page->frame;
	if(!frame) return;
	clear_frame(frame);
	page->frame = 0x0;
}

qmos_page_directory_t *current_dir;
qmos_page_directory_t *kernel_dir;
void qmos_init_paging()
{
	uint32_t mem_end_page = 0x1000000;
	frame_count = mem_end_page / 0x1000;
	frames = qmos_alloc(IDX_FROM_BIT(frame_count));
	memset(frames, 0, IDX_FROM_BIT(frame_count));
	kernel_dir = qmos_alloc_aligned(sizeof(qmos_page_directory_t));
	memset(kernel_dir, 0, sizeof(qmos_page_directory_t));

	for(uint32_t i = 0; i < qmos_alloc_addr; i += 0x1000)
		alloc_frame(qmos_get_page(i, 1, kernel_dir), 0, 0);
	
	qmos_reg_int_handler(14, &qmos_page_fault_handler);
	qmos_switch_page_dir(kernel_dir);
}

void qmos_switch_page_dir(qmos_page_directory_t *dir)
{
	current_dir = kernel_dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tables_phys));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

qmos_page_t *qmos_get_page(uint32_t addr, int make, qmos_page_directory_t *dir)
{
	addr /= 0x1000;
	uint32_t idx = addr / 1024;
	if(dir->tables[idx])
	{
		return &dir->tables[idx]->pages[addr%1024];
	}

	if(make)
	{
		uint32_t tmp;
		dir->tables[idx] = qmos_alloc_aligned_phys(sizeof(qmos_page_table_t), &tmp);
		memset(dir->tables[idx], 0, 0x1000);
		dir->tables_phys[idx] = tmp | 0x7;
		return &dir->tables[idx]->pages[addr % 1024];
	}

	return 0;
}

#include <qmos/qmc/stdio.h>
void qmos_page_fault_handler(qmos_registers_t regs)
{
	// writes_serial("Page Fault Handler called!");
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	// The error code gives us details of what happened.
	int present 	= !(regs.err_code & 0x1);
	int rw 			= regs.err_code & 0x2;
	int us 			= regs.err_code & 0x4;
	int reserved 	= regs.err_code & 0x8;
	// int id 			= regs.err_code & 0x10;

	
	// terminal_set_fg(VGA_COLOR_RED);
	// TODO: terminal_get_fg/bg() -> uint8_t (or single terminal_get_col())
	printf("Page Fault @ 0x%x: ");
	if(present) 	fputs("present " , stdout);
	if(rw) 			fputs("readonly ", stdout);
	if(us) 			fputs("usermode ", stdout);
	if(reserved)	fputs("reserved ", stdout);
	printf("\n");
	fflush(stdout);
	while(1) asm volatile("hlt");
}
