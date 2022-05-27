#ifndef LIB_QMC_FS_H
#define LIB_QMC_FS_H
#include <stdint.h>
#include <stddef.h>

enum FILEMODE
{
	FM_READ         = 1 << 0,
	FM_WRITE        = 1 << 1,
	FM_READ_TEXT    = 1 << 2,
	FM_WRITE_TEXT   = 1 << 3,
	FI_BUFFER_LINES = 1 << 4,
	FI_ECHO_BACK    = 1 << 5,
};

typedef struct FILEBUF_tag
{
	// The file descriptor
	uint16_t __fd;

	// The length of the buffer contents.
	size_t __buflen;

	// File mode (read/write/...) - enum FILEMODE | enum FILEINFO
	uint8_t __mode;

	// May be anything.
	// for stdin, stdout, stderr -> NULL
	// for a real file this is pointer to the file structure. 
	void *__data;

	// A function that is called when the buffer is flushed.
	void (*__flush_func)(struct FILEBUF_tag*);

	// A function that is called when the io system needs a character.
	int (*__getc_func)(struct FILEBUF_tag*);

	// The buffer.
	char __buffer[1024];
} FILEBUF;

#endif

