Running and controlling bare-metal SE application from Linux
-----------------------------------------------------------------------------
$Id: $

NOTE:
This utility replaced the "bootoct" and "oct-shutdown-app" utilities of
the previous releases of the SDK.

The utility supports 5 basic operations which are described below. The specific operation may be either specified in the first argument or by using a link to
the utility executable specifically identifiyng the desired operation.

STARTING APPS

    - # oct-app-ctl boot <options> <app_file_name> [<app_arguments>]
    - # oct-app-boot <options> <app_file_name> [<app_arguments>]

    <options> are optional, and include these options:

    -verbose=<num> 
	Increase verbosity level of the program up to the value of 3.
	Default is 0.
    -numcores=<N>
	The number of CPU cores to start the application on. Default is 1.
    -mask=<hex-number>
	The coremask of CPUs that the application should be started on.
    -heapsize=<bytes>
	Set the size of the heap memory allocated per CPU core.
	Default is 3 Megabytes.
    -stacksize=<bytes>
	Set the size of the stack memory allocated per CPU cure.
	Default is 1 Megabytes.

NOTES:
Any SE application can be started this way, whether it supports
hot-plugging or not, but the remaining operations require the support of
hot-plugging in the application.  The core-mask argument only supports up
to 64 cores, and should be abandoned in favour of the -numcores argument.
If the core-mask argument is specified is contains cores that are not
presently available, the unavailable cores will be replaced with the
same number of available cores to at least satisfy the core count in
the requested mask, if possible.

DISPLAYING STATUS

    -# oct-app-ctl info
    -# oct-app-info

The information displayed indicates which SE applications are currently
running on which cores. It does not specify which cores are being
currently used by Linux itself. In order to control and display the
CPU cores running Linux please refer to the Linux CPU hot-plugging
documentation.

The "Active" core mask indicates which applications are capable of
supporting hot-plugging, which is required for the following operations.

The first column displays an index for every instance of an application,
which is useful for performing any of the remaining operations on
a specific instance of an applications, should there be multiple
applications started from the same executable file and thus having the
same name.

The last item displayed by this operation is the coremask of the CPU
coremask that are presently not utilized, and are thus available for
starting new applications, or adding into existing ones.

SHUTTING DOWN APPLICATIONS

    -# oct-app-ctl shutdown <options> {-index=<app-index> | <app-name>}
    -# oct-app-shutdown <options> {-index=<app-index> | <app-name>}

    <options> are optional, and include these options:

    -verbose=<num>
	Increase verbosity level of the program up to the value of 3.
	Default is 0.

This operation will shut down a running application on all the CPU cores
it runs.

The above operation as well as all the subsequently described operations
only work if the application supports hot-plugging, and has installed
call-back routines to be notified of the request of relinquishing or add
resources. Please see the example program in "sdk/examples/hotplug_app"
for additional information.

The application effected must be identified either by its name, which is
identical to the name of the executable object file that was used to start
the application, or by means of the application instance index number.

Note that the executable file needs not be present in the current
(or any other) directory for this operation to succeed, as it is not
accessed for any operation other than the "boot" operation.

ADDING CORES TO AN APPLICATIONS

    - # oct-app-ctl add <options> {-index= <app-index> | <app-name>}
    - # oct-app-add <options> {-index= <app-index> | <app-name>}

    <options> are optional, and include these options:

    -verbose=<num>
	Increase verbosity level of the program up to the value of 3.
	Default is 0.
    -numcores=<N>
	The number of CPU cores to start the application on. Default is 1.
    -mask=<hex-number>
	The coremask of CPUs that the application should be started on.

NOTES:
The core-mask argument only supports up to 64 cores, and should be abandoned
in favour of the -numcores argument.

If the core-mask argument is specified is contains cores that are not
presently available, the unavailable cores will be replaced with the
same number of available cores to at least satisfy the core count in
the requested mask, if possible.

DELETING CORES FROM AN APPLICATIONS

    -# oct-app-ctl del <options> {-index= <app-index> | <app-name>}
    -# oct-app-del <options> {-index= <app-index> | <app-name>}

    <options> are optional, and include these options:

    -verbose=<num>
	Increase verbosity level of the program up to the value of 3.
	Default is 0.
    -numcores=<N>
	The number of CPU cores to relinquish. Default is 1.
    -mask=<hex-number>
	The coremask of CPUs that should be stopped.

NOTES:
this operation will refuse to delete the initial core running
the application, which is the lowest-numbered core that the application
was initially started on. All other cores, which could be numerically
lower than the initial core of these where added subsequently to starting
the application with the aforementioned "add" operation, can be deleted.

If a core-mask is specified in the command line, but it includes some
cores that are not running said application instance, or if the mask
specifies the initial core, the actual mask will be modified to stop
the same number of cores as requested in the mask, if possible.

LIMITATIONS:

This utility presently does not support bare-metal applications which
have been started from the bootloader, it can not add, remove cores to
such applications or shut them down, and it is therefore recommended
that if Linux is being deployed, the bootloader should be only used to
start the kernel, and any further control over bare-metal SE applications
should be then performed from Linux.

Also, the utility has only been tested with big-endian executables
host, and does not presently support little-endian variants
of either.

USE CASES

There are many use cases for controlling stand-alone SE applications
from Linux. In the most simple case, it allows packaging the entire
application, including control and data plane in a single embedded
root file system, or enabling a "live" upgrade of the data-plane
portion wuthout rebooting the control plane.

Other use cases include the dynamic load balancing where the number
of CPU cores can be shiften between data-plane SE application and
Linux to match changing load patterns dynamically. In such cases,
the dynamic load-control supervisor (not provided) would employ the
Linux CPU hot-plugging mechanism to make CPU cores available from
Linux and added to SE applications and vice versa.

