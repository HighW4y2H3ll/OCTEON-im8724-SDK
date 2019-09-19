
Using Macraigor EJTAG Probes with Octeon
----------------------------------------

Macraigor Systems supplies a number of high quality low cost EJTAG probes. All
of their products should work fine with Octeon, but Cavium Inc. has done
extensive testing with, and recommends, the following probes.

	Model		Minimum Firmware
	-----		----------------
	mpDemon		3.0.4.3

Firmware and software updates are available for free from the Macraigor
website. Be sure the software installed meets the minimum version lists
above. Macraigor's ocd-remote and supplied GDB remote stubs provide
excellent visibility into Octeon. In addition to Macraigor's software
products, Cavium Inc. provides native support for Macraigor probes in
the oct-remote-* utilities provides as part of the Octeon SDK. This can be
used to access core registers, access CSRs, access memory, load programs,
perform DDR tuning, and even boot over EJTAG. The specifics of the oct-remote
utilities are covered in the SDK HTML documentation. This file only highlights
some of the Macraigor specific options.


Setting up a Macraigor mpDemon for use with Octeon
--------------------------------------------------

Connect to the 9 pin serial port on the mpDemon with a straight through
serial cable. Using a baud rate of 19200, no parity, 8 bits, and 1 stop bit,
you should get the following menu after hitting return:

        Macraigor Systems mpDemon(tm) version 3.0.4.3

	Licenses           :
	API Baud Rate      : 115200
	Use DHCP?          : Yes
	DHCP Hostname      : mpdemon1
	TCP/IP Address     : 192.168.162. 97
	       Mask        : 255.255.255.  0
	       Gateway     : 192.168.162.  1
	MAC Address        : XX.XX.XX.XX.XX.XX
	OcdRemote TCP Port : 1000
	OCDREMOTE          : JTAG speed 1
			   : JTAG TDO <- [1]CN52xx=8888 [2]CN52xx=8889 [3]CN52xx=8890 [4]CN52xx=8891 <- TDI

	mpDemon Command Menu:

	B - Set Serial API baud Rate             D - Set DHCP option
	N - Set DHCP hostName
	O - Set OCDREMOTE Configuration          T - Test OCDREMOTE Configuration
	L - Update License Code(s)               C - Set Macraigor API TCP port
	P - Ping TCP/IP address                  J - Analyze JTAG Scan Chain
	E - Display Error Log                    R - Reboot mpDemon
	Type Command (<Enter> will refresh screen) ->

Choose "D" to enable DHCP for the probe.

	Type Command (<Enter> will refresh screen) -> D
	Use DHCP? (Y/N) ->y

	MUST REBOOT mpDEMON FOR DHCP CHANGE TO TAKE EFFECT

Choose "N" to set the hostname that will be used during DHCP to name the
device. The following example configures the probe to use the name of
"mpdemon1".

	Type Command (<Enter> will refresh screen) -> N
	DHCP hostname ->mpdemon1

	MUST REBOOT mpDEMON FOR DHCP CHANGE TO TAKE EFFECT

Choose "J" to scan the JTAG chain and determine the Octeon you are connecting
to. The following example is from a four core CN5220.

	Type Command (<Enter> will refresh screen) -> J
	Enter JTAG Clock rate (1 - 8) ->1

	JTAG SCAN CHAIN (4 device(s) detected, IR length 20):

	device[1] ID: 00c01399  IR:5   Cavium O52/CN52xx
	device[2] ID: 00c01399  IR:5   Cavium O52/CN52xx
	device[3] ID: 00c01399  IR:5   Cavium O52/CN52xx
	device[4] ID: 00c01399  IR:5   Cavium O52/CN52xx

	hit any key to continue

Choose "O" to configure the builtin GDB remote stub. This is used to allow
interactive debugging using the GDB remote protocol. Again, the following
example is from a CN5220.

	Type Command (<Enter> will refresh screen) -> O

	Initial JTAG speed (1 - 8) [1] ->1
	OCDREMOTE          : JTAG speed 1
			: JTAG TDO <- [1]CN52xx=8888 [2]CN52xx=8889 [3]CN52xx=8890 [4]CN52xx=8891 <- TDI
	Do you want to change the JTAG chain [no]? y
	Number of Devices in Scan Chain (0 - 16) [4] ->4
	Device List:
	    ARM 7/9/11 Families:
	      ...
	    Cavium 64-bit Family:
	      301x CN300x CN3020 CN31xx CN36xx
	      CN38xx CN502X CN52xx CN56xx CN58xx
	      CN63XX N3 O1P O2P O9N O52
	      O56 O5020
	    Freescale Coldfire Family:
	    ...

	Device1 [CN52xx] --> CN52xx
	TCP/IP port number : [8888] ->

	Device2 [CN52xx] --> CN52xx
	TCP/IP port number : [8889] ->

	Device3 [CN52xx] --> CN52xx
	TCP/IP port number : [8890] ->

	Device4 [CN52xx] --> CN52xx
	TCP/IP port number : [8891] ->
	Reboot mpDemon to run with new parameters
	hit any key to continue

