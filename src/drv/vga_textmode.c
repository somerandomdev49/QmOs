#include <qmos/vga.h>
#include <stdint.h>
#include <stddef.h>
#include <qmos/serial.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *buffer;
static uint8_t ansi_in_mode;
static struct {
	int mod;
	int col;
} ansi_in_data;

static uint16_t entry(unsigned char uc, uint8_t color) 
{ return (uint16_t) uc | (uint16_t)color << 8; }

void qmos_driver__vga_textmode_init()
{
	buffer = (uint16_t*)0xB8000;
	ansi_in_mode = 0;
	ansi_in_data.mod = 0;
	ansi_in_data.col = 0;
}

void qmos_driver__vga_textmode_deinit() {}

static void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void qmos_driver__vga_textmode_putchar(int ch, uint16_t x, uint16_t y, uint8_t color)
{
	// Example ANSI \e[0;31m
	if(ch == '\e') { ansi_in_mode = 1; return; }
	switch(ansi_in_mode)
	{
	case 0: {
		uint16_t idx = y * VGA_WIDTH + x;
		if(idx >= VGA_WIDTH * VGA_HEIGHT) return;
		buffer[idx] = entry(ch == '\b' ? ' ' : ch, color);
		if(x >= VGA_WIDTH) return;

		if(ch == '\b') update_cursor(x, y);
		else update_cursor(x + 1, y);
	} break;
	case 1:
		if(ch != '[') ansi_in_mode = 0;
		else ansi_in_mode = 2;
		break;
	case 2:
		if(ch >= '0' && ch <= '9')
			ansi_in_data.mod = ansi_in_data.mod * 10 + (ch - '0');
		else ansi_in_mode = 3;
		break;
	case 3:
		if(ch != ';') ansi_in_mode = 0;
		else ansi_in_mode = 4;
		break;
	case 4:
		if(ch >= '0' && ch <= '9')
			ansi_in_data.col = ansi_in_data.col * 10 + (ch - '0');
		else ansi_in_mode = 5;
		break;
	case 5: ansi_in_mode = 0; // Ignore 'm'.
	default: break;
	}
}

uint16_t qmos_driver__vga_textmode_getchar(uint16_t x, uint16_t y)
{
	uint16_t idx = y * VGA_WIDTH + x;
	if(idx >= VGA_WIDTH * VGA_HEIGHT) return 0;
	return buffer[idx];
}
