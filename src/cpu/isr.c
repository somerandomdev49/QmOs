#include <qmos/cpu/isr.h>
#include <qmos/cpu/gdt.h>

void qmos_init_isr()
{
    qmos_set_idt_gate(0 , (uint32_t)qmos_isr0 , 0x08, 0x8e);
    qmos_set_idt_gate(1 , (uint32_t)qmos_isr1 , 0x08, 0x8e);
    qmos_set_idt_gate(2 , (uint32_t)qmos_isr2 , 0x08, 0x8e);
    qmos_set_idt_gate(3 , (uint32_t)qmos_isr3 , 0x08, 0x8e);
    qmos_set_idt_gate(4 , (uint32_t)qmos_isr4 , 0x08, 0x8e);
    qmos_set_idt_gate(5 , (uint32_t)qmos_isr5 , 0x08, 0x8e);
    qmos_set_idt_gate(6 , (uint32_t)qmos_isr6 , 0x08, 0x8e);
    qmos_set_idt_gate(7 , (uint32_t)qmos_isr7 , 0x08, 0x8e);
    qmos_set_idt_gate(8 , (uint32_t)qmos_isr8 , 0x08, 0x8e);
    qmos_set_idt_gate(9 , (uint32_t)qmos_isr9 , 0x08, 0x8e);
    qmos_set_idt_gate(10, (uint32_t)qmos_isr10, 0x08, 0x8e);
    qmos_set_idt_gate(11, (uint32_t)qmos_isr11, 0x08, 0x8e);
    qmos_set_idt_gate(12, (uint32_t)qmos_isr12, 0x08, 0x8e);
    qmos_set_idt_gate(13, (uint32_t)qmos_isr13, 0x08, 0x8e);
    qmos_set_idt_gate(14, (uint32_t)qmos_isr14, 0x08, 0x8e);
    qmos_set_idt_gate(15, (uint32_t)qmos_isr15, 0x08, 0x8e);
    qmos_set_idt_gate(16, (uint32_t)qmos_isr16, 0x08, 0x8e);
    qmos_set_idt_gate(17, (uint32_t)qmos_isr17, 0x08, 0x8e);
    qmos_set_idt_gate(18, (uint32_t)qmos_isr18, 0x08, 0x8e);
    qmos_set_idt_gate(19, (uint32_t)qmos_isr19, 0x08, 0x8e);
    qmos_set_idt_gate(20, (uint32_t)qmos_isr20, 0x08, 0x8e);
    qmos_set_idt_gate(21, (uint32_t)qmos_isr21, 0x08, 0x8e);
    qmos_set_idt_gate(22, (uint32_t)qmos_isr22, 0x08, 0x8e);
    qmos_set_idt_gate(23, (uint32_t)qmos_isr23, 0x08, 0x8e);
    qmos_set_idt_gate(24, (uint32_t)qmos_isr24, 0x08, 0x8e);
    qmos_set_idt_gate(25, (uint32_t)qmos_isr25, 0x08, 0x8e);
    qmos_set_idt_gate(26, (uint32_t)qmos_isr26, 0x08, 0x8e);
    qmos_set_idt_gate(27, (uint32_t)qmos_isr27, 0x08, 0x8e);
    qmos_set_idt_gate(28, (uint32_t)qmos_isr28, 0x08, 0x8e);
    qmos_set_idt_gate(29, (uint32_t)qmos_isr29, 0x08, 0x8e);
    qmos_set_idt_gate(30, (uint32_t)qmos_isr30, 0x08, 0x8e);
    qmos_set_idt_gate(31, (uint32_t)qmos_isr31, 0x08, 0x8e);
}

static const char *interrupt_names[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

#include <qmos/qmc/stdio.h>

void qmos_isr_handler(qmos_registers_t regs)
{
    printf("Interrupt #%d: %s\n", regs.int_no, interrupt_names[regs.int_no]);
	fflush(stdout);
}
