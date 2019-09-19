
Simple Executive: passthrough_o3 sample application
-------------------------------------------------------------------

This is an example application on the basic use of Simple Executive
packet processing API designed uniquely for the CN78XXX Octeon-III
SoC.

It is similar to $OCTEON_ROOT/examples/passthrough with the following
differences:

This example application will only run on the CN78XX and CN76XX models,
and can not run on older models, and is thus not portable.
The use of CN78XX-specific API calls is meant to achceive the fastest
throughput possible for the model.
The generic "passthrough" example application in contrast uses the
generic packet processing API calls, and is thus portable across all
currently supported Octeon models (including the CN78XX), but does not
perform as fast as this example application, thereby trading off some
performance for portability.

The "lockless" configuration option that is present in the generic
(and portable) "passthrough" example application, is not available
in this example application.

Please refer to $OCTEON_ROOT/examples/passthrough/README.txt for further
details on the use of this example application

Packet Ordering
---------------

This example applications supports the maintenance of packet order.
Packet ordering is marginally slower than unordered packet processing,
and for that reason, it is disabled by default.
Packet ordering can be enabled by adding the following argument to the
invokation, e.g.

  make run packet_order=no

When packet order is not preserved, the packet stimulae generated
for running this example in the simulator contain identical packet
so that the output will match input even if packets are forwarded
out of order. With packet odering however, the stimulae is made
of several different packets, such that comparing the output
packet data to input verifies that indeed packet order was maintained.

Packet ordering is disabled by default (see Makefile for details).

