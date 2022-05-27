#ifndef QMOS_IRQ_H
#define QMOS_IRQ_H
#include <qmos/cpu/cpu.h>

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

extern void qmos_irq0 (); 
extern void qmos_irq1 (); 
extern void qmos_irq2 (); 
extern void qmos_irq3 (); 
extern void qmos_irq4 (); 
extern void qmos_irq5 (); 
extern void qmos_irq6 (); 
extern void qmos_irq7 (); 
extern void qmos_irq8 (); 
extern void qmos_irq9 (); 
extern void qmos_irq10();
extern void qmos_irq11();
extern void qmos_irq12();
extern void qmos_irq13();
extern void qmos_irq14();
extern void qmos_irq15();

void qmos_init_irq();

typedef void (*qmos_isr_t)(qmos_registers_t);
void qmos_reg_int_handler(uint8_t n, qmos_isr_t handler);

#endif
