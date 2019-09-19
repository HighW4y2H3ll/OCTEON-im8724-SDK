/*
 * (C) Copyright 2004-2007, 2011
 * Cavium Inc.
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

/* Environment is in DRAM, and was placed there by an external entity.
 * This is only valid for "ram based" bootloader images which don't initialized
 * the dram controller.
 *
 * This is used in cases such as booting over JTAG or PCI.  This is currently
 * used by the Cavium Octeon series of processors via their oct-remote-boot
 * command line utility.
 */

#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <malloc.h>
#ifndef CONFIG_SYS_NO_FLASH
# include <mtd/cfi_flash.h>
#endif
#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

extern int default_environment_size;
env_t *env_ptr = NULL;

char *env_name_spec = "RAM";

/* Read from the 'special' ??? environment, before running from RAM. */
uchar env_get_char_spec(int index)
{
	return (*((uchar *) (gd->env_addr + index)));
}

/* Copy environment from special early boot location to normal location.
 * env_ptr has been moved by caller to a malloced buffer.
 */
void env_relocate_spec(void)
{
	debug("Importing environment at 0x%08x\n", CONFIG_ENV_ADDR);
	env_import((char *)CONFIG_ENV_ADDR, 1);
}

int saveenv(void)
{
	printf("saveenv not supported for environment in RAM.\n");
	return -1;
}

int env_import(const char *buf, int check)
{
	env_t *ep = (env_t *)buf;

	if (check) {
		uint32_t crc;

		memcpy(&crc, &ep->crc, sizeof(crc));

		if (crc32(0, ep->data, ENV_SIZE) != crc) {
			set_default_env("!bad CRC");
			return 0;
		}
	}

	set_default_env("Adding default environment to RAM\n");

	if (himport_r(&env_htab, (char *)ep->data, ENV_SIZE, '\0', H_NOCLEAR,
		0, NULL)) {
		gd->flags |= GD_FLG_ENV_READY;
		return 1;
	}

	error("Cannot import environment from RAM: errno = %d\n", errno);

	return 0;
}

/************************************************************************
 * Initialize Environment use
 *
 *
 */
int env_init(void)
{
	env_ptr = (void *)CONFIG_ENV_ADDR;
	uint32_t computed_crc32;

	debug("%s: Looking for environment at %p\n", __func__, env_ptr);
	computed_crc32 = crc32(0, env_ptr->data, ENV_SIZE);
	if (computed_crc32 == env_ptr->crc) {
		gd->env_addr = (ulong) & (env_ptr->data);
		gd->env_valid = 1;
		debug("Found valid environment at 0x%x\n", gd->env_addr);
	} else {
		debug("computed: 0x%08x, read: 0x%08x\n",
				     computed_crc32,
				     env_ptr->crc);
		debug("c_ptr: %p, size: 0x%x\n", env_ptr->data,
				     ENV_SIZE);
		gd->env_addr = (ulong) &default_environment[0];
		gd->env_valid = 0;
	}
	return 0;
}
