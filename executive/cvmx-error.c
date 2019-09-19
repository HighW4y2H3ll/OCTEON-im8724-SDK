/***********************license start***************
 * Copyright (c) 2003-2017  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Interface to the Octeon extended error status.
 *
 * <hr>$Revision: 44252 $<hr>
 */
#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-error.h>
#include <asm/octeon/cvmx-error-custom.h>
#include <asm/octeon/cvmx-pcie.h>
#include <asm/octeon/cvmx-srio.h>
#include <asm/octeon/cvmx-ciu2-defs.h>
#include <asm/octeon/cvmx-dfm-defs.h>
#include <asm/octeon/cvmx-lmcx-defs.h>
#include <asm/octeon/cvmx-pexp-defs.h>
#else
#include "cvmx.h"
#include "cvmx-error.h"
#include "cvmx-error-custom.h"
#include "cvmx-l2c.h"
#include "cvmx-pcie.h"
#include "cvmx-srio.h"
#include "cvmx-interrupt.h"
#endif

#define MAX_TABLE_SIZE 1024	/* Max number of error status bits we can support */

/* Each entry in this array represents a status bit function or chain */
static CVMX_SHARED cvmx_error_info_t __cvmx_error_table[MAX_TABLE_SIZE];
static CVMX_SHARED int __cvmx_error_table_size = 0;
static CVMX_SHARED cvmx_error_flags_t __cvmx_error_flags;

#define REG_MATCH(h, reg_type, status_addr, status_mask) \
    ((h->reg_type == reg_type) && (h->status_addr == status_addr) && ((h->status_mask & status_mask) == h->status_mask))

/**
 * @INTERNAL
 * Read a status or enable register from the hardware
 *
 * @param reg_type Register type to read
 * @param addr     Address to read
 *
 * @return Result of the read
 */
static uint64_t __cvmx_error_read_hw(cvmx_error_register_t reg_type, uint64_t addr)
{
	switch (reg_type) {
	case __CVMX_ERROR_REGISTER_NONE:
		return 0;
	case CVMX_ERROR_REGISTER_IO64:
		return cvmx_read_csr(addr);
	case CVMX_ERROR_REGISTER_IO32:
		return cvmx_read64_uint32(addr ^ 4);
	case CVMX_ERROR_REGISTER_PCICONFIG:
		return cvmx_pcie_cfgx_read(addr >> 32, addr & 0xffffffffull);
	case CVMX_ERROR_REGISTER_SRIOMAINT:
		{
			uint32_t r;
			if (cvmx_srio_config_read32(addr >> 32, 0, -1, 0, 0, addr & 0xffffffffull, &r))
				return 0;
			else
				return r;
		}
	}
	return 0;
}

/**
 * @INTERNAL
 * Write a status or enable register to the hardware
 *
 * @param reg_type Register type to write
 * @param addr     Address to write
 * @param value    Value to write
 */
static void __cvmx_error_write_hw(cvmx_error_register_t reg_type, uint64_t addr, uint64_t value)
{
	switch (reg_type) {
	case __CVMX_ERROR_REGISTER_NONE:
		return;
	case CVMX_ERROR_REGISTER_IO64:
		cvmx_write_csr(addr, value);
		return;
	case CVMX_ERROR_REGISTER_IO32:
		cvmx_write64_uint32(addr ^ 4, value);
		return;
	case CVMX_ERROR_REGISTER_PCICONFIG:
		cvmx_pcie_cfgx_write(addr >> 32, addr & 0xffffffffull, value);
		return;
	case CVMX_ERROR_REGISTER_SRIOMAINT:
		{
			cvmx_srio_config_write32(addr >> 32, 0, -1, 0, 0, addr & 0xffffffffull, value);
			return;
		}
	}
}

/**
 * @INTERNAL
 * Function for processing non leaf error status registers. This function
 * calls all handlers for this passed register and all children linked
 * to it.
 *
 * @param info   Error register to check
 *
 * @return Number of error status bits found or zero if no bits were set.
 */
