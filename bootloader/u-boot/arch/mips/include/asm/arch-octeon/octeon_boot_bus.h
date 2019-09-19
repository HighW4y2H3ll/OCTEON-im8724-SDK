/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
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

/* This file sets up various devices on the boot bus.  The functions are
 * defined as weak so any board may override this functionality.
 */
#ifndef __OCTEON_BOOT_BUS_H__
#define __OCTEON_BOOT_BUS_H__

#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-mio-defs.h>

/* Properties of the boot bus stored in the flat device tree */
/* Be sure to update fdt_prop_to_str with any changes made here */
typedef enum octeon_boot_bus_config {
	T_ADR,
	T_ALE,
	T_CE,
	T_OE,
	T_WE,
	T_RD_HLD,
	T_WR_HLD,
	T_PAUSE,
	T_WAIT,
	T_PAGE,
	T_RD_DLY,
	PAGES,
	BUS_WIDTH,
	WAIT_MODE,
	PAGE_MODE,
	ALE_MODE,
	UNKNOWN
} boot_bus_config_t;

/** Boot bus configuration property */
struct octeon_boot_bus_fdt_prop {
	/** Which configuration property */
	enum octeon_boot_bus_config conf;
	/** String representation in device tree */
	const char *str;
	/** Mode parameter if !0 */
	uint16_t mode;
	/** Maximum register value, usually 0x3f or 0xff */
	uint16_t max;
};

/**
 * Converts a time in ns and clock period to the number of clocks.
 *
 * @param cs		Chip select
 * @param prop		Timing parameter properties (name, max, etc.)
 * @param time		time to encode in nanoseconds, -1 for maximum
 * @param clock_period	clock period to use for encoding
 * @param max		maximum allowed value, usually 0x3f
 * @param mult		Clock multiplier used, normally 4 or 8, if < 0 then it
 *			will be calculated to be 4 or 8
 *
 * @return number of clock cycles
 *
 * NOTE: This function assumes that the timing multiplier value in
 * MIO_BOOT_REG_CFGX is set to the default value of 3 (8x).
 *
 * NOTE: This function is weak.
 */
uint32_t boot_bus_time_encode(int cs,
			      const struct octeon_boot_bus_fdt_prop *prop,
			      int64_t time, uint64_t clock_period, int mult);

/**
 * Converts a time in ns and clock period to the number of clocks.
 *
 * @param cs		Chips select number for warning message
 * @param[in] name	parameter name for warning message
 * @param time		time to encode in nanoseconds, -1 for maximum
 * @param clock_period	clock period to use for encoding
 * @param max		maximum allowed value, usually 0x3f
 *
 * @return number of clock cycles
 *
 * NOTE: This function assumes that the timing multiplier value in
 * MIO_BOOT_REG_CFGX is set to the default value of 3 (8x).
 *
 * NOTE: This function is weak.
 */
uint32_t boot_bus_time_encode2(int cs, const char *name,
			      int64_t time, uint64_t clock_period, int max);
/**
 * Gets a boot-bus property from the FDT for the specified bus
 * @param chip_sel - chip select of device
 * @param prop - property to get
 * @param[out] value - value of property
 *
 * @return 0 for success, -1 on error
 */
int octeon_boot_bus_get_property(u8 chip_sel, enum octeon_boot_bus_config prop,
				 u32 *value);

/**
 * Outputs the address range for the specified chip select.
 * @param chip_sel	The chip select number [0-7]
 * @param bootbus_node_offset FDT node offset of /soc/bootbus,
 *			      can be -1 to calculate
 * @param[out] address	Pointer to address to output
 * @param[out] size		Pointer to size to output, can be NULL.
 *
 * @return 0 for success, -1 on error.
 */
int octeon_boot_bus_get_range(u8 chip_sel, int bootbus_node_offset,
			      u64 *address, u32 *size);

