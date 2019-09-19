OCTEON Register files for Wind River Workbench On-Chip Debugging (OCD)
---------------------------------------------

The directory "register-files" contains register definitions and detail
files for all OCTEON chips for use with the Wind River Workbench On-Chip
Debugging environment. These files are named using the following scheme:

	windriver-ejtag-[CHIP ID][PASS].reg
	windriver-ejtag-[CHIP ID][PASS].dtl

[CHIP ID] is replaced with the OCTEON family model number. For example:

	windriver-ejtag-cn30XX*
		This is for OCTEON CN3005 and CN3010.
	windriver-ejtag-cn31XX*
		This is for OCTEON CN3120, CN3110, and CN3020.
	windriver-ejtag-cn38XX*
		This is for OCTEON CN3860, CN3850, etc.
	windriver-ejtag-cn50XX*
		This is for OCTEON CN5020, CN5010, and CN5000.
	windriver-ejtag-cn52XX*
		This is for OCTEON CN53XX, CN52XX, and CN51XX.
	windriver-ejtag-cn56XX*
		This is for OCTEON CN57XX, CN56XX, CN55XX and CN54XX.
	windriver-ejtag-cn58XX*
		This is for OCTEON CN5860, CN5850, etc.

[PASS] is replaced with the OCTEON chip major pass number.
	"p1" stands for Pass 1.x parts.
	"p2" stands for Pass 2.x parts.
	"p3" stands for Pass 3.x parts.
	If the pass is omitted, the file is for the latest major pass of
	the chip. In general, these are the files you should use.

The exact OCTEON part number is printed as part of the U-Boot banner during
board initialization. The following is an example from a CN5230 on a EBT5200
evaluation board.

---
U-Boot 1.1.1 (Development build, svnversion: u-boot:39202, exec:39202)

EBT5200 board revision major:1, minor:1, serial #: board-12
OCTEON CN5230-SCP pass 2.0, Core clock: 750 MHz, DDR clock: 199 MHz (398 Mhz data rate)
---


System Requirements
-------------------

Wind River Workbench On-Chip Debugging version 3.1 and higher when
used with Wind River ICE 2 or Wind River Probe.

Notes:
If you are using a version of Wind River Workbench On-Chip Debugging
prior to version 3.1, you must update to version 3.1 or higher to use
these register files.  These register files are not supported when
using Wind River ICE or Wind River ICE SX (blue-colored JTAG unit).
Please contact your local Wind River Sales representative for more
information on the Wind River ICE 2.  A trade-in program may be
available.


Installing the Register Files
-----------------------------

The register and detail files in this SDK supercede those currently shipped
in Workbench 3.1 OCD releases (for revisions of future revisions of
Workbench OCD, please contact Cavium Inc. Technical Support). The
following procedures should be used for Installing the files.


Workbench OCD 3.1

1. Remove all Cavium Inc. register files other than those with "ALT" in
   the name (e.g. cn5860_MC_AltCPU_BE.reg) from the following directory

   <install dir>\ocd-3.1\RegisterFiles\mips64\Cavium

2. Copy *.reg files from the SDK into the above directory.

3. Remove all of the Cavium Inc. detail files from the following directory

   <install dir>\ocd-3.1\DetailFiles

4. Copy the detail file that is relevant to your target to the above directory.

5. Rename the detail file to match the exact name of the core
   (e.g. rename windriver-ejtag-cn30xx.dtl to cn3010.dtl)


Please direct questions related to Wind River Workbench to Wind River through
http://www.windriver.com/support/.

Questions specific to the Cavium Inc. OCTEON register files can be
directed to Cavium Inc. though support@cavium.com.

---

