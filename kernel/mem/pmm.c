#include <stddef.h>
#include <limine/limine.h>
#include <mem/pmm.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
	.revision = 0
};

static uint64_t entry_count;
static struct limine_memmap_entry **entries;

uint8_t pmm_init() {
	if (memmap_request.response == NULL) {
		return 1;
	}
	entry_count = memmap_request.response->entry_count;
	entries = memmap_request.response->entries;
	return 0;
}