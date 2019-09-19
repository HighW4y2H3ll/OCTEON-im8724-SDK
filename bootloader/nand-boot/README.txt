
NAND Boot Stage 1
-----------------

The details of the boot process for booting from NAND is documented in the
Octeon hardware manual and the SDK doxygen documentation. Refer to
"Booting Octeon from NAND Flash" for more information. Here are a couple of
key points:

1) Stage 1 must be linked at 0xffffffff80100000.
2) Stage 2 should be linked at 0xffffffff80400000. (or be relocatable/TLB mapped)
3) Stage 2 must contain a bootloader information header.
4) In order to reduce the amount of changes to the critical NAND block 0, place
	stage 2 in the 2nd NAND block, or later. This way block 0 is only
	updated for stage 1, which changes infrequently.
5) It may be necessary to change the defines in start.S for your specific
	NAND configuration (page size, etc.)  and board clocks.  If these
	don't match the NAND part used stage 1 will fail.

Here is a short description of the files that can be found in this directory:

Makefile
	This build the NAND stage 1 bootloader and optionally runs it in the
	Octeon simulator when the "run" target is used.

start.S, main.c
	The is the source code for the stage 1 bootloader.

nand-boot.elf
	This is the stage 1 bootloader packaged as an ELF file with debugging
	information. Use this with EJTAG probes and other debuggers for source
	information. Debuggers will not give correct information until the
	stage 1 copies itself into L2.

nand-boot.bin
	This is the raw binary code that needs to be put at the beginning of
	NAND flash. It is not ECC encoded yet.

nand-boot.nand
	This is the stage 1 image encoded into 256 byte ECC blocks read for
	direct burn into NAND flash.

ecc-encode
ecc-encode.c
	This is a simple utility to convert a "*.bin" into a "*.nand". It takes
	every 256 bytes and encodes it into an Octeon 264 byte ECC block.

trivial-stage2.S
	This is very minimal sample of a stage 2 bootloader used for testing
	stage 1. This serves no practical purpose and is replaced by a more
	full featured u-boot base stage 2 in the real system.

trivial-stage2.elf
trivial-stage2.bin
trivial-stage2.nand
	Elf file, raw binary, and ECC encode of the trivial test stage 2.

combined.nand
	A NAND image with both stage 1 and stage 2 encoded in ECC format. This
	is used for simulation testing.

