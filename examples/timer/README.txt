
Shared-Ring Timer API Example and Functional Test Application

The program contained in this directory is intended to demonstrate
and functionaly test the new Shared-Ring (a.k.a. lockless) timer
API.

The API supports all SoC models, including CN78XX, and co-exists
with the legacy TIM API, where the number of TIM rings exceeds the
number of processor cores. The legacy TIM API avoids any kind of locking
by dedicating one hardware ring to one processing core, in a 1-to-1
scrict relationship. Since it is not known if the legacy API will be
invoked (initialized) first, or subsequently to the use of this new
API, the number of TIM rings equal to the number of processor cores
is reserved for the use of legacy API.

This API makes use of standard C11 "atomic" operations to implement
a method for protecting the shared ring resource in a way that
does not require exlusive access most of the time, and is therefore
scalable to multi-core applications sharing the timers.

All remaining TIM rings are available for this API, and each ring
is mapped to one "timer" object, that can be created and destroyed
at any time, and can be associated by the "timer_id" returned from
the creator function, or by the string name provided as argument
during creation, that can be converted to a "timer_id" handle at any time.

CAVEAT:
Beware when destroyong a timer instance that the destructor does not
check for completion of processing of all time-outs, and does not
verify that all work entries of expired time-outs have been processed.

Please see "cvmx-tim-atomic.h", and "cvmx-tim-atomic.c" in the "executive"
directory for complete description of the API.

To run the test program on a simulator, on a single core do:

	make run

or to run it on 4 coures do:

	make run4

