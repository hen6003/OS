#!/bin/sh

trace() { trap 'ERR=$?' ERR; set -Ex; $1 2>&1; set +Ex; trap ERR; } 2>&-
try() {
    echo -ne "\e[37m$2..."
    local output=$( trace $1 )
    if [[ "$output" =~ "ERR=" ]]; then
        echo -e "\e[31m failed\e[0m"
        echo "$output" | sed $'s/.*ERR=\(.*\)/\a\033[36mEXIT CODE: \\1\033[0m/g'
	exit
    else
        echo -e "\e[32m done\e[0m"
    fi
}

compile()
{
    cd src
    make
}

iso()
{
    cp src/mykernel.elf iso/boot
    grub-mkrescue iso -o mykernel.iso
}

run()
{
    qemu-system-x86_64 -cdrom mykernel.iso
}

try compile "Building kernel"
try iso "Building iso"

if [ "$1" = "run" ]
then
    try run "Running OS"
fi
