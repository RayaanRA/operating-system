#include <drivers/serial.h>
#include <arch/x86_64/io.h>

uint8_t serial_received();
uint8_t is_transmit_empty();

uint8_t init_serial() {
	outb(PORT + 1, 0x00);
	outb(PORT + 3, 0x80);
	outb(PORT + 0, 0x03);
	outb(PORT + 1, 0x00);
	outb(PORT + 3, 0x03);
	outb(PORT + 2, 0xC7);
	outb(PORT + 4, 0x0B);
	outb(PORT + 4, 0x1E);
	outb(PORT + 0, 0xAE);

	if (inb(PORT + 0) != 0xAE) {
		return 1;
	}

	outb(PORT + 4, 0x0F);
	return 0;
}

uint8_t serial_received() {
	return inb(PORT + 5) & 1; // check Data ready (DR) bit  of Line Status Register
}

uint8_t is_transmit_empty() {
	return inb(PORT + 5) & 0x20; // check Transmitter holding register empty (THRE) of LSR
}

void write_serial(char c) {
	while (is_transmit_empty() == 0);

	outb(PORT, c);
}

void write_serial_string(const char *s) {
	while (*s) {
		write_serial(*s);
		s++;
	}
}

char read_serial() {
	while (serial_received() == 0);

	return inb(PORT);
}