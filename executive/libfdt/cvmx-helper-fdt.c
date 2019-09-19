/***********************license start***************
 * Copyright (c) 2014  Cavium Inc. (support@cavium.com). All rights
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
 * FDT Helper functions similar to those provided to U-Boot.
 *
 */

#if defined(CVMX_BUILD_FOR_LINUX_KERNEL)
# error This file should not be used with Linux!
#endif

#ifdef __U_BOOT__
# include <common.h>
# include <malloc.h>
# include <asm/arch/cvmx.h>
# include <asm/arch/cvmx-helper.h>
# include <asm/arch/cvmx-helper-fdt.h>
# include <asm/arch/cvmx-helper-util.h>
# include <asm/arch/cvmx-helper-board.h>
# include <asm/arch/cvmx-helper-cfg.h>
# include <asm/arch/octeon-model.h>
# include <asm/arch/cvmx-access.h>
# include <asm/arch/cvmx-gpio.h>
# include <asm/arch/cvmx-swap.h>
# include <asm/arch/cvmx-malloc.h>
# include <i2c.h>
# include <asm/arch/octeon-model.h>
#else
# include "../cvmx.h"
# include "../cvmx-helper.h"
# include "cvmx-helper-fdt.h"
# include "../cvmx-helper-util.h"
# include "../cvmx-helper-cfg.h"
# include "../octeon-model.h"
# include "../cvmx-access.h"
# include "../cvmx-twsi.h"
# include "../cvmx-gpio.h"
# include "../cvmx-swap.h"
# include "../cvmx-helper-board.h"
# include "../cvmx-bootmem.h"
# include "../octeon-model.h"
# include <stdlib.h>
# include <string.h>
#endif

#ifndef __U_BOOT__
# ifdef DEBUG
#  define debug(format, args...)	cvmx_dprintf(format, ##args)
# else
#  define debug(format, args...)
# endif
#endif

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/** Structure used to get type of GPIO from device tree */
struct gpio_compat {
	char *compatible;		/** Compatible string */
	enum cvmx_gpio_type type;	/** Type */
	int8_t size;			/** (max) Number of pins */
};


#define GPIO_REG_PCA953X_IN		0
#define GPIO_REG_PCA953X_OUT		1
#define GPIO_REG_PCA953X_INVERT		2
#define GPIO_REG_PCA953X_DIR		3

#define GPIO_REG_PCA957X_IN		0
#define GPIO_REG_PCA957X_INVERT		1
#define GPIO_REG_PCA957X_CFG		4
#define GPIO_REG_PCA957X_OUT		5

/**
 * List of GPIO types
 */
