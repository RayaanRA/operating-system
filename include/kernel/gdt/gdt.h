#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct gdt_entry_t {
    uint16_t limit_16;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_4 : 4;
    uint8_t flags : 4;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_t {
    gdt_entry_t entries[5];
} gdt_t;

typedef struct gdt_ptr_t {
    uint16_t length;
    uint64_t address;
} __attribute__((packed)) gdt_ptr_t;

uint8_t init_gdt();

#endif