#include <qmos/kernel.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <qmos/textmode.h>
#include <qmos/vga.h>
#include <qmos/qmg/qmg.h>
#include <qmos/qmc/stdio.h>
#include <qmos/qmc/proc.h>
#include <qmos/qmc/stdio.h>
#include <qmos/driver.h>
#include <qmos/serial.h>
#include <qmos/cpu/gdt.h>
#include <qmos/memory/memory.h>
#include <qmos/memory/paging.h>
#include <qmos/qmc/printf.h>

#define DRIVERLIST(TYPE, COUNT) \
	static struct TYPE##_driver_list_t \
	{ \
		TYPE##_driver_t drivers[COUNT]; \
		uint16_t current; \
	} TYPE##_driver_list; \
	TYPE##_driver_t *TYPE##_driver_current;

#define DRIVER_SWITCH(TYPE, IDX) (\
	(TYPE##_driver_list.current = IDX), \
	(TYPE##_driver_current = &TYPE##_driver_list.drivers[IDX]))


DRIVERLIST(textmode, 2);
DRIVERLIST(graphics, 1);
DRIVERLIST(keyboard, 1);

extern PROC *__qmc_current_proc;

static void init_drivers();
static void init_proc();
static void init_stdio();

kernel_info_t qmos_kernel_info = { "QmOs", VERSION(0, 1, 0) };

void printf_serial_writec(char c, void* arg)
{
	(void)arg;
	write_serial(c);
}

void kernel_main(uint32_t magic, void *ptr) 
{
	asm volatile("cli");
	init_serial();
	graphics_driver_info_t *framebuffer = (graphics_driver_info_t*)(*(uint32_t*)(ptr + 88));
	fctprintf(&printf_serial_writec, NULL, "magic: 0x%x, ptr: 0x%lx, fbuf_addr: 0x%x\n", magic, ptr, framebuffer->framebuffer_addr);
	
	writes_serial("initializing descriptor tables...");
	qmos_init_gdt();

	writes_serial("initializing idt...");
	qmos_init_idt();
	
	asm volatile("sti");

	writes_serial("initializing paging...");
	qmos_init_paging();

	writes_serial("initializing proc...");
	init_proc();

	writes_serial("initializing drivers...");
	init_drivers();

	writes_serial("initializing stdio...");
	init_stdio(framebuffer);

	writes_serial("initializing terminal...");
	terminal_initialize();
	writes_serial("done");

	terminal_set_fg(VGA_COLOR_GREEN);
	terminal_putstring("Terminal Initialized.\n");
	terminal_set_fg(VGA_COLOR_LIGHT_GREY);

	extern uint32_t qmos_alloc_addr;
	printf("Alloc Placement Address: 0x%X\n", qmos_alloc_addr);

	extern void shell_main();
 	shell_main();

	terminal_set_fg(VGA_COLOR_MAGENTA);
	printf("Kernel Terminated!\n");
	fflush(stdout);
	terminal_set_fg(VGA_COLOR_LIGHT_GREY);
}

static void init_proc()
{
	static PROC current_proc;
	__qmc_current_proc = &current_proc;
	__qmc_current_proc->__name = "kernel";
}

static void init_stdio(graphics_driver_info_t *framebuffer)
{
	extern void iobuf_flush(FILEBUF *fb);
	extern void iobuf_err_flush(FILEBUF *fb);
	extern int iobuf_getc(FILEBUF *fb);
	extern int iobuf_err_getc(FILEBUF *fb);

	static FILEBUF fb_stdin  = { 0, 0, FM_READ|FI_BUFFER_LINES|FI_ECHO_BACK, NULL, &iobuf_err_flush, &iobuf_getc, {0} };
	static FILEBUF fb_stdout = { 0, 0, FM_WRITE, NULL, &iobuf_flush, &iobuf_getc, {0} };
	static FILEBUF fb_stderr = { 0, 0, FM_WRITE, NULL, &iobuf_flush, &iobuf_getc, {0} };
	static FILEBUF *stdio_files[] = { &fb_stdin, &fb_stdout, &fb_stderr };

	__qmc_current_proc->__filecount = 3;
	__qmc_current_proc->__files = stdio_files;

	graphics_driver_current->d.init(framebuffer);
	
	// graphics_driver_current->d.deinit();

	for(int x = 0; x < 320; ++x)
	    for(int y = 0; y < 200; ++y)
			graphics_driver_current->putpixel(x, y, 0);
}

static void init_drivers()
{
	extern void qmos_driver__vga_textmode_init();
	extern void qmos_driver__vga_textmode_deinit();
	extern void qmos_driver__vga_textmode_putchar(int, uint16_t,  uint16_t,  uint8_t);
	extern uint16_t qmos_driver__vga_textmode_getchar(uint16_t,  uint16_t);

	DRIVER_SWITCH(textmode, 0);

	textmode_driver_current->d.name = "VGA [80x25 Textmode] Textmode Driver";
	textmode_driver_current->d.version = VERSION(0, 0, 1);
	textmode_driver_current->d.init = &qmos_driver__vga_textmode_init;
	textmode_driver_current->d.deinit = &qmos_driver__vga_textmode_deinit;
	textmode_driver_current->putchar = &qmos_driver__vga_textmode_putchar;
	textmode_driver_current->getchar = &qmos_driver__vga_textmode_getchar;
	textmode_driver_current->max_x = 80;
	textmode_driver_current->max_y = 25;
	textmode_driver_current->info = 0
		| QMOS_TEXTMODE_DRV_FAST_CLEAR
		| QMOS_TEXTMODE_DRV_HAS_COLOR
		| QMOS_TEXTMODE_DRV_CLEAR_COLOR
		;
	
	extern void qmos_driver__qmg_textmode_init();
	extern void qmos_driver__qmg_textmode_deinit();
	extern void qmos_driver__qmg_textmode_putchar(int, uint16_t,  uint16_t,  uint8_t);
	extern uint16_t qmos_driver__qmg_textmode_getchar(uint16_t,  uint16_t);

	DRIVER_SWITCH(textmode, 1);

	textmode_driver_current->d.name = "QMG Textmode Driver";
	textmode_driver_current->d.version = VERSION(0, 0, 1);
	textmode_driver_current->d.init = &qmos_driver__qmg_textmode_init;
	textmode_driver_current->d.deinit = &qmos_driver__qmg_textmode_deinit;
	textmode_driver_current->putchar = &qmos_driver__qmg_textmode_putchar;
	textmode_driver_current->getchar = &qmos_driver__qmg_textmode_getchar;
	textmode_driver_current->max_x = 40;
	textmode_driver_current->max_y = 17;
	textmode_driver_current->info = 0
		// | QMOS_TEXTMODE_DRV_FAST_CLEAR
		| QMOS_TEXTMODE_DRV_HAS_COLOR
		// | QMOS_TEXTMODE_DRV_CLEAR_COLOR
		;

	// DRIVER_SWITCH(textmode, 1);

	extern void qmos_driver__vga_graphics_init(uint32_t *info);
	extern void qmos_driver__vga_graphics_putpixel(uint16_t, uint16_t, uint8_t);

	DRIVER_SWITCH(graphics, 0);

	graphics_driver_current->d.name = "VGA [320x200 Graphics] Texmode Driver";
	graphics_driver_current->d.version = VERSION(0, 0, 1);
	graphics_driver_current->d.init = &qmos_driver__vga_graphics_init;
	graphics_driver_current->d.deinit = NULL;
	graphics_driver_current->putpixel = &qmos_driver__vga_graphics_putpixel;
	graphics_driver_current->info = 0
		// | QMOS_TEXTMODE_DRV_FAST_CLEAR
		| QMOS_TEXTMODE_DRV_HAS_COLOR
		// | QMOS_TEXTMODE_DRV_CLEAR_COLOR
		;

	extern void qmos_driver__ps2_keyboard_init();
	extern void qmos_driver__ps2_keyboard_deinit();
	extern int  qmos_driver__ps2_keyboard_getc();

	DRIVER_SWITCH(keyboard, 0);
	
	keyboard_driver_current->d.name = "PS/2 ASCII Keyboard Driver";
	keyboard_driver_current->d.version = VERSION(0, 0, 1);
	keyboard_driver_current->d.init = &qmos_driver__ps2_keyboard_init;
	keyboard_driver_current->d.deinit = &qmos_driver__ps2_keyboard_deinit;
	keyboard_driver_current->getchar = &qmos_driver__ps2_keyboard_getc;
}