int __cvmx_error_decode(const cvmx_error_info_t * info)
{
	uint64_t status;
	uint64_t enable;
	int i;
	int handled = 0;

	/* Read the status and enable state */
	status = __cvmx_error_read_hw(info->reg_type, info->status_addr);
	if (info->enable_addr)
		enable = __cvmx_error_read_hw(info->reg_type, info->enable_addr);
	else
		enable = 0;

	for (i = 0; i < __cvmx_error_table_size; i++) {
		const cvmx_error_info_t *h = &__cvmx_error_table[i];
		uint64_t masked_status = status;

		/* If this is a child of the current register then recurse and process
		   the child */
		if ((h->parent.reg_type == info->reg_type) && (h->parent.status_addr == info->status_addr) && (status & h->parent.status_mask))
			handled += __cvmx_error_decode(h);

		if ((h->reg_type != info->reg_type) || (h->status_addr != info->status_addr))
			continue;

		/* If the corresponding enable bit is not set then we have nothing to do */
		if (h->enable_addr && h->enable_mask) {
			if (!(enable & h->enable_mask))
				continue;
		}

		/* Apply the mask to eliminate irrelevant bits */
		if (h->status_mask)
			masked_status &= h->status_mask;

		/* Finally call the handler function unless it is this function */
		if (masked_status && h->func && (h->func != __cvmx_error_decode))
			handled += h->func(h);
	}
	/* Ths should be the total errors found */
	return handled;
}

EXPORT_SYMBOL(__cvmx_error_decode);

/**
 * @INTERNAL
 * This error bit handler simply prints a message and clears the status bit
 *
 * @param info   Error register to check
 *
 * @return
 */
int __cvmx_error_display(const cvmx_error_info_t * info)
{
	const char *message = (const char *)(long)info->user_info;
	/* This assumes that all bits in the status register are RO or R/W1C */
	__cvmx_error_write_hw(info->reg_type, info->status_addr, info->status_mask);
	cvmx_safe_printf("%s", message);

	/* Clear the source to reduce the chance for spurious interrupts.  */

	/* CN68XX has an CIU-15786 errata that accessing the ACK registers
	 * can stop interrupts from propagating
	 */
	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		cvmx_read_csr(CVMX_CIU2_INTR_CIU_READY);

	return 1;
}

