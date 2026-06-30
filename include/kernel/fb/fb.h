#ifndef FB_H
#define FB_H

#include <stdbool.h>
#include <stdint.h>

uint8_t fb_init();
void kprint(const char *str, ...);

#endif