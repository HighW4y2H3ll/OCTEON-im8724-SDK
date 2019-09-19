README for Cavium Inc. OCTEON SDK

Host System Requirements
========================

The host operating system used for development is Scientific Linux 6.4 on a
64-bit host. Other configurations may work, but have not been tested. Running
simulations of multiple cores requires a lot of RAM: a minimum of 512 MBytes is required, and 1 GByte+ is recommended.


Installing the SDK
======================

The SDK can be installed by invoking "rpm -i OCTEON-SDK-rel-build.i386.rpm". 
Where "rel" is the release number and "build" is the build number. The tools
are installed by default under /usr/local/Cavium_Networks/OCTEON-SDK 
directory. Since the tools are installed as root, it is recommended to copy
the OCTEON-SDK into your home directory. 


Environment setup for the OCTEON SDK
=========================================

The installation requires the following environment variable changes:

1) The OCTEON_ROOT environment variable must be set to contain the path to 
	the OCTEON-SDK directory.
2) The directory $OCTEON_ROOT/tools/bin contains toolchain binaries and must
        be added to the path.
3) The directory $OCTEON_ROOT/host/bin contains the simulator and other utilities
        and must be added to the path.
4) (optional) The OCTEON_CPPFLAGS_GLOBAL_ADD variable should be set to
        include the string "-DUSE_RUNTIME_MODEL_CHECKS=1" if runtime model
	checking is desired.  The contents of this variable are added to
	the C preprocessor flags for all files compiled.  Runtime model
	checking is the preferred method for distinguishing different OCTEON
	models.


A script called 'env-setup' is provided to automate this configuration.
(In previous releases there were multiple env-setup* scripts which have
been replaced with a single script.)

Since this script modifies the environment of the current shell, it
must be sourced rather than executed.  The env-setup script takes
one mandatory argument of the OCTEON model, an optional argument
that specifies runtime or compile time model checking. (Please refer to
simple executive HTML documentation for more information on model checking.)
The env-setup script must be run from the OCTEON_ROOT directory, as it sets
the OCTEON_ROOT environment variable to the directory that it is run from.
The available values for the OCTEON_MODEL argument are listed in the
file octeon-models.txt in the SDK root directory.  If the OCTEON_MODEL
string does not specify a pass (revision), the default pass for that
model will be used. (Note that the default pass for a given OCTEON model
may change in future SDK releases.)

The usage is as follows:

   Usage: source ./env-setup <OCTEON_MODEL> [--runtime-model] ...
    OCTEON_MODEL:     Model of OCTEON to build and simulate for.
    --runtime-model:  enables runtime model detection build option by setting environment variable.
		   use --noruntime-model to clear environment variable if desired.
    --verbose:        be verbose about what the script is doing
   
   The env-setup script must be invoked from the root directory of the SDK install,
   as it sets OCTEON_ROOT to be the directory that it is invoked in.


Usage examples:

  OCTEON-SDK$ source ./env-setup OCTEON_CN58XX

This will configure the SDK to build binaries for runtime model checking, and
will simulate an OCTEON_CN58XX OCTEON.  Runtime model checking is the preferred
method of distinguishing different OCTEON models, and the binaries built with this
configuration will run on all supported OCTEON and OCTEON Plus models.

  OCTEON-SDK$ source ./env-setup OCTEON_CN56XX --no-runtime-model

This will configure the SDK to build binaries for compile model checking, and
will simulate an OCTEON_CN56XX OCTEON.  Since compile time checking was specified,
the resulting binary will only run on CN56XX models.


  OCTEON-SDK$ source ./env-setup OCTEON_CN63XX

This will configure the SDK to build binaries with -march=octeon2 for runtime model
checking, and will simulate an OCTEON_CN63XX OCTEON. Runtime model checking is the
preferred method of distinguishing different OCTEON II models, and the binaries 
built with this configuration will run on all supported OCTEON II models. 
NOTE: The binary built with this configuration will not run on OCTEON and OCTEON Plus models. 

  OCTEON-SDK$ source ./env-setup OCTEON_CN63XX --no-runtime-model

This will configure the SDK to build binaries with -march=octeon2 for compile 
model checking, and will simulate an OCTEON_CN63XX OCTEON. Since compile time
checking was specified the resulting binary will only run on CN63XX models.


Verifying installation with simple test
=======================================
cd to the 'examples/hello' directory created from the above rpm installation.

To build and run it, type:

% make run

To run it on 4 cores, type:

% make run4


============================================
The 'oct-version' script in the host/bin directory reports the version
of the OCTEON-SDK that is currently in the user's path.


Please see the release notes file release-notes.txt for the change history 
and file inventory.