/**
 * Initalize the error status system. This should be called once
 * before any other functions are called. This function adds default
 * handlers for most all error events but does not enable them. Later
 * calls to cvmx_error_enable() are needed.
 *
 * @param flags  Optional flags.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_initialize(cvmx_error_flags_t flags)
{
	__cvmx_error_flags = flags;
	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		OCTEON_IS_MODEL(OCTEON_CNF75XX_PASS1_X)) {
		int node;
		cvmx_sysinfo_t *sysinfo = cvmx_sysinfo_get();
		for (node = 0; node < CVMX_MAX_NODES; node++) {
			if (!cvmx_coremask_get64_node(&sysinfo->core_mask, node))
				continue;
			if (cvmx_error_initialize_cn78xx(node))
				return -1;
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN70XX_PASS1_X)) {
		extern int cvmx_error_initialize_cn70xxp1(void);
		if (cvmx_error_initialize_cn70xxp1())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		extern int cvmx_error_initialize_cn70xx(void);
		if (cvmx_error_initialize_cn70xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CNF71XX)) {
		extern int cvmx_error_initialize_cnf71xx(void);
		if (cvmx_error_initialize_cnf71xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN68XX_PASS1_X)) {
		extern int cvmx_error_initialize_cn68xxp1(void);
		if (cvmx_error_initialize_cn68xxp1())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		extern int cvmx_error_initialize_cn68xx(void);
		if (cvmx_error_initialize_cn68xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN66XX)) {
		extern int cvmx_error_initialize_cn66xx(void);
		if (cvmx_error_initialize_cn66xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X)) {
		extern int cvmx_error_initialize_cn63xxp1(void);
		if (cvmx_error_initialize_cn63xxp1())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN63XX)) {
		extern int cvmx_error_initialize_cn63xx(void);
		if (cvmx_error_initialize_cn63xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN61XX)) {
		extern int cvmx_error_initialize_cn61xx(void);
		if (cvmx_error_initialize_cn61xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN58XX_PASS1_X)) {
		extern int cvmx_error_initialize_cn58xxp1(void);
		if (cvmx_error_initialize_cn58xxp1())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN58XX)) {
		extern int cvmx_error_initialize_cn58xx(void);
		if (cvmx_error_initialize_cn58xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN56XX_PASS1_X)) {
		extern int cvmx_error_initialize_cn56xxp1(void);
		if (cvmx_error_initialize_cn56xxp1())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		extern int cvmx_error_initialize_cn56xx(void);
		if (cvmx_error_initialize_cn56xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN50XX)) {
		extern int cvmx_error_initialize_cn50xx(void);
		if (cvmx_error_initialize_cn50xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN52XX_PASS1_X)) {
		extern int cvmx_error_initialize_cn52xxp1(void);
		if (cvmx_error_initialize_cn52xxp1())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN52XX)) {
		extern int cvmx_error_initialize_cn52xx(void);
		if (cvmx_error_initialize_cn52xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2)) {
		extern int cvmx_error_initialize_cn38xxp2(void);
		if (cvmx_error_initialize_cn38xxp2())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN38XX)) {
		extern int cvmx_error_initialize_cn38xx(void);
		if (cvmx_error_initialize_cn38xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN31XX)) {
		extern int cvmx_error_initialize_cn31xx(void);
		if (cvmx_error_initialize_cn31xx())
			return -1;
	} else if (OCTEON_IS_MODEL(OCTEON_CN30XX)) {
		extern int cvmx_error_initialize_cn30xx(void);
		if (cvmx_error_initialize_cn30xx())
			return -1;
	} else {
		cvmx_warn("cvmx_error_initialize() needs update for this Octeon model\n");
		return -1;
	}

	if (__cvmx_error_custom_initialize())
		return -1;

	/* Enable all of the purely internal error sources by default */
	cvmx_error_enable_group(CVMX_ERROR_GROUP_INTERNAL, 0);

	/* According to workaround for errata KEY-14814 in cn63xx, clearing 
	   SLI_INT_SUM[RML_TO] after enabling KEY interrupts */
	if (OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X))
		cvmx_write_csr(CVMX_PEXP_SLI_INT_SUM, 1);

	/* Enable DDR error reporting based on the memory controllers */
	if (OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		cvmx_l2c_cfg_t l2c_cfg;
		l2c_cfg.u64 = cvmx_read_csr(CVMX_L2C_CFG);
		if (l2c_cfg.s.dpres0)
			cvmx_error_enable_group(CVMX_ERROR_GROUP_LMC, 0);
		if (l2c_cfg.s.dpres1)
			cvmx_error_enable_group(CVMX_ERROR_GROUP_LMC, 1);
	} else
		cvmx_error_enable_group(CVMX_ERROR_GROUP_LMC, 0);

	/* Enable error interrupts for other LMC only if it is
	   available. */
	if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		int i;
		for (i = 1; i < 4; i++) {
			cvmx_lmcx_dll_ctl2_t ctl2;
			ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(i));
			if (ctl2.cn68xx.intf_en)
				cvmx_error_enable_group(CVMX_ERROR_GROUP_LMC, i);
			/* Enable other TADX interrupts too */
			cvmx_error_enable_group(CVMX_ERROR_GROUP_INTERNAL, i);
		}
	}

	/* Enable DFM error reporting based on feature availablility */
	if (octeon_has_feature(OCTEON_FEATURE_DFM)) {
		/* Only configure interrupts if DFM clock is enabled. */
		cvmx_dfm_fnt_sclk_t dfm_fnt_sclk;
		dfm_fnt_sclk.u64 = cvmx_read_csr(CVMX_DFM_FNT_SCLK);
		if (!dfm_fnt_sclk.s.sclkdis) {
			cvmx_error_enable_group(CVMX_ERROR_GROUP_DFM, 0);
		}
	}

	/* Old PCI parts don't have a common PCI init, so enable error
	   reporting if the bootloader told us we are a PCI host. PCIe
	   is handled when cvmx_pcie_rc_initialize is called */
	if (!octeon_has_feature(OCTEON_FEATURE_PCIE) && (cvmx_sysinfo_get()->bootloader_config_flags & CVMX_BOOTINFO_CFG_FLAG_PCI_HOST))
		cvmx_error_enable_group(CVMX_ERROR_GROUP_PCI, 0);

	/* Call poll once to clear out any pending interrupts */
	cvmx_error_poll();

	return 0;
}

