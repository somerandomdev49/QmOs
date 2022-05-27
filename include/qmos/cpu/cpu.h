#ifndef QMOS_CPU_H
#define QMOS_CPU_H
#include <stdint.h>

typedef struct {
   uint32_t ds; // data segment
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
   uint32_t int_no, err_code;
   uint32_t eip, cs, eflags, useresp, ss; // pushed by cpu
} qmos_registers_t;

#endif
