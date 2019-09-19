
Running Debian on the Octeon
============================

A port of the Debian GNU/Linux version 6.0 (Squeeze) can be found in this
directory. This is the latest stable version of Debian released on Feb 6th
2011.  Other than a modified oprofiler, this is the standard distribution 
available from http://www.debian.org.

Those interested in the exact changes made to Debian for Octeon, the
makefile "Makefile.debootstrap" shows the process of bootstrapping the
Octeon version of Debian. Currently there are two changes incorporated
into this distribution not found in the standard Debian distribution.

1) Octeon support in oprofile.
        The standard oprofile package from Debian does not support Mips.
        Oprofile version 0.9.7 with support for Octeon performance counters
        was added to the distribution.
2) Octeon optimized toolchain.
        In addition to the normal Debian gcc and binutils, an Octeon
        optimized version has been installed under /usr/local/Cavium_Networks.
        The support libraries for this toolchain are symbolically linked
        to /lib32, /lib64, /usr/lib32, and /usr/lib64.


Setting up a Compact Flash for Debian
=====================================

Performing a make in this directory will setup a 2GB (or larger) compact
flash to boot Debian on the Octeon EBT5800 development board. This will
erase everything on the compact flash. Generally speaking, this needs only
be performed once. Afterwards the Debian package management utility "aptitude"
should be used to install and upgrade packages. The Linux kernel can be
updated using "make debian flash" from the top level Linux directory. In order
to simplify access to the compact flash, it is recommended you add an entry
to /etc/fstab and a mount point for each partition. Here is an example for a
compact flash writer on the device /dev/sda:

/etc/fstab
    /dev/sda1       /mnt/cf1        auto    noauto,noatime,user     0 0
    /dev/sda2       /mnt/cf2        ext3    noauto,noatime,user     0 0

$ mkdir -p /mnt/cf1
$ mkdir -p /mnt/cf2


Contents of the Directory
=========================
Makefile
	Make file used to install Debian on a compact flash.
Makefile.debootstrap
	Make file used to bootstrap Debian from Octeon. Use this as a
	reference, but it is strongly advised that you don't try and run it.
Makefile.octeon-debs
	Helper make file used on Octeon to build and package oprofile.
README.txt
	This file.
debian_octeon-squeeze.tgz
	A gzipped tar file of the Debian root filesystem for Octeon based on
	Squeeze distribution.
debian_octeon.tgz
	A link to Debian root filesystem for Octeon based on Squeeze.
inittab.patch
	Patch applied to the standard Debian /etc/inittab & /etc/securetty
	for serial console & PCI-mapped console support.
module-init-tools-depmod.patch
	Patch applied to the standard Debian /etc/init.d/module-init-tools
	to call "depmod -a" on every boot.
ntp-ntpdate.patch
	Patch applied to the standard Debian /etc/init.d/ntp to call
	ntpdate to set the correct date before starting ntp.
source
	Debian source packages with Octeon specific changes.