/**
 * Poll the error status registers and call the appropriate error
 * handlers. This should be called in the RSL interrupt handler
 * for your application or operating system.
 *
 * @return Number of error handlers called. Zero means this call
 *         found no errors and was spurious.
 */
int cvmx_error_poll(void)
{
	int i;
	int count = 0;
	if (octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;
	/* Call all handlers that don't have a parent */
	for (i = 0; i < __cvmx_error_table_size; i++)
		if (__cvmx_error_table[i].parent.reg_type == __CVMX_ERROR_REGISTER_NONE)
			count += __cvmx_error_decode(&__cvmx_error_table[i]);
	return count;
}

/**
 * Register to be called when an error status bit is set. Most users
 * will not need to call this function as cvmx_error_initialize()
 * registers default handlers for most error conditions. This function
 * is normally used to add more handlers without changing the existing
 * handlers.
 *
 * @param new_info Information about the handler for a error register. The
 *                 structure passed is copied and can be destroyed after the
 *                 call. All members of the structure must be populated, even the
 *                 parent information.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_add(const cvmx_error_info_t * new_info)
{
	if (__cvmx_error_table_size >= MAX_TABLE_SIZE) {
		cvmx_warn("cvmx-error table full\n");
		return -1;
	}
	__cvmx_error_table[__cvmx_error_table_size] = *new_info;
	__cvmx_error_table_size++;
	return 0;
}

/**
 * Remove all handlers for a status register and mask. Normally
 * this function should not be called. Instead a new handler should be
 * installed to replace the existing handler. In the even that all
 * reporting of a error bit should be removed, then use this
 * function.
 *
 * @param reg_type Type of the status register to remove
 * @param status_addr
 *                 Status register to remove.
 * @param status_mask
 *                 All handlers for this status register with this mask will be
 *                 removed.
 * @param old_info If not NULL, this is filled with information about the handler
 *                 that was removed.
 *
 * @return Zero on success, negative on failure (not found).
 */
int cvmx_error_remove(cvmx_error_register_t reg_type, uint64_t status_addr, uint64_t status_mask, cvmx_error_info_t * old_info)
{
	int found = 0;
	int i;
	for (i = 0; i < __cvmx_error_table_size; i++) {
		cvmx_error_info_t *h = &__cvmx_error_table[i];
		if (!REG_MATCH(h, reg_type, status_addr, status_mask))
			continue;
		if (old_info)
			*old_info = *h;
		memset(h, 0, sizeof(*h));
		found = 1;
	}
	if (found)
		return 0;
	else {
		cvmx_warn("cvmx-error remove couldn't find requested register\n");
		return -1;
	}
}

/**
 * Change the function and user_info for an existing error status
 * register. This function should be used to replace the default
 * handler with an application specific version as needed.
 *
 * @param reg_type Type of the status register to change
 * @param status_addr
 *                 Status register to change.
 * @param status_mask
 *                 All handlers for this status register with this mask will be
 *                 changed.
 * @param new_func New function to use to handle the error status
 * @param new_user_info
 *                 New user info parameter for the function
 * @param old_func If not NULL, the old function is returned. Useful for restoring
 *                 the old handler.
 * @param old_user_info
 *                 If not NULL, the old user info parameter.
 *
 * @return Zero on success, negative on failure
 */
int cvmx_error_change_handler(cvmx_error_register_t reg_type,
			      uint64_t status_addr, uint64_t status_mask,
			      cvmx_error_func_t new_func, uint64_t new_user_info, cvmx_error_func_t * old_func, uint64_t * old_user_info)
{
	int found = 0;
	int i;
	for (i = 0; i < __cvmx_error_table_size; i++) {
		cvmx_error_info_t *h = &__cvmx_error_table[i];
		if (!REG_MATCH(h, reg_type, status_addr, status_mask))
			continue;
		if (old_func)
			*old_func = h->func;
		if (old_user_info)
			*old_user_info = h->user_info;
		h->func = new_func;
		h->user_info = new_user_info;
		found = 1;
	}
	if (found)
		return 0;
	else {
		cvmx_warn("cvmx-error change couldn't find requested register\n");
		return -1;
	}
}

/**
 * Enable all error registers for a logical group. This should be
 * called whenever a logical group is brought online.
 *
 * @param group  Logical group to enable
 * @param group_index
 *               Index for the group as defined in the cvmx_error_group_t
 *               comments.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_enable_group(cvmx_error_group_t group, int group_index)
{
	int i;
	uint64_t enable;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;

	if (octeon_has_feature(OCTEON_FEATURE_CIU3)) {
		return cvmx_error_enable_group_v3(group, group_index);
	} else {
		for (i = 0; i < __cvmx_error_table_size; i++) {
			const cvmx_error_info_t *h = &__cvmx_error_table[i];
			/* SGMII and XAUI has different ipd_port, use the same group_index
			for both the interfaces */
			switch (group_index) {
			case 0x840:
				group_index = 0x800;
				break;
			case 0xa40:
				group_index = 0xa00;
				break;
			case 0xb40:
				group_index = 0xb00;
				break;
			case 0xc40:
				group_index = 0xc00;
				break;
			}

			/* Skip entries that have a different group or group index. We
			also skip entries that don't have an enable */
			if ((h->group != group) || (h->group_index != group_index) || (!h->enable_addr))
				continue;
			/* Skip entries that have flags that don't match the user's
			selected flags */
			if (h->flags && (h->flags != (h->flags & __cvmx_error_flags)))
				continue;
			/* Update the enables for this entry */
			enable = __cvmx_error_read_hw(h->reg_type, h->enable_addr);
			if (h->reg_type == CVMX_ERROR_REGISTER_PCICONFIG)
				enable &= ~h->enable_mask;	/* PCI bits have reversed polarity */
			else
				enable |= h->enable_mask;
			__cvmx_error_write_hw(h->reg_type, h->enable_addr, enable);
		}
	}
	return 0;
}