static const struct gpio_compat gpio_compat_list[] = {
	{ "cavium,octeon-3860-gpio", CVMX_GPIO_PIN_OCTEON, 20 },
	{ "cavium,octeon-7890-gpio", CVMX_GPIO_PIN_OCTEON, 20 },
	{ "nxp,pca9505",	CVMX_GPIO_PIN_PCA9698, 40 },
 	{ "pca9505",		CVMX_GPIO_PIN_PCA9698, 40 },
	{ "nxp,pca9698",	CVMX_GPIO_PIN_PCA9698, 40 },
	{ "pca9698",		CVMX_GPIO_PIN_PCA9698, 40 },
	{ "nxp,pca9534",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9535",	CVMX_GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9536",	CVMX_GPIO_PIN_PCA953X, 4 },
	{ "nxp,pca9537",	CVMX_GPIO_PIN_PCA953X, 4 },
	{ "nxp,pca9538",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9539",	CVMX_GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9554",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9554a",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9555",	CVMX_GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9555a",	CVMX_GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9557",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9574",	CVMX_GPIO_PIN_PCA957X, 8 },
	{ "nxp,pca9575",	CVMX_GPIO_PIN_PCA957X, 16 },
	{ "maxim,max7310",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "maxim,max7312",	CVMX_GPIO_PIN_PCA953X, 16 },
	{ "maxim,max7313",	CVMX_GPIO_PIN_PCA953X, 16 },
	{ "maxim,max7315",	CVMX_GPIO_PIN_PCA953X, 8 },
	{ "ti,pca6107",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "ti,tca6408",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "ti,tca6416",		CVMX_GPIO_PIN_PCA953X, 16 },
	{ "ti,tca6424",		CVMX_GPIO_PIN_PCA953X, 24 },
	{ "pca9534",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "pca9535",		CVMX_GPIO_PIN_PCA953X, 16 },
	{ "pca9536",		CVMX_GPIO_PIN_PCA953X, 4 },
	{ "pca9537",		CVMX_GPIO_PIN_PCA953X, 4 },
	{ "pca9538",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "pca9539",		CVMX_GPIO_PIN_PCA953X, 16 },
	{ "pca9554",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "pca9554a",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "pca9555",		CVMX_GPIO_PIN_PCA953X, 16 },
	{ "pca9555a",		CVMX_GPIO_PIN_PCA953X, 16 },
	{ "pca9558",		CVMX_GPIO_PIN_PCA953X, 8 },
	{ "pca9574",		CVMX_GPIO_PIN_PCA957X, 8 },
	{ "pca9575",		CVMX_GPIO_PIN_PCA957X, 16 },
	{ "nxp,pcf8574",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "nxp,pcf8574a",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "nxp,pc8574",		CVMX_GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca9670",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca9672",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca9674",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca8575",	CVMX_GPIO_PIN_PCF857X, 16 },
	{ "nxp,pcf8575",	CVMX_GPIO_PIN_PCF857X, 16 },
	{ "nxp,pca9671",	CVMX_GPIO_PIN_PCF857X, 16 },
	{ "nxp,pca9673",	CVMX_GPIO_PIN_PCF857X, 16 },
	{ "nxp,pca9675",	CVMX_GPIO_PIN_PCF857X, 16 },
	{ "maxim,max7328",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "maxim,max7329",	CVMX_GPIO_PIN_PCF857X, 8 },
	{ "ti,tca9554",		CVMX_GPIO_PIN_PCF857X, 8 },
};

enum cvmx_i2c_mux_type {
	I2C_MUX,
	I2C_SWITCH
};

/** Structure used to get type of GPIO from device tree */
struct mux_compat {
	char *compatible;		/** Compatible string */
	enum cvmx_i2c_bus_type type;	/** Mux chip type */
	enum cvmx_i2c_mux_type mux_type;/** Type of mux */
	uint8_t enable;			/** Enable bit for mux */
	uint8_t size;			/** (max) Number of channels */
};

/** List of i2c MUX and switch devices */
static const struct mux_compat mux_compat_list[] = {
	{ "nxp,pca9540", CVMX_I2C_MUX_PCA9540, I2C_MUX,    4, 2 },
	{ "nxp,pca9542", CVMX_I2C_MUX_PCA9542, I2C_MUX,    4, 2 },
	{ "nxp,pca9543", CVMX_I2C_MUX_PCA9543, I2C_SWITCH, 0, 2 },
	{ "nxp,pca9544", CVMX_I2C_MUX_PCA9544, I2C_MUX,    4, 4 },
	{ "nxp,pca9545", CVMX_I2C_MUX_PCA9545, I2C_SWITCH, 0, 4 },
	{ "nxp,pca9546", CVMX_I2C_MUX_PCA9546, I2C_SWITCH, 0, 4 },
	{ "nxp,pca9547", CVMX_I2C_MUX_PCA9547, I2C_MUX,    8, 8 },
	{ "nxp,pca9548", CVMX_I2C_MUX_PCA9548, I2C_SWITCH, 0, 8 },
};

extern int cvmx_sfp_vsc7224_mod_abs_changed(struct cvmx_fdt_sfp_info *sfp_info,
					    int val, void *data);

/**
 * Local allocator to handle both SE and U-Boot that also zeroes out memory
 *
 * @param	size	number of bytes to allocate
 *
 * @return	pointer to allocated memory or NULL if out of memory.
 *		Alignment is set to 8-bytes.
 */
static void *cvm_fdt_alloc(size_t size)
{
#ifndef __U_BOOT__
	void *ptr;
	ptr = cvmx_bootmem_alloc(size, 8);
	if (ptr)
		memset(ptr, 0, size);
	return ptr;
#else
	return calloc(size, 1);
#endif
}

/**
 * Free allocated memory.
 *
 * @param	ptr	pointer to memory to free
 *
 * NOTE: This only works in U-Boot since SE does not really have a freeing
 *	 mechanism.  In SE the memory is zeroed out.
 */
static inline void cvm_fdt_free(void *ptr, size_t size)
{
#ifdef __U_BOOT__
	free(ptr);
#else
	if (ptr)
		memset(ptr, 0, size);
#endif
}

#ifndef __U_BOOT__
/**
 * Helper to return the address property
 *
 * @param[in] fdt_addr	pointer to FDT blob
 * @param node		node to read address from
 * @param[in] prop_name	property name to read
 *
 * @return address of property or FDT_ADDR_T_NONE if not found
 */
uint64_t cvmx_fdt_get_addr(const void *fdt_addr, int node,
			   const char *prop_name)
{
	const uint64_t *pval;
	int len;

	pval = fdt_getprop(fdt_addr, node, prop_name, &len);
	if (pval && ((len == sizeof(uint64_t)) ||
		     (len == sizeof(uint64_t) * 2)))
		return fdt64_to_cpu(*pval);
	else
		return FDT_ADDR_T_NONE;
}

/**
 * Helper function to return an integer property
 *
 * @param[in] fdt_addr	pointer to FDT blob
 * @param node		node to read integer from
 * @param[in] prop_name	property name to read
 * @param default_val	default value to return if property doesn't exist
 *
 * @return	integer value of property or default_val if it doesn't exist.
 */
int cvmx_fdt_get_int(const void *fdt_addr, int node, const char *prop_name,
		     int default_val)
{
	const uint32_t *pval;
	int len;

	pval = fdt_getprop(fdt_addr, node, prop_name, &len);
	if (pval && (unsigned)len >= sizeof(int))
		return fdt32_to_cpu(*pval);
	else
		return default_val;
}

/**
 * Helper function to return a boolean property
 *
 * @param[in]	fdt_addr	Pointer to FDT blob
 * @param	node		node to check
 * @param[in]	prop_name	Property name to check
 *
 * @return	true if property exists, false if it doesn't
 */
bool cvmx_fdt_get_bool(const void *fdt_addr, int node, const char *prop_name)
{
	const uint32_t *pval;

	pval = fdt_getprop(fdt_addr, node, prop_name, NULL);
	return (pval != NULL);
}

/**
 * Helper function to return a uint64_t property
 *
 * @param[in] fdt_addr	pointer to FDT blob
 * @param node		node to read integer from
 * @param[in] prop_name	property name to read
 * @param default_val	default value to return if property doesn't exist
 *
 * @return	uint64_t value of property or default_val if it doesn't exist.
 */
uint64_t cvmx_fdt_get_uint64(const void *fdt_addr, int node,
			     const char *prop_name, uint64_t default_val)
{
	const uint64_t *pval;
	int len;

	pval = fdt_getprop(fdt_addr, node, prop_name, &len);
	if (pval && (unsigned)len >= sizeof(uint64_t))
		return fdt64_to_cpu(*pval);
	else
		return default_val;
}

/**
 * Look up a phandle and follow it to its node then return the offset of that
 * node.
 *
 * @param[in] fdt_addr	pointer to FDT blob
 * @param node		node to read phandle from
 * @param[in] prop_name	name of property to find
 *
 * @return	node offset if found, -ve error code on error
 */
int cvmx_fdt_lookup_phandle(const void *fdt_addr, int node,
			    const char *prop_name)
{
	const uint32_t *phandle;

	phandle = fdt_getprop(fdt_addr, node, prop_name, NULL);
	if (phandle)
		return fdt_node_offset_by_phandle(fdt_addr,
						  fdt32_to_cpu(*phandle));
	else
		return -FDT_ERR_NOTFOUND;
}

static inline uint64_t __cvmx_fdt_read_num(const uint32_t *addr, int num_addr)
{
	if (num_addr == 2)
		return ((uint64_t)fdt32_to_cpu(*addr) << 32) |
			fdt32_to_cpu(*(addr + 1));
	else
		return fdt32_to_cpu(*addr);
}

/**
 * Map an address
 *
 * @param	addr		address to translate
 * @param[in]	range		range parameter
 * @param	num_addr	Number of words in the address
 * @param	num_size	Number of words in the size of
 * @param	pna		Parent num address cekks
 *
 * @return	Mapped address
 */
static uint64_t cvmx_fdt_bus_map(const uint64_t addr,
				 const uint32_t *range,
				 int num_addr, int num_size, int pna)
{
	uint64_t cp, size;

	cp = __cvmx_fdt_read_num(range, num_addr);
	range += num_addr;
	range += pna;
	size = __cvmx_fdt_read_num(range, num_size);

	if ((addr < cp) || addr >= (cp + size))
		return FDT_ADDR_T_NONE;

	return addr - cp;
}

/**
 * Outputs the number of address and size cells of a node
 *
 * @param[in] fdt_addr		Address of flat device tree
 * @param parent		node address of parent
 * @param[out] num_addr_cells	Number of address cells (NULL for none)
 * @param[out] num_size_cells	Number of size cells (NULL for none)
 */
static void cvmx_fdt_count_cells(const void *fdt_addr, int parent,
				 int *num_addr_cells, int *num_size_cells)
{
	if (num_addr_cells)
		*num_addr_cells = cvmx_fdt_get_int(fdt_addr, parent,
						   "#address-cells", 2);

	if (num_size_cells)
		*num_size_cells = cvmx_fdt_get_int(fdt_addr, parent,
						   "#size-cells", 1);
}

/**
 * Translate one address
 *
 * @param[in]		fdt_addr	Pointer to flat device tree
 * @param		parent		parent node
 * @param[in,out]	in_addr		Address to translate (native ENDIAN)
 * @param		num_addr	number of address words
 * @param		num_size	number of size words
 * @param		p_num_addr	number of parent address words
 * @param[in]		prop_name	property name (usually "ranges")
 *
 * @return	0 for success, error otherwise
 *
 * NOTE: Regardless of num_addr in_addr will always contain a 64-bit address.
 */
int __cvmx_fdt_translate_one(const void *fdt_addr, int parent,
			     uint64_t *in_addr,
			     int num_addr, int num_size, int p_num_addr,
			     const char *prop_name)
{
	const uint32_t *ranges;
	int rlen;
	int rone;
	uint64_t offset = FDT_ADDR_T_NONE;
	int rindex = 0;

	debug("%s(%p, %d, 0x%llx, %d, %d, %d, %s)\n", __func__, fdt_addr, parent,
	      (unsigned long long)*in_addr,
	      num_addr, num_size, p_num_addr, prop_name);

	ranges = fdt_getprop(fdt_addr, parent, prop_name, &rlen);
	/* If no ranges don't bother translating */
	if (!ranges || !rlen) {
		debug("  property %s not found, no translation\n", prop_name);
		return 0;
	}

	/* Walk through the ranges */
	rlen /= sizeof(uint32_t);
	rone = num_addr + p_num_addr + num_size;

	debug("  ranges size: %d, rone: %d\n", rlen, rone);
	/* Walk through the ranges */
	do {
		offset = cvmx_fdt_bus_map(*in_addr, &ranges[rindex],
					  num_addr, num_size, p_num_addr);
		debug("%s: mapped bus offset: 0x%llx\n",
		      __func__, (unsigned long long)offset);
		if (offset == FDT_ADDR_T_NONE)
			return 1;
		rlen -= rone;
		rindex += rone;
	} while (rlen >= rone);

	debug("%s: offset: 0x%llx\n", __func__, (unsigned long long)offset);
	debug("  p_num_addr: %d, num_addr: %d, rlen: %d, ranges: ",
	      p_num_addr, num_addr, rlen);

	if (p_num_addr == 2)
		*in_addr = (uint64_t)fdt32_to_cpu(ranges[num_addr]) << 32 |
			   fdt32_to_cpu(ranges[num_addr + 1]);
	else
		*in_addr = fdt32_to_cpu(ranges[num_addr + p_num_addr]);

	debug("  in_addr: 0x%llx, offset: 0x%llx\n",
	      (unsigned long long)(*in_addr), (unsigned long long)offset);

	/* Translate to the parent bus space */
	*in_addr += offset;

	return 0;
}

/**
 * Translate an address from the device tree into a CPU physical address by
 * walking up the device tree and applying bus mappings along the way.
 *
 * This uses #size-cells and #address-cells.
 *
 * @param[in]	fdt_addr	Address of flat device tree
 * @param	node		node to start translating from
 * @param	in_addr		Address to translate
 *				NOTE: in_addr must be in the native ENDIAN
 *				format.
 *
 * @return	Translated address or FDT_ADDR_T_NONE if address cannot be
 * 		translated.
 */
uint64_t cvmx_fdt_translate_address(const void *fdt_addr, int node,
				    const uint32_t *in_addr)
{
	int parent;
	int num_addr, num_size;
	int p_num_addr, p_num_size;
	uint64_t addr64;

	debug("%s(%p, %d, %p)\n", __func__, fdt_addr, node, in_addr);
	parent = fdt_parent_offset(fdt_addr, node);
	if (parent < 0) {
		cvmx_dprintf("%s: No parent node!\n", __func__);
		return FDT_ADDR_T_NONE;
	}

	cvmx_fdt_count_cells(fdt_addr, parent, &num_addr, &num_size);
	debug("%s: parent: %d, num addr: %d, num size: %d\n",
	      __func__, parent, num_addr, num_size);
	if (num_addr < 0 || num_addr > 4 || num_size < 0) {
		cvmx_dprintf("%s: Bad cell count for %s\n", __func__,
			     fdt_get_name(fdt_addr, node, NULL));
		return FDT_ADDR_T_NONE;
	}

	if (num_addr == 2)
		memcpy(&addr64, in_addr, num_addr * sizeof(uint32_t));
	else
		addr64 = in_addr[0];

	/* Translate */
	do {
		node = parent;
		parent = fdt_parent_offset(fdt_addr, node);

		/* Check if root */
		if (parent < 0)
			return addr64;

		cvmx_fdt_count_cells(fdt_addr, parent, &p_num_addr, &p_num_size);
		if (p_num_addr < 0 || p_num_addr > 4 || p_num_size < 0) {
			cvmx_dprintf("%s: Bad cell count for %s\n", __func__,
				     fdt_get_name(fdt_addr, node, NULL));
			return FDT_ADDR_T_NONE;
		}
		if (__cvmx_fdt_translate_one(fdt_addr, node, &addr64,
					     num_addr, num_size, p_num_addr,
					     "ranges"))
			return FDT_ADDR_T_NONE;
		num_addr = p_num_addr;
		num_size = p_num_size;
	} while (1);
}
#endif /* !defined(__U_BOOT__) */

/**
 * Given a phandle to a GPIO device return the type of GPIO device it is.
 *
 * @param[in]	fdt_addr	Address of flat device tree
 * @param	phandle		phandle to GPIO
 * @param[out]	size		Number of pins (optional, may be NULL)
 *
 * @return	Type of GPIO device or PIN_ERROR if error
 */
enum cvmx_gpio_type cvmx_fdt_get_gpio_type(const void *fdt_addr, int phandle,
					   int *size)
{
	int node = fdt_node_offset_by_phandle(fdt_addr, phandle);
	int i;
	enum cvmx_gpio_type type = CVMX_GPIO_PIN_OTHER;

	for (i = 0; i < (int)(sizeof(gpio_compat_list)/sizeof(gpio_compat_list[0])); i++) {
		if (!fdt_node_check_compatible(fdt_addr, node,
					       gpio_compat_list[i].compatible)) {
			if (size)
				*size = gpio_compat_list[i].size;
			type = gpio_compat_list[i].type;
			break;
		}
	}

	/* The Octeon CN73XX and CNF75XX have 32 GPIO pins whereas all other
	 * Octeon devices have either 16 or 20.
	 */
	if ((OCTEON_IS_MODEL(OCTEON_CN73XX) || OCTEON_IS_MODEL(OCTEON_CNF75XX))
	    && type == CVMX_GPIO_PIN_OCTEON)
		if (*size)
			*size = 32;

	debug("%s(%p, %d, %p): type: %d, size: %d, compatible: %s\n", __func__,
	      fdt_addr, phandle, size, type, size ? *size : -1,
	      type != CVMX_GPIO_PIN_OTHER ? gpio_compat_list[i].compatible :
	      "Unknown");
	return type;
}

static int cvmx_octeon_i2c_reg_to_bus(const void *fdt_addr, int of_offset)
{
	fdt_addr_t addr = cvmx_fdt_get_addr(fdt_addr, of_offset, "reg");
	uint32_t addr32[2];
	int bus_num = 0;

	if (addr == FDT_ADDR_T_NONE) {
		cvmx_dprintf("%s: Could not get reg address\n", __func__);
		cvmx_dprintf("%s: reg: 0x%x\n", __func__,
			     cvmx_fdt_get_int(fdt_addr, of_offset, "reg", -1));
		return -1;
	}
	switch (addr) {
	case 0x1180000001000ULL:
		bus_num = 0;
		break;
	case 0x1180000001100ULL:
		if (OCTEON_IS_MODEL(OCTEON_CNF75XX))
			bus_num = 1;
		break;
	case 0x1180000001200ULL:
		if (OCTEON_IS_MODEL(OCTEON_CNF75XX))
			bus_num = 2;
		else
			bus_num = 1;
		break;
	case FDT_ADDR_T_NONE:
		puts("Error: i2c node in device tree missing \"reg\" field.\n");
		cvmx_dprintf("Node name: %s\n",
			     fdt_get_name(fdt_addr, of_offset, NULL));
		return -1;
	default:
		cvmx_dprintf("Invalid i2c CSR address 0x%llx in device tree\n",
			     (unsigned long long)addr);
		return -1;
	}
	addr32[0] = addr >> 32;
	addr32[1] = addr & 0xffffffff;
	addr = cvmx_fdt_translate_address(fdt_addr, of_offset, addr32);
	bus_num |= (cvmx_csr_addr_to_node(addr) << 1);
	return bus_num;
}

/**
 * Given the parent offset of an i2c device build up a list describing the bus
 * which can contain i2c muxes and switches.
 *
 * @param[in]	fdt_addr	address of device tree
 * @param	of_offset	Offset of the parent node of a GPIO device in
 *				the device tree.
 *
 * @return	pointer to list of i2c devices starting from the root which
 *		can include i2c muxes and switches or NULL if error.  Note that
 *		all entries are allocated on the heap.
 *
 * @see cvmx_fdt_free_i2c_bus()
 */
struct cvmx_fdt_i2c_bus_info *cvmx_fdt_get_i2c_bus(const void *fdt_addr,
						   int of_offset)
{
	unsigned i;
	struct cvmx_fdt_i2c_bus_info *businfo = NULL;
	struct cvmx_fdt_i2c_bus_info *last_businfo = NULL;
	struct cvmx_fdt_i2c_bus_info *first_businfo = NULL;
	int parent;
	debug("%s(%p, %d): Getting i2c bus for %s\n",
	      __func__, fdt_addr, of_offset,
	      fdt_get_name(fdt_addr, of_offset, NULL));

	do {
		last_businfo = businfo;
		businfo = cvm_fdt_alloc(sizeof(*businfo));
		if (!first_businfo)
			first_businfo = businfo;
		if (last_businfo)
			last_businfo->parent = businfo;
		if (!businfo) {
			cvmx_dprintf("Out of memory\n");
			return NULL;
		}
		businfo->child = last_businfo;
		if (!fdt_node_check_compatible(fdt_addr, of_offset,
					       "cavium,octeon-7890-twsi") ||
		    !fdt_node_check_compatible(fdt_addr, of_offset,
					       "cavium,octeon-3860-twsi")) {
			debug("%s: Found %s at offset %d\n", __func__,
			      (char *)fdt_getprop(fdt_addr, of_offset,
						  "compatible", NULL),
			      of_offset);
			businfo->of_offset = of_offset;
			businfo->type = CVMX_I2C_BUS_OCTEON;
			businfo->i2c_addr = -1;
			businfo->channel = cvmx_octeon_i2c_reg_to_bus(fdt_addr,
								      of_offset);
			return businfo;
		}
		parent = fdt_parent_offset(fdt_addr, of_offset);
		if (parent < 0) {
			cvmx_dprintf("%s: Error: did not find root i2c bus\n",
				     __func__);
			break;
		}
		if (!fdt_node_check_compatible(fdt_addr, parent,
					       "cavium,octeon-7890-twsi") ||
			!fdt_node_check_compatible(fdt_addr, parent,
						   "cavium,octeon-3860-twsi")) {
			businfo->of_offset = of_offset;
			businfo->type = CVMX_I2C_BUS_OCTEON;
			businfo->i2c_addr = -1;
			businfo->channel =
			cvmx_octeon_i2c_reg_to_bus(fdt_addr, parent);
			debug("%s: Found %s at offset %d\n", __func__,
			      (char *)fdt_getprop(fdt_addr, of_offset,
						  "compatible", NULL),
			      of_offset);
			return businfo;
		}
		for (i = 0; i < ARRAY_SIZE(mux_compat_list); i++) {
			if (!fdt_node_check_compatible(fdt_addr, parent,
					     mux_compat_list[i].compatible)) {
				businfo->of_offset = parent;
				businfo->type = mux_compat_list[i].type;
				businfo->channel = cvmx_fdt_get_int(fdt_addr,
								     of_offset,
								     "reg", -1);
				businfo->i2c_addr = cvmx_fdt_get_int(fdt_addr,
								     parent,
								     "reg", -1);
				of_offset = fdt_parent_offset(fdt_addr, parent);
				businfo->is_mux =
					mux_compat_list[i].mux_type == I2C_MUX;
				businfo->enable_bit =
					mux_compat_list[i].enable;
				debug("%s: Found %s at offset %d, channel %d at i2c address 0x%x\n", __func__,
				      mux_compat_list[i].compatible, parent,
				      businfo->channel, businfo->i2c_addr);
				break;
			}
		}
		if (i == ARRAY_SIZE(mux_compat_list)) {
			cvmx_dprintf("%s: Error: could not find match for mux type %s\n",
				     __func__,
				     (const char *)fdt_getprop(fdt_addr, parent,
							       "compatible",
							       NULL));
			return NULL;
		}
	} while (of_offset >= 0);

	cvmx_dprintf("%s: Something bad happened\n", __func__);
	cvmx_fdt_free_i2c_bus(businfo);
	return NULL;
}

static void cvmx_fdt_i2c_reg_write(int bus, int addr, uint8_t val)
{
	debug("%s(%d, 0x%x) data: 0x%x\n", __func__, bus, addr, val);
#ifdef __U_BOOT__
	i2c_set_bus_num(bus);
	i2c_write(addr, 0, 0, &val, 1);
#else
	cvmx_twsix_write(bus, addr, 1, (uint64_t)val);
#endif
}

#ifdef __U_BOOT__
static int cvmx_fdt_i2c_reg_read(int bus, int addr)
{
	int err;
	uint8_t buffer[1];

	i2c_set_bus_num(bus);
	err = i2c_read(addr, 0, 0, buffer, 1);
	if (err)
		return -1;
	else
		return buffer[0];
}
#else
static int cvmx_fdt_i2c_reg_read(int bus, int addr)
{
	int err;
	uint64_t data;

	err = cvmx_twsix_read(bus, addr, 1, &data);
	debug("%s(%d, 0x%x) err: %d, data: 0x%llx\n", __func__, bus, addr,
	      err, (unsigned long long)data);
	if (err < 0)
		return -1;
	else
		return data & 0xff;
}
#endif

static int cvmx_fdt_i2c_write8(int bus, int addr, int reg, uint8_t val)
{
	debug("%s(%d, 0x%x, 0x%x, 0x%x)\n", __func__, bus, addr, reg, val);
#ifdef __U_BOOT__
	i2c_set_bus_num(bus);
	i2c_reg_write(addr, reg, val);
	return 0;
#else
	return cvmx_twsix_write_ia(bus, addr, reg, 1, 1, val);
#endif
}

static int cvmx_fdt_i2c_read8(int bus, int addr, int reg)
{
#ifdef __U_BOOT__
	i2c_set_bus_num(bus);
	return i2c_reg_read(addr, reg);
#else
	return cvmx_twsix_read_ia8(bus, addr, reg, 1);
#endif
}

static int __cvmx_fdt_disable_i2c_bus(const struct cvmx_fdt_i2c_bus_info *bus)
{
	int octbus;

	octbus = bus->channel;

	/* Start at the end and work backwards */
	while (bus->child)
		bus = bus->child;

	do {
		if (bus->type == CVMX_I2C_BUS_OCTEON) {
#ifdef __U_BOOT__
			debug("%s: Returning Octeon bus to 0\n", __func__);
			i2c_set_bus_num(0);
#endif
		} else if (bus->is_mux) {
			debug("%s: disabling mux channel %d at address 0x%x\n",
			      __func__, bus->channel, bus->i2c_addr);
			cvmx_fdt_i2c_reg_write(octbus, bus->i2c_addr,
					       bus->channel | 0);
		} else {
			uint8_t val;

			debug("%s: disabling switch bit %d at address 0x%x\n",
			      __func__, bus->channel, bus->i2c_addr);
			val = cvmx_fdt_i2c_reg_read(octbus, bus->i2c_addr);
			val &= ~(1 << bus->channel);
			cvmx_fdt_i2c_reg_write(octbus, bus->i2c_addr, val);

		}
		bus = bus->parent;
	} while (bus);

	return 0;
}

/**
 * Given the bus to a device, enable it.
 *
 * @param[in]	bus	i2c bus descriptor to enable or disable
 * @param	enable	set to true to enable, false to disable
 *
 * @return	0 for success or -1 for invalid bus
 *
 * This enables the entire bus including muxes and switches in the path.
 */
int cvmx_fdt_enable_i2c_bus(const struct cvmx_fdt_i2c_bus_info *bus,
			    bool enable)
{
	int octbus;

	if (!bus) {
		debug("%s: Error: NULL bus\n", __func__);
		return -1;
	}

	if (bus->type != CVMX_I2C_BUS_OCTEON) {
		debug("%s: Root bus is not Octeon\n", __func__);
		return -1;
	}
	if (!enable)
		return __cvmx_fdt_disable_i2c_bus(bus);

	octbus = bus->channel;

	do {
		if (bus->type == CVMX_I2C_BUS_OCTEON) {
#ifdef __U_BOOT__
			debug("%s: enabling native i2c bus %d\n", __func__,
			      bus->channel);
			i2c_set_bus_num(bus->channel);
#endif
		} else if (bus->is_mux) {
			debug("%s: enabling mux channel %d at address %d:0x%x\n",
			      __func__, bus->channel, octbus, bus->i2c_addr);
			debug("%s: Writing 0x%x\n", __func__,
			      bus->channel | bus->enable_bit);
			cvmx_fdt_i2c_reg_write(octbus, bus->i2c_addr,
					       bus->channel | bus->enable_bit);
		} else {
			uint8_t val;

			debug("%s: enabling switch bit %d at address %d:0x%x\n",
			      __func__, bus->channel, octbus, bus->i2c_addr);
			val = cvmx_fdt_i2c_reg_read(octbus, bus->i2c_addr);
			val |= 1 << bus->channel;
			debug("%s: Writing 0x%x\n", __func__, val);
			cvmx_fdt_i2c_reg_write(octbus, bus->i2c_addr, val);
		}
		bus = bus->child;
	} while (bus);
	return 0;
}

/**
 * Return the Octeon bus number for a bus descriptor
 *
 * @param[in]	bus	bus descriptor
 *
 * @return	Octeon twsi bus number or -1 on error
 */
int cvmx_fdt_i2c_get_root_bus(const struct cvmx_fdt_i2c_bus_info *bus)
{
	if (bus->type != CVMX_I2C_BUS_OCTEON)
		return -1;
	return bus->channel;
}

/**
 * Frees all entries for an i2c bus descriptor
 *
 * @param	bus	bus to free
 *
 * @return	0
 */
int cvmx_fdt_free_i2c_bus(struct cvmx_fdt_i2c_bus_info *bus)
{
	struct cvmx_fdt_i2c_bus_info *last;

	while (bus) {
		last = bus;
		bus = bus->child;
		cvm_fdt_free(last, sizeof(*last));
	}
	return 0;
}

/**
 * Given a phandle to a GPIO node output the i2c bus and address
 *
 * @param[in]	fdt_addr	Address of FDT
 * @param	phandle		phandle of GPIO device
 * @param[out]	bus		TWSI bus number with node in bits 1-3, can be
 * 				NULL for none.
 * @param[out]	addr		TWSI address number, can be NULL for none
 *
 * @return	0 for success, error otherwise
 */
int cvmx_fdt_get_twsi_gpio_bus_addr(const void *fdt_addr, int phandle,
				    int *bus, int *addr)
{
	int node = fdt_node_offset_by_phandle(fdt_addr, phandle);
	int parent = fdt_parent_offset(fdt_addr, node);
	int reg;
	uint64_t addr64;

	if (node < 0 || parent < 0) {
		cvmx_dprintf("%s: Invalid phandle 0x%x\n", __func__, phandle);
		return -1;
	}

	if (fdt_node_check_compatible(fdt_addr, parent,
				      "cavium,octeon-3860-twsi") &&
	    fdt_node_check_compatible(fdt_addr, parent,
				      "cavium,octeon-7890-twsi")) {
		cvmx_dprintf("%s: Incompatible parent \"%s\", must be native OCTEON i2c/twsi bus\n",
			     __func__, (char *)fdt_getprop(fdt_addr, parent,
							   "compatible", NULL));
		return -1;
	}

	if (addr) {
		reg = cvmx_fdt_get_int(fdt_addr, node, "reg", -1);
		if (reg < 0)
			return reg;
		*addr = reg;
	}
	if (bus) {
		addr64 = cvmx_fdt_get_addr(fdt_addr, parent, "reg");
		addr64 = cvmx_fdt_translate_address(fdt_addr, parent,
						    (uint32_t *)&addr64);
		if (addr64 == FDT_ADDR_T_NONE) {
			cvmx_dprintf("Cannot get TWSI node address\n");
			return -1;
		}

		node = cvmx_csr_addr_to_node(addr64);
		switch (cvmx_csr_addr_strip_node(addr64)) {
		case 0x1180000001000:
			*bus = node * 2;
			break;
		case 0x1180000001100:
			if (OCTEON_IS_MODEL(OCTEON_CNF75XX))
				*bus = 1;
			else
				return -1;
			break;
		case 0x1180000001200:
			if (OCTEON_IS_MODEL(OCTEON_CNF75XX))
				*bus = 2;
			else
				*bus = (node * 2) + 1;
			break;
		default:
			cvmx_dprintf("Error: unknown TWSI node address 0x%llx\n",
				     (unsigned long long)addr64);
			return -1;
		}
	}
	return 0;
}

/**
 * Given a FDT node return the CPU node number
 *
 * @param[in]	fdt_addr	Address of FDT
 * @param	node		FDT node number
 *
 * @return	CPU node number or error if negative
 */
int cvmx_fdt_get_cpu_node(const void *fdt_addr, int node)
{
	int parent = node;
	const uint32_t *ranges;
	int len = 0;

	while (fdt_node_check_compatible(fdt_addr, parent, "simple-bus") != 0) {
		parent = fdt_parent_offset(fdt_addr, parent);
		if (parent < 0)
			return parent;
	}
	ranges = fdt_getprop(fdt_addr, parent, "ranges", &len);
	if (!ranges)
		return len;

	if (len == 0)
		return 0;

	if (len < 24)
		return -FDT_ERR_TRUNCATED;

	return fdt32_to_cpu(ranges[2]) / 0x10;
}

/**
 * Get the total size of the flat device tree
 *
 * @param[in]	fdt_addr	Address of FDT
 *
 * @return	Size of flat device tree in bytes or error if negative.
 */
int cvmx_fdt_get_fdt_size(const void *fdt_addr)
{
	int rc;

	rc = fdt_check_header(fdt_addr);
	if (rc)
		return rc;
	return fdt_totalsize(fdt_addr);
}

/**
 * Returns if a node is compatible with one of the items in the string list
 *
 * @param[in]	fdt_addr	Pointer to flat device tree
 * @param	node		Node offset to check
 * @param[in]	strlist		Array of FDT device compatibility strings,
 *				must end with NULL or empty string.
 *
 * @return	0 if at least one item matches, 1 if no matches
 */
int cvmx_fdt_node_check_compatible_list(const void *fdt_addr, int node,
				        const char * const *strlist)
{
	while (*strlist && **strlist) {
		if (!fdt_node_check_compatible(fdt_addr, node, *strlist))
			return 0;
		strlist++;
	}
	return 1;
}

/**
 * Given a FDT node, return the next compatible node.
 *
 * @param[in]	fdt_addr	Pointer to flat device tree
 * @param	start_offset	Starting node offset or -1 to find the first
 * @param	strlist		Array of FDT device compatibility strings, must
 *				end with NULL or empty string.
 *
 * @return	next matching node or -1 if no more matches.
 */
int cvmx_fdt_node_offset_by_compatible_list(const void *fdt_addr,
					    int startoffset,
					    const char * const *strlist)
{
	int offset;

	for (offset = fdt_next_node(fdt_addr, startoffset, NULL);
	     offset >= 0;
	     offset = fdt_next_node(fdt_addr, offset, NULL)) {
		if (!cvmx_fdt_node_check_compatible_list(fdt_addr, offset,
							 strlist))
			return offset;
	}
	return -1;
}

/**
 * Return a GPIO handle given a GPIO phandle of the form <&gpio pin flags>
 *
 * @param[in]	fdt_addr	Address of flat device tree
 * @param	of_offset	node offset for property
 * @param	prop_name	name of property
 *
 * @return	pointer to GPIO handle or NULL if error
 */
struct cvmx_fdt_gpio_info *cvmx_fdt_gpio_get_info_phandle(const void *fdt_addr,
							  int of_offset,
							  const char *prop_name)
{
	int phandle;
	int len;
	const struct fdt_property *gpio_prop;
	struct cvmx_fdt_gpio_info *gpio_info = NULL;
	const uint32_t *data;
	int node;
	int max_pin;

	debug("%s(%p, %d, %s)\n", __func__, fdt_addr, of_offset, prop_name);
	gpio_prop = fdt_get_property(fdt_addr, of_offset, prop_name, &len);
	if (!gpio_prop) {
		cvmx_dprintf("%s: Error: could not find property %s at node offset %d in the device tree\n",
			     __func__, prop_name, of_offset);
		return NULL;
	}

	debug("%s: len: %d\n", __func__, len);
	if (len != 3 * sizeof(uint32_t)) {
		cvmx_dprintf("%s: Error: GPIO entry size %d wrong, must contain phandle, pin and flags\n",
			     __func__, len);
		return NULL;
	}

	gpio_info = cvm_fdt_alloc(sizeof(*gpio_info));
	if (!gpio_info) {
		cvmx_dprintf("%s: Error: out of memory\n", __func__);
		return NULL;
	}

	data = (uint32_t *)gpio_prop->data;
	gpio_info->name = fdt_string(fdt_addr, fdt32_to_cpu(gpio_prop->nameoff));
	gpio_info->pin = fdt32_to_cpu(data[1]);
	if (octeon_has_feature(OCTEON_FEATURE_MULTINODE) &&
	    cvmx_fdt_get_cpu_node(fdt_addr, of_offset))
		gpio_info->pin += 20;
	gpio_info->flags = fdt32_to_cpu(data[2]);
	phandle = fdt32_to_cpu(data[0]);

	gpio_info->gpio_type = cvmx_fdt_get_gpio_type(fdt_addr, phandle, &len);
	gpio_info->num_pins = len;

	max_pin = octeon_has_feature(OCTEON_FEATURE_MULTINODE) ? 2 * len : len;
	if ((gpio_info->gpio_type == CVMX_GPIO_PIN_OCTEON) &&
	    (gpio_info->pin >= max_pin)) {
		cvmx_dprintf("%s: Error: pin %d for %s out of range, max: %d\n",
			     __func__, gpio_info->pin, gpio_info->name,
			     max_pin);
		goto error;
	}

	if (gpio_info->gpio_type == CVMX_GPIO_PIN_OCTEON)
		return gpio_info;

	node = fdt_node_offset_by_phandle(fdt_addr, phandle);
	if (node < 0) {
		cvmx_dprintf("%s: Invalid GPIO phandle for %s\n",
			     __func__, gpio_info->name);
		goto error;
	}
	gpio_info->i2c_addr = cvmx_fdt_get_int(fdt_addr, node, "reg", -1);
	if (gpio_info->i2c_addr < 0) {
		cvmx_dprintf("%s: GPIO controller reg field missing for %s\n",
			     __func__, gpio_info->name);
		goto error;
	}
	node = fdt_parent_offset(fdt_addr, node);

	gpio_info->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr, node);
	if (!gpio_info->i2c_bus) {
		cvmx_dprintf("%s: Invalid i2c bus for %s\n",
			     __func__, gpio_info->name);
		goto error;
	}

	return gpio_info;

error:
	if (gpio_info)
		cvm_fdt_free(gpio_info, sizeof(*gpio_info));
	return NULL;
}

