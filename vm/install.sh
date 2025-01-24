#!/bin/sh

if [ ! -f disk.img ]; then
dd if=/dev/zero of=disk.img bs=1M count=20000
fi
/usr/bin/qemu-system-x86_64 \
  -machine q35 \
  -boot d \
  -cdrom $1 \
  -drive file=disk.img,format=raw \
  -m 4G \
  -enable-kvm \
  -smp 2 \
  -cpu host