EXPORT_SYMBOL(cvmx_error_enable_group);

/**
 * Disable all error registers for a logical group. This should be
 * called whenever a logical group is brought offline. Many blocks
 * will report spurious errors when offline unless this function
 * is called.
 *
 * @param group  Logical group to disable
 * @param group_index
 *               Index for the group as defined in the cvmx_error_group_t
 *               comments.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_disable_group(cvmx_error_group_t group, int group_index)
{
	int i;
	uint64_t enable;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;

	if (octeon_has_feature(OCTEON_FEATURE_CIU3)) {
		return cvmx_error_disable_group_v3(group, group_index);
	} else {
		for (i = 0; i < __cvmx_error_table_size; i++) {
			const cvmx_error_info_t *h = &__cvmx_error_table[i];

			/* SGMII and XAUI has different ipd_port, use the same group_index
			for both the interfaces */
			switch (group_index) {
			case 0x840:
				group_index = 0x800;
				break;
			case 0xa40:
				group_index = 0xa00;
				break;
			case 0xb40:
				group_index = 0xb00;
				break;
			case 0xc40:
				group_index = 0xc00;
				break;
			}
			/* Skip entries that have a different group or group index. We
			also skip entries that don't have an enable */
			if ((h->group != group) || (h->group_index != group_index) || (!h->enable_addr))
				continue;
			/* Update the enables for this entry */
			enable = __cvmx_error_read_hw(h->reg_type, h->enable_addr);
			if (h->reg_type == CVMX_ERROR_REGISTER_PCICONFIG)
				enable |= h->enable_mask;	/* PCI bits have reversed polarity */
			else
				enable &= ~h->enable_mask;
			__cvmx_error_write_hw(h->reg_type, h->enable_addr, enable);
		}
	}
	return 0;
}

EXPORT_SYMBOL(cvmx_error_disable_group);

