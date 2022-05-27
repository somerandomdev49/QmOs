#include <qmos/qmc/qm/iobuf.h>
#include <qmos/qmc/stdio.h>
#include <qmos/textmode.h>
#include <qmos/qmc/proc.h>
#include <qmos/serial.h>
#include <qmos/qmc/fs.h>

FILE __qmc_stdin_file = 0,
	 __qmc_stdout_file = 1,
	 __qmc_stderr_file = 2;

void fputc(int ch, FILE *stream)
{
	FILEBUF *fb = get_this_proc()->__files[*stream];
	if(fb->__buflen == 1024) fflush(stream);
	fb->__buffer[fb->__buflen++] = ch;
}

static char getc_wrap_(FILEBUF *fb)
{
	// writes_serial("io write (getc_wrap_)");
	int c = fb->__getc_func(fb);
	if(fb->__mode & FI_ECHO_BACK)
		if(c != '\b' || fb->__buflen > 0)
			iobuf_write(fb, c);
	return c;
}

// returns first char
static char fill_buffer_(FILEBUF *fb, char until)
{
	int c, fc = until;
	while((c = getc_wrap_(fb)) != until)
	{
		if(fb->__buflen == 1024)   
		{
			fb->__buflen = 0;
			break;
		}

		if(fc == until) fc = c;
		
		if(c == '\b')
		{
			if(fb->__buflen != 0)
				fb->__buffer[fb->__buflen--] = 0;
		}
		else
		{
			fb->__buffer[fb->__buflen++] = c;
		}
	}

	return fc;
}

char *fgetline(char *s, size_t size, FILE *stream)
{
	FILEBUF *fb = get_this_proc()->__files[*stream];
	if(!(fb->__mode & FM_READ)) return s;

	if(fb->__buflen == 0) fill_buffer_(fb, '\n');
	size_t read = 0;
	for(size_t i = 0; i < size - 2 && i < fb->__buflen; ++i, ++read)
		s[i] = fb->__buffer[i];
	s[size-1] = 0;
	fb->__buflen -= read;

	return s;
}

int fgetc(FILE *stream)
{
	FILEBUF *fb = get_this_proc()->__files[*stream];
	if(!(fb->__mode & FM_READ)) return EOF;
	if(fb->__mode & FI_BUFFER_LINES) return fill_buffer_(fb, '\n');
	else return getc_wrap_(fb);
}

void fflush(FILE *stream)
{
	FILEBUF *fb = get_this_proc()->__files[*stream];
	fb->__flush_func(fb); // iobuf_flush(fb)
	fb->__buflen = 0;
}

void fputs(const char *s, FILE *stream)
{
	while(*s) fputc(*s++, stream);
}

void __printf_impl_fputchar(FILE *f, char c)
{
	fputc(c, f);
}