/**
 * Get boot bus information based on the chip select
 *
 * @param[in] chip_sel - chip select to get info for
 * @param[in] clock_period - clock period used for timing information
 * @param[out] reg_cfg - Boot bus config register, may be NULL
 * @param[out] reg_tim - Boot bus timing register
 *
 * This function iterates through the flat device tree and validates
 * the compatible and cavium,cs-index fields and fills in reg_cfg and
 * reg_tim based on the parameters found in the FDT.
 */
int octeon_boot_bus_get_fdt_info(int chip_sel, uint64_t clock_period,
				 cvmx_mio_boot_reg_cfgx_t *reg_cfg,
				 cvmx_mio_boot_reg_timx_t *reg_tim);

/**
 * Gets the FDT bus width for the specified chip select
 *
 * @param chip_sel - chip select to look up
 * @param[out] width - width of chip select
 * @return 0 for success, -1 on error.
 */
int octeon_boot_bus_get_fdt_bus_width(int chip_sel, int *width);

/**
 * Gets the ALE mode for the specified chip select
 *
 * @param chip_sel - chip select to look up
 * @param[out] ale_mode - 1 if enabled, 0 otherwise
 *
 * @return 0 for success, -1 on error.
 */
int octeon_boot_bus_get_fdt_ale_mode(int chip_sel, int *ale_mode);

/**
 * Generic boot bus device initialization
 *
 * @param fdt_addr - device tree address
 * @param node_offset - offset of node in device tree for this device
 * @param bootbus_node_offset - node offset of /soc/bootbus, can be -1
 * @param param - NULL terminated array of strings to check
 *                for compatibility, may be NULL for no compatibility checking.
 * @param[out] dev_addr - address of device on the boot bus, can be NULL.
 *
 * @return 0 for success, -1 on error.
 *
 * This is a generic function that can be used to initialize many different
 * devices on the boot bus using information from the flat device tree.
 * It is safe to call this before relocation to RAM.  The function is also
 * weak and can be redefined elsewhere.
 */
int octeon_boot_bus_generic_init(const void *fdt_addr, int node_offset,
				 int bootbus_node_offset, void *param,
				 uint64_t *dev_addr);


#ifdef CONFIG_OCTEON_ENABLE_LED_DISPLAY
/**
 * Maps the 8 character LED display on the boot bus.
 *
 * @param fdt_addr - device tree address
 * @param node_offset - offset of node in device tree for this device
 * @param bootbus_node_offset - node offset of /soc/bootbus, can be -1
 * @param param - Set to NULL to support the Avago part or pass in a
 *                NULL terminated array of compatible strings for the FDT.
 * @param[out] dev_addr - address of LED display on boot bus
 *
 * @return 0 for success, !0 for error
 *
 * This just ends up calling the generic function with the avago compatibility
 * string.
 */
int octeon_boot_bus_led_init(const void *fdt_addr, int node_offset,
			     int bootbus_node_offset, void *param,
			     uint64_t *dev_addr);
#endif
/**
 * Board specific boot bus initialization before relocation.
 */
void octeon_boot_bus_board_init(void);

/**
 * Board specific boot bus initialization after relocation.
 */
void octeon_boot_bus_board_late_init(void);

#if defined(CONFIG_SYS_FLASH_SIZE) && !defined CONFIG_SYS_NO_FLASH
/* Maps the flash on the boot bus. */

/**
 * Maps the NOR flash on the boot bus and configures the timing.
 *
 * @param fdt_addr - device tree address
 * @param node_offset - offset of node in device tree for this device
 * @param bootbus_node_offset - node offset of /soc/bootbus, can be -1
 * @param param - Not used, set to NULL.
 * @param[out] dev_addr - address of flash on the boot bus, can be NULL
 *
 * @return 0 for success, !0 for error
 *
 */
int octeon_boot_bus_flash_init(const void *fdt_addr, int node_offset,
			       int bootbus_node_offset, void *param,
			       uint64_t *dev_addr);
#endif

