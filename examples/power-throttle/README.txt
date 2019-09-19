The example tries to demonstrate how to use the power-throttle API.

SYNOPSIS:
    power-throttle [tgt_ppid=core_id [tgt_pct=percentage]]

DESCRIPTION:

    When invoked with no parameters, power-throttle first calls
    cvmx_power_throttle_self() to lower the power consumption limit to
    20% for the core the thread is running on. Next, it calls
    pth_power_throttle() to further lower the limit to 10%. Fields of
    the PowerThrottle COP0 register are displayed before and after the
    changes.

    tgt_ppid
	    This is the id of the core whose PowerThrottle register is
	    to be displayed or changed, where
	        core_id >= 0 && core_id < CVMX_MAX_CORES

	    When this is the only parameter, power-throttle displays
	    the PowerThrottle register's content.

    tgt_pct
	    This is the percentage (0 <= percentage <= 100) to which
	    the target core's power limit is to be adjusted to.

	    For CN63XX, the result is
		    POWLIM = (percentage * MAXPOW)
            For other OcteonIIs,
		    POWLIM = (percentage * (MAXPOW - HRMPOWADJ))
