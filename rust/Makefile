
NASM_PARAMS = -f elf64
LINKER_PARAMS = -n -o

ASSEMBLY_FILES = multiboot_header.asm boot.asm
RUST_FILES = test.rs

OBJECTS_OS = multiboot_header.o boot.o

%.o: %.asm
	nasm $(NASM_PARAMS) -o $@ $<
	
kernel.bin : linker.ld $(OBJECTS_OS)
	ld $(LINKER_PARAMS) -T $< -o $@ $(OBJECTS_OS)
	
kernel.iso : kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0' 			> iso/boot/grub/grub.cfg
	echo 'set default=0' 			>> iso/boot/grub/grub.cfg
	echo 'menuentry "Okton" {' 		>> iso/boot/grub/grub.cfg
	echo 'multiboot2 /boot/kernel.bin' 	>> iso/boot/grub/grub.cfg
	echo 'boot' 				>> iso/boot/grub/grub.cfg
	echo '}' 				>> iso/boot/grub/grub.cfg
	grub-mkrescue --output=OktonOS.iso iso
	
	
clean: 
	rm -r iso 
	rm $(OBJECTS_OS)
	
install:

	sudo cp $< /boot/kernel.bin
	
emulate:
	
	qemu-system-x86_64 -cdrom OktonOS.iso