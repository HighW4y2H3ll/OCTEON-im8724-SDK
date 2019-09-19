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
 * If compiled for U-Boot, just provide wrappers to the equivalent U-Boot
 * functions.
 */

#ifndef __CVMX_HELPER_FDT_H__
#ifdef __U_BOOT__
# include <common.h>
# include <libfdt.h>
# include <fdt_support.h>
# include <fdtdec.h>
# include <asm/arch/cvmx.h>
# include <asm/arch/cvmx-helper-sfp.h>
#else
# include "libfdt.h"
# include "../cvmx.h"
# include "../cvmx-helper-sfp.h"

typedef uint64_t fdt_addr_t;
typedef uint64_t fdt_size_t;

# define FDT_ADDR_T_NONE (-1ULL)
#endif

enum cvmx_phy_type;

/** Type of GPIO pin */
enum cvmx_gpio_type {
	CVMX_GPIO_PIN_OCTEON,	/** GPIO pin is directly connected to OCTEON */
	CVMX_GPIO_PIN_PCA953X,	/** GPIO pin is NXP PCA953X compat chip */
	CVMX_GPIO_PIN_PCA957X,
	CVMX_GPIO_PIN_PCF857X,	/** GPIO pin is NXP PCF857X compat chip */
	CVMX_GPIO_PIN_PCA9698,	/** GPIO pin is NXP PCA9698 compat chip */
	CVMX_GPIO_PIN_OTHER,	/** GPIO pin is something else */
};

enum cvmx_i2c_bus_type {
	CVMX_I2C_BUS_OCTEON,
	CVMX_I2C_MUX_PCA9540,
	CVMX_I2C_MUX_PCA9542,
	CVMX_I2C_MUX_PCA9543,
	CVMX_I2C_MUX_PCA9544,
	CVMX_I2C_MUX_PCA9545,
	CVMX_I2C_MUX_PCA9546,
	CVMX_I2C_MUX_PCA9547,
	CVMX_I2C_MUX_PCA9548,
	CVMX_I2C_MUX_OTHER
};

struct cvmx_sfp_mod_info;	/** Defined in cvmx-helper-sfp.h */
struct cvmx_phy_info;		/** Defined in cvmx-helper-board.h */

/**
 * This data structure holds information about various I2C muxes and switches
 * that may be between a device and the Octeon chip.
 */
struct cvmx_fdt_i2c_bus_info {
	/** Parent I2C bus, NULL if root */
	struct cvmx_fdt_i2c_bus_info *parent;
	/** Child I2C bus or NULL if last entry in the chain */
	struct cvmx_fdt_i2c_bus_info *child;
	/** Offset in device tree */
	int			of_offset;
	/** Type of i2c bus or mux */
	enum cvmx_i2c_bus_type	type;
	/** I2C address of mux */
	uint8_t			i2c_addr;
	/** Mux channel number */
	uint8_t			channel;
	/** For muxes, the bit(s) to set to enable them */
	uint8_t			enable_bit;
	/** True if mux, false if switch */
	bool			is_mux;
};

/**
 * GPIO flags associated with a GPIO pin (can be combined)
 */
enum cvmx_gpio_flags {
	CVMX_GPIO_ACTIVE_HIGH = 0,	/** Active high (default) */
	CVMX_GPIO_ACTIVE_LOW = 1,	/** Active low (inverted) */
	CVMX_GPIO_OPEN_COLLECTOR = 2,	/** Output is open-collector */
};

/**
 * GPIO descriptor
 */
struct cvmx_fdt_gpio_info {
	const char		*name;		/** Name of GPIO */
	int			pin;		/** GPIO pin number */
	enum cvmx_gpio_type	gpio_type;	/** Type of GPIO controller */
	int			of_offset;	/** Offset in device tree */
	struct cvmx_fdt_i2c_bus_info *i2c_bus;	/** I2C bus descriptor */
	int			i2c_addr;	/** Address on i2c bus */
	enum cvmx_gpio_flags	flags;		/** Flags associated with pin */
	int			num_pins;	/** Total number of pins */
	unsigned		latch_out;	/** Latched output for 857x */
	/* Two values are used to detect the initial case where nothing has
	 * been configured.  Initially, all of the following will be false
	 * which will force the initial state to be properly set.
	 */
	/** Set if configured to invert */
	bool			invert_set;
	/** Set if configured to not invert */
	bool			no_invert_set;
	/** Set if direction is configured as output */
	bool			dir_out;
	/** Set if direction is configured as input */
	bool			dir_in;
};

