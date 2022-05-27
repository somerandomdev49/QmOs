#include <qmos/driver.h>
#include <qmos/qmg/qmg.h>
#include <qmos/memory/memory.h>

static uint8_t ansi_in_mode;
static struct {
	int mod;
	int col;
} ansi_in_data;
static uint16_t width, height;
static uint8_t *buffer;

void qmos_driver__qmg_textmode_init()
{
	// ansi_in_mode = 0;
	// ansi_in_data.mod = 0;
	// ansi_in_data.col = 0;
	// width = 40;
	// height = 17;
	// buffer = qmos_alloc(width * height);
}

void qmos_driver__qmg_textmode_deinit() {}


void qmos_driver__qmg_textmode_putchar(int ch, uint16_t x, uint16_t y, uint8_t color)
{
	// Example ANSI \e[0;31m
	if(ch == '\e') { ansi_in_mode = 1; return; }
	switch(ansi_in_mode)
	{
	case 0: {
		qmg_draw_char(ch, x, y, color);
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

uint16_t qmos_driver__qmg_textmode_getchar(uint16_t x, uint16_t y)
{
	uint16_t idx = y * width + x;
	if(idx >= width * height) return 0;
	return buffer[idx];
}
