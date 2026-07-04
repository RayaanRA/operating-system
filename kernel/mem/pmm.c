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
		if (entries[i]->type == LIMINE_MEMMAP_USABLE && entries[i]->length >= bitmap_size) {
			bitmap = (uint8_t*)(entries[i]->base + hhdm_offset);
			memset(bitmap, 0xFF, bitmap_bytes);
			entries[i]->base += bitmap_size;
			entries[i]->length -= bitmap_size;
			return;
		}
	}
}

uint64_t count_usable_pages() {
	uint64_t pages = 0;
	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
			pages += (entries[i]->length + (PAGE_FRAME_SIZE - 1)) / PAGE_FRAME_SIZE;
		}
	}
	return pages;	
}