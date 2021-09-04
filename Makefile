KERNEL = src/kernel.elf
DEST   = iso/boot/kernel.elf
ISO    = kernel.iso

all: ${DEST} ${ISO}

${KERNEL}: FORCE
	${MAKE} -C src

${DEST}: ${KERNEL}
	cp ${KERNEL} ${DEST}

${ISO}:
	grub-mkrescue iso -o kernel.iso

run: all
	qemu-system-x86_64 -cdrom kernel.iso

clean:
	rm -f ${DEST} ${ISO}
	make -C src clean

FORCE: ;
