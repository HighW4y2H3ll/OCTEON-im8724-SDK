/***********************license start***************
 * Copyright (c) 2008-2013 Cavium, inc. (support@cavium.com).  All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * This Software, including technical data, may be subject to U.S.  export
 * control laws, including the U.S.  Export Administration Act and its
 * associated regulations, and may be subject to export or import regulations
 * in other countries.  You warrant that You will comply strictly in all
 * respects with all such regulations and acknowledge that you have the
 * responsibility to obtain licenses to export, re-export or import the
 * Software.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#include <asm/arch/octeon_boot.h>
#include <usb.h>
#include <asm/arch/cvmx-usb.h>
#include <exports.h>
#include <watchdog.h>
#include "ehci.h"

DECLARE_GLOBAL_DATA_PTR;

struct octeon_usb_dev {
	int index;
	int rootdev;
	int split_hub_id;		/**
					 * hub info for split transactions.
					 * hub_id of 0 means no splitting
					 */
	int split_hub_port;
	int initialized;
	int speed_hack_done;
};

struct octeon_usb_dev octeon_controller[USB_MAX_DEVICE];

static cvmx_usb_state_t *cvmx_usb_state_array;
static int oct_usb_index;
static int num_ports = 2;

#define PIPE_DEBUG_FLAGS    0	/* CVMX_USB_PIPE_FLAGS_DEBUG_TRANSFERS */

/* Keep the u-boot pipe value for each cvmx pipe structure opened */
#define PIPE_LIST_SIZE 32
unsigned int pipe_handle_list[USB_MAX_DEVICE][PIPE_LIST_SIZE];

static struct descriptor {
	struct usb_hub_descriptor hub;
	struct usb_device_descriptor device;
	struct usb_linux_config_descriptor config;
	struct usb_linux_interface_descriptor interface;
	struct usb_endpoint_descriptor endpoint;
}  __attribute__ ((packed)) descriptor = {
	{
		0x8,		/* bDescLength */
		0x29,		/* bDescriptorType: hub descriptor */
		1,		/* bNrPorts -- runtime modified */
		0x8,		/* wHubCharacteristics */
		1,		/* bPwrOn2PwrGood */
		0,		/* bHubCntrCurrent */
		{},		/* Device removable */
		{}		/* at most 7 ports! XXX */
	},
	{
		0x12,		/* bLength */
		1,		/* bDescriptorType: UDESC_DEVICE */
		cpu_to_le16(0x0200), /* bcdUSB: v2.0 */
		9,		/* bDeviceClass: UDCLASS_HUB */
		0,		/* bDeviceSubClass: UDSUBCLASS_HUB */
		1,		/* bDeviceProtocol: UDPROTO_HSHUBSTT */
		64,		/* bMaxPacketSize: 64 bytes */
		0x0000,		/* idVendor */
		0x0000,		/* idProduct */
		cpu_to_le16(0x0100), /* bcdDevice */
		1,		/* iManufacturer */
		2,		/* iProduct */
		0,		/* iSerialNumber */
		1		/* bNumConfigurations: 1 */
	},
	{
		0x9,
		2,		/* bDescriptorType: UDESC_CONFIG */
		cpu_to_le16(0x19),
		1,		/* bNumInterface */
		1,		/* bConfigurationValue */
		0,		/* iConfiguration */
		0x40,		/* bmAttributes: UC_SELF_POWER */
		0		/* bMaxPower */
	},
	{
		0x9,		/* bLength */
		4,		/* bDescriptorType: UDESC_INTERFACE */
		0,		/* bInterfaceNumber */
		0,		/* bAlternateSetting */
		1,		/* bNumEndpoints */
		9,		/* bInterfaceClass: UICLASS_HUB */
		0,		/* bInterfaceSubClass: UISUBCLASS_HUB */
		0,		/* bInterfaceProtocol: UIPROTO_HSHUBSTT */
		0		/* iInterface */
	},
	{
		0x7,		/* bLength */
		5,		/* bDescriptorType: UDESC_ENDPOINT */
		0x81,		/* bEndpointAddress:
				 * UE_DIR_IN | EHCI_INTR_ENDPT
				 */
		3,		/* bmAttributes: UE_INTERRUPT */
		cpu_to_le16(8),	/* wMaxPacketSize */
		255		/* bInterval */
	},
};

