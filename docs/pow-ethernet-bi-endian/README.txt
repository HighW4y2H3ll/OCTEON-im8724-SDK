See SDK section "Mixing Big-Endian & Litte-Endian Kernels"

bootscript.in:
	Sample u-boot script for loading two different kernels,
	copying FDT & trimming each to partition machine resources.
	Heavily commented, so may be too big for u-boot environment,
	so it's annotated with directions for creation of a loadable
	(from tftp or filesystem) u-boot script.

pow-setup:
	Script to run on each kernel, to configure octeon-pow-ethernet
	for endian-swap as packets pass between, allowing both sides
	to communicate with outsode world
