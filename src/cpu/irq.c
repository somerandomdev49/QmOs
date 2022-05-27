#include <qmos/cpu/gdt.h>
#include <qmos/cpu/irq.h>
#include <qmos/cpu/cpu.h>
#include <qmos/port_io.h>
#include <qmos/serial.h>
#include <stdint.h>

void qmos_init_irq()
{
	writes_serial("Setting idt gates for irqs");


	qmos_set_idt_gate(32, (uint32_t)qmos_irq0 , 0x08, 0x8E);
	qmos_set_idt_gate(33, (uint32_t)qmos_irq1 , 0x08, 0x8E);
	qmos_set_idt_gate(34, (uint32_t)qmos_irq2 , 0x08, 0x8E);
	qmos_set_idt_gate(35, (uint32_t)qmos_irq3 , 0x08, 0x8E);
	qmos_set_idt_gate(36, (uint32_t)qmos_irq4 , 0x08, 0x8E);
	qmos_set_idt_gate(37, (uint32_t)qmos_irq5 , 0x08, 0x8E);
	qmos_set_idt_gate(38, (uint32_t)qmos_irq6 , 0x08, 0x8E);
	qmos_set_idt_gate(39, (uint32_t)qmos_irq7 , 0x08, 0x8E);
	qmos_set_idt_gate(40, (uint32_t)qmos_irq8 , 0x08, 0x8E);
	qmos_set_idt_gate(41, (uint32_t)qmos_irq9 , 0x08, 0x8E);
	qmos_set_idt_gate(42, (uint32_t)qmos_irq10, 0x08, 0x8E);
	qmos_set_idt_gate(43, (uint32_t)qmos_irq11, 0x08, 0x8E);
	qmos_set_idt_gate(44, (uint32_t)qmos_irq12, 0x08, 0x8E);
	qmos_set_idt_gate(45, (uint32_t)qmos_irq13, 0x08, 0x8E);
	qmos_set_idt_gate(46, (uint32_t)qmos_irq14, 0x08, 0x8E);
	qmos_set_idt_gate(47, (uint32_t)qmos_irq15, 0x08, 0x8E);
}

qmos_isr_t int_handlers[256] = {0};

void qmos_irq_handler(qmos_registers_t regs)
{
	// writes_serial("qmos_irq_handler called.");
	if(regs.int_no >= 40) outb(0xa0, 0x20);
	outb(0x20, 0x20);
	if(int_handlers[regs.int_no] != 0)
	{
		qmos_isr_t handler = int_handlers[regs.int_no];
		handler(regs);
	}
}

// #include <qmos/qmc/stdio.h>
void qmos_reg_int_handler(uint8_t n, qmos_isr_t handler)
{
	// printf("Registering interrupt handler #%d to 0x%x\n", n, handler);
	// fflush(stdout);
	int_handlers[n] = handler;
}