static int octeon_submit_root(struct usb_device *dev, unsigned long pipe,
			      void *buffer, int length, struct devrequest *req);

static int lookup_pipe_handle(int port, uint32_t uboot_pipe)
{
	int i;
	for (i = 0; i < PIPE_LIST_SIZE; i++) {
		if (uboot_pipe == pipe_handle_list[port][i])
			return i;
	}
	return -1;
}

int usb_lowlevel_init(int index, enum usb_init_type init, void **controller)
{
	cvmx_usb_status_t status;
	int64_t addr;
	struct octeon_usb_dev *octdev;

	debug("%s(%d, %p (%p))\n", __func__, index, controller, *controller);

	if (fdt_node_offset_by_compatible(gd->fdt_blob, 0,
					  "cavium,octeon-5750-usbn") < 0) {
		puts("USB not present.\n");
		return -1;
	}
	if (!cvmx_usb_get_num_ports()) {
		printf("ERROR: This device does not support USB\n");
		return -1;
	}

	oct_usb_index = index;

	num_ports = cvmx_usb_get_num_ports();
	if (index >= num_ports) {
		printf("USB port %d out of range\n", index);
		return -1;
	}
	octdev = &octeon_controller[index];
	if (octdev->initialized) {
		printf("USB port %d already initialized\n", index);
		return 0;
	}
	if (!cvmx_usb_state_array) {
		debug("Allocating USB state data for %d structures\n", num_ports);
		addr =
		    cvmx_bootmem_phy_named_block_alloc(num_ports * sizeof(cvmx_usb_state_t),
						       0, 0x7fffffff, 128,
						       "__tmp_bootloader_usb",
						       CVMX_BOOTMEM_FLAG_END_ALLOC);
		if (addr < 0) {
			printf("ERROR: Unable to allocate memory for USB "
			       "structures.... (1)\n");
			return -1;
		}
		cvmx_usb_state_array = cvmx_phys_to_ptr(addr);
		if (!cvmx_usb_state_array) {
			printf("ERROR: Unable to allocate memory for USB "
			       "structures.... (2)\n");
			return -1;
		}
	}
	debug("USB state array at 0x%p\n", cvmx_usb_state_array);

	printf("(port %d) ", index);

	status = cvmx_usb_initialize(&cvmx_usb_state_array[index], index,
				     CVMX_USB_INITIALIZE_FLAGS_CLOCK_AUTO
#ifdef DEBUG
				     | CVMX_USB_INITIALIZE_FLAGS_DEBUG_INFO
				     | CVMX_USB_INITIALIZE_FLAGS_DEBUG_INFO
				     | CVMX_USB_INITIALIZE_FLAGS_DEBUG_CALLS
				     | CVMX_USB_INITIALIZE_FLAGS_DEBUG_TRANSFERS
				     | CVMX_USB_INITIALIZE_FLAGS_DEBUG_CALLBACKS
#endif
		 );

	if (status != CVMX_USB_SUCCESS) {
		printf("ERROR: cvmx_usb_initialize failed, status: %d\n",
		       status);
		mdelay(2000);
		if (CVMX_USB_SUCCESS !=
		    cvmx_usb_shutdown(&cvmx_usb_state_array[index]))
			printf("#### ERROR: Unable to shutdown usb block\n");
			return -1;
	}

	debug("%s: cvmx_usb_initialize finished for port %d.\n",
	      __func__, index);
	mdelay(2000);

	octdev = &octeon_controller[index];
	memset(octdev, 0, sizeof(*octdev));

	status = cvmx_usb_enable(&cvmx_usb_state_array[index]);
	if (CVMX_USB_SUCCESS != status) {
		if (CVMX_USB_SUCCESS !=
		    cvmx_usb_shutdown(&cvmx_usb_state_array[index])) {
			printf("#### ERROR: Unable to shutdown usb block %d\n",
			       index);
			return -1;
		}
		debug("cvmx_usb_enable returned %d for port %d\n",
		      status, index);
		goto done;
	}

	octdev->initialized = 1;

done:
	octdev->index = index;
	octdev->speed_hack_done = 0;
	*controller = (void *)octdev;

	return 0;
}

