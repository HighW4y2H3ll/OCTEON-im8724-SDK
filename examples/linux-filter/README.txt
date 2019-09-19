README for the linux-filter example

Overview
--------
This example is a demonstration of using a simple executive application to
completely control packet input and output to the Linux kernel.  The
application linux-filter acts as a proxy for the Linux kernel filtering
out all IP broadcasts.  Linux-filter intercepts all incoming packets on
eth0 before they are received by the kernel and optionally forwards them.
As a hypothetical example, assume you want the Linux kernel to answer ICMP
pings for specific IP addresses, but want to ignore any broadcast pings.
In this hypothetical example packet processing would occur in the
following sequence.

	1) A host pings the Linux kernel at address 192.168.1.2
	2) The host operating system sends an ARP request to the OCTEON.
	3) The OCTEON PIP hardware receives the ARP request and marks it with
		POW group 0 (FROM_INPUT_PORT_GROUP).
	4) The simple executive application linux-filter receives this request
		when it performs a cvmx_pow_work_request_sync().
	5) Linux-filter checks the incoming packet to make sure it isn't
		an IP broadcast. It is a broadcast, but not IP.
	6) Linux-filter then resubmits the packet with POW group 14
		(TO_LINUX_GROUP) using cvmx_pow_work_submit() or
		cvmx_pow_tag_sw_desched(). Linux has been configured to only
		accept packets from POW group 14 (octeon-pow-ethernet ptp_rx_group).
	7) The Linux kernel receives the ARP request and submits a reply using
		POW group 13 (octeon-pow-ethernet ptp_tx_group).
	8) linux-filter receives the ARP reply, check that it isn't a IP
		broadcast, and send it out the PKO hardware.
	9) The original host receives the ARP reply and sends the ping request.
	10) The ping request, not being an IP broadcast follows the same
		process as the ARP request in steps 3 - 9.
	11) The original host performs a broadcast ping using
		"ping -b 192.168.1.255". This doesn't require an ARP request /
		reply. Instead the ICMP ping is sent immediately as an IP
		broadcast.
	12) Linux-filter receives this request when it performs a
		cvmx_pow_work_request_sync().
	13) Linux-filter checks the incoming packet, determining that it is an
		IP broadcast.
	14) Linux-filter uses cvmx_pip_free_packet_data() and cvmx_fpa_free()
		to drop both the incoming packet and its associated work queue
		entry.

A key component of this example is configuring the Linux
octeon-ethernet and octeon-pow-ethernet drivers to coexist with a
simple executive application properly.  In this example all OCTEON
hardware setup is performed by the Linux octeon-ethernet module.  The
octeon-pow-ethernet driver is used to bridge packets between the Linux
kernel and the linux-filter application.  Several octeon-pow-ethernet
driver parameters must be set.

	ptp_tx_group
		If this parameter is set, the Linux
		octeon-pow-ethernet modules creates a virtual ethernet
		device not connected to any physical ports.  This
		device (pow0) will accept work from pow receive group
		and transmit using the supplied pow send group. Valid
		range is 0 - 15.

	ptp_rx_group
                POW Group for pow0 to receive packets from
		(0-15). Only packets from this group will be received
		by the pow0 virtual ethernet device in the Linux
		kernel.

And on CN7XXX (except cn70xx):
        pki_packet_pool
		This is the packet aura id used by the port to filter on.
		It must be extracted from the output of the octeon3-ethernet
		module as it's loaded (see bellow: EBB7800 - How to find
		pki_packet_pool). This parameter must be provided on cn7xxx
		chips.


Also make sure that the Linux kernel is built with CONFIG_OCTEON_ETHERNET_LOCKED. 

Implementation Notes
--------------------
- Single port
	Linux-filter intercepts packets destine to the Linux kernel only on a
	single port, eth0. Intercepting more incoming ports is a trivial
	change, but routing outgoing packets would be more difficult. The
	ethernet driver only supports one virtual device, "pow0". A scheme
	would be needed to differentiate packets by payload instead of just
	POW group.

