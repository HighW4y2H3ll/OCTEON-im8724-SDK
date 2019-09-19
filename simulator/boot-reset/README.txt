Readme file for the Octeon simulator boot_reset stub


Using the boot_reset exe to boot an OS
=====================================================================
The boot_reset file provides minimal initialization before branching
to a compiled in address where the operating system takes over.  To
build boot_reset, simply edit the boot_reset2.S file and change
the START_ADDR define to the start address of the OS, and type
'make boot_reset'.  This will build boot_reset with the appropriate start 
address.
To use boot_reset, type:
pp boot_reset YOUR_ELF_FILE

This will load boot_reset and YOUR_ELF_FILE, and execution will start at 
the reset vector handler provided in boot_reset.  Control will pass to the
start address compiled into boot_reset.
(In this case, the simualtor 'pp' should be invoked directly, not using 
oct-sim.)



