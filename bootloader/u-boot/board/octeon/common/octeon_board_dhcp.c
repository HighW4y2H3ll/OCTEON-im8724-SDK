/*
 * (C) Copyright 2012 Cavium, Inc. <support@cavium.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <net.h>

#ifdef CONFIG_OCTEON_NET_OUR_BOOT_COMMAND
char NetOurBootCommand[256] = { '\0', };
#endif

#ifdef CONFIG_BOOTP_VENDOREX_OPTIONS
/**
 * This function prepares vendor defined options.  It is marked as weak and
 * provides an example function to do this.
 * 
 * @param ppopt - pointer to a pointer to the DHCP option data
 * 
 * @return number of options added
 */
uint8_t dhcp_vendorex_prep_options(uint8_t **ppopt)
	__attribute__((weak, alias("__dhcp_vendorex_prep_options")));

uint8_t __dhcp_vendorex_prep_options(uint8_t **ppopt)
{
	uint8_t cnt = 0;
#ifdef CONFIG_OCTEON_NET_OUR_BOOT_COMMAND
	**ppopt = 224;
	(*ppopt)++;
	cnt++;
#endif
	return cnt;
}
#endif

/**
 * If this returns anything other than NULL it must replace most of the
 * functionality of DhcpExtended.  In the full U-Boot source code this
 * can be seen in the Cray L1 board code.  It is not recommended.
 */
uint8_t *dhcp_vendorex_prep(uint8_t *e) __attribute__((weak));

uint8_t *dhcp_vendorex_prep(uint8_t *e)
{
	return NULL;
}

/**
 * Process vendor extensions.
 * 
 * @param e - pointer to vendor DHCP option data
 * 
 * @return pointer to next DHCP option or NULL if none are recognized.
 * 
 * All extensions not recognized by U-Boot will result in this function
 * being called.
 * 
 * Currently option 224 can specify a custom boot command which is placed
 * in the environment variable "NetOurBootCommand" which is passed from 
 * the DHCP server.
 */
uint8_t *dhcp_vendorex_proc(uint8_t *e)
	__attribute__((weak, alias("__dhcp_vendorex_proc")));

uint8_t *__dhcp_vendorex_proc(uint8_t *e)
{
	uint8_t size = 0;
#ifdef CONFIG_OCTEON_NET_OUR_BOOT_COMMAND
	if (*e == 224) {
		size = *(e + 1);
		memcpy(NetOurBootCommand, e + 2, size);
		NetOurBootCommand[size] = '\0';
		setenv("NetOurBootCommand", NetOurBootCommand);
		return (e + 2 + size);
	}
#endif
	return NULL;
}
