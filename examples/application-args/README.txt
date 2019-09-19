Readme for the Application arguments and multiple application examples

This example shows how to pass arguments to OCTEON executive applications.
This can be done in two ways:

1) with the -appargs option to oct-sim.  The string provided with the 
-appargs option will be passed to the application in argc/argv.  The 
command 'make run-args' demonstrates this method.

2) with arguments specified in the environment file passed to the 
bootloader.  All boot command arguments after the 'endbootargs' argument
are passed to the application in argc/argv.  The command 'make run-env2' 
demonstrates this method.

Multiple application images can be loaded on different sets of cores 
within an OCTEON.  This is demonstrated by the 'run-env-multi' make 
target.  This example shows two simple executive applications being run on 
two different sets of cores.  This same method is used for running Linux 
and simple executive applications on different sets of cores.

NOTE:  The multi application example is not supported on CN30XX chips
since they only have a single core.