/**
 * Enable all handlers for a specific status register mask.
 *
 * @param reg_type Type of the status register
 * @param status_addr
 *                 Status register address
 * @param status_mask
 *                 All handlers for this status register with this mask will be
 *                 enabled.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_enable(cvmx_error_register_t reg_type, uint64_t status_addr, uint64_t status_mask)
{
	int found = 0;
	int i;
	uint64_t enable;
	for (i = 0; i < __cvmx_error_table_size; i++) {
		cvmx_error_info_t *h = &__cvmx_error_table[i];
		if (!REG_MATCH(h, reg_type, status_addr, status_mask) || !h->enable_addr)
			continue;
		enable = __cvmx_error_read_hw(h->reg_type, h->enable_addr);
		if (h->reg_type == CVMX_ERROR_REGISTER_PCICONFIG)
			enable &= ~h->enable_mask;	/* PCI bits have reversed polarity */
		else
			enable |= h->enable_mask;
		__cvmx_error_write_hw(h->reg_type, h->enable_addr, enable);
		h->flags &= ~CVMX_ERROR_FLAGS_DISABLED;
		found = 1;
	}
	if (found)
		return 0;
	else {
		cvmx_warn("cvmx-error enable couldn't find requested register\n");
		return -1;
	}
}

/**
 * Disable all handlers for a specific status register and mask.
 *
 * @param reg_type Type of the status register
 * @param status_addr
 *                 Status register address
 * @param status_mask
 *                 All handlers for this status register with this mask will be
 *                 disabled.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_disable(cvmx_error_register_t reg_type, uint64_t status_addr, uint64_t status_mask)
{
	int found = 0;
	int i;
	uint64_t enable;
	for (i = 0; i < __cvmx_error_table_size; i++) {
		cvmx_error_info_t *h = &__cvmx_error_table[i];
		if (!REG_MATCH(h, reg_type, status_addr, status_mask) || !h->enable_addr)
			continue;
		enable = __cvmx_error_read_hw(h->reg_type, h->enable_addr);
		if (h->reg_type == CVMX_ERROR_REGISTER_PCICONFIG)
			enable |= h->enable_mask;	/* PCI bits have reversed polarity */
		else
			enable &= ~h->enable_mask;
		__cvmx_error_write_hw(h->reg_type, h->enable_addr, enable);
		h->flags |= CVMX_ERROR_FLAGS_DISABLED;
		found = 1;
	}
	if (found)
		return 0;
	else {
		cvmx_warn("cvmx-error disable couldn't find requested register\n");
		return -1;
	}
}

/**
 * Find the handler for a specific status register and mask
 *
 * @param status_addr
 *                Status register address
 *
 * @return  Return the handler on success or null on failure.
 */
cvmx_error_info_t *cvmx_error_get_index(uint64_t status_addr)
{
	int i;
	for (i = 0; i < __cvmx_error_table_size; i++) {
		if (__cvmx_error_table[i].status_addr == status_addr)
			return &__cvmx_error_table[i];
	}

	return NULL;
}

EXPORT_SYMBOL(cvmx_error_get_index);

/**
 * This function powers down all other cores and suspends the current core
 */
void cvmx_octeon_hang(void)
{
	struct cvmx_coremask coremask;
	int core;
	int current_core = cvmx_get_core_num();
	int current_node = cvmx_coremask_core_to_node(current_core);

	current_core = cvmx_coremask_core_on_node(current_core);

	cvmx_get_hardware_coremask(&coremask);
	cvmx_coremask_clear_self(&coremask);

	/* Power down all of the cores except this one */
	cvmx_coremask_for_each_core(core, &coremask) {
		int node = cvmx_coremask_core_to_node(core);
		int node_core = cvmx_coremask_core_on_node(core);

		/* Put the core into reset */
		cvmx_write_csr_node(node, CVMX_CIU_PP_RST,
				    1ULL << node_core);
		/* Power it down */
		cvmx_write_csr_node(node, CVMX_RST_PP_POWER,
				    1ULL << node_core);
	}
	cvmx_safe_printf("All cores except node %d, core %d powered down\n"
		"Setting node %d, core %d to wait forever\n",
		current_node, current_core,
		current_node, current_core);

	/* Power down all the cores */
	while (1)
		__asm__("wait\n");
	__builtin_unreachable();
}

