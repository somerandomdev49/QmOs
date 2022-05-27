#ifndef QMOS_PAGING_H
#define QMOS_PAGING_H
#include <stdint.h>
#include <qmos/cpu/cpu.h>

typedef struct
{
   uint32_t present    : 1;   // Page present in memory
   uint32_t rw         : 1;   // Read-only if clear, readwrite if set
   uint32_t user       : 1;   // Supervisor level only if clear
   uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
   uint32_t dirty      : 1;   // Has the page been written to since last refresh?
   uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
   uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} qmos_page_t;

typedef struct
{
   qmos_page_t pages[1024];
} qmos_page_table_t;

typedef struct
{
   qmos_page_table_t *tables[1024];
   uint32_t tables_phys[1024];
   uint32_t phys_addr;
} qmos_page_directory_t;

void qmos_init_paging();
void qmos_switch_page_dir(qmos_page_directory_t *new_dir);
qmos_page_t *qmos_get_page(uint32_t address, int make, qmos_page_directory_t *dir);
void qmos_page_fault_handler(qmos_registers_t regs);

#endif
