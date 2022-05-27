#ifndef QMOS_DRIVER_H
#define QMOS_DRIVER_H
#include <stdint.h>
#include <qmos/typedefs.h>

typedef struct
{
	const char *name;
	version_t version;
	void (*init)(void *info);
	void (*deinit)();
} driver_t;

enum
{
	/** Can the driver draw with color? */
	QMOS_GRAPHICS_DRV_HAS_COLOR   = 1,

	/** Can the driver clear the screen quickly? */
	QMOS_GRAPHICS_DRV_FAST_CLEAR  = 2,

	/** Can the drvier clear with color? */
	QMOS_GRAPHICS_DRV_CLEAR_COLOR = 4,
};

typedef struct
{
	driver_t d;
	uint8_t info;
	uint8_t palette[16];
	void (*putpixel)(uint16_t, uint16_t, uint8_t);
	void (*fast_clear)(uint8_t);
} graphics_driver_t;

enum
{
	/** Does the driver have the ability to print with color? */
	QMOS_TEXTMODE_DRV_HAS_COLOR   = 1,

	/** Can the driver set a character at a specific location? */
	QMOS_TEXTMODE_DRV_RAND_ACCESS = 2,
	
	/** Can the driver clear the screen quickly? */
	QMOS_TEXTMODE_DRV_FAST_CLEAR  = 4,

	/** Can the drvier clear with color? */
	QMOS_TEXTMODE_DRV_CLEAR_COLOR = 8,

	/** Can the drvier read a char written before? */
	QMOS_TEXTMODE_DRV_HAS_GETCHAR = 16,
};

typedef struct
{
	driver_t d;
	uint8_t info;
	uint8_t palette[16];
	uint16_t max_x, max_y;
	void (*putchar)(int c, uint16_t x, uint16_t y, uint8_t color);
	uint16_t (*getchar)(uint16_t x, uint16_t y);
	void (*fast_clear)(uint8_t color);
} textmode_driver_t;

typedef struct
{
	driver_t d;
	int (*getchar)();
} keyboard_driver_t;

typedef struct
{
	driver_t d;
	void *(*open)(const char *path);
	void (*close)(void *f);
	void (*getsize)(void *f);
	void (*read)(void *f, uint32_t limit, void *target);
	void (*write)(void *f, uint32_t limit, void *source);
} fs_driver_t;

typedef struct
{
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
  	uint8_t framebuffer_type;
	union
	{
		struct
		{
			uint32_t framebuffer_palette_addr;
			uint16_t framebuffer_palette_num_colors;
		};
		struct
		{
		 	uint8_t framebuffer_red_field_position;
		 	uint8_t framebuffer_red_mask_size;
		 	uint8_t framebuffer_green_field_position;
		 	uint8_t framebuffer_green_mask_size;
		 	uint8_t framebuffer_blue_field_position;
		 	uint8_t framebuffer_blue_mask_size;
		};
	};
} graphics_driver_info_t;

#endif