At this time, configuration is complete. Reboot the probe with the "R" command
and view the new configuration.

	Type Command (<Enter> will refresh screen) -> r
	Are you sure you want to reboot? y
	Rebooting...
	OK

	Macraigor Systems mpDemon(tm) version 3.0.4.3

	Licenses           :
	API Baud Rate      : 115200
	Use DHCP?          : Yes
	DHCP Hostname      : mpdemon1
	TCP/IP Address     : 192.168.162.116
	       Mask        : 255.255.255.  0
	       Gateway     : 192.168.162.  1
	MAC Address        : 00.60.bf.01.01.b7
	OcdRemote TCP Port : 1000
	OCDREMOTE          : JTAG speed 1
			   : JTAG TDO <- [1]CN52xx=8888 [2]CN52xx=8889 [3]CN52xx=8890 [4]CN52xx=8891 <- TDI


Using oct-remote-* utilities with the mpDemon
---------------------------------------------

The environment variable OCTEON_REMOTE_PROTOCOL controls how the oct-remote
utilities communicate with Octeon. It use the native capabilities of the
Macraigor mpDemon, this variable should be set as follows:

	export OCTEON_REMOTE_PROTOCOL=macraigor:mpdemon1,1000

The "macraigor" is a keyword to tell the Octeon remote library to use
EJTAG. The "mpDemon1" is the DHCP name configured for the probe above. This
could also be an IP address. The "1000" is the port number for the probe. This
is normally 1000, but can be seen in the probes serial output as
"OcdRemote TCP Port : 1000".

Once the environment variable is set, the oct-remote utilities should work.
Here is an example to try:

	oct-remote-csr CIU_FUSE

This should return a set bit for each core in your Octeon part.


Debugging using the mpDemon
---------------------------

The GDB provided with the Octeon SDK can be used with the Macraigor probes. All
you need to do is set the "target remote" command appropriately access the
probe.

	$ mips64-octeon-linux-gnu-gdb kernel/linux/vmlinux.64
	(gdb) target remote mpdemon1:8888
	Remote debugging using mpdemon1:8888
	r4k_wait () at arch/mips/kernel/cpu-probe.c:62
	62      }
	(gdb) info registers
			  zero               at               v0               v1
	 R0   0000000000000000 ffffffff816f0000 ffffffff8016aa98 0000000000000000
			    a0               a1               a2               a3
	 R4   a80000041c759000 ffffffff816ee018 000000001000cce0 ffffffff816f0000
			    a4               a5               a6               a7
	 R8   ffffffff816f0000 ffffffff816f0000 ffffffff81710000 ffffffff808b7d00
			    t0               t1               t2               t3
	 R12  ffffffff808affe0 000000000000cc00 a800000411f98000 ffffffff808f0000
			    s0               s1               s2               s3
	 R16  ffffffff816f0000 ffffffff808b0000 ffffffff81710000 ffffffff816f0000
			    s4               s5               s6               s7
	 R20  0000000000008000 00000000000036c8 ffffffff816f0000 ffffffff816f0000
			    t8               t9               k0               k1
	 R24  ffffffff81710000 ffffffff808f0000 0000000000000000 000000000000000b
			    gp               sp               s8               ra
	 R28  ffffffff808ac000 ffffffff808aff50 ffffffff80930000 ffffffff8016d7f0
			    sr               lo               hi              bad
	      000000001000cce1 0000000000000000 0000000000000000 00000000201ef370
			 cause               pc
	      0000000000000000 ffffffff8016aaa4
	(gdb)

GDB sometimes will truncate register values incorrectly based on the current
Mips API. The following command can be useful to stop GDB from this horridness.

	(gdb) set mips abi n64

Macraigor implements a number of extended commands for getting at registers
and other data not available through GDB. The following command gives a list
of the extensions.

	(gdb) monitor help
	DLbuf <addr>
	allrun
	allstop
	char/short/long/longlong <addr>
	char/short/long/longlong <addr> = <val>
	endian [<big|little>]
	halt
	reg <regname>
	reg <regname> = <value>
	reset
	resetrun
	runfrom <addr>
	set memspace <virtual|physical|#>
	set cpu <cpu number>
	set/clear hbreak [<address>]
	set regbufaddr <address>
	sleep <seconds>
	softbkpts [on/off]
	status
	sync cpus <on/off>
	test endian <address>

Here is how you would get the value COP0 CVMCTL register:
	(gdb) monitor reg cvmctl
	cvmctl   00000000b00043f0

Get a list of the registers with:
	(gdb) monitor help regname
	sr inx rand elo0 elo1 ctxt pgmask wired hwrena badvaddr count ehi
	compare status cause epc prid config iwatchlo iwatchhi dwatchlo
	dwatchhi xcontext multicoredebug debug depc perfcntctl icacheerr
	icachetaglo errorepc dsave pagegrain intctl ebase config1 perfcntcnt
	dcacheerr icachedatalo icachedatahi srsctl config2 perfcntctl1
	dcachetaglo dcachetaghi config3 perfcntcnt1 dcachedatalo dcachedatahi
	cvmcount cvmctl memctl

---
