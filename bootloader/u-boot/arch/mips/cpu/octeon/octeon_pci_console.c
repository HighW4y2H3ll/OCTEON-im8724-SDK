/***********************license start************************************
 * Copyright (c) 2003-2011 Cavium, Inc. (support@cavium.com). All rights
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
#include <stdio_dev.h>
#include <watchdog.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon-pci-console.h>
#include <asm/arch/octeon_mem_map.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_board_common.h>

DECLARE_GLOBAL_DATA_PTR;

uint64_t pci_cons_desc_addr;
static struct stdio_dev pci_console_dev;
static int octeon_pci_console_initialized = 0;
#ifdef CONFIG_OCTEON_BOOTCMD
static struct stdio_dev pci_bootcmd_dev;
#endif
int octeon_pci_console_start(void);
int octeon_pci_console_stop(void);
void octeon_pci_console_putc(const char c);
void octeon_pci_console_puts(const char *s);
int octeon_pci_console_tstc(void);
int octeon_pci_console_getc(void);

#ifdef CONFIG_OCTEON_BOOTCMD
int octeon_pci_bootcmd_start(void);
int octeon_pci_bootcmd_stop(void);
void octeon_pci_bootcmd_putc(const char c);
void octeon_pci_bootcmd_puts(const char *s);
int octeon_pci_bootcmd_tstc(void);
int octeon_pci_bootcmd_getc(void);
#endif

int uboot_octeon_pci_console_init(void)
{
	debug("Registering PCI console\n");
	memset(&pci_console_dev, 0, sizeof(pci_console_dev));
	strcpy(pci_console_dev.name, "pci");
	pci_console_dev.flags = DEV_FLAGS_OUTPUT | DEV_FLAGS_INPUT | DEV_FLAGS_SYSTEM;
	pci_console_dev.start = octeon_pci_console_start;
	pci_console_dev.putc = octeon_pci_console_putc;
	pci_console_dev.puts = octeon_pci_console_puts;
	pci_console_dev.getc = octeon_pci_console_getc;
	pci_console_dev.tstc = octeon_pci_console_tstc;
	if (stdio_register(&pci_console_dev)) {
		puts("WARNING: Could not register PCI console I/O\n");
	}

#ifdef CONFIG_OCTEON_BOOTCMD
	debug("Registering bootcmd console\n");
	memset(&pci_bootcmd_dev, 0, sizeof(pci_bootcmd_dev));
	strcpy(pci_bootcmd_dev.name, "bootcmd");
	pci_bootcmd_dev.flags = DEV_FLAGS_INPUT;
	pci_bootcmd_dev.start = octeon_pci_bootcmd_start;
	pci_bootcmd_dev.getc = octeon_pci_bootcmd_getc;
	pci_bootcmd_dev.tstc = octeon_pci_bootcmd_tstc;
	if (stdio_register(&pci_bootcmd_dev)) {
		puts("WARNING: Could not register PCI bootcmd input\n");
		return -1;
	}
#endif
	return 0;
}

int octeon_pci_console_start(void)
{
	int console_count;
	int console_size;
	char *ep;

	if (octeon_pci_console_initialized) {
		debug("%s: PCI console already initialized\n", __func__);
		return 0;
	}
	octeon_pci_console_initialized = 1;

	ep = getenv("pci_console_count");
	if (ep)
		console_count = simple_strtoul(ep, NULL, 10);
	else
		console_count = 1;
	if (console_count < 1)
		console_count = 1;

	ep = getenv("pci_console_size");
	if (ep)
		console_size = simple_strtoul(ep, NULL, 10);
	else
		console_size = 1024;
	if (console_size < 1024)
		console_size = 1024;

	debug("Initializing PCI console with count %d, size %d\n",
	      console_count, console_size);
	pci_cons_desc_addr =
		octeon_pci_console_init(console_count, console_size);
	if (!pci_cons_desc_addr) {
		printf("WARNING: PCI console init FAILED\n");
		return -1;
	}
	printf("PCI console init succeeded, %d consoles, %d bytes each\n",
	       console_count, console_size);
	return 0;
}

void octeon_pci_console_putc(const char c)
{
	static const char rn[] = "\r\n";
	if (pci_cons_desc_addr) {
		if (c == '\n') {
			octeon_pci_console_write(pci_cons_desc_addr,
						 0, (char *)&rn, 2, 0);
			WATCHDOG_RESET();
		} else
			octeon_pci_console_write(pci_cons_desc_addr, 0,
						 (char *)&c, 1, 0);
	}
}

void octeon_pci_console_puts(const char *s)
{
	while (*s)
		octeon_pci_console_putc(*s++);
	OCTEON_SYNC;
}

int octeon_pci_console_tstc(void)
{
	if (!pci_cons_desc_addr) {
		debug("%s: pci_cons_desc_addr not initialized\n", __func__);
		return 0;
	}
	octeon_board_poll();
	udelay(100);
	return (0 < octeon_pci_console_read_avail(pci_cons_desc_addr, 0));
}

int octeon_pci_console_getc(void)
{
	char c;

	if (!pci_cons_desc_addr)
		return 0;

	octeon_pci_console_read(pci_cons_desc_addr, 0, &c, 1, 0);
	return c;
}

/**********************************************************************/
#ifdef CONFIG_OCTEON_BOOTCMD
static int octeon_pci_bootcmd_started = 0;
static int octeon_pci_bootcmd_copy_offset = 0;
static int octeon_pci_bootcmd_eol = 0;
static int octeon_pci_bootcmd_unlocked = 0;
octeon_pci_io_buf_t *octeon_pci_read_buf_ptr =
	(void *)MAKE_KSEG0(BOOTLOADER_PCI_READ_BUFFER_BASE);