#if defined(CONFIG_CMD_IDE)
/**
 * Initialize the compact flash in non-True IDE mode on the boot bus
 *
 * @param fdt_addr - device tree address
 * @param node_offset - offset of node in device tree for this device
 * @param bootbus_node_offset - node offset of /soc/bootbus, can be -1
 * @param param - Not used, set to NULL.
 * @param[out] dev_addr - address of CF common area on boot bus, can be NULL
 *
 * @return 0 for success, !0 for error
 */
int octeon_boot_bus_cf_init(const void *fdt_addr, int node_offset,
			    int bootbus_node_offset, void *param,
			    uint64_t *dev_addr);

/**
 * Initialize the compact flash in true IDE mode on the boot bus
 *
 * @param fdt_addr - device tree address
 * @param node_offset - offset of node in device tree for this device
 * @param bootbus_node_offset - node offset of /soc/bootbus, can be -1
 * @param param - Not used, set to NULL.
 * @param[out] dev_addr - address of CF CS0 on boot bus, can be NULL.
 *
 * @return 0 for success, !0 for error
 *
 */
int octeon_boot_bus_true_ide_init(const void *fdt_addr, int node_offset,
				  int bootbus_node_offset, void *param,
				  uint64_t *dev_addr);
#endif /* CONFIG_CMD_IDE */

#ifdef CONFIG_OCTEON_ENABLE_PAL
/**
 * Initialize PAL on the boot bus
 *
 * @param fdt_addr - device tree address
 * @param node_offset - offset of node in device tree for this device
 * @param bootbus_node_offset - node offset of /soc/bootbus, can be -1
 * @param param - Not used, set to NULL.
 * @param[out] dev_addr - address of PAL on boot bus, can be NULL.
 *
 * @return 0 for success, !0 for error
 */
int octeon_boot_bus_pal_init(const void *fdt_addr, int node_offset,
			     int bootbus_node_offset, void *param,
			     uint64_t *dev_addr);
#endif

/**
 * Initializes devices on the boot bus and assignes them address space.
 *
 * Note that this function is defined as "weak".  If a board requires
 * separate initialization all that is required is to define this function
 * in the appropriate board file.  See the nicpro2 board for an example.
 */
int octeon_boot_bus_init(void);

/**
 * Initializes devices on the boot bus and assignes them address space.
 *
 * Note that this function is defined as "weak".  If a board requires
 * separate initialization all that is required is to define this function
 * in the appropriate board file.  See the nicpro2 board for an example.
 */
int octeon_boot_bus_late_init(void);

/**
 * NOTE: This is called when the Watchdog is disabled
 * since the watchdog overrides the first moveable region.
 */
int octeon_boot_bus_moveable_init(void);

/**
 * User-defined function which should call octeon_boot_bus_add_fdt_handler
 * to add fdt handlers for custom devices.
 */
int octeon_add_user_boot_bus_fdt_devices(void);

/**
 * Adds a fdt handler for boot bus devices
 *
 * @param name - name of boot bus device to handler
 * @param param - parameter to pass to callback function
 * @param callback - callback function to be called when a node of "name" is
 *                   found.
 */
int octeon_boot_bus_add_fdt_handler(const char *name, void *param,
				    int (*callback)(const void *fdt_addr,
						    int node_offset,
						    int bootbus_node_offset,
						    void *param,
						    uint64_t *dev_addr));

/**
 * Outputs device information about the boot bus device specified in the path
 *
 * @param path - path of boot bus device.  Should be /soc/bootbus/devname.
 * @param compatible - compatible string, or NULL to skip compatibility check
 * @param[out] addr - address of device on the boot bus
 * @param[out] size - size of the device address space on the boot bus,
 *                    can be NULL
 *
 * @return 0 for success, -1 on error
 */
int octeon_boot_bus_get_dev_info(const char *path, const char *compatible,
				 uint64_t *addr, uint32_t *size);

#endif /* __OCTEON_BOOT_BUS_H__ */
