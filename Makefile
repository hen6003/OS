KERNEL = build/kernel.elf
ISO    = build/kernel.iso

all: ${ISO}

${KERNEL}:
	${MAKE} -C src

${ISO}: ${KERNEL}
	cp build/kernel.elf iso/boot
	grub-mkrescue iso -o build/kernel.iso
