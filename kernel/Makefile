#
# Prep packages install in order to reach the make fetch step
#
prepare:
	sudo apt-get install debootstrap git-core gcc vim bc

#
# This will actually pull the whole Precise Pangolin ( ubuntu 12.04 LTS ) 
# distribution from the web and adjust the FS layout so that we can 
# track it into git
#
# NOTE: Once in a git repo we don't to do this step instead we pull the
# binaries from git directly
#

fetch:
	if [ ! -e chroot ] ; then mkdir chroot; \
	cd chroot;\
	sudo debootstrap --variant=buildd bionic ./ http://archive.ubuntu.com/ubuntu/; \
	sudo chown -R etmartin *; \
	sudo chgrp -R etmartin *; \
	touch boot/.empty;\
	touch home/.empty;\
	touch media/.empty;\
	touch mnt/.empty;\
	touch opt/.empty;\
	touch root/.empty;\
	touch selinux/.empty;\
	touch srv/.empty;\
	touch tmp/.empty;\
	touch sys/.empty;\
	touch dev/.empty;\
	touch proc/.empty;\
	touch dev/pts/.empty;\
	touch dev/shm/.empty;\
	touch var/log/apt/.empty;\
	touch var/lib/dpkg/updates/.empty;\
	touch etc/apt/preferences.d/.empty;\
	touch var/cache/apt/archives/partial/.empty;\
	touch var/lib/apt/lists/partial/.empty;\
	touch usr/src/.empty; \
	fi;

#
# Enter in the jail
#
enter:
	sudo mount -o bind /proc ./chroot/proc/
	sudo mount -o bind /dev ./chroot/dev 
	sudo mount -o bind /dev/pts ./chroot/dev/pts 
	sudo mount -o bind /dev/shm ./chroot/dev/shm
	sudo mount -o bind /sys ./chroot/sys 
	sudo mount -o bind /sys/kernel/debug ./chroot/sys/kernel/debug
	sudo chroot ./chroot env -i HOME=/root TERM=xterm PATH=/bin:/usr/bin:/sbin:/usr/sbin:\
	/usr/local/sbin:/usr/local/bin LANG=en_US.utf8 LC_ALL=C LANGUAGE="en_US" PERL_BADLANG=0 COLORTERM=gnome-terminal /bin/bash --login +h

#
# Leave the jail
#
exit:
	sudo umount ./chroot/proc
	sudo umount ./chroot/dev/pts
	sudo umount ./chroot/dev/shm
	sudo umount ./chroot/dev
	sudo umount ./chroot/sys/kernel/debug
	sudo umount ./chroot/sys

# 
# In the jail the shell is root but the outside the jail the binaries are 
# tracked by git as standard user so...
#
chmod:
	sudo chown -R etmartin *
	sudo chgrp -R etmartin *