/**
 * When a L2 cache single-bit ECC error occurs in the data this function
 * calculates the proper cache index and flushes and invalidates the cache
 * line.
 *
 * @param	node	node where L2 ECC error occurred
 * @param	tad	tad number where L2 ECC error occurred
 */
static void __cvmx_flush_l2c_l2d_ecc_error(int node, int tad)
{
	uint64_t cindex;

	cindex = cvmx_l2c_tqdl2d_to_index_7xxx(node, tad);
	cindex = CVMX_ADD_IO_SEG(cindex);
	/* Write back and invalidate the cache line */
	CVMX_SYNCW;
	CVMX_CACHE_WBIL2I(cindex, 0);
}

/**
 * @INTERNAL
 *
 * Process L2C single and multi-bit ECC errors
 *
 */
int __cvmx_cn7xxx_l2c_l2d_ecc_error_display(int node, int intsn)
{
	union cvmx_l2c_tqdx_err tqdx_err;
	union cvmx_l2c_ecc_ctl ecc_ctl;
	int tad = intsn;
	unsigned max_tad;
	bool dbe = false;

	if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		max_tad = 8;
	else if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		max_tad = 1;
	else
		max_tad = 4;

	/* See if this is some other error type */
	if (!OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		if (((intsn & 0xff) >= 2) ||
		    ((intsn & 0xff00) > (max_tad << 8)))
			return 0;
		tad = (intsn >> 8) & 7;
	}

	ecc_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_ECC_CTL);
	switch (ecc_ctl.s.l2dflip) {
	case 0:
		break;
	case 1:
		cvmx_safe_printf("Single-bit L2C ECC error generated artificially on ECC[0]\n");
		break;
	case 2:
		cvmx_safe_printf("Single-bit L2C ECC error generated artificially on ECC[1]\n");
		break;
	case 3:
		cvmx_safe_printf("Double-bit L2C ECC error generated artifically on ECC[0:1]\n");
		break;
	}
	if (ecc_ctl.s.l2dflip) {
		ecc_ctl.s.l2dflip = 0;
		cvmx_write_csr_node(node, CVMX_L2C_ECC_CTL, ecc_ctl.u64);
	}
	tqdx_err.u64 = cvmx_read_csr_node(node, CVMX_L2C_TQDX_ERR(tad));
	/* FBF = fill buffer
	 * SBF = store buffer
	 * VBF = victim buffer
	 * LFB + VAB = in-flight address buffers
	 */
	/* Check if double-bit error */
	if (tqdx_err.s.l2ddbe) {
		dbe = true;
	} else if (!tqdx_err.s.l2dsbe) {
		cvmx_safe_printf("Unknown L2 cache error detected\n"
			         "L2D double-bit L2D error: %d\n"
				 "L2D double-bit SBF error: %d\n"
				 "L2D double-bit FBF error: %d\n"
				 "L2D single-bit L2D error: %d\n"
				 "L2D single-bit SBF error: %d\n"
				 "L2D single-bit FBF error: %d\n",
				 tqdx_err.s.l2ddbe, tqdx_err.s.sbfdbe,
				 tqdx_err.s.fbfdbe,
				 tqdx_err.s.l2dsbe, tqdx_err.s.sbfsbe,
				 tqdx_err.s.fbfsbe);
	}

	cvmx_safe_printf("L2 cache %s-bit ECC error detected\n",
			 dbe ? "double" : "single");
	cvmx_safe_printf("Node: %d, TAD: %d, L2D index: 0x%x, quad half: 0x%x, quad number: 0x%x, error syndrome: 0x%x\n",
			 node, tad,  tqdx_err.s.l2didx, tqdx_err.s.qdhlf,
			 tqdx_err.s.qdnum, tqdx_err.s.syn);
	if (dbe) {
		cvmx_safe_printf("\nCannot continue due to uncorrectable cache line!\n"
				 "Please reset or power cycle this system!\n");
		cvmx_octeon_hang();
	}

	__cvmx_flush_l2c_l2d_ecc_error(node, tad);

	return 1;
}

