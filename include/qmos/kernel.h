#ifndef QMOS_KERNEL_H
#define QMOS_KERNEL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <qmos/typedefs.h>

extern uint32_t __SEC_BEGIN_KERNEL_CALL_START, __SEC_END_KERNEL_CALL_START;

typedef struct
{
	const char *name;
	version_t version;
} kernel_info_t;

extern kernel_info_t qmos_kernel_info;

#endif//QMOS_KERNEL_H
