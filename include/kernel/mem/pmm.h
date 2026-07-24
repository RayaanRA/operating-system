#ifndef PMM_H
#define PMM_H

uint8_t pmm_init();
uint64_t pmm_allocate();
void pmm_free(uint64_t phys_addr);

#endif