#include <lib/kprintf.h>
#include <drivers/serial.h>
#include <fb/fb.h>
#include <stddef.h>
#include <stdarg.h>

static void put_int(void (*put_char)(char), int64_t i);
static void put_uint(void (*put_char)(char), uint64_t i);
static void put_binary(void (*put_char)(char), int64_t n);
static void kprintf(void (*put_char)(char), const char *s, va_list args);

void kprintf(void (*put_char)(char), const char *s, va_list args) {
	while (*s) {
    if (*s == '%') {
      s++;
      switch (*s) {
      	case 'd':
        	put_int(put_char, va_arg(args, int64_t));
        	s++;
        	break;
      	case 'u':
    		put_uint(put_char, va_arg(args, uint64_t));
    		s++;
    		break;
      	case 'b':
    		put_binary(put_char, va_arg(args, uint64_t));
    		s++;
    		break;
     	}
    }

    put_char(*s);
    s++;
  }
}

void put_int(void (*put_char)(char), int64_t i) {
	uint64_t u;
	if (i < 0) {
		put_char('-');
		u = (uint64_t)0 - (uint64_t)i;
	} else {
		u = (uint64_t)i;
	}

	put_uint(put_char, u);
}

void put_uint(void (*put_char)(char), uint64_t i) {
	if (i / 10 != 0) {
		put_uint(put_char, i / 10);
	}

	put_char((i % 10) + '0');
}

void put_binary(void (*put_char)(char), int64_t i) {
	size_t bytes_needed = 0;
	uint64_t temp = i;
	uint64_t space_counter = 0;

	do {
    	bytes_needed++;
    	temp >>= 8; // Move to the next byte
	} while (temp > 0);

	size_t bits_needed = bytes_needed * 8;

	put_char('[');
	for (size_t j = bits_needed; j > 0; j--) {
		if ((i >> (j - 1)) & 1) {
			put_char('1');
		} else {
			put_char('0');
		}
		space_counter++;
		if (space_counter % 4 == 0 && j != 1) put_char(' ');
	}
	put_char(']');
}

void kprintf_screen(const char *s, ...) {
	va_list args;
	va_start(args, s);
	kprintf(fb_put_char, s, args);
	va_end(args);
}

void kprintf_serial(const char *s, ...) {
	va_list args;
	va_start(args, s);
	kprintf(serial_put_char, s, args);
	va_end(args);
}
