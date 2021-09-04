KERNEL = src/kernel.elf
DEST   = iso/boot/kernel.elf
ISO    = kernel.iso

all: ${DEST} ${ISO}

${KERNEL}: FORCE
	${MAKE} -C src

${DEST}: ${KERNEL}
	cp ${KERNEL} ${DEST}

${ISO}: ${KERNEL}
	grub-mkrescue iso -o kernel.iso

FORCE: ;
