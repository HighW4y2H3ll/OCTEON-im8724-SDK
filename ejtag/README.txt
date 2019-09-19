
OCTEON EJTAG Files

This directory contains scripts, utilities, and documentation to help
in using various EJTAG probes with OCTEON. Each directory here contains
support files for the EJTAG probe mentioned in the directory name. These
files are supplied in the interest of making development with OCTEON easier.
Questions on how to use the various EJTAG tools should be referred to the
probe's vendor.

Here is short summary of some of what is available:

abatron-bdi3000/
	This directory contains configuration files for using the Abatron
	BDI3000 EJTAG probe with OCTEON. These files should be placed in
	the tftp server in use by the BDI3000. See README.txt in the
	directory for more information.

macraigor/
	In this directory you will find documentation on using Macraigor
	EJTAG probes with OCTEON. In addition to this documentation, the
	OCTEON remote utilities directly support Macraigor probes. The
	source for these utilities can be found in the SDK under
	host/remote-utils and host/remote-lib. Binaries should already be
	in your PATH after proper SDK installation and setup. Usage of these
	utilities is documented as part of the normal SDK HTML documentation
	system.

mentor-graphics-majic/
	This directory contains OCTEON CSR definitions for each OCTEON
	chip. These files can be used with the Majic's graphical debugger
	to symbolically dump all OCTEON CSRs. You must replace the stub
	OCTEON CSR file supplied by Mentor Graphics with the appropriate
	file. You should generally use the CSRs from the latest pass of
	the chip. These are in the files without "p*" after the chip model
	number. Instructions pertaining to using Majic probes with the
	OCTEON SDK utilities can be found in the README.txt inside the
	directory.

windriver-ice2/
	This directory contains OCTEON CSR definitions for each OCTEON
	chip. These files can be used with the Windriver's Workbench
	graphical debugger to symbolically dump all OCTEON CSRs. See
	README.txt in the directory for more information.