int usb_close_all_pipes(int port)
{
	int retval = 0;
	int i;
	for (i = 0; i < PIPE_LIST_SIZE; i++) {
		if (pipe_handle_list[port][i]) {
			if (CVMX_USB_SUCCESS !=
				cvmx_usb_close_pipe(&cvmx_usb_state_array[port],
						    i)) {
				printf("#### ERROR: Unable to close pipe "
				       "handle: %d on port %d\n",
					i, port);
				retval = -1;
			}
			pipe_handle_list[port][i] = 0;
		}
	}

	return retval;
}

int usb_lowlevel_stop(int index)
{
	/* Go through all cached pipes and shut them down. */
	int retval = 0;
	struct octeon_usb_dev *octdev;

	octdev = &octeon_controller[index];

	if (!octdev->initialized || !cvmx_usb_state_array)
		return 0;

	debug("%s(%d)\n", __func__, index);
	retval = usb_close_all_pipes(index);

	if (CVMX_USB_SUCCESS !=
	    cvmx_usb_disable(&cvmx_usb_state_array[index])) {
		printf("#### ERROR: Unable to disable usb port\n");
		retval = -1;
	}
	if (CVMX_USB_SUCCESS !=
	    cvmx_usb_shutdown(&cvmx_usb_state_array[index])) {
		printf("#### ERROR: Unable to shutdown usb block\n");
		retval = -1;
	}
	cvmx_bootmem_free_named("__tmp_bootloader_usb");

	octdev = &octeon_controller[index];
	memset(octdev, 0, sizeof(*octdev));

	return retval;
}

static int callback_done = 0;

void control_msg_callback(cvmx_usb_state_t * state,
			  cvmx_usb_callback_t reason,
			  cvmx_usb_complete_t status,
			  int pipe_handle, int submit_handle,
			  int bytes_transferred, void *user_data)
{

	debug("####### %s: state: %p, reason: %d, status: %d, pipe handle: %d, "
	      "submit_handle: %d, bytes: %d, user: %p\n",
	      __func__, state, reason, status, pipe_handle, submit_handle,
	      bytes_transferred, user_data);

	struct usb_device *dev = user_data;

	if (CVMX_USB_CALLBACK_TRANSFER_COMPLETE == reason) {
		debug("Transfer complete on pipe handle %d, bytes trans: %d\n",
		      pipe_handle, bytes_transferred);

		dev->status &= ~USB_ST_NOT_PROC;/* Request has been processed */
		callback_done = 1;
		dev->act_len = bytes_transferred;
	}
}

