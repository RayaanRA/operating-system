#include <arch/arch.h>
#include <lib/string.h>
#include <lib/kprintf.h>
#include <limine/limine.h>
#include <mem/pmm.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
	.revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST_ID,
	.revision = 0
};

uint64_t find_highest_addr();
void place_bitmap();
void populate_bitmap();

static uint64_t entry_count;
static uint8_t *bitmap;
static uint64_t page_count; // pages existing in address space
static uint64_t bitmap_bytes; // raw bitmap size
static uint64_t bitmap_size; // page-aligned allocation size
static uint64_t bitmap_pages; // rounded-up page count
static uint64_t hhdm_offset;
static struct limine_memmap_entry **entries;

uint8_t pmm_init() {
	if (memmap_request.response == NULL) {
		return 1;
	}
	if (hhdm_request.response == NULL) {
		return 1;
	}
	entry_count = memmap_request.response->entry_count;
	entries = memmap_request.response->entries;
	hhdm_offset = hhdm_request.response->offset;
	page_count = (find_highest_addr() + (PAGE_FRAME_SIZE - 1)) / PAGE_FRAME_SIZE;
	bitmap_bytes = (page_count + 7) / 8;
	bitmap_pages = (bitmap_bytes + (PAGE_FRAME_SIZE - 1)) / PAGE_FRAME_SIZE;
	bitmap_size = bitmap_pages * PAGE_FRAME_SIZE;
	place_bitmap();
	populate_bitmap();
	return 0;
}

uint64_t find_highest_addr() {
	uint64_t highest = 0;
	uint64_t curr = 0;
	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
			curr = entries[i]->base + entries[i]->length;
			if (curr > highest) highest = curr;
		}
	}
	return highest;
}

void place_bitmap() {
	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type == LIMINE_MEMMAP_USABLE && entries[i]->length >= bitmap_size) { // find first largest entry to place bitmap
			bitmap = (uint8_t*)(entries[i]->base + hhdm_offset);
			memset(bitmap, 0xFF, bitmap_bytes);
			entries[i]->base += bitmap_size;
			entries[i]->length -= bitmap_size;
			return;
		}
	}
}

void populate_bitmap() {
    for (size_t i = 0; i < entry_count; i++) {
        if (entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        uint64_t start_page = entries[i]->base / PAGE_FRAME_SIZE;
        uint64_t page_count = entries[i]->length / PAGE_FRAME_SIZE;
        for (uint64_t i = 0; i < page_count; i++) { 
            uint64_t current_page = start_page + i;
            uint64_t byte_i = current_page / 8;
            uint8_t bit_i = current_page % 8;

            bitmap[byte_i] &= ~(1 << bit_i);
        }
    }
}

uint64_t pmm_allocate() {
	uint64_t phys_addr = 0;
	for (uint64_t i = 0; i < bitmap_bytes; i++) {
		for (size_t j = 0; j < 8; j++) {
        	if (!((bitmap[i] >> j) & 1)) {
            	kprintf_screen("Byte %u before alloc: %b\n", i, bitmap[i]);
            	bitmap[i] |= 1 << j;
            	kprintf_screen("Byte %u after alloc: %b\n", i, bitmap[i]);           	
            	phys_addr = ((i * 8) + j) * PAGE_FRAME_SIZE;
            	goto loop_end;
        	} 				
		}
	}
	loop_end:
	return phys_addr;
}

void pmm_free(uint64_t phys_addr) {
	uint64_t page_i = phys_addr / PAGE_FRAME_SIZE;
	uint64_t byte_i = page_i / 8;
	uint8_t bit_i = page_i % 8;
	bitmap[byte_i] &= ~(1 << bit_i);
	kprintf_screen("Freed page starting at physical address %u", phys_addr);
}