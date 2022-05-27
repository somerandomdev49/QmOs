#ifndef QMOS_TEXTMODE
#define QMOS_TEXTMODE
#include <qmos/vga.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
// static inline uint16_t vga_entry(unsigned char uc, uint8_t color);

/** Initializes the terminal. */
void terminal_initialize(void);

/** Sets the background color of the terminal. */
void terminal_set_bg(uint8_t color);

/** Sets the foreground color of the terminal. */
void terminal_set_fg(uint8_t color);

// void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

/** Writes a character in a terminal. */
void terminal_putchar(int c);

/** Writes a string in a terminal. @deprecated */
void terminal_putstring(const char* data);

/** Clears the terminal. */
void terminal_clear();

/** Retreives a character from the terminal the terminal. @deprecated */
int terminal_getchar();

#endif
