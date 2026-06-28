#ifndef FB_P_H
#define FB_P_H

#include <stddef.h>
#include <stdint.h>

#define FONT_HEIGHT 16
#define FONT_WIDTH 8
#define SCALE_FACTOR 2

void draw_pixel(int32_t x, int32_t y, uint64_t color);

#endif