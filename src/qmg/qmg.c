#include <qmos/qmg/qmg.h>
#include <qmos/driver.h>
#define GLOBAL_MARGIN 10
extern graphics_driver_t *graphics_driver_current;

static void putpixel(uint16_t x, uint16_t y, uint8_t c)
{
	graphics_driver_current->putpixel(x, y, c);
}

static void draw_bitmap_fontB(uint8_t idx, uint16_t off_x, uint16_t off_y, uint8_t color)
{
	const uint8_t *c = FONT_B[idx];
	for(int y = 0; y != 8; ++y)
		for(int x = 0; x != 8; ++x)
			if(c[y] & 1 << x)
				putpixel(x + off_x + GLOBAL_MARGIN, y + off_y + GLOBAL_MARGIN, color);
}

static void draw_bitmap_fontA(uint8_t idx, uint16_t off_x, uint16_t off_y, uint8_t color)
{
	const uint8_t *c = FONT_A[idx];
	for(int y = 0; y < 13; ++y)
		for(int x = 7; x >= 0; --x)
			if(c[y] & 1 << x)
				putpixel(8 - x + off_x, 13 - y + off_y, color);
}

typedef void (*bitmapper_func)(uint8_t, uint16_t, uint16_t, uint8_t);

static uint8_t current_font = 1;
static bitmapper_func bitmappers[] = { &draw_bitmap_fontA, &draw_bitmap_fontB };
static uint8_t font_sizes[] = { 8, 13, 8, 8 };

static void draw_bitmap(uint8_t idx, uint16_t off_x, uint16_t off_y, uint8_t color)
{ bitmappers[current_font](idx, off_x, off_y, color); }

void qmg_draw_char(char c, uint16_t x, uint16_t y, uint8_t color)
{
	// if(c < 32 || c > 126) return;
	draw_bitmap(c, x * (font_sizes[current_font*2] + 2), y * (font_sizes[current_font*2 + 1] + 2), color);
}

void qmg_draw_string(const char *c, uint16_t x, uint16_t y, uint8_t color)
{
	uint16_t xo = 0, yo = 0;
	for(; *c; ++c)
		if(*c == '\n') ++yo, xo = 0;
		else qmg_draw_char(*c, x + xo++, y + yo, color);
}

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color)
{
	for(uint16_t xx = x; xx < x + w; ++xx)
	for(uint16_t yy = y; yy < y + h; ++yy)
		putpixel(xx, yy, color);
}
