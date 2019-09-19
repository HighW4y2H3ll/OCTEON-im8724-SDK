Readme for "zip" example

This example shows how to use the zip block for OCTEON.  It takes an 
uncompressed input file ('input') and a compressed input file ('input.gz') 
and uses the zip core in OCTEON to create the corresponding 'output' and 
'output.gz' files.  The Makefile creates 'input.gz' from the 'input' file.  
After the test has run, 'input' and 'output' are compared to make sure 
that they are identical.  Note that 'input.gz' and 'output.gz' will likely 
be different because they were compressed using different algorithms, 
although they are both valid deflate compressed representations of the 
original data.  The input file consistes of several web site front pages
concatenated together.

Requirements
------------ 

1) This test uses the gzip program, and expects it to be in the path.

2) The unix file difference utility (diff) is used to compare results. This 
should be standard on all linux distributions. 

Running
-------

Build and run the sample on the simulator:

	$ make clean
	$ make
	$ make run


To use a different input file (the executable itself is used by default) 
simply copy a different file to the name 'input' before running the 
example.  Both the compressed and uncompressed size of this file must be 
less than 15 megabytes.  This will use the simulator magic file IO to 
read/write the files at runtime.  

Build the sample for the hardware:

	$ make clean
	$ make COMPILED_INPUT=1


To use a different input file (the executable itself is used by default) simply
copy a different file to the name 'input' before building the example.  




Notes
------
This examples uses 'simulator magic' to do file IO to the Linux file system.  This is only
supported on the simulator, there is no file IO supported in the Simple Executive.

