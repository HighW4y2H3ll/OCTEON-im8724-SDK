Linux for the Octeon Processor

The Makefile in this directory will build a small, but functional Linux
distribution from scratch. It is based on the Linux 2.6 kernel with MIPS
changes (http://www.linux-mips.org/) and the Busybox user application suite
version 1.2.1 (http://www.busybox.net/).

Make targets:
    kernel      - Build the Linux kernel supporting all Cavium Octeon reference boards
    kernel-deb  - Linux kernel without the rootfs
    sim         - Octeon simulation environment
    flash       - Copy kernel onto compact flash at mount /mnt/cf1
    strip       - Strip symbols out of the kernel image
    tftp        - Copy a stripped kernel to /tftpboot
    test        - Test an existing simulator build
    clean       - Remove all generated files and the KERNEL CONFIG
					
Both the kernel and the filesystem are independent and may be built
separately. Simply execute make in the appropriate directory. The Debian
targets assumes the Debian filesystem is already on the compact flash in the
second partition. See the Makefile and README under the "debian" directory
for more information.

Running Linux in the Octeon Simulator:

	$ cd kernel
	$ ./oct-linux -quiet -noperf
	
"oct-linux" is simply a wrapper for "oct-sim" to provide the needed bootloader
arguments and default parameters. Most of the usual "oct-sim" arguments should
work fine. You may need to modify "oct-linux" to tweak the TCP ports, amount
of memory, and filesystem used.

More detailed documentation can be found in the Octeon SDK HTML pages.


