
Readme for the crypto example under Linux


Building for linux:

The example can also be compiled to run under Linux on OCTEON. Use the 
following make command
	
	$ make OCTEON_TARGET=linux_64

to build n64 ABI executable and
                                                                                
        $ make OCTEON_TARGET=linux_n32
                                                                                
to build n32 ABI executable.

	
Running under linux:

This example is built and copied to the Linux filesystem by the
Makefile in the linux directory.  These instructions assume that
the crypto-linux_64 executable has already been built and added
to the default minimal_fs filesystem image.  IMPORTANT: You must rebuild 
the minimal filesystem image in order to have an updated copy of this 
executable included in the image.
To run this example under Linux, perform the following steps:

1) In the linux/kernel directory run the command:
  ./oct-linux -quiet -numcores=1
(The '-noperf' option may be used to disable performance 
modelling which results in much faster simulation.)

2) In another terminal window run the command:
   telnet localhost 2020

This will connect to the Linux console.

3) When the bash prompt is available in the telnet session, run the 
command:
   ./examples/crypto-linux_64
  
(Step 1 is automated with the 'run-linux' makefile target.)
