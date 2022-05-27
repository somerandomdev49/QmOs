#ifndef QMOS_GDT_H
#define QMOS_GDT_H
#include <stdint.h>

struct qmos_gdt_entry_tag
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

typedef struct qmos_gdt_entry_tag qmos_gdt_entry_t;

struct qmos_gdt_ptr_tag
{
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint32_t base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
 
typedef struct qmos_gdt_ptr_tag qmos_gdt_ptr_t;

struct qmos_idt_entry_tag
{
   uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   uint16_t sel;                 // Kernel segment selector.
   uint8_t  always0;             // This must always be zero.
   uint8_t  flags;               // More flags. See documentation.
   uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct qmos_idt_entry_tag qmos_idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct qmos_idt_ptr_tag
{
   uint16_t limit;
   uint32_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct qmos_idt_ptr_tag qmos_idt_ptr_t;


void qmos_init_gdt();
void qmos_init_idt();
void qmos_set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
