#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#define PORT 0x3f8 // COM1

uint8_t serial_init();
char read_serial();
void serial_put_char(char c);

#endif