/**
 * Sets a GPIO pin given the GPIO descriptor
 *
 * @param	pin	GPIO pin descriptor
 * @param	value	value to set it to, 0 or 1
 *
 * @return	0 on success, -1 on error.
 *
 * NOTE: If the CVMX_GPIO_ACTIVE_LOW flag is set then the output value will be
 * inverted.
 */
int cvmx_fdt_gpio_set(struct cvmx_fdt_gpio_info *pin, int value)
{
	int cpu_node = 0;
	uint8_t reg;
	uint8_t val8;
	int i2c_bus;
	int err;
#ifdef __U_BOOT__
	uint8_t buffer[2];
#endif
	debug("%s(%s, %d)\n", __func__, pin->name, value);

	if (pin->gpio_type == CVMX_GPIO_PIN_OCTEON) {
		int loc_pin = pin->pin;	/** local pin number */
		if (octeon_has_feature(OCTEON_FEATURE_MULTINODE)) {
			if (loc_pin > 19) {
				cpu_node = 1;
				loc_pin -= 20;
			}
		}
		if (!pin->dir_out || pin->dir_in) {
			debug("%s: Configuring Octeon pin %d as output\n",
			      __func__, loc_pin);
			/* Configure pin as output */
			cvmx_gpio_cfg_node(cpu_node, 1 << loc_pin, 1);
			pin->dir_out = true;
			pin->dir_in = false;
		}
		/* Set or clear the pin */
		debug("%s: %s pin %d\n", __func__,
		      value ? "setting" : "clearing", loc_pin);
		if (value)
			cvmx_gpio_set_node(cpu_node, 1 << loc_pin);
		else
			cvmx_gpio_clear_node(cpu_node, 1 << loc_pin);
		return 0;
	}
	/* If we're here then we're dealing with an i2c GPIO expander */
	debug("%s: Enabling i2c bus\n", __func__);
	err = cvmx_fdt_enable_i2c_bus(pin->i2c_bus, true);
	if (err) {
		cvmx_dprintf("%s: Error enabling i2c bus for %s\n",
			     __func__, pin->name);
		return -1;
	}
	i2c_bus = cvmx_fdt_i2c_get_root_bus(pin->i2c_bus);
	debug("%s: root i2c bus: %d\n", __func__, i2c_bus);
	switch (pin->gpio_type) {
	case CVMX_GPIO_PIN_PCF857X:
		/* There is no way to read back the previous value written to
		 * this device so a latch_out value is kept as a copy of the
		 * last value written.
		 */
		if (value || (!value && (pin->flags & CVMX_GPIO_ACTIVE_LOW)))
			pin->latch_out |= 1 << pin->pin;
		else
			pin->latch_out &= ~(1 << pin->pin);
		if (pin->num_pins <= 8) {
			cvmx_fdt_i2c_reg_write(i2c_bus, pin->i2c_addr,
					       pin->latch_out & 0xff);
		} else {
#ifdef __U_BOOT__
			buffer[0] = pin->latch_out & 0xff;
			buffer[1] = pin->latch_out >> 8;
			i2c_write(pin->i2c_addr, 0, 0, buffer, 2);
#else
			cvmx_twsix_write(i2c_bus, pin->i2c_addr, 2,
					 cvmx_cpu_to_le16(pin->latch_out));
#endif
		}
		debug("%s: %s pcf857x at 0x%x, pin %d\n",
		      __func__, value ? "setting" : "clearing", pin->i2c_addr,
		      pin->pin);
		break;
	case CVMX_GPIO_PIN_PCA953X:
	case CVMX_GPIO_PIN_PCA9698:
		/* First set direction to output */
		if (!pin->dir_out || pin->dir_in) {
			reg = GPIO_REG_PCA953X_DIR * ((pin->num_pins + 7) / 8);
			reg += pin->pin / 8;
			val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
			val8 &= ~(1 << (pin->pin & 7));
			cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
			debug("%s: Configuring PCA953x@0x%x pin %d as output, reg: 0x%x, val8: 0x%x\n",
			      __func__, pin->i2c_addr, pin->pin, reg, val8);
			pin->dir_out = true;
			pin->dir_in = false;
		}
		/* Now set the output value */
		reg = GPIO_REG_PCA953X_OUT * ((pin->num_pins + 7) / 8);
		reg += pin->pin / 8;
		val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
		debug("%s: pin %d active %s, value: %d\n", __func__, pin->pin,
		      pin->flags & CVMX_GPIO_ACTIVE_LOW ? "low" : "high", value);
		if (((pin->flags & CVMX_GPIO_ACTIVE_LOW) && value) || !value)
			val8 &= ~(1 << (pin->pin & 7));
		else
			val8 |= (1 << (pin->pin & 7));
		cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
		debug("%s: PCA953X: wrote 0x%x to reg 0x%x at i2c address 0x%x\n",
		      __func__, val8, reg, pin->i2c_addr);
		break;
	case CVMX_GPIO_PIN_PCA957X:
		/* First set the pin as an output */
		if (!pin->dir_out || pin->dir_in) {
			debug("%s: Setting PCA957x pin %d as output\n",
			      __func__, pin->pin);
			reg = GPIO_REG_PCA957X_CFG * ((pin->num_pins + 7) / 8) +
				pin->pin / 8;
			val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
			val8 &= ~(1 << (pin->pin & 7));
			cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
			pin->dir_out = true;
			pin->dir_in = false;
		}
		/* Now set/clear the pin */
		reg = GPIO_REG_PCA957X_OUT * ((pin->num_pins + 7) / 8) +
			pin->pin / 8;
		val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
		debug("%s: Read 0x%x from PCA957x register 0x%x at 0x%x\n",
		      __func__, val8, reg, pin->i2c_addr);
		if (((pin->flags & CVMX_GPIO_ACTIVE_LOW) && value) || !value)
			val8 &= ~(1 << (pin->pin & 7));
		else
			val8 |= (1 << (pin->pin & 7));
		debug("%s: Writing back value 0x%x for pin %d\n", __func__,
		      val8, pin->pin);
		cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
		break;

	default:
		cvmx_dprintf("%s(%s): Unknown GPIO type\n", __func__,
			     pin->name);
		break;
	}
	debug("%s: Disabling i2c bus\n", __func__);
	cvmx_fdt_enable_i2c_bus(pin->i2c_bus, false);
	return 0;
}

