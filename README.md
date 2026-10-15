# x86-64 Operating System

This is an operating system for x86-64 machines. It uses the Limine bootloader.

It includes:  
+ Bitmap font rendering via Limine framebuffers  
+ Basic `kprintf` with format specifiers `%d`, `%u`, `%b`  
	+ Output to either screen or serial (`kprintf_screen`, `kprintf_serial`)  
+ Global Descriptor Table (GDT)  
+ Interrupt Descriptor Table (IDT) with exception handling  
+ Physical memory manager using a bitmap  

Currently being worked on:  
+ Virtual memory manager
---
When building, have the [`limine-binary`](https://github.com/Limine-Bootloader/Limine/releases) 
folder at the root of the project folder.


This project started from [OSDev's Limine Bare Bones](wiki.osdev.org/Limine_Bare_Bones) setup, and is also a
major resource in writing this OS so far.
