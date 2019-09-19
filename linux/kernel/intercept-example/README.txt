
Low Level Packet Intercept Interface
------------------------------------

The Octeon Linux ethernet driver supports a custom interface for intercepting
packets at a low level before they are passed to the Linux kernel. This
interface is designed to allow custom loadable modules to intercept incoming
ethernet packets with a minimal amount of overhead. Loadable modules can use
this interface to perform packet processing at much higher rates than the
normal Linux networking stack. This interface has been released under a BSD
style license to allow customer proprietary drivers to use it.

The Interface
-------------

The Intercept interface is defined and documented in the cavium-ethernet.h
header file under the Octeon ethernet driver. The functions are defined as
weak symbols that are dynamically linked with the exported symbols of the
ethernet driver.

The exported functions are:

struct net_device *cvm_oct_register_callback(
	const char *device_name,
	cvm_oct_callback_t callback);

	Registers a intercept callback for the names ethernet
	device. It returns the Linux device structure for the
	ethernet port. Using a callback of NULL will remove
	the callback. Note that this callback must not disturb
	scratch. It will be called with SYNCIOBDMAs in progress
	and userspace may be using scratch. It also must not
	disturb the group mask.

	device_name	Device name to register for. (Example: "eth0")
	callback 	Intercept callback to set.
	return 		Device structure for the ethernet port or NULL on failure.

int cvm_oct_free_work(void *work_queue_entry);

	Free a work queue entry received in a intercept callback.
	
	work_queue_entry        Work queue entry to free
	return 			Zero on success, Negative on failure.

int cvm_oct_transmit(
	struct net_device *dev,
	void *work_queue_entry,
	int do_free);

	Transmit a work queue entry out of the ethernet port. Both
	the work queue entry and the packet data can optionally be
	freed. The work will be freed on error as well.
	
	dev     		Device to transmit out.
	work_queue_entry	Work queue entry to send
	do_free 		True if the work queue entry and packet data
				should be freed. If false, neither will be freed.
	return 			Zero on success, negative on failure.

typedef cvm_oct_callback_result_t cvm_oct_callback_t(
	struct net_device *dev,
	void *work_queue_entry,
	struct sk_buff *skb);

	The is the definition of the Ethernet driver intercept
	callback. The callback receives three parameters and
	returns a cvm_oct_callback_result_t code.
	
	dev			The first parameter is the Linux device for
				the ethernet port the packet came in on.
	work_queue_entry	The second parameter is the raw work queue
				entry from the hardware.
	skb			The third parameter is the packet converted
				into a Linux skbuff.

	These enumerations are the return codes for the Ethernet
	driver intercept callback. Depending on the return code,
	the ethernet driver will continue processing in different
	ways.
	
	CVM_OCT_PASS		The ethernet driver will pass the packet
				to the kernel, just as if the intercept
				callback didn't exist.
	CVM_OCT_DROP		The ethernet driver will drop the packet,
				cleaning of the work queue entry and the
				skbuff.
	CVM_OCT_TAKE_OWNERSHIP_WORK
				The intercept callback takes over ownership
				of the work queue entry. It is the
				responsibility of the callback to free the
				work queue entry and all associated packet
				buffers. The ethernet driver will dispose of
				the skbuff without affecting the work queue
				entry.
	CVM_OCT_TAKE_OWNERSHIP_SKB
				The intercept callback takes over ownership of
				the skbuff. The work queue entry and packet
				buffer will be disposed of in a way keeping
				the skbuff valid.

The Example
-----------

The module in this directory, intercept-example, uses ethernet interface to
filter out broadcast IP packets. Broadcast IP packets less than 256 bytes are
echoed out of the eth1. Broadcast IP packets greater than or equal to 256
bytes are silently dropped.

