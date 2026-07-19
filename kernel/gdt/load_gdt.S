.code64
.intel_syntax noprefix

.globl load_gdt
load_gdt:
    lgdt [rdi]

    push 0x08
    lea rax, [.reload_CS]
    push rax
    retfq

.reload_CS:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret