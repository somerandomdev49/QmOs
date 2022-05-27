#ifndef LIB_QMC_STDIO_H
#define LIB_QMC_STDIO_H
#include <stdint.h>
#include <stddef.h>
#include <qmos/qmc/file.h>

extern FILE __qmc_stdin_file, __qmc_stdout_file, __qmc_stderr_file;

#define stdin  (&__qmc_stdin_file )
#define stdout (&__qmc_stdout_file)
#define stderr (&__qmc_stderr_file)

#define EOF ((int)-1)

void fputc(int ch, FILE *stream);
void fputs(const char *s, FILE *stream);
int  fgetc(FILE *stream);

/** Not from C stdlib! */
char *fgetline(char *s, size_t size, FILE *stream);

void fflush(FILE *stream);

#include <qmos/qmc/printf.h>

#endif
