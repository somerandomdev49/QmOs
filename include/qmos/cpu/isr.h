#ifndef QMOS_ISR_H
#define QMOS_ISR_H
#include <stdint.h>
#include <qmos/cpu/cpu.h>

extern void qmos_isr0();
extern void qmos_isr1();
extern void qmos_isr2();
extern void qmos_isr3();
extern void qmos_isr4();
extern void qmos_isr5();
extern void qmos_isr6();
extern void qmos_isr7();
extern void qmos_isr8();
extern void qmos_isr9();
extern void qmos_isr10();
extern void qmos_isr11();
extern void qmos_isr12();
extern void qmos_isr13();
extern void qmos_isr14();
extern void qmos_isr15();
extern void qmos_isr16();
extern void qmos_isr17();
extern void qmos_isr18();
extern void qmos_isr19();
extern void qmos_isr20();
extern void qmos_isr21();
extern void qmos_isr22();
extern void qmos_isr23();
extern void qmos_isr24();
extern void qmos_isr25();
extern void qmos_isr26();
extern void qmos_isr27();
extern void qmos_isr28();
extern void qmos_isr29();
extern void qmos_isr30();
extern void qmos_isr31();

void qmos_init_isr();
void qmos_isr_handler(qmos_registers_t r);

#endif