- Waiting for packets
	When there are no packets coming in, linux-filter needs to stop
	processing and not hog chip resources. In the standalone simple
	executive environment this is very simple. Here we can use the
	CVMX_POW_WAIT flags to put the core to sleep while we don't have
	packets. Under Linux userspace, this scheme doesn't work well; under
	Linux, other processes may need to execute. When we are idle we call
	usleep(0) which tells the kernel to prematurely end our timeslice.
	This way we yield to any other pending processes.

- FAU timeouts
	During initialization, linux-filter disables FAU tag switch timeouts.
	These timeouts are suppose to be disabled by the kernel, but SDKs
	before 1.6.0 didn't set this correctly. Linux-filter sets it as a
	workaround for this bug.

- Output queue locking
	The normal recommended simple executive output locking cannot be used
	in Linux with the ethernet driver loaded. When the ethernet driver
	receives a packet, it asynchronously interrupts processing and
	retrieves packets. This has the side affect of freeing the POW tag
	associated with any program running at the time. This means that you
	can't rely on atomic tags for mutual exclusion when the ethernet driver
	is loaded. Linux-filter uses a spinlock to get around this limitation
	when run under Linux. This method is slower than the normal tag
	switches.

- Submit vs Desched
	For the same reason you can't use POW tags for output locking, you also
	can't assume that you have the same POW tag as when you received work.
	This means cvmx_pow_tag_sw_desched() can't be used under Linux.
	Linux-filter uses cvmx_pow_work_submit() as a functional equivalent.

- Input port
	Linux-filter can be passed an optional port number on the
	command line.  This is needed on EBB6800/EBB7X00 boards.  It can also
	be used of override the default port number on non-EBB6800/EBB7X00
	systems.

Compiling
---------
Linux-filter can either be run as a standalone simple executive application
parallel to Linux or as a userspace application. The standalone model provides
optimal performance while the userspace application is more flexible.

To build for the standalone simple executive:

	$ cd ${OCTEON_ROOT}/examples/linux-filter
	$ make 				# For 64bit
	$ make OCTEON_TARGET=cvmx_n32 	# For 32bit

To build for Linux userspace:

	$ cd ${OCTEON_ROOT}/examples/linux-filter
	$ make OCTEON_TARGET=linux_64 	# For 64bit
	$ make OCTEON_TARGET=linux_n32 	# For 32bit

	Note that this example is already built and installed in the
	embedded_rootfs by default. It can be found in the /examples directory.


EBB7800 - How to find pki_packet_pool
-------------------------------------

The pki_packet_pool is the aura used by the port to filter on. This information
is displayed when octeon3-ethernet ethernet driver is installed.

For example if SGMII module is connected to QLM4, and want to use 2nd port for
linux-filter example, then ipd_port will be 0xa10 for eth1 and pki_packet_pool
will be 1020 based on the following output

  ~ # modprobe octeon3-ethernet
  octeon_bgx_nexus 11800e2000000.ethernet-mac-nexus: Created PKI 0: 80000007fded3600
  octeon_bgx_nexus 11800e2000000.ethernet-mac-nexus: Created PKI 1: 80000007fded3a00
  octeon_bgx_nexus 11800e2000000.ethernet-mac-nexus: Created PKI 2: 80000007fded3e00
  octeon_bgx_nexus 11800e2000000.ethernet-mac-nexus: Created PKI 3: 80000007fdd38000
  octeon_bgx_nexus 11800e2000000.ethernet-mac-nexus: Probed
  octeon_bgx_port 11800e2000000.0.ethernet-mac: Probed
  octeon_bgx_port 11800e2000000.1.ethernet-mac: Probed
  octeon_bgx_port 11800e2000000.2.ethernet-mac: Probed
  octeon_bgx_port 11800e2000000.3.ethernet-mac: Probed
  octeon3_eth_global_init  SSO:63:1023, PKO:62:1022
  ethernet-mac-pki ethernet-mac-pki.0: Probing 0-2:0
  ethernet-mac-pki ethernet-mac-pki.0: rx sso grp:1..2 aura:1021 pknd:0 pko_queue:1
  ethernet-mac-pki ethernet-mac-pki.0 eth0: Registered
  <<<<<<<<<<<<<<
  ethernet-mac-pki ethernet-mac-pki.1: Probing 0-2:1
  ethernet-mac-pki ethernet-mac-pki.1: rx sso grp:2..3 aura:1020 pknd:1 pko_queue:2
  ethernet-mac-pki ethernet-mac-pki.1 eth1: Registered
  >>>>>>>>>>>>>>
  ethernet-mac-pki ethernet-mac-pki.2: Probing 0-2:2
  ethernet-mac-pki ethernet-mac-pki.2: rx sso grp:3..4 aura:1019 pknd:2 pko_queue:3
  ethernet-mac-pki ethernet-mac-pki.2 eth2: Registered
  ethernet-mac-pki ethernet-mac-pki.3: Probing 0-2:3
  ethernet-mac-pki ethernet-mac-pki.3: rx sso grp:4..5 aura:1018 pknd:3 pko_queue:4
  ethernet-mac-pki ethernet-mac-pki.3 eth3: Registered


