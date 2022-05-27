#ifndef LIB_QMC_PROC_H
#define LIB_QMC_PROC_H
#include <stdint.h>
#include <stddef.h>
#include <qmos/qmc/fs.h>

typedef struct
{
	const char *__name;
	size_t __filecount;
	FILEBUF **__files;
} PROC;

PROC *get_this_proc();

#endif

