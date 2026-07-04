#include <lib/kprintf.h>
#include <drivers/serial.h>
#include <fb/fb.h>
#include <stddef.h>
#include <stdarg.h>

static void put_int(void (*put_char)(char), int64_t i);
static void put_uint(void (*put_char)(char), uint64_t i);
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
      }
    } else {
    	put_char(*s);
    	s++;
	}
  }
}

void put_int(void (*put_char)(char), int64_t i) {
	if (i < 0) {
		put_char('-');
		i = -i;
	}

	if (i / 10 != 0) {
		put_int(put_char, i / 10);
	}

	put_char((i % 10) + '0');
}

void put_uint(void (*put_char)(char), uint64_t i) {
	if (i / 10 != 0) {
		put_uint(put_char, i / 10);
	}

	put_char((i % 10) + '0');
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