/**
 * Data structure containing information about SFP/QSFP slots
 */
struct cvmx_fdt_sfp_info {
	/** Used for a linked list of slots */
	struct cvmx_fdt_sfp_info	*next, *prev;
	/** Name from device tree of slot */
	const char			*name;
	/** I2C bus for slot EEPROM */
	struct cvmx_fdt_i2c_bus_info	*i2c_bus;
	/** Data from SFP or QSFP EEPROM */
	struct cvmx_sfp_mod_info	sfp_info;
	/** Data structure with PHY information */
	struct cvmx_phy_info		*phy_info;
	/** IPD port(s) slot is connected to */
	int				ipd_port[4];
	/** Offset in device tree of slot */
	int				of_offset;
	/** EEPROM address of SFP module (usually 0x50) */
	uint8_t				i2c_eeprom_addr;
	/** Diagnostic address of SFP module (usually 0x51) */
	uint8_t				i2c_diag_addr;
	/** True if QSFP module */
	bool				is_qsfp;
	/** True if EEPROM data is valid */
	bool				valid;
	/** SFP tx_disable GPIO descriptor */
	struct cvmx_fdt_gpio_info	*tx_disable;
	/** SFP mod_abs/QSFP mod_prs GPIO descriptor */
	struct cvmx_fdt_gpio_info	*mod_abs;
	/** SFP tx_error GPIO descriptor */
	struct cvmx_fdt_gpio_info	*tx_error;
	/** SFP rx_los GPIO discriptor */
	struct cvmx_fdt_gpio_info	*rx_los;
	/** QSFP select GPIO descriptor */
	struct cvmx_fdt_gpio_info	*select;
	/** QSFP reset GPIO descriptor */
	struct cvmx_fdt_gpio_info	*reset;
	/** QSFP interrupt GPIO descriptor */
	struct cvmx_fdt_gpio_info	*interrupt;
	/** QSFP lp_mode GPIO descriptor */
	struct cvmx_fdt_gpio_info	*lp_mode;
	/** Last mod_abs value */
	int				last_mod_abs;
	/** Last rx_los value */
	int				last_rx_los;
	/** Function to call to check mod_abs */
	int (*check_mod_abs)(struct cvmx_fdt_sfp_info *sfp_info, void *data);
	/** User-defined data to pass to check_mod_abs */
	void *mod_abs_data;
	/** Function to call when mod_abs changes */
	int (*mod_abs_changed)(struct cvmx_fdt_sfp_info *sfp_info, int val,
			       void *data);
	/** User-defined data to pass to mod_abs_changed */
	void *mod_abs_changed_data;
	/** Function to call when rx_los changes */
	int (*rx_los_changed)(struct cvmx_fdt_sfp_info *sfp_info, int val,
			      void *data);
	/** User-defined data to pass to rx_los_changed */
	void *rx_los_changed_data;
	/** True if we're connected to a Microsemi VSC7224 reclocking chip */
	bool is_vsc7224;
	/** Data structure for first vsc7224 channel we're attached to */
	struct cvmx_vsc7224_chan *vsc7224_chan;
	/** xinterface we're on */
	int xiface;
	/** port index */
	int index;
};

#ifdef __U_BOOT__

/**
 * Helper to return the address property
 *
 * @param[in] fdt_addr	pointer to FDT blob
 * @param node		node to read address from
 * @param prop_name	property name to read
 *
 * @return address of property or FDT_ADDR_T_NONE if not found
 */