/**
 * Handle L2 cache TAG ECC errors and noway errors
 *
 * @param	CCPI node
 * @param	intsn	intsn from error array.
 * @param	remote	true for remote node (cn78xx only)
 *
 * @return	1 if handled, 0 if not handled
 */
int __cvmx_cn7xxx_l2c_tag_error_display(int node, int intsn, bool remote)
{
	union cvmx_l2c_ttgx_err ttgx_err;
	union cvmx_l2c_rtgx_err rtgx_err;
	union cvmx_l2c_ecc_ctl ecc_ctl;
	uint64_t cindex;
	int tad;
	unsigned max_tad;
	unsigned index, way, syn;
	bool dbe = false;

	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		tad = intsn;
	else
		tad = (intsn >> 8) & 7;

	if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		max_tad = 8;
	else if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		max_tad = 1;
	else
		max_tad = 4;

	/* See if this is some other error type */
	if (!OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		if ((intsn & 0xff00) > (max_tad << 8))
			return 0;
		switch (intsn & 0xff) {
		case 0x6:
		case 0x7:
		case 0xf:
		case 0x10:
			break;
		default:
			return 0;
		}
	}

	if (remote && OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_X)) {
		cvmx_safe_printf("Cannot process remote L2 cache ECC tags with CN78XX pass 1.x\n");
		return 0;
	}

	ecc_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_ECC_CTL);
	switch (ecc_ctl.s.l2tflip) {
		case 0:
			break;
		case 1:
			cvmx_safe_printf("Single-bit L2C TAG ECC error generated artificially on ECC[0]\n");
			break;
		case 2:
			cvmx_safe_printf("Single-bit TAG L2C ECC error generated artificially on ECC[1]\n");
			break;
		case 3:
			cvmx_safe_printf("Double-bit TAG L2C ECC error generated artifically on ECC[0:1]\n");
			break;
	}
	if (ecc_ctl.s.l2tflip) {
		ecc_ctl.s.l2tflip = 0;
		cvmx_write_csr_node(node, CVMX_L2C_ECC_CTL, ecc_ctl.u64);
	}
	if (remote)
		rtgx_err.u64 = cvmx_read_csr_node(node, CVMX_L2C_RTGX_ERR(tad));
	else
		ttgx_err.u64 = cvmx_read_csr_node(node, CVMX_L2C_TTGX_ERR(tad));

	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		if (remote) {
			way = rtgx_err.s.way;
			index = rtgx_err.s.l2idx;
			syn = rtgx_err.s.syn;
		} else {
			way = ttgx_err.cn78xx.way;
			index = ttgx_err.cn78xx.l2idx;
			syn = ttgx_err.cn78xx.syn;
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		way = ttgx_err.cn70xx.way;
		index = ttgx_err.cn70xx.l2idx;
		syn = ttgx_err.cn70xx.syn;
	} else {
		way = ttgx_err.cn73xx.way;
		index = ttgx_err.cn73xx.l2idx;
		syn = ttgx_err.cn73xx.syn;
	}

	/* Check if double-bit error */
	if ((remote && rtgx_err.s.rtgdbe) || (!remote && ttgx_err.s.tagdbe))
		dbe = true;
	else if (!remote && ttgx_err.s.noway)
		/* Ignore this case */
		return 1;

	cvmx_safe_printf("L2 cache %s-bit %stag ECC error detected\n",
			 dbe ? "double" : "single", remote ? "remote " : "");
	cvmx_safe_printf("Node: %d, TAD: %d, L2D index: 0x%x, way: 0x%x, error syndrome: 0x%x\n",
			 node, tad,  index, way, syn);
	if (dbe) {
		cvmx_safe_printf("\nCannot continue due to uncorrectable cache line!\n"
				 "Please reset or power cycle this system!\n");
		cvmx_octeon_hang();
	}

	cindex = cvmx_l2c_ttgx_to_index_7xxx(node, tad, remote);
	cindex = CVMX_ADD_IO_SEG(cindex);
	CVMX_SYNCW;
	CVMX_CACHE_WBIL2I(cindex, 0);
	CVMX_SYNC;

	return 1;
}
