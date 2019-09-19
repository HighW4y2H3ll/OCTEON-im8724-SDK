Readme for passthrough example

This example implements a loopback pass through for RGMII/SGMII/XAUI
ports. Each port loops every packet received back out the same port.  


Support for simulator
======================

Requirements
------------ 

1) This test uses the oct-packet-io utility to pipe files into the
   simulator.

2) The unix file difference utility (diff) is used to compare results.
   This should be standard on all unix distributions. 

4) The example only runs for a set number of packets. If you change
   the number of packets, it will be necessary to change the count.
   See PACKET_TERMINATION_NUM in the source.  

5) For pre-CN68XX models, port numbers are not sequential for all
   eight ports. The first four are 0-3, the next four are 16-19. For
   CN68XX, IPD port numbers are reorganized. For example, the four
   SGMII ports on interface 0 are 2048, 2064, 2080, and 2096. And
   those on interface 2 are 2560, 2576, 2592, and 2608. See HRM on
   WORD2[PORT] of the work-queue entry for details.

Running on simulator
--------------------

As an example case the test scripts send some sample data into the
simulator and log the results into "output-*.data". These files are
then compared with the original for pass / fail criteria.

Build and run the sample:

	$ make clean
	$ make
	$ make run

For CN68XX, the last command simulates the four SGMII ports on
interface 0. To simulate other SGMII ports on interface 2, 3, or 4, do

	$ INTF_TO_USE="sgmiiX" make run

where X is the interface number, 0, 2, 3, or 4.

To simulate XAUI on interface 0, invoke the following command

	$ INTF_TO_USE="xaui0" QLM_LANE_MASK=0x1 make run

This command will create input file to send packets to XAUI port on
interface 0 connected to QLM0. Similarly other XAUI interfaces can be
passed by replacing 'xaui0' with 'xauiX', where X is 2, 3 or 4.

The value set to QLM_LANE_MASK is passed to simulator option -qlm_is_xaui_mask
indicates which QLM to use for XAUI/RXAUI interface (mask with one bit per qlm).

Similarly ILK0, ILK1, NPI interfaces can be simulated by passing
'ilk0', 'ilk1', 'npi' to INTF_TO_USE variable.

The number of processors simulated can be modified by changing
NUM_PROCESSORS in the makefile. Feel free to modify the packet input
files. The utility oct-packet-io supports other input formats. It can
read the output of "tcpdump -s0 -w <some_file>" as well as the OCTEON
text format.

The script file build_data.sh shows how the fake data were generated.


Additional features
===================

Support for Lockless PKO operation (Dropped in SDK-2.1 for CN68XX.)
----------------------------------
Lockless PKO operation by default is not enabled. To enable, uncomment 

	// #define ENABLE_LOCKLESS_PKO

towards the beginning of passthrough.c. And define the following
macros properly in config/executive-config.h (As the values for the
macros are chip-specific and please see the comments on lockless PKO
in the file for details.):

	CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE0
	CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE1
	CVMX_HELPER_PKO_MAX_PORTS_INTERFACE0
	CVMX_HELPER_PKO_MAX_PORTS_INTERFACE1

Lockless PKO builds on top of the command queue PKO infrastructure
provided by the Simple Executive library. For each transmitting port,
software maps a dedicated PKO command queue for each CPU core, thus
allowing PKO operation to be lockless. 

There are some limitations of lockless PKO operation:

1. Lockless PKO operation does not preserve packet ordering. Unlike
   using atomic tag switching and single output queue, Once packets
   are placed in multiple queues, PKO will transmit them irrspect of
   their receiving order.

2. Lockless PKO operation requires each transmitting port to have
   equal or more queues than the running CPU cores.

3. Lockless PKO operation requires additional memory space for the
   extra command queues.

On the other hand, Lockless PKO operation is more efficient since no
locking is necessary for each CPU core to access the command queue.
Lockless PKO operation is recommended for performance critical
networking applications that are not affected by limitations listed
above.