int octeon_pci_bootcmd_start(void)
{
	if (octeon_pci_bootcmd_started) {
		debug("%s: already started.\n", __func__);
	}
	octeon_pci_bootcmd_copy_offset = 0;
	octeon_pci_bootcmd_eol = 0;
	octeon_pci_read_buf_ptr->len = 0;
	octeon_pci_read_buf_ptr->data[0] = '\0';
	/* When the bootcmd console is first started it is started as locked to
	 * block any calls sending a command until U-Boot is ready to accept
	 * commands.  Just before the main loop starts to accept commands the
	 * bootcmd console is unlocked.
	 */
	if (octeon_pci_bootcmd_unlocked)
		octeon_pci_read_buf_ptr->owner = OCTEON_PCI_IO_BUF_OWNER_HOST;
	else
		octeon_pci_read_buf_ptr->owner = OCTEON_PCI_IO_BUF_OWNER_OCTEON;
	debug("%s called, buffer ptr: 0x%p, owner: %s\n", __func__,
	      octeon_pci_read_buf_ptr,
	      octeon_pci_read_buf_ptr->owner == OCTEON_PCI_IO_BUF_OWNER_HOST ?
	      "host" : "octeon");
	debug("&octeon_pci_bootcmd_copy_offset: 0x%p\n",
	      &octeon_pci_bootcmd_copy_offset);
	octeon_pci_bootcmd_started = 1;
	OCTEON_SYNC;
	return 0;
}

int octeon_pci_bootcmd_unlock(void)
{
	int timeout = 10000;	/* 10 seconds */
	octeon_pci_bootcmd_unlocked = 1;

	if (!octeon_pci_bootcmd_started) {
		printf("Error: Called %s before octeon_pci_bootcmd_start()\n",
		       __func__);
		return -1;
	}
	while (octeon_pci_read_buf_ptr->owner != OCTEON_PCI_IO_BUF_OWNER_OCTEON
	       && timeout > 0) {
		OCTEON_SYNC;
		mdelay(1);
		timeout--;
	}
	if (!timeout)
		puts("Error unlocking PCI bootcmd console: timeout waiting for host\n");

	octeon_pci_read_buf_ptr->owner = OCTEON_PCI_IO_BUF_OWNER_HOST;
	OCTEON_SYNC;
	return 0;
}

int octeon_pci_bootcmd_tstc(void)
{
	if (!octeon_pci_bootcmd_started) {
		debug("%s: octeon_pci_bootcmd_start() not called\n", __func__);
		return 0;
	}
	if (octeon_pci_bootcmd_eol)
		return TRUE;

	OCTEON_SYNC;
	if (octeon_pci_read_buf_ptr->owner != OCTEON_PCI_IO_BUF_OWNER_OCTEON)
		return 0;

	if ((octeon_pci_read_buf_ptr->len > octeon_pci_bootcmd_copy_offset) &&
	    (octeon_pci_read_buf_ptr->data[octeon_pci_bootcmd_copy_offset] != '\0'))
		return TRUE;
	return 0;
}

int octeon_pci_bootcmd_getc(void)
{
	char c;

	if (!octeon_pci_bootcmd_started) {
		debug("%s: octeon_pci_bootcmd_start() not called\n", __func__);
		return -1;
	}

	/* There's no EOL for boot commands so we fake it. */
	if (octeon_pci_bootcmd_eol) {
		octeon_pci_bootcmd_eol = 0;
		return '\n';
	}

	while (!octeon_pci_bootcmd_tstc()) {
		WATCHDOG_RESET();
		octeon_board_poll();
		udelay(100);
	}
	c = octeon_pci_read_buf_ptr->data[octeon_pci_bootcmd_copy_offset];
	octeon_pci_read_buf_ptr->data[octeon_pci_bootcmd_copy_offset++] = '\0';

	if ((octeon_pci_bootcmd_copy_offset >=
		MIN(CONFIG_SYS_CBSIZE - 1,
		    BOOTLOADER_PCI_READ_BUFFER_STR_LEN - 1))
	    || (octeon_pci_read_buf_ptr->data[octeon_pci_bootcmd_copy_offset] == '\0')) {
		octeon_pci_bootcmd_copy_offset = 0;
		octeon_pci_read_buf_ptr->len = 0;
		octeon_pci_read_buf_ptr->owner = OCTEON_PCI_IO_BUF_OWNER_HOST;
		octeon_pci_bootcmd_eol = 1;
		OCTEON_SYNC;
	}
	return c;
}
#endif /* CONFIG_OCTEON_BOOTCMD */
