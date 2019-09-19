/***********************license start************************************
 * Copyright (c) 2011 - 2012 Cavium Inc. (support@cavium.com). All rights
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
 * support@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#include <asm/arch/cvmx-bootloader.h>
#include <environment.h>
#include <asm/arch/octeon_boot.h>
#include <asm/io.h>
#include <watchdog.h>

#if !defined(CONFIG_SYS_NO_FLASH) && !defined(CONFIG_ENV_IS_IN_RAM) && \
    !defined(CONFIG_ENV_IS_IN_NAND) && !defined(CONFIG_ENV_IS_IN_SPI_FLASH) && \
    !defined(CONFIG_ENV_IS_IN_EMMC)

DECLARE_GLOBAL_DATA_PTR;

extern uchar environment[];
extern env_t *env_ptr;

#if defined(CONFIG_ENV_ADDR_REDUND)
static env_t *flash_addr = (env_t *)CONFIG_ENV_ADDR;
static env_t *flash_addr_new = (env_t *)CONFIG_ENV_ADDR_REDUND;

int env_init(void)
{
	env_t *local_flash_addr = flash_addr;
	env_t *local_flash_addr_new = flash_addr_new;
	int crc1_ok = 0, crc2_ok = 0;
	uint8_t flag1, flag2;
	ulong addr_default;
	ulong addr1, addr2;

	/* If we are doing a TLB boot from flash, then the flash mapping will
	 * be adjusted from the expected settings to allow a single TLB entry to be
	 * used. Here we need to fixup the environment address in flash so
	 * that we can read it.
	 * When the environment is copied to RAM, the flash mappings have been
	 * returned to normal, so no fixup is required.
	 * Fixup only needed when booting from flash
	 */
	if (!(gd->flags & GD_FLG_RAM_RESIDENT)) {
		cvmx_mio_boot_reg_cfgx_t __attribute__((unused)) reg_cfg;
		reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFG0);

		/* normal_base is what the 'expected' in the non-TLB case, and
		 * what the flash_addr is based on. It will always be >= the
		 * currently configured base address.
		 */
		uint32_t normal_base = ((CONFIG_SYS_FLASH_BASE >> 16) & 0x1fff);
		uint32_t offset = normal_base - reg_cfg.s.base;
		offset = offset << 16;
		local_flash_addr = (void *)((char *)flash_addr - offset);
		local_flash_addr_new = (void *)((char *)flash_addr_new - offset);
		debug("Local flash addr %p, normal_base=0x%x, offset=0x%x, flash_addr=0x%p\n",
		      local_flash_addr, normal_base, offset, flash_addr);
		debug("Local flash addr new %p, normal_base=0x%x, offset=0x%x, flash_addr_new=0x%p\n",
		      local_flash_addr_new, normal_base, offset, flash_addr_new);
		debug("CONFIG_SYS_FLASH_BASE=0x%x, CONFIG_ENV_ADDR=0x%x, CONIG_ENV_ADDR_REDUND=0x%x, gd->env_addr=0x%p\n",
		      CONFIG_SYS_FLASH_BASE, CONFIG_ENV_ADDR,
		      CONFIG_ENV_ADDR_REDUND, gd->env_addr);
	}

	flag1 = local_flash_addr->flags;
	flag2 = local_flash_addr_new->flags;

	addr_default = (ulong)&default_environment[0];
	addr1 = (ulong)&(local_flash_addr->data);
	addr2 = (ulong)&(local_flash_addr_new->data);

	crc1_ok = (crc32(0, local_flash_addr->data, ENV_SIZE) ==
		   local_flash_addr->crc);
	crc2_ok = (crc32(0, local_flash_addr_new->data, ENV_SIZE) ==
		   local_flash_addr_new->crc);

	if (crc1_ok && !crc2_ok) {
		gd->env_addr = addr1;
		gd->env_valid = 1;
	} else if (!crc1_ok && crc2_ok) {
		gd->env_addr = addr2;
		gd->env_valid = 1;
	} else if (!crc1_ok && !crc2_ok) {
		gd->env_addr = addr_default;
		gd->env_valid = 0;
	} else if (flag1 == ACTIVE_FLAG && flag2 == OBSOLETE_FLAG) {
		gd->env_addr = addr1;
		gd->env_valid = 1;
	} else if (flag1 == OBSOLETE_FLAG && flag2 == ACTIVE_FLAG) {
		gd->env_addr = addr2;
		gd->env_valid = 1;
	} else if (flag1 == flag2) {
		gd->env_addr = addr1;
		gd->env_valid = 2;
	} else if (flag1 == 0xFF) {
		gd->env_addr = addr1;
		gd->env_valid = 2;
	} else if (flag2 == 0xFF) {
		gd->env_addr = addr2;
		gd->env_valid = 2;
	}

	debug("%s: crc1_ok %u crc2_ok %u gd->env_addr 0x%p gv->env_valid %d\n",
	      __func__, crc1_ok, crc2_ok, gd->env_addr, gd->env_valid);

	return 0;
}
#else
int env_init(void)
{
	env_t *local_env_ptr = env_ptr;

	/* If we are doing a TLB boot from flash, then the flash mapping will
	 * be adjusted from the expected settings to allow a single TLB entry
	 * to be used.  Here we need to fixup the environment address in flash
	 * so that we can read it.
	 * When the environment is copied to RAM, the flash mappings have been
	 * returned to  normal, so no fixup is required.
	 * Fixup only needed when booting from flash
	 */
	if (!(gd->flags & GD_FLG_RAM_RESIDENT)) {
		cvmx_mio_boot_reg_cfgx_t __attribute__((unused)) reg_cfg;
		uint32_t normal_base;
		uint32_t offset;

		reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFG0);

		/* normal_base is what the 'expected' in the non-TLB case, and
		 * what the env_ptr is based on. It will always be >= the
		 * currently configured base address.
		 */
		normal_base = ((CONFIG_SYS_FLASH_BASE >> 16) & 0x1fff);
		offset = (normal_base - reg_cfg.s.base) << 16;

		local_env_ptr = (void *)((char *)env_ptr - offset);
		debug("Local environment ptr %p, normal_base=0x%x, offset=0x%x, env_ptr=0x%p\n",
		      local_env_ptr, normal_base, offset, env_ptr);
		debug("CONFIG_SYS_FLASH_BASE=0x%x, CONFIG_ENV_ADDR=0x%x, gd->env_addr=0x%lx\n",
		      CONFIG_SYS_FLASH_BASE, CONFIG_ENV_ADDR, gd->env_addr);
		debug("cs0 base: 0x%x\n", reg_cfg.s.base << 16);
	}

	if (crc32(0, local_env_ptr->data, ENV_SIZE) == local_env_ptr->crc
	    && !(gd->flags & GD_FLG_FAILSAFE_MODE)) {
		gd->env_addr  = (ulong)&(local_env_ptr->data);
		gd->env_valid = 1;
		debug("%s: Valid environment found at 0x%p, gd->env_addr now 0x%lx\n",
		      __func__, &(local_env_ptr->data), gd->env_addr);
	} else {
		gd->env_addr  = (ulong)&default_environment[0];
		gd->env_valid = 0;
		debug("%s: CRC mismatch, using default environment\n",
		      __func__);
	}

	return 0;
}
#endif
/**
 * Fix gd->env_addr after relocation
 */
void octeon_fix_env_reloc(void)
{
	gd->env_addr = (ulong)env_ptr + sizeof(uint32_t);
}
#else
/**
 * Fix gd->env_addr after relocation
 */
void octeon_fix_env_reloc(void)
{
}
#endif /* CONFIG_SYS_NO_FLASH */
