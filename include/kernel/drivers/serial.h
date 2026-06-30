#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#define PORT 0x3f8 // COM1

uint8_t init_serial();
void write_serial(char c);
char read_serial();

#endif