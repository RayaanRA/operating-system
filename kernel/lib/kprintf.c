#include <lib/kprintf.h>
#include <drivers/serial.h>
#include <fb/fb.h>
#include <stddef.h>
#include <stdarg.h>

static void put_int(void (*put_char)(char), int32_t i);
static void kprintf(void (*put_char)(char), const char *s, va_list args);

void kprintf(void (*put_char)(char), const char *s, va_list args) {
	while (*s) {
    if (*s == '%') {
      s++;
      switch (*s) {
      case 'd':
        int32_t i = va_arg(args, int32_t);
        put_int(put_char, i);
        s++;
        break;
      }
    } else {
    	put_char(*s);
    	s++;
	}
  }
}

void put_int(void (*put_char)(char), int32_t i) {
	if (i < 0) {
		put_char('-');
		i = -i;
	}

	if (i / 10 != 0) {
		put_int(put_char, i / 10);
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