static inline fdt_addr_t cvmx_fdt_get_addr(const void *fdt_addr, int node,
					   const char *prop_name)
{
	return fdtdec_get_addr(fdt_addr, node, prop_name);
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
static inline int cvmx_fdt_get_int(const void *fdt_addr, int node,
				   const char *prop_name, int default_val)
{
	return fdtdec_get_int(fdt_addr, node, prop_name, default_val);
}

static inline bool cvmx_fdt_get_bool(const void *fdt_addr, int node,
				     const char *prop_name)
{
	return fdtdec_get_bool(fdt_addr, node, prop_name);
}

static inline uint64_t cvmx_fdt_get_uint64(const void *fdt_addr, int node,
					   const char *prop_name,
					   uint64_t default_val)
{
	return fdtdec_get_uint64(fdt_addr, node, prop_name, default_val);
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
static inline int cvmx_fdt_lookup_phandle(const void *fdt_addr, int node,
					  const char *prop_name)
{
	return fdtdec_lookup_phandle(fdt_addr, node, prop_name);
}

/**
 * Translate an address from the device tree into a CPU physical address by
 * walking up the device tree and applying bus mappings along the way.
 *
 * This uses #size-cells and #address-cells.
 *
 * @param[in]	fdt_addr	Address of flat device tree
 * @param	node		node to start translating from
 * @param[in]	in_addr		Address to translate
 *				NOTE: in_addr must be in the native ENDIAN
 *				format.
 *
 * @return	Translated address or FDT_ADDR_T_NONE if address cannot be
 * 		translated.
 */
static inline uint64_t cvmx_fdt_translate_address(const void *fdt_addr,
						  int node,
						  const uint32_t *in_addr)
{
	return fdt_translate_address((void *)fdt_addr, node, in_addr);
}

#else
/**
 * Helper to return the address property
 *
 * @param[in] fdt_addr	pointer to FDT blob
 * @param node		node to read address from
 * @param prop_name	property name to read
 *
 * @return address of property or FDT_ADDR_T_NONE if not found
 */
uint64_t cvmx_fdt_get_addr(const void *fdt_addr, int node,
			   const char *prop_name);

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
		     int default_val);

/**
 * Helper function to return a boolean property
 *
 * @param[in]	fdt_addr	Pointer to FDT blob
 * @param	node		node to check
 * @param[in]	prop_name	Property name to check
 *
 * @return	true if property exists, false if it doesn't
 */
bool cvmx_fdt_get_bool(const void *fdt_addr, int node, const char *prop_name);

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
			     const char *prop_name, uint64_t default_val);

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
			    const char *prop_name);

/**
 * Translate an address from the device tree into a CPU physical address by
 * walking up the device tree and applying bus mappings along the way.
 *
 * This uses #size-cells and #address-cells.
 *
 * @param[in]	fdt_addr	Address of flat device tree
 * @param	node		node to start translating from
 * @param[in]	in_addr		Address to translate
 *				NOTE: in_addr must be in the native ENDIAN
 *				format.
 *
 * @return	Translated address or FDT_ADDR_T_NONE if address cannot be
 * 		translated.
 */
uint64_t cvmx_fdt_translate_address(const void *fdt_addr, int node,
				    const uint32_t *in_addr);
#endif

/**
 * Compare compatibile strings in the flat device tree.
 *
 * @param[in] s1	First string to compare
 * @param[in] sw	Second string to compare
 *
 * @return	0 if no match
 *		1 if only the part number matches and not the manufacturer
 *		2 if both the part number and manufacturer match
 */
int cvmx_fdt_compat_match(const char *s1, const char *s2);

/**
 * Returns wheter a list of strings contains the specified string
 *
 * @param[in]	slist	String list
 * @param	llen	string list total length
 * @param[in]	str	string to search for
 *
 * @return	1 if string list contains string, 0 if it does not.
 */
int cvmx_fdt_compat_list_contains(const char *slist, int llen, const char *str);

/**
 * Check if a node is compatible with the specified compat string
 *
 * @param[in]	fdt_addr	FDT address
 * @param	node		node offset to check
 * @param[in]	compat		compatible string to check
 *
 * @return	0 if compatible, 1 if not compatible, error if negative
 */
int cvmx_fdt_node_check_compatible(const void *fdt_addr, int node,
				   const char *compat);

/**
 * @INTERNAL
 * Compares a string to a compatible field.
 *
 * @param[in]	compat		compatible string
 * @param[in]	str		string to check
 *
 * @return	0 if not compatible, 1 if manufacturer compatible, 2 if
 * 		part is compatible, 3 if both part and manufacturer are
 * 		compatible.
 */
int __cvmx_fdt_compat_match(const char *compat, const char *str);

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
					   int *size);

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
				    int *bus, int *addr);

/**
 * Given a FDT node return the CPU node number
 *
 * @param[in]	fdt_addr	Address of FDT
 * @param	node		FDT node number
 *
 * @return	CPU node number or error if negative
 */
