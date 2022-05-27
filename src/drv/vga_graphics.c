#include <qmos/port_io.h>
#include <qmos/kernel.h>
#include <qmos/vga.h>

// new code from https://forum.osdev.org/viewtopic.php?f=1&t=10534

#define VGA_ADDRESS 0xA0000
#define VGA_MAX 0xF9FF
#define VGA_MAX_WIDTH 320
#define VGA_MAX_HEIGHT 200

// /* Attribute Controller Registers */
// #define	VGA_AC_INDEX 0x3C0
// #define	VGA_AC_READ 0x3C1
// #define	VGA_AC_WRITE 0x3C0

// /* Miscellaneous Output */
// #define	VGA_MISC_READ 0x3CC
// #define	VGA_MISC_WRITE 0x3C2

// /* Sequencer Registers */
// #define VGA_SEQ_INDEX 0x3C4
// #define VGA_SEQ_DATA 0x3C5

// /* VGA Color Palette Registers */
// #define	VGA_DAC_READ_INDEX 0x3C7
// #define	VGA_DAC_WRITE_INDEX 0x3C8
// #define	VGA_DAC_DATA 0x3C9

// /* Graphics Controller Registers */
// #define VGA_GC_INDEX 0x3CE
// #define VGA_GC_DATA 0x3CF

// /* CRT Controller Registers */
// #define VGA_CRTC_INDEX 0x3D4
// #define VGA_CRTC_DATA 0x3D5

// /* General Control and Status Registers */
// #define	VGA_INSTAT_READ 0x3DA


// /*
// See Intel® OpenSource HD Graphics PRM pdf file
// for following defined data for each vga register
// and its explaination
// */
// static uint8_t seq_data[5] = {0x03, 0x01, 0x0F, 0x00, 0x0E};
// static uint8_t crtc_data[25] = {0x5F, 0x4F, 0x50, 0x82, 
//                               0x54, 0x80, 0xBF, 0x1F,
//                               0x00, 0x41, 0x00, 0x00, 
//                               0x00, 0x00, 0x00, 0x00,
//                               0x9C, 0x0E, 0x8F, 0x28,	
//                               0x40, 0x96, 0xB9, 0xA3,
//                               0xFF};

// static uint8_t gc_data[9] = {0x00, 0x00, 0x00, 0x00, 
//                           0x00, 0x40, 0x05, 0x0F,
//                           0xFF};

// static uint8_t ac_data[21] = {0x00, 0x01, 0x02, 0x03,
//                             0x04, 0x05, 0x06, 0x07,
//                             0x08, 0x09, 0x0A, 0x0B,
//                             0x0C, 0x0D, 0x0E, 0x0F,
//                             0x41, 0x00, 0x0F, 0x00,
//                             0x00};

// static void set_misc() { outb(VGA_MISC_WRITE, 0x63); }

// static void set_seq()
// {
// 	for(uint8_t index = 0; index < 5; index++)
// 	{
// 		outb(VGA_SEQ_INDEX, index);
//     	outb(VGA_SEQ_DATA, seq_data[index]);
// 	}
// }

// static void set_crtc()
// {
// 	for(uint8_t index = 0; index < 25; index++){
// 		outb(VGA_CRTC_INDEX, index);
// 		outb(VGA_CRTC_DATA, crtc_data[index]);
// 	}
// }

// static void set_gc()
// {
// 	for(uint8_t index = 0; index < 9; index++){
// 		outb(VGA_GC_INDEX, index);
// 		outb(VGA_GC_DATA, gc_data[index]);
// 	}
// }

// static void set_ac()
// {
// 	uint8_t d;
// 	for(uint8_t index = 0; index < 21; index++){
// 		outb(VGA_AC_INDEX, index);
// 		outb(VGA_AC_WRITE, ac_data[index]);
// 	}
// 	d = inb(VGA_INSTAT_READ);
// 	outb(VGA_AC_INDEX, d | 0x20);
// }
typedef char byte;
typedef unsigned short word;
typedef unsigned long dword;

#define SZ(x) (sizeof(x)/sizeof(x[0]))

// a global VGA buffer
static uint32_t *g_vga_buffer;

// misc out (3c2h) value for various modes

#define R_COM  0x63 // "common" bits

#define R_W256 0x00
#define R_W320 0x00
#define R_W360 0x04
#define R_W376 0x04
#define R_W400 0x04

#define R_H200 0x00
#define R_H224 0x80
#define R_H240 0x80
#define R_H256 0x80
#define R_H270 0x80
#define R_H300 0x80
#define R_H360 0x00
#define R_H400 0x00
#define R_H480 0x80
#define R_H564 0x80
#define R_H600 0x80

static const byte hor_regs [] = { 0x0,  0x1,  0x2,  0x3,  0x4, 0x5,  0x13 };
static const byte width_256[] = { 0x5f, 0x3f, 0x40, 0x82, 0x4a, 0x9a, 0x20 };
static const byte width_320[] = { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0x28 };
static const byte width_360[] = { 0x6b, 0x59, 0x5a, 0x8e, 0x5e, 0x8a, 0x2d };
static const byte width_376[] = { 0x6e, 0x5d, 0x5e, 0x91, 0x62, 0x8f, 0x2f };
static const byte width_400[] = { 0x70, 0x63, 0x64, 0x92, 0x65, 0x82, 0x32 };

