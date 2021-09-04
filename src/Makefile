CFLAGS = -target i386-pc-sys-elf -ffreestanding -Iinclude

C_SRCS := $(wildcard */*.c)
A_SRCS := $(wildcard */*.s)

OBJS := $(C_SRCS:.c=.o)
OBJS += $(A_SRCS:.s=.o)

KERNEL = kernel.elf

all: ${BUILD} ${KERNEL}

%.o: %.c
	clang ${CFLAGS} -c $< -o $@

%.o: %.s
	nasm -f elf $< -o $@

${KERNEL}: ${OBJS}
	i686-elf-ld -n -o $@ -T linker.ld ${OBJS}

$(BUILD):
	mkdir -p $@

clean:
	rm -f ${OBJS} ${KERNEL}

.PHONY: clean ${SUBDIRS}
