#ifndef QMOS_PROG_SHELL_H
#define QMOS_PROG_SHELL_H
#include <qmos/qmc/stdlib.h>

typedef int (*qmosh_command_handler_func)(int, char**);

typedef struct
{
	const char *name;
	qmosh_command_handler_func handler;
} qmosh_command_t;

void qmosh_add_command(const char *name, qmosh_command_handler_func handler);

// TODO: A hash table of commands, but too complicated for now.
extern qmosh_command_t *qmosh_command_table;
extern size_t qmosh_command_count;
extern const size_t qmosh_max_command_count;

#endif
