README for hello example

This is a very simple example used to test for correct
toolchain and simulator installation.  It shows two
ways of printing output to the console.

This example uses the simple executive build system, but
does not link against the simple executive library.  This example
does not support generating Linux binaries because the build system
requires linking with the simple executive library when building for
Linux.

Running
-------

Build and run the sample:

	$ make clean
	$ make
	$ make run
