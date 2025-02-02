#!/bin/sh
zcat -9 $1 | cpio -idmv
# zstd -cd $1 | cpio -idv
# unmkinitramfs ../initrd.img-5.4.0-109-generic  ./
