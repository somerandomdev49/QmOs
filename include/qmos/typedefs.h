#ifndef QMOS_TYPEDEFS_H
#define QMOS_TYPEDEFS_H
#include <stdint.h>

typedef struct
{
	uint8_t major, minor;
	uint16_t patch;
} version_t;

#define VERSION(MAJOR, MINOR, PATCH) ((version_t){ MAJOR, MINOR, PATCH })
#define VERSION_UNPACK(V) V.major, V.minor, V.patch

void NULL_FUNC();

#endif