/**
 * Attaches a PHY to a SFP or QSFP.
 *
 * @param	sfp		sfp to attach PHY to
 * @param	phy_info	phy descriptor to attach or NULL to detach
 */
void cvmx_sfp_attach_phy(struct cvmx_fdt_sfp_info *sfp,
			 struct cvmx_phy_info *phy_info)
{
	sfp->phy_info = phy_info;
	if (phy_info)
		phy_info->sfp_info = sfp;
}

/**
 * Given a GPIO pin descriptor, input the value of that pin
 *
 * @param	pin	GPIO pin descriptor
 *
 * @return	0 if low, 1 if high, -1 on error.  Note that the input will be
 *		inverted if the CVMX_GPIO_ACTIVE_LOW flag bit is set.
 */
int cvmx_fdt_gpio_get(struct cvmx_fdt_gpio_info *pin)
{
#ifndef __U_BOOT__
	uint64_t val64;
#else
	uint8_t buffer[2];
#endif
	int err = 0;
	int i2c_bus;
	uint8_t val8 = 0;
	uint8_t reg;

	if (pin->gpio_type == CVMX_GPIO_PIN_OCTEON) {
		int loc_pin = pin->pin;
		int cpu_node = 0;

		if (OCTEON_IS_MODEL(OCTEON_CN78XX))
			/* Can have two nodes */
			if (loc_pin > 19) {
				cpu_node = 1;
				loc_pin -= 20;
			}
		/* Configure pin as input */
		if (pin->dir_out || !pin->dir_in) {
			cvmx_gpio_cfg_node(cpu_node, 1 << loc_pin, 0);
			pin->dir_out = false;
			pin->dir_in = true;
		}
		/* Invert as needed */
		if (pin->flags & CVMX_GPIO_ACTIVE_LOW && !pin->invert_set) {
			cvmx_gpio_invert_input(cpu_node, loc_pin, 1);
			pin->invert_set = true;
			pin->no_invert_set = false;
		} else if (!pin->no_invert_set) {
			cvmx_gpio_invert_input(cpu_node, loc_pin, 0);
			pin->no_invert_set = true;
			pin->invert_set = false;
		}
		return !!(cvmx_gpio_read_node(cpu_node) & (1 << loc_pin));
	}
	/* If we're here then we're dealing with an i2c GPIO expander */
	err = cvmx_fdt_enable_i2c_bus(pin->i2c_bus, true);
	if (err) {
		cvmx_dprintf("%s: Error enabling i2c bus for %s\n",
			     __func__, pin->name);
		return -1;
	}
	i2c_bus = cvmx_fdt_i2c_get_root_bus(pin->i2c_bus);

	switch (pin->gpio_type) {
	case CVMX_GPIO_PIN_PCF857X:
		if (pin->pin < 8) {
			val8 = cvmx_fdt_i2c_reg_read(i2c_bus, pin->i2c_addr);
			val8 = (val8 >> pin->pin) & 1;
		} else {
#ifdef __U_BOOT__
			err = i2c_read(pin->i2c_addr, 0, 0, buffer, 2);
			if (err < 0)
				goto error;
			val8 = (buffer[pin->pin / 8] >> (pin->pin & 7)) & 1;
#else
			err = cvmx_twsix_read(i2c_bus, pin->i2c_addr,
					      2, &val64);
			if (err < 0)
				goto error;
			val8 = (cvmx_le16_to_cpu(val64) >> pin->pin) & 1;
#endif
			if (pin->flags & CVMX_GPIO_ACTIVE_LOW)
				val8 ^= 1;
		}
		break;

	case CVMX_GPIO_PIN_PCA953X:
	case CVMX_GPIO_PIN_PCA9698:
		/* First set direction to input */
		if (pin->dir_out || !pin->dir_in) {
			reg = GPIO_REG_PCA953X_DIR *
					((pin->num_pins + 7) / 8);
			reg += pin->pin / 8;
			val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
			val8 |= (1 << (pin->pin & 7));
			cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
			pin->dir_out = false;
			pin->dir_in = true;
		}
		/* Set inversion */
		if ((pin->flags & CVMX_GPIO_ACTIVE_LOW && !pin->invert_set) ^
		    !pin->no_invert_set) {
			reg = GPIO_REG_PCA953X_INVERT *
				((pin->num_pins + 7) / 8);
			reg += pin->pin / 8;
			val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
			if (pin->flags & CVMX_GPIO_ACTIVE_LOW)
				val8 |= 1 << (pin->pin & 7);
			else
				val8 &= ~(1 << (pin->pin & 7));
			cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
		}

		/* Now get the input value */
		reg = GPIO_REG_PCA953X_IN * ((pin->num_pins + 7) / 8);
		reg += pin->pin / 8;
		val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
		val8 = (val8 >> pin->pin & 7) & 1;
		break;

	case CVMX_GPIO_PIN_PCA957X:
			/* First set the pin as an input */
		if (pin->dir_out || !pin->dir_in) {
			reg = GPIO_REG_PCA957X_CFG * ((pin->num_pins + 7) / 8) +
				pin->pin / 8;
			val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
			val8 |= (1 << (pin->pin & 7));
			cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
			pin->dir_out = false;
			pin->dir_in = true;
		}

		/* Set inversion */
		if ((pin->flags & CVMX_GPIO_ACTIVE_LOW && !pin->invert_set) ^
		    !pin->no_invert_set) {
			reg = GPIO_REG_PCA957X_INVERT *
				((pin->num_pins + 7) / 8);
			reg += pin->pin / 8;
			val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
			if (pin->flags & CVMX_GPIO_ACTIVE_LOW)
				val8 |= 1 << (pin->pin & 7);
			else
				val8 &= ~(1 << (pin->pin & 7));
			cvmx_fdt_i2c_write8(i2c_bus, pin->i2c_addr, reg, val8);
		}

		/* Now get the pin */
		reg = GPIO_REG_PCA957X_IN * ((pin->num_pins + 7) / 8) +
				pin->pin / 8;
		val8 = cvmx_fdt_i2c_read8(i2c_bus, pin->i2c_addr, reg);
		val8 = (val8 >> (pin->pin & 7)) & 1;
		break;

	default:
		cvmx_dprintf("%s(%s): Unknown GPIO type\n", __func__,
			     pin->name);
		return -1;
	}

error:
	cvmx_fdt_enable_i2c_bus(pin->i2c_bus, false);
	return (err < 0) ? err : val8;
}

