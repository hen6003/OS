KERNEL_SOURCES += start.o kernel.o asm.o cursor.o term.o keyboard.o util.o

all: mykernel.elf

%.o: %.c
	clang -target i386-pc-sys-elf -ffreestanding -c $< -o $@

%.o: %.s
	nasm -f elf $< -o $@

mykernel.elf: ${KERNEL_SOURCES}
	i686-elf-ld -n -o mykernel.elf -T linker.ld *.o

clean:
	rm *.o mykernel.elf

.PHONY: clean
