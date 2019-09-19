Readme for "low-latency-mem" example

This example uses low latency memory (LLM) to add consecutive numbers.  It 
uses three different replication levels (1x, 2x, and 8x) along with two 
memory access widths (36 bit and 64 bit).  

The basic idea is to write the integers 0 to 1023 into low latency memory, 
then read them back and sum them.  The result is compared with the correct 
value (1024 * 1023 / 2) and the time to perform the sum is displayed.  The 
numbers are written to LLM with multiple replication modes to show the 
affect replication has on processor memory contention.  

The example also shows different methods of accessing LLM for improved 
speed. See the source code for more information.  

Running
-------

Build and run the sample:

	$ make clean
	$ make
	$ make run

The number of processors running can be modified by changing NUM_PROCESSORS
in the make file. 

NOTE: This example is supported only on CN38XX, CN58XX, CN31XX chips, which has
LLM interface.
