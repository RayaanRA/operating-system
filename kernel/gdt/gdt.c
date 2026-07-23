#include <gdt/gdt.h>

extern void load_gdt(gdt_ptr_t *);
static void set_gdt_entry(uint8_t i, uint8_t access, uint8_t flags);

gdt_t gdt;
gdt_ptr_t gdt_ptr;

static void set_gdt_entry(uint8_t i, uint8_t access, uint8_t flags) {
    gdt.entries[i].base_low = 0;
    gdt.entries[i].base_mid = 0;
    gdt.entries[i].base_high = 0;
    gdt.entries[i].limit_16 = 0;
    gdt.entries[i].limit_4 = 0;

    gdt.entries[i].access = access;
    gdt.entries[i].flags = flags;
}

uint8_t init_gdt() {
    set_gdt_entry(0, 0, 0); // Null descriptor
    set_gdt_entry(1, 0x9A, 0xA); // Kernel code
    set_gdt_entry(2, 0x92, 0xC); // Kernel data
    set_gdt_entry(3, 0xF2, 0xC); // User data
    set_gdt_entry(4, 0xFA, 0xA); // User code
    
    gdt_ptr.address = (uint64_t) &gdt;
    gdt_ptr.length = sizeof(gdt) - 1;

    load_gdt(&gdt_ptr);
    return 0;
}