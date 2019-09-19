Readme for "mailbox" example

This example shows how to use the mailbox interrupts
to pass messages between cores.  The low 16 bits
of each core's mailbox register are used, the
high 16 bits are available for other uses.

NOTE: This example does not run properly with the
'-noperf' simulator option

NOTE: This example requires multiple cores and therefore
is not supported on the CN30XX chips.
