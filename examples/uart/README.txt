Readme for "uart" example

This is a modification of a standard textbook example.  Using the uart it 
prompts the user for their name and then says hello.  This process is 
repeated until the user enters quit.  The main goal here isn't a useful 
program, but to show basic uart input and output.  

NOTE: This example doesn't support OCTEON3 multinode configuration to
maintain the program code compatibility with the previous OCTEON models.

Getting Uart Output
-------------------
The OCTEON simulator uses a TCP/IP connection to simulate the uart. Any 
standard unix network utility can be used to communicate with the uart. Netcat
and telnet generally work well.

Running
-------

Build and run the sample:

	$ make clean
	$ make
	$ make run

In another terminal perform the following to get the uart output:

	$ telnet localhost 2020
	
Interrupts
----------
A second example is available demonstrating interrupts with the uart. It 
performs the same steps as the first uart example, but uses interrupts 
to reduce the amount of hardware polling. Run the second example with:

	$ make run_irq
	
---
