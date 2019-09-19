
Overview
--------

The Mentor Graphic Majic debugger and EJTAG probe support all Octeon
models and provide robust GUI debugging even in early boot.  Combining
their Windows graphical environment with the Linux based command line
tools provides an environment with high visibility into Octeon.  Board
bringup, DRAM tuning, and remote access is directly supported by the
Octeon SDK.  This along with EDB under Windows provides a fairly complete
environment.


Installing Octeon CSR Files
---------------------------

The "majic-ejtag-*.rd" files in this directory supply register definitions
for all Octeon chips suitable for use with the Mentor Graphics EDB
graphical debugger.  These files provide detailed decoding of every field
in each Octeon CSR.  To use these files, copy them into the EDB "bin"
directory and modify your configuration files to include the appropriate
file.  Each file is named by the Octeon model family and the silicon pass
number.  Files without a pass number are for the latest revision of the
chip.  These should normally be used.  Depending on the version of the EDB
startup files you are using, the proper include lines for these files may
already be in "cpu_select.cmd".  If this is the case, all that is needed
is to copy all of the "majic-ejtag-*.rd" files to
"C:\Program Files\EPITools\edtm24c\bin".

Note:	Make sure the Majic EJTAG probe's firmware is at least version
	5.2. The Mentor Graphics windows utilities provide information
	and instructions on how to upgrade your firmware.


Using the Majic with the Octeon Remote Utilities
------------------------------------------------

The Octeon remote utilities, oct-remote-*, can be used with Mentor
Graphics Majic probes.  The Octeon SDK contains a minimal set of
configuration files and Linux binaries to support a GDB interface to the
Majic.  These support files are located in the "bin" and "cavium_minimal"
directories.

Note: 	The binary files under the "bin" directory are originally from Mentor
	Graphics. These are packaged with the Octeon SDK to facilitate easy
	access to the Majic. To access the full capabilities of the Majic,
	install the Mentor Graphics support software.

The first step in using the Majic with the oct-remote utilities is to
startup a MDI GDB server.  The script bin/mdi-gdbserver.sh simplifies this
quite a bit, so the SDK adds this to your default path as "mdi-gdbserver".
"mdi-gdbserver" takes two arguments.  The first argument is the IP address
or hostname of your Majic probe.  The second argument is the number of
cores supported by the Octeon chip being debugged.  Once started, it is
unusual for you to need to restart the MDI server.  It is recommended you
start it in a separate Xterm and leave it minimized.  Here is an example
for a CN5860 connected to a Majic probe named "epi1-162".

	$ mdi-gdbserver epi1-162 16
	Starting mdi-server.static for core 0 on port 8888
	Starting mdi-server.static for core 1 on port 8889
	Starting mdi-server.static for core 2 on port 8890
	Starting mdi-server.static for core 3 on port 8891
	Starting mdi-server.static for core 4 on port 8892
	Starting mdi-server.static for core 5 on port 8893
	Starting mdi-server.static for core 6 on port 8894
	Starting mdi-server.static for core 7 on port 8895
	Starting mdi-server.static for core 8 on port 8896
	Starting mdi-server.static for core 9 on port 8897
	Starting mdi-server.static for core 10 on port 8898
	Starting mdi-server.static for core 11 on port 8899
	Starting mdi-server.static for core 12 on port 8900
	Starting mdi-server.static for core 13 on port 8901
	Starting mdi-server.static for core 14 on port 8902
	Starting mdi-server.static for core 15 on port 8903
	Hit Control-C to kill MDI servers

Once the server is started, you set the Octeon remote protocol as follows:

	$ export OCTEON_REMOTE_PROTOCOL=gdb:localhost,8888

At this point all of the oct-remote utilities are available.  Below is an
example of using oct-remote-csr to read an Octeon CSR.

	$ oct-remote-csr CIU_INT0_EN0
	CIU_INT0_EN0(0x0001070000000200) = 0x0100400700000000
	  [63:59] RESERVED_59_63       =          0 (0x0)
	  [   58] MPI                  =          0 (0x0)
	  [   57] PCM                  =          0 (0x0)
	  [   56] USB                  =          1 (0x1)
	  [55:52] TIMER                =          0 (0x0)
	  [   51] RESERVED_51_51       =          0 (0x0)
	  [   50] IPD_DRP              =          0 (0x0)
	  [   49] RESERVED_49_49       =          0 (0x0)
	  [   48] GMX_DRP              =          0 (0x0)
	  [   47] RESERVED_47_47       =          0 (0x0)
	  [   46] RML                  =          1 (0x1)
	  [   45] TWSI                 =          0 (0x0)
	  [   44] RESERVED_44_44       =          0 (0x0)
	  [43:40] PCI_MSI              =          0 (0x0)
	  [39:36] PCI_INT              =          0 (0x0)
	  [35:34] UART                 =          1 (0x1)
	  [33:32] MBOX                 =          3 (0x3)
	  [31:16] GPIO                 =          0 (0x0)
	  [15: 0] WORKQ                =          0 (0x0)

See the SDK HTML documentation for more information about the Octeon
remote utilities.

---
