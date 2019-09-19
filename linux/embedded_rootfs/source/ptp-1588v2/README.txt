Linux Userspace IEEE1588-2008 (PTPv2) Daemon
--------------------------------------------

The Cavium Inc. implementation of the IEEE1588 specification for
Precision Time Protocol (PTP) supports all OCTEON models.  The
implementation uses hardware timestamps available in OCTEON II for
better accuracy, or reverts to software timestamps on OCTEON and
OCTEON Plus chips.  The daemon's binary is called "ptp-1588v2" and is
installed under "/usr/bin" in the embedded root filesystem. Detailed
documentation is contained in the OCTEON SDK HTML pages, titled
"1588 Precision Time Protocol".

PTP Clock versus Kernel Wall Time
---------------------------------
The 1588v2 program can be compiled to either synchronized the Linux
kernel's wall time, or the hardware PTP clock on OCTEON II chips.  If
the define USE_PTP_CLOCK is set to 1 in "Makefile", the the hardware
PTP clock is used.  Here are some notes about the two clocks:

USE_PTP_CLOCK=0 (Use kernel's wall time)
	* Does not require PTP hardware.
	* Can run on any Linux host, including x86.
	* Synchronizes using adjtimex().
	* Accuracy is limited by the granularity of adjtimex().
	* Empirically clock synchronization accuracy is limited to around 2us.

USE_PTP_CLOCK=1 (Use hardware PTP clock)
	* Requires hardware PTP clock only found on OCTEON II.
	* Synchronizes by adjusting the PTP clock directly.
	* Raw hardware timestamps are used for all packets.
	* Linux's wall clock time is not synchronized.
	* Empirically clock synchronization accuracy is under 10ns.

Cavium recommends setting USE_PTP_CLOCK=1, which is the default.

License
-------
The source code for "ptp-1588v2" is supplied by Cavium Inc. through a
proprietary license. Other license options may be available. Contact
"support@cavium.com" for more information.

Limitations
-----------
Ptp-1588v2 is still in development and currently has the following issues
and missing features compared to the documentation in
"1588 Precision Time Protocol".

1) Only interactive runs are supported. Extra debug output cannot be
	suppressed so the application does not run as a daemon.
2) UDP transport over multiple ethernet interfaces has issues. The socket
	bind calls needed to register for UDP multicast packets currently
	don't tie to a specific interface. This means incomming packets may
	be mistakely connected to the wrong interface, breaking the 1588
	protocol. Running ptp-1588v2 with only one interface has no issues.
3) UDP6 does not support hardware TX timestamps. The Linux kernel doesn't
	support hardware tiemstamps for UDP over IPv6, so accuracy will be
	much less than IPv4.

Example Command Lines
---------------------

Perform IEEE1588-2008 time synchronization using ethernet interface "eth0"
with 802.3 encapsulation.

	$ ptp-1588v2 eth0

Perform IEEE1588-2008 time synchronization using ethernet interface "eth1"
with 802.3 encapsulation and use "eth2" with UDP encapsulation.

	$ ptp-1588v2 eth1:802.3 eth2:udp

---


