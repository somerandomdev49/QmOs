#include <qmos/cpu/gdt.h>
#include <qmos/cpu/isr.h>
#include <qmos/cpu/irq.h>
#include <qmos/serial.h>
#include <qmos/qmc/string.h>
#include <qmos/port_io.h>

extern void qmos_gdt_flush();
extern void qmos_idt_flush();

qmos_gdt_entry_t gdt_entries[5];
qmos_gdt_ptr_t   gdt_ptr;
qmos_idt_entry_t idt_entries[256];
qmos_idt_ptr_t   idt_ptr;

static void qmos_gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}

void qmos_init_idt()
{
	idt_ptr.limit = sizeof(qmos_idt_entry_t) * 256 -1;
	idt_ptr.base  = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(qmos_idt_entry_t)*256);

	// Remap the IRQ table:
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	qmos_init_isr();
	qmos_init_irq();

	qmos_idt_flush((uint32_t)&idt_ptr);
}

void qmos_set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
}

void qmos_init_gdt()
{
	gdt_ptr.limit = (sizeof(qmos_gdt_entry_t) * 5) - 1;
	gdt_ptr.base  = (uint32_t)&gdt_entries;

	qmos_gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	qmos_gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	qmos_gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	qmos_gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	qmos_gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	
	qmos_gdt_flush((uint32_t)&gdt_ptr);
}
