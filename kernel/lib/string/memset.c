#include <lib/string.h>

void *memset(void *s, int c, size_t n) {
	uint8_t *p = s;

	for (size_t i = 0; i < n; i++) {
		p[i] = (uint8_t)c;
	}

	return s;
}