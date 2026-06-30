#include <fb/fb.h>
#include <limine/limine.h>
#include "fb_p.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 0
};

static struct limine_framebuffer *framebuffer;

uint8_t fb_init() {
	if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
		return 1;
	}
	framebuffer = framebuffer_request.response->framebuffers[0];
	return 0;
}

void draw_pixel(int32_t x, int32_t y, uint64_t color) {
	uint32_t *fb_ptr = framebuffer->address;
	fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
}

