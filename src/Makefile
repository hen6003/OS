CFLAGS = -target i386-pc-sys-elf -ffreestanding -Iinclude

C_SRCS := $(wildcard */*.c)
A_SRCS := $(wildcard */*.s)

O_SRCS := $(C_SRCS:.c=.o)
O_SRCS += $(A_SRCS:.s=.o)

KERNEL = kernel.elf

all: ${BUILD} ${KERNEL}

%.o: %.c
	clang ${CFLAGS} -c $< -o $@

%.o: %.s
	nasm -f elf $< -o $@

${KERNEL}: ${O_SRCS}
	i686-elf-ld -n -o $@ -T linker.ld ${O_SRCS}

$(BUILD):
	mkdir -p $@

clean:
	rm ${OBJ} ${KERNEL}

.PHONY: clean ${SUBDIRS}
