KERNEL_C_SRCS := $(wildcard *.c)
KERNEL_A_SRCS := $(wildcard *.s)

KERNEL_O_SRCS := $(KERNEL_C_SRCS:.c=.o)
KERNEL_O_SRCS += $(KERNEL_A_SRCS:.s=.o)

BUILD = ../build/

KERNEL = ${BUILD}kernel.elf
OBJ = $(addprefix ${BUILD}, ${KERNEL_O_SRCS})

all: ${BUILD} mykernel.elf

$(BUILD)%.o: %.c
	clang -target i386-pc-sys-elf -ffreestanding -c $< -o $@

$(BUILD)%.o: %.s
	nasm -f elf $< -o $@

mykernel.elf: ${OBJ}
	i686-elf-ld -n -o ${KERNEL} -T linker.ld ${OBJ}

$(BUILD):
	mkdir -p $@

clean:
	rm ${OBJ} ${KERNEL}

.PHONY: clean