EBT5800 - Running as a standalone Simple Exec Application
---------------------------------------------------------
Put both the Linux kernel and the linux-filter example on a compact flash.
If your compact flash has limited space, it may be necessary to strip the
binaries first.

	$ mount /mnt/cf1
	$ cp ${OCTEON_ROOT}/examples/linux-filter/linux-filter /mnt/cf1/
	$ cp ${OCTEON_ROOT}/linux/kernel/linux/vmlinux.64 /mnt/cf1/
	$ umount /mnt/cf1

Put the compact flash into the EBT5800 and boot both the Linux kernel and
the linux-filter example.

	Octeon ebt5800# fatload ide 0 ${loadaddr} vmlinux.64
	Octeon ebt5800# bootoctlinux ${loadaddr} coremask=ff00
	Octeon ebt5800# fatload ide 0 ${loadaddr} linux-filter
	Octeon ebt5800# bootoct ${loadaddr} coremask=ff endbootargs

When booting on EBB6800 or on cn7xxx based boards that has BGX interfaces, we
need to specify the IPD port used by ethX, if a single quad SGMII module is
installed on QLM4 (or BGX2) the command would
be:
	on 68xx:

	Octeon ebb6800# bootoct ${loadaddr} coremask=ff endbootargs linux-filter 0xc00
	on 7xxx:

	Octeon ebb7800# bootoct ${loadaddr} coremask=ff endbootargs linux-filter 0xa00

After the Linux kernel boots, load the ethernet driver.  Use the
pow_send_group and pow_receive_group parameters to control where packets
are send and received from.

	# modprobe octeon-ethernet
	# modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=14 ptp_tx_group=13

These commands are different on the CN7X00 devices.

	# modprobe octeon3-ethernet
	# modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=23 ptp_tx_group=22 pki_packet_pool=1021

Please refer to "EBB7800 - How to find pki_packet_pool" section, on how to
calculate pki_packet_pool.

Next, configure the interfaces under Linux.  eth0 needs to be in
promiscuous mode since the pow0 device has a different MAC address than
eth0.  This allows the linux-filter to receive packets for any MAC
address.

	# ifconfig eth0 promisc up
	# ifconfig pow0 192.168.1.2

Now it should be possible to ping the EBT5800 with the IP address of
192.168.1.2.  If you perform a broadcast ping to 192.168.1.255, the
EBT5800 will not answer.

EBT5800 - Running as a Linux Userspace Application
--------------------------------------------------
The default embedded rootfs in Linux includes the linux-filter example.
You don't need to copy anything special, just boot Linux as normal
according to the SDK documentation.  Once Linux boots to a busybox prompt,
you're ready to continue these instructions.

First, load the ethernet driver.  Use the pow_send_group and
pow_receive_group parameters to control where packets are send and
received from.

	# modprobe octeon-ethernet
	# modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=14 ptp_tx_group=13

