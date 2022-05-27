#include <qmos/textmode.h>
#include <qmos/driver.h>

static uint8_t current_color;
static uint16_t cur_x;
static uint16_t cur_y;

static uint8_t textmode_color(enum vga_color fg, enum vga_color bg) 
{ return fg | bg << 4; }

extern textmode_driver_t *textmode_driver_current;
extern keyboard_driver_t *keyboard_driver_current;

// TODO: Handle drivers with no color support! (clear funcs can always call fast_clear)

static void driver_fast_clear(uint8_t col);
static void driver_fast_clear_nocol(uint8_t col);
static void driver_manual_clear(uint8_t col);
static void (*clear_func)(uint8_t);

void terminal_initialize(void)
{
	textmode_driver_current->d.init(NULL);
	keyboard_driver_current->d.init(NULL);
	cur_x = cur_y = 0;
	current_color = textmode_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	uint8_t drv_info = textmode_driver_current->info;

	clear_func = &driver_manual_clear;

	if((drv_info & QMOS_TEXTMODE_DRV_FAST_CLEAR) &&
	  (~drv_info & QMOS_TEXTMODE_DRV_CLEAR_COLOR)) clear_func = &driver_fast_clear_nocol;
	
	else if((drv_info & QMOS_TEXTMODE_DRV_FAST_CLEAR) &&
	  ( drv_info & QMOS_TEXTMODE_DRV_CLEAR_COLOR)) clear_func = &driver_fast_clear;
	
	else if(~drv_info & QMOS_TEXTMODE_DRV_FAST_CLEAR) clear_func = &driver_manual_clear;
	
	terminal_clear();
}

void terminal_set_fg(uint8_t color) { current_color = color | (current_color & 0xf0); };
void terminal_set_bg(uint8_t color) { current_color = (current_color & 0x0f) | color << 4; };

void scroll()
{
	if(cur_y != textmode_driver_current->max_y) return;
	--cur_y;
	for(uint16_t y = 1; y != textmode_driver_current->max_y; ++y)
		for(uint16_t x = 0; x != textmode_driver_current->max_x; ++x)
		{
			uint16_t e = textmode_driver_current->getchar(x, y);
			textmode_driver_current->putchar(e & 0x00ff, x, y - 1, e >> 8);
		}
	for(uint16_t x = 0; x != textmode_driver_current->max_x; ++x)
	{
		textmode_driver_current->putchar(' ', x, textmode_driver_current->max_y - 1, 0);
	}
}

void terminal_putchar(int c)
{
	// writes_serial("io write (terminal_putchar)");
	if(c == '\n') { ++cur_y; cur_x = 0; scroll(); return; }
	if(c == '\r') { cur_x = 0; scroll(); return; }
	if(c == '\b')
	{
		if(cur_x == 0) cur_x = 1;
		textmode_driver_current->putchar('\b', --cur_x, cur_y, current_color);
		return;
	}

	textmode_driver_current->putchar(c, cur_x, cur_y, current_color);
	++cur_x;
	if(cur_x == textmode_driver_current->max_x) { ++cur_y; cur_x = 0; scroll(); }  
}

int terminal_getchar()
{
	return keyboard_driver_current->getchar();
}

void terminal_clear()
{
	driver_manual_clear(current_color);
	textmode_driver_current->putchar(' ', cur_x = 0, cur_y = 0, current_color);
}

void driver_fast_clear(uint8_t col) { textmode_driver_current->fast_clear(col); }
void driver_fast_clear_nocol(uint8_t col)
{
	if(col == 0) driver_fast_clear(0);
	else driver_manual_clear(col);
}

void driver_manual_clear(uint8_t col)
{
	for(uint16_t y = 0; y != textmode_driver_current->max_y; ++y)
	for(uint16_t x = 0; x != textmode_driver_current->max_x; ++x)
		textmode_driver_current->putchar(' ', x, y, col);
}

void terminal_putstring(const char *s)
{
	while(*s) terminal_putchar(*s++);
}
