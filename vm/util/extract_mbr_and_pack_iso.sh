#!/bin/sh

# Firt, mount and rsync out the content of the ISO under iso/
# mount ubuntu-24.04.1-live-server-amd64.iso t/
# mkdir iso
# rsync -av ./t/ iso/
#

orig=$1.iso
mbr=$1.mbr
efi=$1.efi

if [ ! -f $mbr ]; then
# Extract the MBR template
dd if="$orig" bs=1 count=446 of="$mbr"
fi

if [ ! -f $efi ]; then
# Extract EFI partition image
skip=$(/sbin/fdisk -l "$orig" | fgrep '.iso2 ' | awk '{print $2}')
size=$(/sbin/fdisk -l "$orig" | fgrep '.iso2 ' | awk '{print $4}')
dd if="$orig" bs=512 skip="$skip" count="$size" of="$efi"
fi

new=image.iso

xorriso -as mkisofs \
  -r -V "test" -J -joliet-long -l \
  -iso-level 3 \
  -partition_offset 16 \
  --grub2-mbr "$mbr" \
  --mbr-force-bootable \
  -append_partition 2 0xEF "$efi" \
  -appended_part_as_gpt \
  -c /boot.catalog \
  -b /boot/grub/i386-pc/eltorito.img \
  -no-emul-boot -boot-load-size 4 -boot-info-table --grub2-boot-info \
  -eltorito-alt-boot \
  -e '--interval:appended_partition_2:all::' \
  -no-emul-boot \
  -o "$new" \
  ./iso/