/**
 * Assigns an IPD port to a SFP slot
 *
 * @param	sfp		Handle to SFP data structure
 * @param	ipd_port	Port to assign it to
 *
 * @return	0 for success, -1 on error
 */
int cvmx_sfp_set_ipd_port(struct cvmx_fdt_sfp_info *sfp, int ipd_port)
{
	int i;

	if (sfp->is_qsfp) {
		int xiface;
		cvmx_helper_interface_mode_t mode;
		xiface = cvmx_helper_get_interface_num(ipd_port);
		mode = cvmx_helper_interface_get_mode(xiface);
		sfp->ipd_port[0] = ipd_port;

		switch (mode) {
		case CVMX_HELPER_INTERFACE_MODE_SGMII:
		case CVMX_HELPER_INTERFACE_MODE_XFI:
		case CVMX_HELPER_INTERFACE_MODE_10G_KR:
			for (i = 1; i < 4; i++)
				sfp->ipd_port[i] =
					cvmx_helper_get_ipd_port(xiface, i);
			break;
		case CVMX_HELPER_INTERFACE_MODE_XLAUI:
		case CVMX_HELPER_INTERFACE_MODE_40G_KR4:
			sfp->ipd_port[0] = ipd_port;
			for (i = 1; i < 4; i++)
				sfp->ipd_port[i] = -1;
			break;
		default:
			cvmx_dprintf("%s: Interface mode %s for interface 0x%x, ipd_port %d not supported for QSFP\n",
				     __func__,
				     cvmx_helper_interface_mode_to_string(mode),
				     xiface, ipd_port);
			return -1;
		}
	} else {
		sfp->ipd_port[0] = ipd_port;
		for (i = 1; i < 4; i++)
			sfp->ipd_port[i] = -1;
	}
	return 0;
}

