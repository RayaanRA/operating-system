BUILD := build
ISO_ROOT := $(BUILD)/iso_root
ISO := $(BUILD)/os.iso

KERNEL := $(BUILD)/kernel/bin/os

LIMINE := limine-binary

.PHONY: all kernel iso clean run

all: iso

kernel:
	$(MAKE) -C kernel

iso: kernel
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot/limine
	mkdir -p $(ISO_ROOT)/EFI/BOOT

	cp $(KERNEL) $(ISO_ROOT)/boot/os
	cp boot/limine.conf $(ISO_ROOT)/boot/limine.conf

	cp $(LIMINE)/limine-bios.sys $(ISO_ROOT)/boot/limine/
	cp $(LIMINE)/limine-bios-cd.bin $(ISO_ROOT)/boot/limine/
	cp $(LIMINE)/limine-uefi-cd.bin $(ISO_ROOT)/boot/limine/

	cp $(LIMINE)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	cp $(LIMINE)/BOOTIA32.EFI $(ISO_ROOT)/EFI/BOOT/

	xorriso -as mkisofs -R -r -J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		$(ISO_ROOT) -o $(ISO)

	./$(LIMINE)/limine bios-install $(ISO)

clean:
	rm -rf $(BUILD)