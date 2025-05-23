#!/bin/sh
# ssh localhost -p1810
#
# -serial telnet:127.0.0.1:50000,server,nowait \
# -serial telnet:127.0.0.1:50001,server,nowait \
# -monitor telnet:127.0.0.1:4444,server,nowait \
#

/usr/bin/qemu-system-x86_64 \
  -machine q35 \
  -boot d \
  -drive file=$1,format=raw \
  -device virtio-serial -device virtconsole,chardev=vserial0 \
  -chardev socket,id=vserial0,host=127.0.0.1,port=50000,telnet,server,nowait \
  -device virtio-serial -device virtconsole,chardev=vserial1 \
  -chardev socket,id=vserial1,host=127.0.0.1,port=50001,telnet,server,nowait \
  -net nic -net user,hostfwd=tcp::1810-:22 \
  -m 4G \
  -enable-kvm \
  -smp 4 \
  -cpu host

