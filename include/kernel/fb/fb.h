#ifndef FB_H
#define FB_H

#include <stdbool.h>
#include <stdint.h>

uint8_t fb_init();
void fb_put_char(char c);

#endif