static const byte ver_regs  [] = { 0x6,  0x7,  0x9,  0x10, 0x11, 0x12, 0x15, 0x16 };

static const byte height_200[] = { 0xbf, 0x1f, 0x41, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
static const byte height_224[] = { 0x0b, 0x3e, 0x41, 0xda, 0x9c, 0xbf, 0xc7, 0x04 };
static const byte height_240[] = { 0x0d, 0x3e, 0x41, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
static const byte height_256[] = { 0x23, 0xb2, 0x61, 0x0a, 0xac, 0xff, 0x07, 0x1a };
static const byte height_270[] = { 0x30, 0xf0, 0x61, 0x20, 0xa9, 0x1b, 0x1f, 0x2f };
static const byte height_300[] = { 0x70, 0xf0, 0x61, 0x5b, 0x8c, 0x57, 0x58, 0x70 };
static const byte height_360[] = { 0xbf, 0x1f, 0x40, 0x88, 0x85, 0x67, 0x6d, 0xba };
static const byte height_400[] = { 0xbf, 0x1f, 0x40, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
static const byte height_480[] = { 0x0d, 0x3e, 0x40, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
static const byte height_564[] = { 0x62, 0xf0, 0x60, 0x37, 0x89, 0x33, 0x3c, 0x5c };
static const byte height_600[] = { 0x70, 0xf0, 0x60, 0x5b, 0x8c, 0x57, 0x58, 0x70 };

// the chain4 parameter should be 1 for normal 13h-type mode, but
// only allows 320x200 256x200, 256x240 and 256x256 because you
// can only access the first 64kb

// if chain4 is 0, then plane mode is used (tweaked modes), and
// you'll need to switch planes to access the whole screen but
// that allows you using any resolution, up to 400x600

// returns 1=ok, 0=fail
int init_graph_vga(int width, int height, int chain4)
{
	const byte *w, *h;
	byte val;

	switch(width)
	{
		case 256: w = width_256; val = R_COM + R_W256; break;
		case 320: w = width_320; val = R_COM + R_W320; break;
		case 360: w = width_360; val = R_COM + R_W360; break;
		case 376: w = width_376; val = R_COM + R_W376; break;
		case 400: w = width_400; val = R_COM + R_W400; break;
		default: return 0; // fail
	}

	switch(height)
	{
		case 200: h = height_200; val |= R_H200; break;
		case 224: h = height_224; val |= R_H224; break;
		case 240: h = height_240; val |= R_H240; break;
		case 256: h = height_256; val |= R_H256; break;
		case 270: h = height_270; val |= R_H270; break;
		case 300: h = height_300; val |= R_H300; break;
		case 360: h = height_360; val |= R_H360; break;
		case 400: h = height_400; val |= R_H400; break;
		case 480: h = height_480; val |= R_H480; break;
		case 564: h = height_564; val |= R_H564; break;
		case 600: h = height_600; val |= R_H600; break;
		default: return 0; // fail
	}

	// chain4 not available if mode takes over 64k
	if(chain4 && (long)width * (long)height > 65536L) return 0;

	// here goes the actual modeswitch
	outb(0x3c2, val);
	outw(0x3d4, 0x0e11); // enable regs 0-7

	for(unsigned long a = 0; a < SZ(hor_regs); ++a)
		outw(0x3d4, (w[a] << 8) + hor_regs[a]);

	for(unsigned long a = 0; a < SZ(ver_regs); ++a)
		outw(0x3d4, (h[a] << 8) + ver_regs[a]);

	outw(0x3d4, 0x0008); // vert.panning = 0

	if(chain4)
	{
		outw(0x3d4, 0x4014);
		outw(0x3d4, 0xa317);
		outw(0x3c4, 0x0e04);
	}
	else
	{
		outw(0x3d4, 0x0014);
		outw(0x3d4, 0xe317);
		outw(0x3c4, 0x0604);
	}

	outw(0x3c4, 0x0101);
	outw(0x3c4, 0x0f02); // enable writing to all planes
	outw(0x3ce, 0x4005); // 256 color mode
	outw(0x3ce, 0x0506); // graph mode & A000-AFFF

	inb(0x3da);
	outb(0x3c0, 0x30); outb(0x3c0, 0x41);
	outb(0x3c0, 0x33); outb(0x3c0, 0x00);

	for(byte a = 0; a < 16; ++a)
	{
		// ega pal
		outb(0x3c0, a);
		outb(0x3c0, a);
	}
	
	outb(0x3c0, 0x20); // enable video

	return 1;

}

static void clear_color(uint8_t color)
{
	for(uint32_t index = 0; index < VGA_MAX; index++)
		g_vga_buffer[index] = color;
}

void qmos_driver__vga_graphics_init(uint32_t *addr)
{
	// init_graph_vga(400, 360, 0);
	g_vga_buffer = addr;
}

void qmos_driver__vga_graphics_putpixel(uint16_t x, uint16_t y, uint8_t color)
{
	uint32_t index = 1024 * y + x;
	if(index < VGA_MAX)
		g_vga_buffer[index] = color;
}