/**
 * Parses all of the channels assigned to a VSC7224 device
 *
 * @param[in]		fdt_addr	Address of flat device tree
 * @param		of_offset	Offset of vsc7224 node
 * @param[in,out]	vsc7224		Data structure to hold the data
 *
 * @return	0 for success, -1 on error
 */
static int cvmx_fdt_parse_vsc7224_channels(const void *fdt_addr, int of_offset,
					   struct cvmx_vsc7224 *vsc7224)
{
	int parent_offset = of_offset;
	int err = 0;
	int reg;
	int num_chan = 0;
	struct cvmx_vsc7224_chan *channel;
	struct cvmx_fdt_sfp_info *sfp_info;
	int len;
	int num_taps;
	int i;
	const uint32_t *tap_values;
	int of_mac;
	int xiface, index;
	bool is_tx;
	bool is_qsfp __attribute__((unused));
	const char *mac_str;

	debug("%s(%p, %d, %s)\n", __func__, fdt_addr, of_offset, vsc7224->name);
	do {
		/* Walk through all channels */
		of_offset = fdt_node_offset_by_compatible(fdt_addr, of_offset,
							  "vitesse,vsc7224-channel");
		if (of_offset == -FDT_ERR_NOTFOUND)
			break;
		else if (of_offset < 0) {
			cvmx_dprintf("%s: Failed finding compatible channel\n",
				     __func__);
			err = -1;
			break;
		}
		if (fdt_parent_offset(fdt_addr, of_offset) != parent_offset)
			break;
		reg = cvmx_fdt_get_int(fdt_addr, of_offset, "reg", -1);
		if (reg < 0 || reg > 3) {
			cvmx_dprintf("%s: channel reg is either not present or out of range\n",
				     __func__);
			err = -1;
			break;
		}
		is_tx = cvmx_fdt_get_bool(fdt_addr, of_offset, "direction-tx");

		debug("%s(%s): Adding %cx channel %d\n", __func__,
		      vsc7224->name, is_tx ? 't' : 'r', reg);
		tap_values = (const uint32_t *)fdt_getprop(fdt_addr, of_offset,
							   "taps", &len);
		if (!tap_values) {
			cvmx_dprintf("%s: Error: no taps defined for vsc7224 channel %d\n",
				     __func__, reg);
			err = -1;
			break;
		}

		if (vsc7224->channel[reg]) {
			cvmx_dprintf("%s: Error: channel %d already assigned at %p\n",
				     __func__, reg, vsc7224->channel[reg]);
			err = -1;
			break;
		}
		if (len % 16) {
			cvmx_dprintf("%s: Error: tap format error for channel %d\n",
				     __func__, reg);
			err = -1;
			break;
		}
		num_taps = len / 16;
		debug("%s: Adding %d taps\n", __func__, num_taps);

		channel = cvm_fdt_alloc(sizeof(*channel) +
					num_taps * sizeof(struct cvmx_vsc7224_tap));
		if (!channel) {
			cvmx_dprintf("%s: Out of memory\n", __func__);
			err = -1;
			break;
		}
		vsc7224->channel[reg] = channel;
		channel->num_taps = num_taps;
		channel->lane = reg;
		channel->of_offset = of_offset;
		channel->is_tx = is_tx;
		channel->pretap_disable = cvmx_fdt_get_bool(fdt_addr, of_offset,
							    "pretap-disable");
		channel->posttap_disable = cvmx_fdt_get_bool(fdt_addr,
							     of_offset,
							     "posttap-disable");
		channel->vsc7224 = vsc7224;
		/* Read all the tap values */
		for (i = 0; i < num_taps; i++) {
			channel->taps[i].len =
					fdt32_to_cpu(tap_values[i * 4 + 0]);
			channel->taps[i].main_tap =
					fdt32_to_cpu(tap_values[i * 4 + 1]);
			channel->taps[i].pre_tap =
					fdt32_to_cpu(tap_values[i * 4 + 2]);
			channel->taps[i].post_tap =
					fdt32_to_cpu(tap_values[i * 4 + 3]);
			debug("%s: tap %d: len: %d, main_tap: 0x%x, pre_tap: 0x%x, post_tap: 0x%x\n",
			      __func__, i, channel->taps[i].len,
			      channel->taps[i].main_tap,
			      channel->taps[i].pre_tap,
			      channel->taps[i].post_tap);
		}
		/* Now find out which interface it's mapped to */
		channel->ipd_port = -1;

		mac_str = "sfp-mac";
		if (fdt_getprop(fdt_addr, of_offset, mac_str, NULL)) {
			is_qsfp = false;
		} else if (fdt_getprop(fdt_addr, of_offset, "qsfp-mac", NULL)) {
			is_qsfp = true;
			mac_str = "qsfp-mac";
		} else {
			cvmx_dprintf("%s: Error: MAC not found for %s channel %d\n",
				     __func__, vsc7224->name, reg);
			return -1;
		}
		of_mac = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, mac_str);
		if (of_mac < 0) {
			cvmx_dprintf("%s: Error %d with MAC %s phandle for %s\n",
				     __func__, of_mac, mac_str, vsc7224->name);
			return -1;
		}

