#include <exception/handler.h>

void exception_handler(registers_t *regs) {
	kprintf_serial("Exception occured, interrupt number %d\n", regs->type);
}