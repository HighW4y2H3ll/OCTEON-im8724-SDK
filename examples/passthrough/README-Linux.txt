Readme for passthrough example under Linux


Building for linux
==================

The example can also be compiled to run under Linux on OCTEON. Use the
the following make command 
	
	$ make OCTEON_TARGET=linux_64
                                                                                
to build n64 ABI executable and
                                                                                
        $ make OCTEON_TARGET=linux_n32
                                                                                
to build n32 ABI executable.

	
Running under linux
===================

This example is built and copied to the Linux filesystem by the
Makefile in the linux directory. These instructions assume that the
passthrough-linux_64 executable has already been built and added to
the default minimal_fs filesystem image. IMPORTANT: You must rebuild
the minimal filesystem image in order to have an updated copy of this
executable included in the image.

To run this example under Linux, perform the following steps:

1) In the examples/passthrough directory run the command:

	oct-packet-io -p 2000 -c 1 -o output-%d.data -i		\
		input-IPD_PORT0.data -i input-IPD_PORT1.data -i \
		input-IPD_PORT2.data -i input-IPD_PORT3.data

   where IPD_PORTX is X for pre-CN68XX models and the IPD port number
   of the X'th port on the simulated interface.

2) In the linux/kernel directory run the command:

	./oct-linux -quiet -numcores=4

   Change the numcores argument to the desired value, which is
   chip-specific. Also, the '-noperf' option may be used to disable
   performance modelling which results in much faster simulation.

3) In another terminal window run the command:

	telnet localhost 2020

   This will connect to the Linux console.

4) When the bash prompt is available in the telnet session, run the
   command:

	./examples/passthrough-linux_64
  
(Steps 1 and 2 are automated with the 'run-linux' makefile target.)