These commands are again different on the CN7800 devices.

	# modprobe octeon3-ethernet
	# modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=23 ptp_tx_group=22 pki_packet_pool=1021

Please refer to "EBB7800 - How to find pki_packet_pool" section, on how to
calculate pki_packet_pool.

Next, configure the interfaces under Linux.  eth0 needs to be in
promiscuous mode since the pow0 device has a different MAC address than
eth0.  This allows the linux-filter to receive packets for any MAC
address.

	# ifconfig eth0 promisc up
	# ifconfig pow0 192.168.1.2

Finally start linux-filter.  You can optionally control which cores it
runs on using the "schedtool" utility.

	$ schedtool -a 0xff -e /examples/linux-filter

When running on EBB6800 or on EBB7800, we need to specify the IPD port used by
eth0, if a single quad SGMII module is installed on QLM4 the command would
be:

	on 68xx: 
	$ schedtool -a 0xff -e /examples/linux-filter 0xc00

	on 78xx: 
	$ schedtool -a 0xff -e /examples/linux-filter 0xa00

Now it should be possible to ping the EBT5800 with the IP address of
192.168.1.2.  If you perform a broadcast ping to 192.168.1.255, the
EBT5800 will not answer.  You may want to start linux-filter in the
background so the shell is still available.

OCTEON Simulator - Running
--------------------------
Running this example will require three separate terminal windows. Here is a
quick description of each:

	Terminal 1: Simulator output and linux-filter output
	Terminal 2: Linux interactive terminal
	Terminal 3: For oct-packet-io and packet input / output

In Terminal 1:
	Start the simulator up running both Linux and the linux-filter example.

		$ make run

	Almost immediately you will begin to see the following messages:

		waiting for a connection to uart 0 1
		waiting for a connection to uart 0 2
		...

In Terminal 2:
	Connect to the simulator to receive UART output.

		$ telnet localhost 2020

	You should first get some messages from the boot loader followed by
	the Linux kernel boot messages. These messages will eventually end
	in a shell prompt (after about 30M cycles). At the Linux shell prompt
	you need to load the Cavium OCTEON ethernet driver. As the module is loaded,
	configure it for POW output.

		$ modprobe octeon-ethernet
		$ modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=14 ptp_tx_group=13

These commands are different on the CN7800 devices.

      	       	# modprobe octeon3-ethernet
		# modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=23 ptp_tx_group=22 pki_packet_pool=1021

        Please refer to "EBB7800 - How to find pki_packet_pool" section, on how to
        calculate pki_packet_pool.

	Now bring up the Linux ethernet interface "pow0" with an IP address of
	192.168.1.2.

		$ ifconfig eth0 promisc up
		$ ifconfig pow0 192.168.1.2

In Terminal 3:
	Send tcpdump packets from linux-filter.input to the simulator using
	oct-packet-io. Log the output packets to linux-filter.output in
	tcpdump format.

		$ oct-packet-io -I 0:linux-filter.input -t linux-filter.output

In Terminal 1:
	You should have received the following messages as packets were
	transfered between the linux-filter application and the Linux kernel.

		Received 60 byte packet. Sending to Linux.
		Received 98 byte packet. Sending to Linux.
		Received 98 byte packet. Sending to Linux.
		Received 98 byte packet. Sending to Linux.
		Received 98 byte packet. Sending to Linux.
		Received 98 byte packet. Sending to Linux.
		Received 42 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet. Sending to Linux.
		Received 98 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet. Sending to Linux.
		Received 98 byte packet from Linux. Sending to PKO.
		Received 98 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet. Sending to Linux.
		Received 98 byte packet from Linux. Sending to PKO.
		Received 98 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet. Sending to Linux.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 98 byte packet. Filtered.
		Received 1442 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet from Linux. Sending to PKO.
		Received 1442 byte packet from Linux. Sending to PKO.

	Kill the simulation by hitting Control-C in Terminal 1. The other
	two terminals will also stop processing.

---
