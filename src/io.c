#include <qmos/textmode.h>
#include <qmos/serial.h>
#include <qmos/qmc/stdio.h>
#include <qmos/qmc/fs.h>
#include <stddef.h>

void iobuf_write(FILEBUF *fb, char c)
{ (void)fb;
	// writes_serial("io write (iobuf_write)");
	terminal_putchar(c);
}

void iobuf_flush(FILEBUF *fb)
{
	(void)fb;
	for(size_t i = 0; i != fb->__buflen; ++i)
		terminal_putchar(fb->__buffer[i]);
}

void iobuf_err_flush(FILEBUF *_)
{ (void)_;
	terminal_putstring("Tried to flush stdin!");
}

int iobuf_getc(FILEBUF *fb)
{ (void)fb;
	char c = terminal_getchar();
	return c;
}
int iobuf_err_getc(FILEBUF *_) { (void)_; terminal_putstring("Tried to getc stdout/stderr!"); return EOF; }