int submit_bulk_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
		    int transfer_len)
{
	int pipe_handle;
	int device_addr = usb_pipedevice(pipe);
	int endpoint_num = usb_pipeendpoint(pipe);
	cvmx_usb_direction_t transfer_dir =
	    usb_pipeout(pipe) ? CVMX_USB_DIRECTION_OUT : CVMX_USB_DIRECTION_IN;
	cvmx_usb_transfer_t transfer_type = CVMX_USB_TRANSFER_BULK;
	int max_packet = usb_maxpacket(dev, pipe);
	int retval;
	int pipe_speed;
	struct octeon_usb_dev *octdev = (struct octeon_usb_dev *)dev->controller;
	int port = octdev->index;
	cvmx_usb_state_t *state;
	unsigned long ts, time;
	int timeout = USB_TIMEOUT_MS(pipe);

	if (!octdev || !cvmx_usb_state_array) {
		printf("%s: Error: controller not initialized\n", __func__);
		return -1;
	}

	if (!octdev->initialized) {
		debug("%s: Error: controller not initialized\n", __func__);
		return -1;
	}

	state = &cvmx_usb_state_array[port];

	debug("%s: dev: %p, port %d, pipe: 0x%08lx, buf: %p, len: %d\n", __func__,
	      dev, port, pipe, buffer, transfer_len);
	debug("Dev addr: %d, endp: %d, dir: %d, trans len: %d max: %d\n",
	      device_addr, endpoint_num, transfer_dir, transfer_len,
	      max_packet);

	if (!buffer || !cvmx_ptr_to_phys(buffer)) {
		printf("ERROR: Buffer NULL in submit_bulk_message, len: %d, "
		       "ptr: %p, addr: 0x%llx\n",
		       transfer_len, buffer, cvmx_ptr_to_phys(buffer));
	}

	if (dev->speed == USB_SPEED_HIGH)
		pipe_speed = CVMX_USB_SPEED_HIGH;
	else if (dev->speed == USB_SPEED_LOW)
		pipe_speed = CVMX_USB_SPEED_LOW;
	else
		pipe_speed = CVMX_USB_SPEED_FULL;

	debug("%s: pipe speed: %d\n", __func__, pipe_speed);
	if ((pipe_handle = lookup_pipe_handle(port, pipe)) < 0) {
		debug("%s: octdev: %p, split_hub_id: %d, split_hub_port: %d\n",
		      __func__, octdev, octdev->split_hub_id,
		      octdev->split_hub_port);
		pipe_handle =
		    cvmx_usb_open_pipe(state,
				       PIPE_DEBUG_FLAGS, device_addr,
				       endpoint_num, pipe_speed, max_packet,
				       transfer_type, transfer_dir, 0, 0,
				       octdev->split_hub_id,
				       octdev->split_hub_port);

		if (pipe_handle < 0) {
			printf("ERROR: Unable to open pipe!(%d)\n",
			       pipe_handle);
			return -1;
		}
		pipe_handle_list[port][pipe_handle] = pipe;
	}

	/* Now send message */
	retval = cvmx_usb_submit_bulk(state, pipe_handle,
				      cvmx_ptr_to_phys(buffer), transfer_len,
				      control_msg_callback, dev);
	if (retval < 0) {
		printf("#### ERROR: cvmx_usb_submit_bulk() failed: %d\n",
		       retval);
		return -1;
	}

	cvmx_usb_status_t status;
	callback_done = 0;
	status = cvmx_usb_poll(state);
	ts = get_timer(0);
	while ((time = get_timer(ts)) < timeout && !callback_done
	       && status == CVMX_USB_SUCCESS) {
		status = cvmx_usb_poll(state);
		WATCHDOG_RESET();
	}

	if (status < 0) {
		printf("#### ERROR: USB bulk polling returned error: %d!\n",
		       status);
		return -1;
	}
	if (time >= timeout) {
		printf("#### ERROR: USB bulk polling timed out!\n");
		return -1;
	}

	return 0;
}