		debug("%s: Found mac at offset %d\n", __func__, of_mac);
		err = cvmx_helper_cfg_get_xiface_index_by_fdt_node_offset(of_mac,
									  &xiface,
									  &index);
		if (!err) {
			channel->xiface = xiface;
			channel->index = index;
			channel->ipd_port = cvmx_helper_get_ipd_port(xiface,
								     index);

			debug("%s: Found MAC, xiface: 0x%x, index: %d, ipd port: %d\n",
			      __func__, xiface, index, channel->ipd_port);
			if (channel->ipd_port >= 0) {
				cvmx_helper_cfg_set_vsc7224_chan_info(xiface,
								      index,
								      channel);
				debug("%s: Storing config channel for xiface 0x%x, index %d\n",
				      __func__, xiface, index);
			}
			sfp_info = cvmx_helper_cfg_get_sfp_info(xiface, index);
			if (!sfp_info) {
				cvmx_dprintf("%s: Warning: no (Q)SFP+ slot found for xinterface 0x%x, index %d for channel %d\n",
					     __func__, xiface, index,
					     channel->lane);
				continue;
			}

			/* Link it */
			channel->next = sfp_info->vsc7224_chan;
			if (sfp_info->vsc7224_chan)
				sfp_info->vsc7224_chan->prev = channel;
			sfp_info->vsc7224_chan = channel;
			sfp_info->is_vsc7224 = true;
			debug("%s: Registering VSC7224 %s channel %d with SFP %s\n",
			      __func__, vsc7224->name,
			      channel->lane, sfp_info->name);
			if (!sfp_info->mod_abs_changed) {
				debug("%s: Registering cvmx_sfp_vsc7224_mod_abs_changed at %p for xinterface 0x%x, index %d\n",
				      __func__,
				      &cvmx_sfp_vsc7224_mod_abs_changed,
				      xiface, index);
				cvmx_sfp_register_mod_abs_changed(sfp_info,
					&cvmx_sfp_vsc7224_mod_abs_changed,
					NULL);
			}
		}
	} while (!err && num_chan < 4);

	return err;
}

/**
 * @INTERNAL
 * Parses all instances of the Vitesse VSC7224 reclocking chip
 *
 * @param[in]	fdt_addr	Address of flat device tree
 *
 * @return	0 for success, error otherwise
 */
int __cvmx_fdt_parse_vsc7224(const void *fdt_addr)
{
	int of_offset = -1;
	struct cvmx_vsc7224 *vsc7224 = NULL;
	struct cvmx_fdt_gpio_info *gpio_info = NULL;
	int err = 0;
	int of_parent;
	static bool parsed = false;

	debug("%s(%p)\n", __func__, fdt_addr);

	if (parsed) {
		debug("%s: Already parsed\n", __func__);
		return 0;
	}
	do {
		of_offset = fdt_node_offset_by_compatible(fdt_addr, of_offset,
							  "vitesse,vsc7224");
		debug("%s: of_offset: %d\n", __func__, of_offset);
		if (of_offset == -FDT_ERR_NOTFOUND)
			break;
		else if (of_offset < 0) {
			err = -1;
			cvmx_dprintf("%s: Error %d parsing FDT\n",
				     __func__, of_offset);
			break;
		}


		vsc7224 = cvm_fdt_alloc(sizeof(*vsc7224));

		if (!vsc7224) {
			cvmx_dprintf("%s: Out of memory!\n", __func__);
			return -1;
		}
		vsc7224->of_offset = of_offset;
		vsc7224->i2c_addr = cvmx_fdt_get_int(fdt_addr,of_offset,
						     "reg", -1);
		of_parent = fdt_parent_offset(fdt_addr, of_offset);
		vsc7224->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr, of_parent);
		if (vsc7224->i2c_addr < 0) {
			cvmx_dprintf("%s: Error: reg field missing\n",
				     __func__);
			err = -1;
			break;
		}
		if (!vsc7224->i2c_bus) {
			cvmx_dprintf("%s: Error getting i2c bus\n", __func__);
			err = -1;
			break;
		}
		vsc7224->name = fdt_get_name(fdt_addr, of_offset, NULL);
		debug("%s: Adding %s\n", __func__, vsc7224->name);
		if (fdt_getprop(fdt_addr, of_offset, "reset", NULL)) {
			gpio_info = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
								   of_offset,
								   "reset");
			vsc7224->reset_gpio = gpio_info;
		}
		if (fdt_getprop(fdt_addr, of_offset, "los", NULL)) {
			gpio_info = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
								   of_offset,
								   "los");
			vsc7224->los_gpio = gpio_info;
		}
		debug("%s: Parsing channels\n", __func__);
		err = cvmx_fdt_parse_vsc7224_channels(fdt_addr, of_offset,
						      vsc7224);
		if (err) {
			cvmx_dprintf("%s: Error parsing VSC7224 channels\n",
				     __func__);
			break;
		}
	} while (of_offset > 0);

	if (err) {
		debug("%s(): Error\n", __func__);
		if (vsc7224) {
			if (vsc7224->reset_gpio)
				cvm_fdt_free(vsc7224->reset_gpio,
					     sizeof(*vsc7224->reset_gpio));
			if (vsc7224->los_gpio)
				cvm_fdt_free(vsc7224->los_gpio,
					     sizeof(*vsc7224->los_gpio));
			if (vsc7224->i2c_bus)
				cvmx_fdt_free_i2c_bus(vsc7224->i2c_bus);
			cvm_fdt_free(vsc7224, sizeof(*vsc7224));
		}
	}
	if (!err)
		parsed = true;

	return err;
}

/**
 * Parse QSFP GPIOs for SFP
 *
 * @param[in]	fdt_addr	Pointer to flat device tree
 * @param	of_offset	Offset of QSFP node
 * @param[out]	sfp_info	Pointer to sfp info to fill in
 *
 * @return	0 for success
 */
static int cvmx_parse_qsfp(const void *fdt_addr, int of_offset,
			   struct cvmx_fdt_sfp_info *sfp_info)
{
	sfp_info->select = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							  "select");
	sfp_info->mod_abs = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							   "mod_prs");
	sfp_info->reset = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							 "reset");
	sfp_info->interrupt = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
							     of_offset,
							     "interrupt");
	sfp_info->lp_mode = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							   "lp_mode");
	return 0;
}

/**
 * Parse SFP GPIOs for SFP
 *
 * @param[in]	fdt_addr	Pointer to flat device tree
 * @param	of_offset	Offset of SFP node
 * @param[out]	sfp_info	Pointer to sfp info to fill in
 *
 * @return	0 for success
 */
static int cvmx_parse_sfp(const void *fdt_addr, int of_offset,
			  struct cvmx_fdt_sfp_info *sfp_info)
{
	sfp_info->mod_abs = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							   "mod_abs");
	sfp_info->rx_los = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							  "rx_los");
	sfp_info->tx_disable = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
							      of_offset,
							      "tx_disable");
	sfp_info->tx_error = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							    "tx_error");
	return 0;
}

/**
 * Parse SFP/QSFP EEPROM and diag
 *
 * @param[in]	fdt_addr	Pointer to flat device tree
 * @param	of_offset	Offset of SFP node
 * @param[out]	sfp_info	Pointer to sfp info to fill in
 *
 * @return	0 for success, -1 on error
 */
static int cvmx_parse_sfp_eeprom(const void *fdt_addr, int of_offset,
				 struct cvmx_fdt_sfp_info *sfp_info)
{
	int of_eeprom;
	int of_diag;

	debug("%s(%p, %d, %s)\n", __func__, fdt_addr, of_offset, sfp_info->name);
	of_eeprom = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, "eeprom");
	if (of_eeprom < 0) {
		cvmx_dprintf("%s: Missing \"eeprom\" from device tree for %s\n",
			     __func__, sfp_info->name);
		return -1;
	}

	sfp_info->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr,
						 fdt_parent_offset(fdt_addr,
								   of_eeprom));
	sfp_info->i2c_eeprom_addr = cvmx_fdt_get_int(fdt_addr, of_eeprom,
						     "reg", 0x50);
	if (!sfp_info->i2c_bus) {
		cvmx_dprintf("%s: Error: could not determine i2c bus for eeprom for %s\n",
			     __func__, sfp_info->name);
		return -1;
	}
	of_diag = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, "diag");
	if (of_diag >= 0)
		sfp_info->i2c_diag_addr = cvmx_fdt_get_int(fdt_addr, of_diag,
							   "reg", 0x51);
	else
		sfp_info->i2c_diag_addr = 0x51;
	return 0;
}

/**
 * Parse SFP information from device tree
 *
 * @param[in]	fdt_addr	Address of flat device tree
 *
 * @return pointer to sfp info or NULL if error
 */
