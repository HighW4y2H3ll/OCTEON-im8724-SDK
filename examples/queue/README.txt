Readme for "queue" example

This example shows an implementation of a memory access efficient
single writer/single reader message queue.  No locking is required,
and the entire queue structure fits in one cache line.

This example also shows how to use cvmx_bootmem_alloc_named() to
allocate a shared memory block on all cores.

NOTE: This example requires multiple cores and therefore
is not supported on the CN30XX chips.