int submit_control_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
		       int transfer_len, struct devrequest *setup)
{
	int pipe_handle;
	int device_addr = usb_pipedevice(pipe);
	int endpoint_num = usb_pipeendpoint(pipe);
	cvmx_usb_direction_t transfer_dir =
	    usb_pipeout(pipe) ? CVMX_USB_DIRECTION_OUT : CVMX_USB_DIRECTION_IN;
	cvmx_usb_transfer_t transfer_type = CVMX_USB_TRANSFER_CONTROL;
	int max_packet = usb_maxpacket(dev, pipe);
	int retval;
	int pipe_speed;
	struct octeon_usb_dev *octdev = (struct octeon_usb_dev *)dev->controller;
	cvmx_usb_state_t *state;
	unsigned long ts;
	int timeout = USB_TIMEOUT_MS(pipe);
	unsigned long time;
	int port;

	if (!octdev || !cvmx_usb_state_array) {
		printf("%s: Error: controller not initialized\n", __func__);
		return -1;
	}

	if (!octdev->initialized) {
		debug("%s: Error: controller not initialized\n", __func__);
		return -1;
	}

	port = octdev->index;
	state = &cvmx_usb_state_array[port];

	debug("%s: dev: %p, port %d, pipe: 0x%08lx, buf: %p, len: %d, devreq: %p\n",
	      __func__, dev, port, pipe, buffer, transfer_len, setup);
	debug("Dev addr: %d, endp: %d, dir: %d, trans len: %d max: %d\n",
	      device_addr, endpoint_num, transfer_dir, transfer_len,
	      max_packet);

	if (usb_pipetype(pipe) != PIPE_CONTROL) {
		debug("non-control pipe (type=%lu)\n", usb_pipetype(pipe));
		return -1;
	}

	if (usb_pipedevice(pipe) == octdev->rootdev) {
		/* Handle fake root device */
		debug("%s: root device\n", __func__);
		if (!octdev->rootdev)
			dev->speed = USB_SPEED_HIGH;
		return octeon_submit_root(dev, pipe, buffer, transfer_len,
					  setup);
	}
	if (!octdev->speed_hack_done) {
		/* Set the speed of the root hub port when we send the first
		 * control message.  This is all we need to do with with root
		 * hub, and avoid the complexity of doing a virtual root hub
		 * implementation.
		 */
		struct usb_device *root_dev = usb_get_dev_index(0);

		cvmx_usb_port_status_t port_status = cvmx_usb_get_status(state);

		if (port_status.port_speed == CVMX_USB_SPEED_HIGH)
			root_dev->speed = USB_SPEED_HIGH;
		else if (port_status.port_speed == CVMX_USB_SPEED_LOW)
			root_dev->speed = USB_SPEED_LOW;
		else
			root_dev->speed = USB_SPEED_FULL;
		octdev->speed_hack_done = 1;
	}

	if (dev->speed == USB_SPEED_HIGH)
		pipe_speed = CVMX_USB_SPEED_HIGH;
	else if (dev->speed == USB_SPEED_LOW)
		pipe_speed = CVMX_USB_SPEED_LOW;
	else
		pipe_speed = CVMX_USB_SPEED_FULL;

	/* Open a pipe to send this message with */
	if ((pipe_handle = lookup_pipe_handle(port, pipe)) < 0) {
		debug("%s: octdev: %p, split_hub_id: %d, split_hub_port: %d\n",
		      __func__, octdev, octdev->split_hub_id,
		      octdev->split_hub_port);
		pipe_handle =
		    cvmx_usb_open_pipe(state,
				       PIPE_DEBUG_FLAGS, device_addr,
				       endpoint_num, pipe_speed, max_packet,
				       transfer_type, transfer_dir, 0, 0,
				       octdev->split_hub_id,
				       octdev->split_hub_port);

		if (pipe_handle < 0) {
			printf("Unable to open pipe!\n");
			return -1;
		}
		/* Don't cache device 0 pipes */
		if (device_addr)
			pipe_handle_list[port][pipe_handle] = pipe;
	}

	/* Buffer is NULL for some transactions, handle this way to avoid
	 * ptr_to_phys warnings
	 */
	uint64_t buffer_address = 0;
	if (buffer)
		buffer_address = cvmx_ptr_to_phys(buffer);

	/* Now send message */
	retval = cvmx_usb_submit_control(state,
					 pipe_handle, cvmx_ptr_to_phys(setup),
					 buffer_address, transfer_len,
					 control_msg_callback, dev);
	if (retval < 0) {
		printf("%s: cvmx_usb_submit_control returned %d\n",
		       __func__, retval);
	}
	cvmx_usb_status_t status;
	callback_done = 0;
	ts = get_timer(0);
	status = cvmx_usb_poll(state);
	while ((time = get_timer(ts)) < timeout && !callback_done
		&& status == CVMX_USB_SUCCESS) {
		status = cvmx_usb_poll(state);
		WATCHDOG_RESET();
	}
	if (status < 0)
		printf("#### ERROR: USB control polling returned error: %d!\n",
		       status);
	if (time >= timeout)
		printf("#### ERROR: USB control polling timed out!\n");

	/* Close uncached pipes to device 0 */
	if (!device_addr) {
		/* Close the pipe */
		if (CVMX_USB_SUCCESS !=
		    cvmx_usb_close_pipe(state, pipe_handle))
			printf("#### ERROR: Unable to close pipe handle "
			       "(control): %d\n", pipe_handle);
	}

	return (retval < 0 || time > timeout || status < 0) ? -1 : 0;
}

int submit_int_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
		   int transfer_len, int interval)
{
	printf("%s: dev: %p, pipe: 0x%08lx, buf: %p, len: %d, interval: %d\n",
	       __FUNCTION__, dev, pipe, buffer, transfer_len, interval);
	printf("ERROR:  *****   Interrupt messages not supported *******\n");
	return -1;
}

