KERNEL = src/kernel.elf
DEST   = iso/boot/kernel.elf
ISO    = image.iso
LIMINE = /usr/share/limine/

all: ${DEST} ${ISO}

${KERNEL}: FORCE
	${MAKE} -C src

${DEST}: ${KERNEL}
	cp ${KERNEL} ${DEST}

${ISO}:
	cp ${LIMINE}/limine-cd.bin iso/boot/
	cp ${LIMINE}/limine-eltorito-efi.bin iso/boot/
	cp ${LIMINE}/limine.sys iso/boot/

	xorriso -as mkisofs -b boot/limine-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table \
	--efi-boot boot/limine-eltorito-efi.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	iso -o ${ISO}

	limine-install ${ISO}

run: all
	qemu-system-x86_64 -cdrom ${ISO}

clean:
	rm -f ${DEST} ${ISO}
	make -C src clean

FORCE: ;