int cvmx_fdt_get_cpu_node(const void *fdt_addr, int node);

/**
 * Get the total size of the flat device tree
 *
 * @param[in]	fdt_addr	Address of FDT
 *
 * @return	Size of flat device tree in bytes or -1 if error.
 */
int cvmx_fdt_get_fdt_size(const void *fdt_addr);

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
				        const char * const *strlist);

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
					    const char * const *strlist);

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
						   int of_offset);

/**
 * Return the Octeon bus number for a bus descriptor
 *
 * @param[in]	bus	bus descriptor
 *
 * @return	Octeon twsi bus number or -1 on error
 */
int cvmx_fdt_i2c_get_root_bus(const struct cvmx_fdt_i2c_bus_info *bus);

/**
 * Frees all entries for an i2c bus descriptor
 *
 * @param	bus	bus to free
 *
 * @return	0
 */
int cvmx_fdt_free_i2c_bus(struct cvmx_fdt_i2c_bus_info *bus);

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
			    bool enable);


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
							  const char *prop_name);

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
int cvmx_fdt_gpio_set(struct cvmx_fdt_gpio_info *pin, int value);

/**
 * Given a GPIO pin descriptor, input the value of that pin
 *
 * @param	pin	GPIO pin descriptor
 *
 * @return	0 if low, 1 if high, -1 on error.  Note that the input will be
 *		inverted if the CVMX_GPIO_ACTIVE_LOW flag bit is set.
 */
int cvmx_fdt_gpio_get(struct cvmx_fdt_gpio_info *pin);

/**
 * Assigns an IPD port to a SFP slot
 *
 * @param	sfp		Handle to SFP data structure
 * @param	ipd_port	Port to assign it to
 *
 * @return	0 for success, -1 on error
 */
int cvmx_sfp_set_ipd_port(struct cvmx_fdt_sfp_info *sfp, int ipd_port);

/**
 * Get the IPD port of a SFP slot
 *
 * @param[in]	sfp		Handle to SFP data structure
 *
 * @return	IPD port number for SFP slot
 */
static inline int cvmx_sfp_get_ipd_port(const struct cvmx_fdt_sfp_info *sfp)
{
	return sfp->ipd_port[0];
}

/**
 * Get the IPD ports for a QSFP port
 *
 * @param[in]	sfp		Handle to SFP data structure
 * @param[out]	ipd_ports	IPD ports for each lane, if running as 40G then
 *				only ipd_ports[0] is valid and the others will
 *				be set to -1.
 */
static inline void cvmx_qsfp_get_ipd_ports(const struct cvmx_fdt_sfp_info *sfp,
					   int ipd_ports[4])
{
	int i;
	for (i = 0; i < 4; i++)
		ipd_ports[i] = sfp->ipd_port[i];
}

/**
 * Attaches a PHY to a SFP or QSFP.
 *
 * @param	sfp		sfp to attach PHY to
 * @param	phy_info	phy descriptor to attach or NULL to detach
 */
void cvmx_sfp_attach_phy(struct cvmx_fdt_sfp_info *sfp,
			 struct cvmx_phy_info *phy_info);

/**
 * Returns a phy descriptor for a SFP slot
 *
 * @param[in]	sfp	SFP to get phy info from
 *
 * @return	phy descriptor or NULL if none.
 */
static inline struct cvmx_phy_info *
cvmx_sfp_get_phy_info(const struct cvmx_fdt_sfp_info *sfp)
{
	return sfp->phy_info;
}

/**
 * @INTERNAL
 * Parses all instances of the Vitesse VSC7224 reclocking chip
 *
 * @param[in]	fdt_addr	Address of flat device tree
 *
 * @return	0 for success, error otherwise
 */
int __cvmx_fdt_parse_vsc7224(const void *fdt_addr);

/**
 * Parse SFP information from device tree
 *
 * @param[in]	fdt_addr	Address of flat device tree
 *
 * @return pointer to sfp info or NULL if error
 */
struct cvmx_fdt_sfp_info *cvmx_helper_fdt_parse_sfp_info(const void *fdt_addr,
							 int of_offset);

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
			  struct cvmx_phy_info *phy_info);


#endif /* CVMX_HELPER_FDT_H__ */