static int octeon_submit_root(struct usb_device *dev, unsigned long pipe,
			      void *buffer, int length, struct devrequest *req)
{
	uint8_t tmpbuf[4];
	u16 typeReq;
	void *srcptr = NULL;
	int len, srclen;
	struct octeon_usb_dev *octdev = dev->controller;
	cvmx_usb_port_status_t port_status;
	cvmx_usb_state_t *state;
	int port = le16_to_cpu(req->index) & 0xff;

	if (!octdev || !cvmx_usb_state_array) {
		printf("%s: Error: controller not initialized\n", __func__);
		return -1;
	}

	if (!octdev->initialized) {
		debug("%s: Error: controller not initialized\n", __func__);
		return -1;
	}

	state = &cvmx_usb_state_array[octdev->index];

	srclen = 0;
	debug("%s: req=%u (%#x), type=%u (%#x), value=%u, index=%u\n", __func__,
	      req->request, req->request,
	      req->requesttype, req->requesttype,
	      le16_to_cpu(req->value), le16_to_cpu(req->value));

	typeReq = req->request | req->requesttype << 8;

	switch (typeReq) {
	case DeviceRequest | USB_REQ_GET_DESCRIPTOR:
		debug("DeviceRequest GET_DESCRIPTOR\n");
		switch (le16_to_cpu(req->value) >> 8) {
		case USB_DT_DEVICE:
			debug("USB_DT_DEVICE request\n");
			srcptr = &descriptor.device;
			srclen = descriptor.device.bLength;
			break;
		case USB_DT_CONFIG:
			debug("USB_DT_CONFIG config\n");
			srcptr = &descriptor.config;
			srclen = descriptor.config.bLength +
				 descriptor.interface.bLength +
				 descriptor.endpoint.bLength;
			break;
		case USB_DT_STRING:
			debug("USB_DT_STRING config\n");
			switch (le16_to_cpu(req->value) & 0xff) {
			case 0:	/* Language */
				srcptr = "\4\3\1\0";
				srclen = 4;
				break;
			case 1:	/* Vendor */
				srcptr = "\16\3u\0-\0b\0o\0o\0t\0";
				srclen = 14;
				break;
			case 2:	/* Product */
				srcptr = "\56\3O\0C\0T\0E\0O\0N\0 "
					 "\0H\0o\0s\0t\0 "
					 "\0C\0o\0n\0t\0r\0o\0l\0l\0e\0r\0";
				srclen = 48;
				break;
			default:
				debug("unknown value DT_STRING 0x%x\n",
				      le16_to_cpu(req->value));
				goto unknown;
			}
			break;
		default:
			debug("unknown value %x\n", le16_to_cpu(req->value));
			goto unknown;
		}
		break;
	case USB_REQ_GET_DESCRIPTOR | ((USB_DIR_IN | USB_RT_HUB) << 8):
		debug("USB_REQ_GET_DESCRIPTOR DIR: in, HUB\n");
		switch (le16_to_cpu(req->value) >> 8) {
		case USB_DT_HUB:
			debug("USB_DT_HUB config\n");
			srcptr = &descriptor.hub;
			srclen = descriptor.hub.bLength;
			break;
		default:
			debug("unknown value %x\n", le16_to_cpu(req->value));
			goto unknown;
		}
		break;
	case USB_REQ_SET_ADDRESS | (USB_RECIP_DEVICE << 8):
		debug("USB_REQ_SET_ADDRESS\n");
		octdev->rootdev = le16_to_cpu(req->value);
		break;
	case DeviceOutRequest | USB_REQ_SET_CONFIGURATION:
		debug("USB_REQ_SET_CONFIGURATION\n");
		break;
	case USB_REQ_GET_STATUS | ((USB_DIR_IN | USB_RT_HUB) << 8):
		debug("USB_REQ_GET_STATUS\n");
		tmpbuf[0] = 1;	/* USB_STATUS_SELFPOWERED */
		tmpbuf[1] = 0;
		srcptr = tmpbuf;
		srclen = 2;
		break;
	case USB_REQ_GET_STATUS | ((USB_RT_PORT | USB_DIR_IN) << 8):
		debug("USB_REQ_GET_STATUS RT_PORT, DIR IN\n");
		memset(tmpbuf, 0, 4);
		debug("state: 0x%p\n", state);
		port_status = cvmx_usb_get_status(state);
		if (port_status.connected)
			tmpbuf[0] |= USB_PORT_STAT_CONNECTION;
		if (port_status.port_enabled)
			tmpbuf[0] |= USB_PORT_STAT_ENABLE;
		if (port_status.port_over_current)
			tmpbuf[0] |= USB_PORT_STAT_OVERCURRENT;
		if (port_status.port_powered)
			tmpbuf[1] |= USB_PORT_STAT_POWER >> 8;
		if (port_status.connect_change)
			tmpbuf[2] |= USB_PORT_STAT_C_CONNECTION;
		if (port_status.port_enabled)
			tmpbuf[2] |= USB_PORT_STAT_C_ENABLE;
		switch (port_status.port_speed) {
		case 0:	/* High Speed */
			tmpbuf[1] |= USB_PORT_STAT_HIGH_SPEED >> 8;
			debug("  High Speed\n");
			break;
		case 1:	/* Full Speed */
			/*tmpbuf[1] |= USB_PORT_STAT_FULL_SPEED >> 8;*/
			debug("  Full Speed\n");
			break;
		case 2:
			tmpbuf[1] |= USB_PORT_STAT_LOW_SPEED >> 8;
			debug("  Low Speed\n");
			break;
		default:
			printf("Unknown port speed 0x%x\n",
			       port_status.port_speed);
			goto unknown;
		}
		debug("port status 0x%x, change: 0x%x\n",
		      tmpbuf[0] | tmpbuf[1] << 8, tmpbuf[2]);
		srcptr = tmpbuf;
		srclen = 4;
		break;
	case USB_REQ_SET_FEATURE | ((USB_DIR_OUT | USB_RT_PORT) << 8):
		debug("USB_REQ_SET_FEATURE RT PORT\n");
		port_status = cvmx_usb_get_status(state);
		switch (le16_to_cpu(req->value)) {
		case USB_PORT_FEAT_ENABLE:
			debug("enable\n");
			cvmx_usb_enable(state);
			break;
		case USB_PORT_FEAT_POWER:
			debug("power on\n");
			break;	/* No power control */
		case USB_PORT_FEAT_RESET:
			debug("reset\n");
			cvmx_usb_disable(state);
			if (cvmx_usb_enable(state))
				printf("Error resetting USB port %d\n", port);
			break;
		case USB_PORT_FEAT_TEST:
			printf("USB test mode not supported for port %d\n",
			       port - 1);
			break;
		default:
			debug("unknown feature %x\n", le16_to_cpu(req->value));
			break;
		}
		break;
	case USB_REQ_CLEAR_FEATURE | ((USB_DIR_OUT | USB_RT_PORT) << 8):
		debug("USB_REQ_CLEAR_FEATURE DIR OUT, RT PORT\n");
		switch (le16_to_cpu(req->value)) {
		case USB_PORT_FEAT_ENABLE:
			debug("enable (disable)\n");
			cvmx_usb_disable(state);
			break;
		case USB_PORT_FEAT_C_ENABLE:
			debug("c enable clear\n");
			cvmx_usb_set_status(state, cvmx_usb_get_status(state));
			cvmx_usb_enable(state);
			break;
		case USB_PORT_FEAT_POWER:
			debug("power off\n");
			break;
		case USB_PORT_FEAT_C_CONNECTION:
			debug("c connection\n");
			cvmx_usb_set_status(state, cvmx_usb_get_status(state));
			break;
		case USB_PORT_FEAT_OVER_CURRENT:
			debug("over current\n");
			cvmx_usb_set_status(state, cvmx_usb_get_status(state));
			break;
		case USB_PORT_FEAT_C_RESET:
			debug("c reset\n");
			break;
		default:
			debug("unknown feature %x\n", le16_to_cpu(req->value));
			goto unknown;
		}
		break;
	default:
		debug("Unknown request\n");
		goto unknown;
	}
	mdelay(1);
	len = min3(srclen, le16_to_cpu(req->length), length);
	if (srcptr != NULL && len > 0)
		memcpy(buffer, srcptr, len);
	else
		debug("Len is 0\n");

	dev->act_len = len;
	dev->status = 0;
	return 0;
unknown:
	debug("requesttype=%x, request=%x, value=%x, index=%x, length=%x\n",
	      req->requesttype, req->request, le16_to_cpu(req->value),
	      le16_to_cpu(req->index), le16_to_cpu(req->length));
	dev->act_len = 0;
	dev->status = USB_ST_STALLED;
	return -1;
}
