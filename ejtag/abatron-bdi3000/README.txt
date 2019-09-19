
Abatron BDI3000 Configuration Files
-----------------------------------

The BDI3000 loads the EJTAG configuration dynamically on every boot through
TFTP. The "*.cfg" files in this directory are the top level configuration
files the BDI3000 should load. Each file is named by the Octeon chip model
number it should be used for. Here are some examples:

cn3860.cfg - Use this with a 16 core CN38XX Octeon.
cn5020.cfg - Use this with a two core CN50XX Octeon+.
cn5645.cfg - Use this with a 12 core CN57XX or CN56XX Octeon+.

The files "cn*.def" should be placed in a subdirectory named "bdi" on the
TFTP server. Here is an example directory layout:

/tftpboot/cn3860.cfg
/tftpboot/cn5020.cfg
/tftpboot/cn5645.cfg
/tftpboot/bdi/cnmips.def
/tftpboot/bdi/cn*-abatron-csrs.def


Sample BDI3000 Configuration
----------------------------

Using the "bdisetup" utility supplied by Abatron, below is a sample
configuration of the BDI3000 for a CN56XX. Make sure the firmware of the
probe is at least version 1.06.

$ bdisetup -v -p/dev/ttyS7 -b115
	BDI Type : BDI3000 (SN: 33063908)
	Loader   : V1.01
	Firmware : V1.06 bdiGDB for MIPS64
	MAC      : 00-0c-01-33-06-39
	IP Addr  : 192.168.162.230
	Subnet   : 255.255.255.0
	Gateway  : 192.168.162.1
	Host IP  : 192.168.162.83
	Config   : cn5645.cfg

Here is the "bdisetup" command used to configure the probe. In this setup
we're using a serial cable connected to ttyS7 to access the BDI3000.

$ bdisetup -c -p/dev/ttyS7 -b115 -i192.168.162.230 -h192.168.162.83 \
	-m255.255.255.0 -g192.168.162.1 -fcn5645.cfg


Using the BDI3000 with the Octeon Remote Utilities
--------------------------------------------------

The SDK utilities for accessing a remote Octeon over EJTAG can be used with
a BDI3000. Oct-remote communicates with the BDI3000 using the GDB remote
protocol stub built into the probe. All that is needed to use the BDI3000
is to configure the OCTEON_REMOTE_PROTOCOL environment variable. Below
is an example for the bash shell.

	export OCTEON_REMOTE=gdb:192.168.162.230,2001

Replace the "192.168.162.230" with the IP address of your BDI3000. The default
GDB server ports begin at 2001, so this should not normally need any changes.

See the SDK HTML documentation for more information about the Octeon remote
utilities.

---
