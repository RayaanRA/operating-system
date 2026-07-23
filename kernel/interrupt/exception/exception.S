.code64
.intel_syntax noprefix
.altmacro

.macro isr_no_err_stub stub
isr_stub_\stub:
    push 0
    push \stub
    jmp isr_common_stub
.endm

.macro isr_err_stub stub
isr_stub_\stub:
    push \stub
    jmp isr_common_stub
.endm

.extern exception_handler

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

isr_common_stub:
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push rbp
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   mov rax, cr2
   push rax
   cld
   mov rdi, rsp # receive stack pointer for C struct pointer
   call exception_handler 
   add rsp, 8
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rbp
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   add rsp, 0x10
   iretq

.section .data
.macro make_isr_entry index
    .quad isr_stub_\index
.endm

.globl isr_stub_table
isr_stub_table:
.set current_id, 0
.rept 32
    make_isr_entry %current_id
    .set current_id, current_id + 1
.endr