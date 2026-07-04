#ifndef ARCH_H
#define ARCH_H

#if defined(ARCH_X86_64)
    #include <arch/x86_64/arch.h>
#else
    #error "Unsupported architecture"
#endif

#endif