struct cvmx_fdt_sfp_info *cvmx_helper_fdt_parse_sfp_info(const void *fdt_addr,
							 int of_offset)
{
	struct cvmx_fdt_sfp_info *sfp_info = NULL;
	int err = -1;
	bool is_qsfp;

	if (!fdt_node_check_compatible(fdt_addr, of_offset,
				       "ethernet,sfp-slot")) {
		is_qsfp = false;
	} else if (!fdt_node_check_compatible(fdt_addr, of_offset,
					      "ethernet,qsfp-slot")) {
		is_qsfp = true;
	} else  {
		cvmx_dprintf("%s: Error: incompatible sfp/qsfp slot, compatible=%s\n",
			     __func__, (char *)fdt_getprop(fdt_addr, of_offset,
							   "compatible", NULL));
		goto error_exit;
	}

	debug("%s: %ssfp module found at offset %d\n", __func__,
	      is_qsfp ? "q" : "", of_offset);
	sfp_info = cvm_fdt_alloc(sizeof(*sfp_info));
	if (!sfp_info) {
		cvmx_dprintf("%s: Error: out of memory\n", __func__);
		goto error_exit;
	}
	sfp_info->name = fdt_get_name(fdt_addr, of_offset, NULL);
	sfp_info->of_offset = of_offset;
	sfp_info->is_qsfp = is_qsfp;
	sfp_info->last_mod_abs = -1;
	sfp_info->last_rx_los = -1;

	if (is_qsfp)
		err = cvmx_parse_qsfp(fdt_addr, of_offset, sfp_info);
	else
		err = cvmx_parse_sfp(fdt_addr, of_offset, sfp_info);
	if (err) {
		cvmx_dprintf("%s: Error in %s parsing %ssfp GPIO info\n",
			     __func__, sfp_info->name,
			     is_qsfp ? "q" : "");
		goto error_exit;
	}
	debug("%s: Parsing %ssfp module eeprom\n", __func__, is_qsfp ? "q" : "");
	err = cvmx_parse_sfp_eeprom(fdt_addr, of_offset, sfp_info);
	if (err) {
		cvmx_dprintf("%s: Error parsing eeprom info for %s\n",
			     __func__, sfp_info->name);
		goto error_exit;
	}

	/* Register default check for mod_abs changed */
	if (!err)
		cvmx_sfp_register_check_mod_abs(sfp_info,
						cvmx_sfp_check_mod_abs, NULL);

error_exit:
	/* Note: we don't free any data structures on error since it gets
	 * rather complicated with i2c buses and whatnot.
	 */
	return err ? NULL : sfp_info;
}

/**
 * @INTERNAL
 * Parse a slice of the Inphi/Cortina CS4343 in the device tree
 *
 * @param[in]	fdt_addr	Address of flat device tree
 * @param	of_offset	fdt offset of slice
 * @param	phy_info	phy_info data structure
 *
 * @return	slice number if non-negative, otherwise error
 */
static int cvmx_fdt_parse_cs4343_slice(const void *fdt_addr, int of_offset,
				       struct cvmx_phy_info *phy_info)
{
	struct cvmx_cs4343_slice_info *slice;
	int reg;
	int reg_offset;

	reg = cvmx_fdt_get_int(fdt_addr, of_offset, "reg", -1);
	reg_offset = cvmx_fdt_get_int(fdt_addr, of_offset, "slice_offset", -1);

	if (reg < 0 || reg >= 4) {
		cvmx_dprintf("%s(%p, %d, %p): Error: reg %d undefined or out of range\n",
			     __func__, fdt_addr, of_offset, phy_info, reg);
		return -1;
	}
	if (reg_offset % 0x1000 || reg_offset > 0x3000 || reg_offset < 0) {
		cvmx_dprintf("%s(%p, %d, %p): Error: reg_offset 0x%x undefined or out of range\n",
			     __func__, fdt_addr, of_offset, phy_info,
			     reg_offset);
		return -1;
	}
	if (!phy_info->cs4343_info) {
		cvmx_dprintf("%s: Error: phy info cs4343 datastructure is NULL\n",
			     __func__);
		return -1;
	}
	debug("%s(%p, %d, %p): %s, reg: %d, slice offset: 0x%x\n", __func__,
	      fdt_addr, of_offset, phy_info,
	      fdt_get_name(fdt_addr, of_offset, NULL), reg, reg_offset);
	slice = &phy_info->cs4343_info->slice[reg];
	slice->name = fdt_get_name(fdt_addr, of_offset, NULL);
	slice->mphy = phy_info->cs4343_info;
	slice->phy_info = phy_info;
	slice->of_offset = of_offset;
	slice->slice_no = reg;
	slice->reg_offset = reg_offset;
	/* SR settings */
	slice->sr_stx_cmode_res = cvmx_fdt_get_int(fdt_addr, of_offset,
						   "sr-stx-cmode-res", 3);
	slice->sr_stx_drv_lower_cm = cvmx_fdt_get_int(fdt_addr, of_offset,
						      "sr-stx-drv-lower-cm", 8);
	slice->sr_stx_level = cvmx_fdt_get_int(fdt_addr, of_offset,
					       "sr-stx-level", 0x1c);
	slice->sr_stx_pre_peak = cvmx_fdt_get_int(fdt_addr, of_offset,
						  "sr-stx-pre-peak", 1);
	slice->sr_stx_muxsubrate_sel = cvmx_fdt_get_int(fdt_addr, of_offset,
							"sr-stx-muxsubrate-sel",
							0);
	slice->sr_stx_post_peak = cvmx_fdt_get_int(fdt_addr, of_offset,
						   "sr-stx-post-peak", 8);
	/* CX settings */
	slice->cx_stx_cmode_res = cvmx_fdt_get_int(fdt_addr, of_offset,
						   "cx-stx-cmode-res", 3);
	slice->cx_stx_drv_lower_cm = cvmx_fdt_get_int(fdt_addr, of_offset,
						      "cx-stx-drv-lower-cm", 8);
	slice->cx_stx_level = cvmx_fdt_get_int(fdt_addr, of_offset,
					       "cx-stx-level", 0x1c);
	slice->cx_stx_pre_peak = cvmx_fdt_get_int(fdt_addr, of_offset,
						  "cx-stx-pre-peak", 1);
	slice->cx_stx_muxsubrate_sel = cvmx_fdt_get_int(fdt_addr, of_offset,
							"cx-stx-muxsubrate-sel",
							0);
	slice->cx_stx_post_peak = cvmx_fdt_get_int(fdt_addr, of_offset,
						   "cx-stx-post-peak", 0xC);
	/* 1000Base-X settings */
	/* CX settings */
	slice->basex_stx_cmode_res = cvmx_fdt_get_int(fdt_addr, of_offset,
						      "basex-stx-cmode-res", 3);
	slice->basex_stx_drv_lower_cm = cvmx_fdt_get_int(fdt_addr, of_offset,
							 "basex-stx-drv-lower-cm",
							 8);
	slice->basex_stx_level = cvmx_fdt_get_int(fdt_addr, of_offset,
						  "basex-stx-level", 0x1c);
	slice->basex_stx_pre_peak = cvmx_fdt_get_int(fdt_addr, of_offset,
						     "basex-stx-pre-peak", 1);
	slice->basex_stx_muxsubrate_sel = cvmx_fdt_get_int(fdt_addr, of_offset,
							   "basex-stx-muxsubrate-sel",
							   0);
	slice->basex_stx_post_peak = cvmx_fdt_get_int(fdt_addr, of_offset,
						      "basex-stx-post-peak", 8);
	return reg;
}

/**
 * @INTERNAL
 * Parses either a CS4343 phy or a slice of the phy from the device tree
 * @param[in]	fdt_addr	Address of FDT
 * @param	of_offset	offset of slice or phy in device tree
 * @param	phy_info	phy_info data structure to fill in
 *
 * @return	0 for success, -1 on error
 */
int cvmx_fdt_parse_cs4343(const void *fdt_addr, int of_offset,
			  struct cvmx_phy_info *phy_info)
{
	int of_slice = -1;
	struct cvmx_cs4343_info *cs4343;
	int err = -1;
	int reg;

	debug("%s(%p, %d, %p): %s (%s)\n", __func__, fdt_addr, of_offset,
	      phy_info, fdt_get_name(fdt_addr, of_offset, NULL),
	      (const char *)fdt_getprop(fdt_addr, of_offset, "compatible", NULL));

	if (!phy_info->cs4343_info)
		phy_info->cs4343_info =
				cvm_fdt_alloc(sizeof(struct cvmx_cs4343_info));
	if (!phy_info->cs4343_info) {
		cvmx_dprintf("%s: Error: out of memory!\n", __func__);
		return -1;
	}
	cs4343 = phy_info->cs4343_info;

	/* If we're passed to a slice then process only that slice */
	if (!fdt_node_check_compatible(fdt_addr, of_offset,
				       "cortina,cs4343-slice")) {
		err = 0;
		of_slice = of_offset;
		of_offset = fdt_parent_offset(fdt_addr, of_offset);
		reg = cvmx_fdt_parse_cs4343_slice(fdt_addr, of_slice, phy_info);
		if (reg >= 0)
			phy_info->cs4343_slice_info = &cs4343->slice[reg];
		else
			err = reg;
	} else if (!fdt_node_check_compatible(fdt_addr, of_offset,
					      "cortina,cs4343")) {
		/* Walk through and process all of the slices */
		of_slice = fdt_node_offset_by_compatible(fdt_addr,
							 of_offset,
							 "cortina,cs4343-slice");
		while (of_slice > 0 &&
		       fdt_parent_offset(fdt_addr, of_slice) == of_offset) {
			debug("%s: Parsing slice %s\n", __func__,
			      fdt_get_name(fdt_addr, of_slice, NULL));
			err = cvmx_fdt_parse_cs4343_slice(fdt_addr, of_slice,
							  phy_info);
			if (err < 0)
				break;
			of_slice = fdt_node_offset_by_compatible(fdt_addr,
								 of_slice,
								 "cortina,cs4343-slice");
		}
	} else {
		cvmx_dprintf("%s: Error: unknown compatible string %s for %s\n",
			     __func__,
			     (const char *)fdt_getprop(fdt_addr, of_offset,
						       "compatible", NULL),
			     fdt_get_name(fdt_addr, of_offset, NULL));
	}
	if (err >= 0) {
		cs4343->name = fdt_get_name(fdt_addr, of_offset, NULL);
		cs4343->phy_info = phy_info;
		cs4343->of_offset = of_offset;
	}
	return err < 0 ? -1 : 0;
}
