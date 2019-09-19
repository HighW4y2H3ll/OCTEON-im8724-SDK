/* AAPL CORE Revision: 2.3.1-beta */

/* Copyright 2014-2015 Avago Technologies. All rights reserved.
 *
 * This file is part of the AAPL CORE library.
 *
 * AAPL CORE is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AAPL CORE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AAPL CORE.  If not, see http://www.gnu.org/licenses.
 */

#ifndef __AVAGO_H__
#define __AVAGO_H__

/**
 * Maximum number of SBus reads to check for completion of SPICO interrupt
 * command
 */
#define AVAGO_SERDES_INT_TIMEOUT        500

/**
 * Maximum number of reads of the SBUS_RESULT register in sbus-over-mdio mode
 */
#define AVAGO_SBUS_MDIO_TIMEOUT         100

/** Maximum milliseconds for pll calibration */
#define AVAGO_SERDES_INIT_RDY_TIMEOUT    20

/**
 * Maximum milliseconds to wait for AAPL to wait for external SPICO upload to
 * complete
 */
#define AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT 500

/**
 * Maximum number of commands to send after a hard I2C reset to wait for bus to
 * come back up
 */
#define AVAGO_I2C_HARD_RESET_TIMEOUT    100

#define AVSP_DEVAD 8 /** for all SBus-over-MDIO operations: */

#define AVAGO_MDIO_REG_STATUS		32777  /** fifo_empty is bit [11]. */
#define AVAGO_MDIO_REG_STAT_MISC1	32787  /** miscelaneous status bits */
#define AVAGO_MDIO_REG_RESET		32784  /** sbus_reset is bit [0]. */

/** PMA/PMD control2 register contains type select */
#define AVAGO_MDIO_REG_CONTROL		0

/** PMA/PMD control register contains speed select */
#define AVAGO_MDIO_REG_CONTROL2		7

#define AVAGO_ADDR_INVALID_LANE		(0xBAD)


/** @name I2C commands */
#define NOP			0x00
#define WRITE_SBUS_DEVICE	0x01
#define READ_SBUS_DEVICE	0x02
#define RESET_SBUS_DEVICE	0x03
#define CORE_SBUS_RESET		0x04
#define TWI_REGISTER_0		0x80

/** @name I2C slave device registers */

/** SBus IP element data register address */
#define SBUS_DATA_REG_ADDR	0x00

/** SBus address of desired IP element */
#define SBUS_DEVICE_ADDR	0x01

/** D0 data out bits [7:0] (SBUS_WRITE) */
#define SBUS_DATA_OUT_0		0x02

/** D1 data out bits [15:8] */
#define SBUS_DATA_OUT_1		0x03

/** D2 data out bits [23:16] */
#define SBUS_DATA_OUT_2		0x04

/** D3 data out bits [31:24] */
#define SBUS_DATA_OUT_3		0x05

/** Status code (status result of most recent SBus transaction, read only) */
#define SBUS_STATUS		0x06

/** General purpose output register bits [7:0] */
#define SBUS_GP_OUT_0		0x07

/** General purpose output register bits [15:8] */
#define SBUS_GP_OUT_1		0x08

/** General purpose input register bits [7:0], read only */
#define SBUS_GP_OUT_2		0x09

/** General purpose input register bits [15:8], read only */
#define SBUS_GP_OUT_3		0x0a

/** D0 data input bits [7:0] (SBUS_READ, read only) */
#define SBUS_DATA_IN_0		0x0b
/** D1 data input bits [15:8] (read only) */
#define SBUS_DATA_IN_1		0x0c
/** D2 data input bits [23:16] (read only) */
#define SBUS_DATA_IN_2		0x0d

/** D3 data input bits [31:24] (read only) */
#define SBUS_DATA_IN_3		0x0e

#define AVAGO_SUPPRESS_ERRORS_PUSH(ah) \
	{ int __return_code = (ah->suppress_errors++, ah->return_code)
#define AVAGO_SUPPRESS_ERRORS_POP(ah) \
	ah->return_code = __return_code; ah->suppress_errors--; }

#define AVAGO_3D_ARRAY_ADDR(addr) (addr).chip][(addr).ring][(addr).sbus

enum avago_process_id {
	AVAGO_PROCESS_D = 1,
	AVAGO_PROCESS_E,
	AVAGO_PROCESS_A,
	AVAGO_PROCESS_F,
	AVAGO_PROCESS_G,
	AVAGO_PROCESS_B,
	AVAGO_PROCESS_C,
	AVAGO_UNKNOWN_PROCESS = 0
};

enum avago_ip_type {
	AVAGO_UNKNOWN_IP                 = 0x00,
	AVAGO_SERDES                     = 0x01,
	AVAGO_SBUS_CONTROLLER            = 0x02,
	AVAGO_SPICO                      = 0x03,
	AVAGO_QPI                        = 0x04,
	AVAGO_FBD                        = 0x05,
	AVAGO_PCS64B66B                  = 0x06,
	AVAGO_AUTO_NEGOTIATION           = 0x07,
	AVAGO_PCS64B66B_FEC              = 0x08,
	AVAGO_PCIE_PCS                   = 0x09,
	AVAGO_CORE_PLL                   = 0x0a,
	AVAGO_PMRO                       = 0x0b,
	AVAGO_DDR_ADDRESS                = 0x0c,
	AVAGO_DDR_DATA                   = 0x0d,
	AVAGO_DDR_TRAIN                  = 0x0e,
	AVAGO_DDR_CTC                    = 0x0f,
	AVAGO_DDR_STOP                   = 0x10,
	AVAGO_THERMAL_SENSOR             = 0x11,
	AVAGO_RMON                       = 0x12,
	AVAGO_LINK_EMULATOR              = 0x13,
	AVAGO_AVSP_CONTROL_LOGIC         = 0x14,
	AVAGO_M4                         = 0x15,
	AVAGO_P1                         = 0x16,
	AVAGO_MLD                        = 0x17,
	AVAGO_RSFEC_BRIDGE               = 0x18,
	AVAGO_CROSSPOINT                 = 0x19,
	AVAGO_SAPPH_GBX                  = 0x20,
	AVAGO_SAPPH_GBX_TOP              = 0x21,
	AVAGO_OPAL_RSFEC528              = 0x22,
	AVAGO_OPAL_RSFEC528_544          = 0x23,
	AVAGO_OPAL_HOST_ALIGNER          = 0x24,
	AVAGO_OPAL_MOD_ALIGNER           = 0x25,
	AVAGO_OPAL_CONTROL               = 0x26,
	AVAGO_OSC_SSC_PLL                = 0x27,
	AVAGO_GARNET_25GE_INTERFACE      = 0x28,
	AVAGO_SGMII                      = 0x29,

	AVAGO_LINK_EMULATOR_2            = 0x81,
	AVAGO_SLE_PKT                    = 0x82,
	AVAGO_SLE                        = 0x83,
	AVAGO_PAD_CONTROL                = 0x88,

	AVAGO_RAM_PMRO,
	AVAGO_PANDORA_LSB,

	AVAGO_MAX_RING_ADDRESS           = 0xdf,
	AVAGO_SGMII_BROADCAST            = 0xea,
	AVAGO_OSC_SSC_BROADCAST          = 0xeb,
	AVAGO_MLD_BROADCAST              = 0xec,
	AVAGO_SERDES_P1_BROADCAST        = 0xed,
	AVAGO_SERDES_M4_BROADCAST        = 0xee,
	AVAGO_SERDES_D6_BROADCAST        = 0xff,
	AVAGO_THERMAL_SENSOR_BROADCAST   = 0xef,
	AVAGO_DDR_STOP_BROADCAST         = 0xf0,
	AVAGO_DDR_CTC_BROADCAST          = 0xf1,
	AVAGO_DDR_TRAIN_BROADCAST        = 0xf2,
	AVAGO_DDR_DATA_BROADCAST         = 0xf3,
	AVAGO_DDR_ADDRESS_BROADCAST      = 0xf4,
	AVAGO_PMRO_BROADCAST             = 0xf5,
	AVAGO_RESERVED_BROADCAST         = 0xf6,
	AVAGO_PCIE_PCS_BROADCAST         = 0xf7,
	AVAGO_PCS64B66B_BROADCAST        = 0xf8,
	AVAGO_AUTO_NEGOTIATION_BROADCAST = 0xf9,
	AVAGO_FBD_BROADCAST              = 0xfb,
	AVAGO_QPI_BROADCAST              = 0xfc,
	AVAGO_SPICO_BROADCAST            = 0xfd,
	AVAGO_SERDES_BROADCAST           = 0xff
};

#define AVAGO_IP_TYPE_MAX 0x28

#define AVAGO_IP_TYPE_ALT_RANGE_LO 0x81
#define AVAGO_IP_TYPE_ALT_RANGE_HI 0x8a

/**
 * Maximum number of devices AAPL supports.
 *
 * Reducing this value to that actually used will reduce the memory footprint
 * of the Aapl_t struct.
 * Valid range: [1-15].
 */
#define AVAGO_MAX_CHIPS 1

/**
 * Maximum number of SBus rings AAPL supports. */
/* Reducing this value to that actually used will reduce the memory footprint
 * of the Aapl_t struct.
 * Valid range: [1-15].
 */
#define AVAGO_MAX_RINGS 15

/** Maximum number of slices to store the state of */
#define AVAGO_MAX_SLICE_COUNT	20

struct phy_device;

/**
 * Avago handle used for most functions in the API
 */
struct avago_hdl {
	/** 28nm SerDes SPICO interrupt maximum number of tries */
	int serdes_int_timeout;
	/** sbus-over-mdio SBUS_RESULT polling maximum number of tries */
	int sbus_mdio_timeout;
	uint32_t chips;		/** Number of die this struct points to */
	uint32_t sbus_rings;	/** Number of SBus rings this struct points to */

	/** Maximum number of commands to queue before sending them */
	uint32_t capabilities;
	uint32_t tap_gen;	/** Avago TAP generation being communicated with */
	uint32_t sbus_commands;	/** SBus commands executed */

	const char *chip_name[AVAGO_MAX_CHIPS];
	const char *chip_rev[AVAGO_MAX_CHIPS];
	uint32_t jtag_idcode[AVAGO_MAX_CHIPS];	/** JTAG IDCODE for each chip */
	/** Process ID of current device */
	enum avago_process_id process_id[AVAGO_MAX_CHIPS];

	int i2c_base_addr;	/** I2C base address */
	int i2c_bus;
	int i2c_addr;
	struct phy_device *phydev;

	/** Set by some functions to indicate success/fail status */
	int return_code;
	int data;
	int data_char_size;	/** data_char memory management */
	char *data_char;	/** Used for functions that return strings */
	/** To truncate data_char, set data_char_end = data_char */
	char *data_char_end;

	bool (*sbus_fn)(struct avago_hdl *ah, uint32_t addr,
			uint8_t reg_addr, uint8_t command,
			uint32_t *sbus_data);
	int (*i2c_write_fn)(struct avago_hdl *ah, uint32_t dev_addr,
			    uint32_t length, uint8_t *buffer);
	int (*i2c_read_fn)(struct avago_hdl *ah, uint32_t dev_addr,
			   uint32_t length, uint8_t *buffer);

	uint32_t (*serdes_int_fn)(struct avago_hdl *ah, uint32_t addr,
				  int int_code, int int_data);

	/** Debug level */
	int debug;

	/** If non-zero, suppress printing errors */
	int suppress_errors;

	/** IP revision for each SBus Rx */
	uint16_t ip_rev[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS][256];

	/** Revision of firmware loaded, populated if ip_type is SERDES or SPICO */
	uint16_t firm_rev[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS][256];

	/** Build of firmware loaded, popuplated if ip_type is SERDES or SPICO */
	uint16_t firm_build[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS][256];

	/** Indicator of SPICO processor is running. */
	bool spico_running[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS][256];

	/** Avago_ip_type_t identifier for each SBus Rx */
	uint8_t ip_type[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS][256];

	/** Revision of LSB block, populated if ip_type is SERDES */
	uint8_t lsb_rev[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS][256];

	/** max SBus address for each die and SBus ring */
	uint8_t max_sbus_addr[AVAGO_MAX_CHIPS][AVAGO_MAX_RINGS];

	void *client_data;

	/** Current chip being used */
	int curr_chip;
};

/** Internal hardware address structure */
struct avago_addr {
	uint32_t chip;	/** Device number */
	uint32_t ring;	/** Ring on device */
	uint32_t sbus;	/** SBus address on ring */
	uint32_t lane;	/** Lane at SBus address */
	struct avago_addr *next_addr;/** Used for a linked list of addresses */
};


/** @brief AAPL addressing constants. */
enum avago_broadcast_control {
	AVAGO_ADDR_BROADCAST      = 0xff,
	AVAGO_ADDR_IGNORE_LANE    = 0xf0,
	AVAGO_ADDR_QUAD_LOW       = 0xf1,
	AVAGO_ADDR_QUAD_HIGH      = 0xf2,
	AVAGO_ADDR_QUAD_ALL       = 0xf3,
#ifdef AVAGO_ADDR_INVALID_LANE
# undef AVAGO_ADDR_INVALID_LANE
	AVAGO_ADDR_INVALID_LANE   = 0xbad,
#endif
};

#define AVAGO_BROADCAST                (AVAGO_ADDR_BROADCAST)
#define AVAGO_SBUS_MASTER_ADDRESS      (0xfd)
#define AVAGO_SBUS_CONTROLLER_ADDRESS  (0xfe)
#define AVAGO_SERDES_BROADCAST_ADDRESS (0xff)

enum avago_mode {
	/** Select the AVSP-110 10:4 operating mode. */
	AVAGO_PMA			= 0,

	/** Select the AVSP-1104 10:4 operating mode. */
	AVAGO_GEARBOX_10_4		= 0,

	/** Select the AVSP-1104 10:10 operating mode. */
	AVAGO_RPT			= 1,

	/** Select the AVSP-1104 10:10 and AVSP-4412 operating mode. */
	AVAGO_REPEATED_DUPLEX		= 1,

	AVAGO_GEARBOX_10_4_RS_FEC	= 2,
	AVAGO_GEARBOX_10_4_MLG		= 3,
	AVAGO_REPEATER_SIMPLEX		= 4,	/** Select the AVSP-8801 mode. */
	AVAGO_GEARBOX_4_1		= 5,
	AVAGO_ADHOC			= 6,

	/** Select the AVSP-8812 Gearbox 2:1 mode */
	AVAGO_GEARBOX_2_1		= 7,

	/** Select the AVSP-8812 Gearbox 1:2 mode */
	AVAGO_GEARBOX_2_1_MOD_HOST	= 8,


	AVAGO_RS_FEC_4X4		= 9,	/** Select AVSP-8812 FEC mode */
	AVAGO_RS_FEC_528		= 10,	/** Select AVSP-7412 FEC mode */
	AVAGO_RS_FEC_544		= 11,	/** Select AVSP-7412 FEC mode */
	AVAGO_RS_FEC			= 12	/** Select AVSP-5410 FEC mode */
};

/**
 * Initialize and construct the Avago handle.
 *
 * @param	i2c_bus		i2c bus number of Avago device
 * @param	i2c_base_addr	i2c address of Avago device
 *
 * @return	Pointer to avago handle or NULL if out of memory.
 */
struct avago_hdl *avago_construct(int i2c_bus, int i2c_base_addr);

/**
 * @brief  Returns the SBus Master address for the addressed chip and ring.
 *
 * @param	addr	chip/ring address
 *
 * @return The SBus Master address corresponding to given input address.
 */
uint32_t avago_make_sbus_master_addr(uint32_t addr);

/**
 * @brief	Returns the SBus Controller address for the addressed chip and
 *		ring.
 *
 * @param	addr	Chip/ring address
 *
 * @return The SBus Controller address corresponding to given input address.
 */
uint32_t avago_make_sbus_controller_addr(uint32_t addr);

/**
 * @brief	Modifies the lane portion of an input address.
 *
 * @param	addr	Chip/ring/sbus address
 * @param	lane	New lane address
 *
 * @return The input address modified to reference the given lane.
 */
uint32_t avago_make_sbus_lane_addr(uint32_t addr, int lane);

/**
 * @brief	Begins traversal of addresses selected by sbus_addr.
 * @details	start, stop and first are updated based on any
 *		chip, ring or sbus broadcast addresses.
 *		start and stop should be passed unchanged to aapl_broadcast_next(),
 *		and first should be passed as the addr field.
 *		Flags can be the logical OR of several values:
 *		+ AVAGO_BROADCAST_IGNORE_LANE - causes all returned lane values
 *		  to be AVAGO_ADDR_IGNORE_LANE.
 *		  Use when lanes are not applicable.
 *		+ AVAGO_BROADCAST_NO_ITER_LANE - causes addr lane value to be
 *		  passed through unchanged.
 *		  Use to send broadcast requests rather than iterate over the
 *		  lane range.
 *		+ AVAGO_BROADCAST_LANE - lane value AVAGO_ADDR_IGNORE_LANE is
 *		  treated as AVAGO_ADDR_BROADCAST.
 *		  Other values of lane are passed through unchanged.
 *		  Use to address all SerDes even when broadcast wasn't requested.
 *		+ AVAGO_BROADCAST_NO_ITER_SBUS - causes addr sbus value to be
 *		  passed through unchanged.
 *		  Use to send broadcast requests rather than iterate over the
 *		  sbus range.
 *
 * @param[in]	ah	Avago handle
 * @param[in]	addr	SBus slice (broadcast) address.
 * @param[out]	start	Start of hardware address range
 * @param[out]	end	End of hardware address range
 * @param[out]	first	First hardware address
 * @param	flags	Flags to control the iteration
 *
 * @return	Returns TRUE and first is initialized to the first address.
 */
bool avago_broadcast_first(struct avago_hdl *ah, struct avago_addr *addr,
			   struct avago_addr *start, struct avago_addr *stop,
			   struct avago_addr *first, uint32_t flags);

/**
 * @brief	Continues traversal of addresses between start and stop.
 * @details	addr must contain the previous address on input and is updated
 *		to the next address on output.
 *
 * @param[in]		ah	Avago handle pointer
 * @param[in,out]	addr	Previous/next hardware address
 * @param[in]		start	Start of hardware address range
 * @param[in]		stop	End of hardware address range
 * @param		flags	Flags to control iteration
 *
 * @return	Updates addr and returns TRUE while traversing the selected
 *		addresses.
 *		Returns FALSE when all addresses have been traversed.
 */
bool avago_broadcast_next(struct avago_hdl *ah, struct avago_addr *addr,
			  struct avago_addr *start, struct avago_addr *stop,
			  uint32_t flags);

/**
 * Checks if the selected address matches a SerDes device.
 * @details Checks if addr_struct is a D6, M4 or P1 SerDes broadcast address
 *              and addr references an instance of the broadcast type
 *              and the SerDes is not ignoring broadcasts;
 *          OR if addr_struct is not a SerDes broadcast
 *              and addr references any SerDes type.
 *
 * @param[in]	ah		Avago handle
 * @param[in]	addr_struct	Original address struct
 * @param	sbus_addr	Device address
 * @param[out]	ignore		If address selected is broadcast, returns SerDes
 *				broadcast ignore bit.
 *
 * @return  Returns TRUE if addr references a matching SerDes device.
 *          The ignore flag is set if broadcast type and ignore bit is set.
 */
bool avago_addr_selects_fw_device(struct avago_hdl *ah,
				  struct avago_addr *addr_struct,
				  uint32_t sbus_addr,
				  bool *ignore);

bool avago_is_octal_p1(struct avago_hdl *ah, uint32_t addr);

/**
 * Returns if an address is in range or not.
 *
 * @param[in]	addr_struct	Address structure to Check
 *
 * @return	True if address is valid, false if not.
 */
static inline bool avago_address_range_check(const struct avago_addr *addr_struct)
{
	if (addr_struct->chip >= AVAGO_MAX_CHIPS ||
	    addr_struct->ring >= AVAGO_MAX_RINGS ||
	    addr_struct->sbus > 255)
		return false;
	return true;
}

/**
 * Get number of chips on the current ring
 *
 * @param[in]	ah	Avago handle
 *
 * @return Number of chips on the current ring
 */
uint32_t avago_get_chips(const struct avago_hdl *ah);

/**
 * Return the maximum sbus address for the given chip and ring
 *
 * @param[in]	ah	Avago handle
 * @param	addr	address containing chip and ring
 *
 * @return	maximum sbus address or 0 if invalid address
 */
uint32_t avago_get_max_sbus_addr(const struct avago_hdl *ah, uint32_t addr);

/**
 * Get the JTAG idcode of the specified chip
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	JTAG idcode of the current sbus ring (specified in the
 *		SBus address)
 */
uint32_t avago_get_jtag_idcode(const struct avago_hdl *ah, uint32_t addr);

/**
 * Get the name of the specified chip
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Chip address
 *
 * @return	pointer to character string with the name of the chip
 */
const char *avago_get_chip_name(const struct avago_hdl *ah, uint32_t addr);

/**
 * Get the revision of the specified chip
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Chip address
 *
 * @return	pointer to character string with the chip revision
 */
const char *avago_get_chip_rev_str(const struct avago_hdl *ah, uint32_t addr);

/**
 * Gets the Logical Sub-Block (LSB) revision number of the device.
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	The Logical Sub-Block (LSB) revision number of the addressed
 *		device.
 */
uint32_t avago_get_lsb_rev(struct avago_hdl *ah, uint32_t addr);

/**
 * Gets the revision number of the SBus device for the specified chip and
 * SBus ring
 *
 * @param[in,out]	ah	Avago handle
 * @param		addr	Address number
 *
 * @return	The revision number of the SBus device for the current chip and
 *		SBus ring (specified by the SBus address)
 */
uint32_t avago_get_ip_rev(struct avago_hdl *ah, uint32_t addr);

/**
 * Returns the firmware rev from Avago's cache
 *
 * @param[in,out]	ah	Avago handle
 * @param		addr	Address number
 *
 * @return		Firmware rev from the cache
 */
int avago_get_firmware_rev(struct avago_hdl *ah, uint32_t addr);

/**
 * Returns the firmware build from Avago's cache
 *
 * @param[in,out]	ah	Avago handle
 * @param		addr	Address number
 *
 * @return	The firmware build from the cache
 */
int avago_get_firmware_build(struct avago_hdl *ah, uint32_t addr);

/**
 * Sets the client data pointer
 *
 * @param[in,out]	ah		Avago handle
 * @param[in]		client_data	Pointer to client data
 */
void avago_bind_set(struct avago_hdl *ah, const void *client_data);

/**
 * Gets the client data pointer
 *
 * @param[in]	ah	Avago handle
 *
 * @return	Pointer to client data
 */
void *avago_bind_get(const struct avago_hdl *ah);

/**
 * Gets the chip process ID
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	The process ID of the specified device.
 */
enum avago_process_id avago_get_process_id(struct avago_hdl *ah, uint32_t addr);

/**
 * Gets the IP type for the SBus address
 *
 * @param	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	The IP type of the SBus device for the current chip and SBus
 *		ring as specified by the SBus address.
 */
enum avago_ip_type avago_get_ip_type(struct avago_hdl *ah, uint32_t addr);

/**
 * Sets the ip_type, ip_rev, spico_running, firm_rev and lsb_rev fields in the
 * avago handle.
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus slice address
 */
void avago_set_ip_type(struct avago_hdl *ah, uint32_t sbus_addr);

/**
 * Returns if SPICO is running or not
 *
 * @param	ah	Avago handle
 * @param	addr	device address
 *
 * @return	true if SPICO is running, false otherwise
 */
bool avago_get_spico_running_flag(struct avago_hdl *ah, uint32_t addr);

/**
 * Sets if SPICO is running or not
 *
 * @param	ah	Avago handle
 * @param	addr	Address of device
 * @param	running	Set true if running, false if not running
 *
 * @return	running flag or false if address out of range.
 */
bool avago_set_spico_running_flag(struct avago_hdl *ah, uint32_t addr,
				  bool running);

/**
 * Fills out the avago handle struct with information about all devices it can
 * communicate with.
 *
 * @param	ah		Avago handle
 * @param	chip_reset	Set true to reset the chip
 */
void avago_init_ip_info(struct avago_hdl *ah, bool reset);

/**
 * @brief	Checks sbus_addr against list of supported IP types.
 *
 * @details	Args is the number of arguments passed in for ip_type or process,
 *		for example: avago_check_ip_type(ah, sbus_addr, __func__,
 *						 __LINE__, true, 2, AVAGO_SERDES,
 *						 AVAGO_QPI);
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus slice address
 * @param[in]	caller		Caller function name
 * @param	line		Caller line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	The number of ip_type arguments which follow
 * @param	...		List of valid IP types
 *
 * @return	true if the device is one of the listed types, false if not.
 */
bool avago_check_ip_type(struct avago_hdl *ah, uint32_t sbus_addr,
			 const char *caller, int line, bool error_on_no_match,
			 int arg_count, ...);

/**
 * Checks sbus_addr against list of supported IP types.
 *
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus address to provide chip and ring to search
 * @param[in]	caller		Caller function name
 * @param	line		Caller line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	The number of ip_type arguments which follow
 * @param	...		List of valid IP types
 *
 * @return	true if the device is one of the listed types, false if not.
 */
bool avago_check_ip_type_exists(struct avago_hdl *ah, uint32_t sbus_addr,
				const char *caller, int line,
				bool error_on_no_match, int arg_count, ...);

/**
 * Checks sbus_addr against list of supported process types.
 *
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus address to provide chip and ring to search
 * @param[in]	caller		Caller function name
 * @param	line		Caller line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	The number of ip_type arguments which follow
 * @param	...		List of valid processes
 *
 * @return	true if the device is one of the listed types, false if not.
 */
bool avago_check_process(struct avago_hdl *ah, uint32_t sbus_addr,
			 const char *caller, int line,
			 bool error_on_no_match, int arg_count, ...);

/**
 * @brief	Checks the sbus_addr against the firmware revisions given in
 *		the arg list.
 *
 * Checks if the device firmware revision is >= what is in the list.  If the
 * revision has bit 19 set (0x80000), it must match exactly (not >=) after
 * ignoring bit 19.
 *
 * @param[in]	ah		Avago Handle
 * @param	sbus_addr	SBus slice address
 * @param[in]	caller		Calling function name
 * @param	line		Caller's line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	Number of firmware revision arguments which
 *				follow.
 * @param	...		List of valid firmware revisions
 *
 * @return	true if the firmware revision matches one of those listed,
 *		false if not.
 */
bool avago_check_firmware_rev(struct avago_hdl *ah, uint32_t sbus_addr,
			      const char *caller, int line,
			      bool error_on_no_match, int arg_count, ...);

/**
 * @brief	Checks sbus_addr against firmware build given in args list.
 *
 * @details	Checks if the device firmware is >= what is in the list.  If
 *		the build has bit 19 set (0x80000), it must match exactly
 *		(not >=) after ignoring bit 19.
 *
 * @param[in]	ah		Avago Handle
 * @param	sbus_addr	SBus slice address
 * @param[in]	caller		Calling function name
 * @param	line		Caller's line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	Number of firmware build arguments which
 *				follow.
 * @param	...		List of valid firmware builds
 *
 * @return	true if the firmware build matches one of those listed,
 *		false if not.
 */
bool avago_check_firmware_build(struct avago_hdl *ah, uint32_t sbus_addr,
				const char *caller, int line,
				bool error_on_no_match, int arg_count, ...);

/**
 * Checks sbus_addr to see if it is a broadcast address (including the SerDes
 * broadcast address).
 *
 * @param[in]	ah		Avago handle
 * @param	sbus_addr	SBus address to Check
 * @param[in]	caller		Calling function name
 * @param	line		Line number of caller
 * @param	error_on_match	True to print error on match.
 *
 * @return	true if sbus_addr belongs to any broadcast address
 *		(chip/SBus/SerDes).
 */
bool avago_check_broadcast_address(struct avago_hdl *ah, uint32_t sbus_addr,
				   const char *caller, int line,
				   bool error_on_match);

/**
 * Reset the chip
 *
 * @details	Performs a MDIO reset (if using MDIO)
 *
 * @param	ah	Avago handle
 * @param	reset	If set, send the "reset" command to the chip
 * @param	chip	The chip number
 */
void avago_system_chip_setup(struct avago_hdl *ah, bool reset, int chip);

/**
 * Initialize avago_addr structure to default values.
 *
 * @param[in]	structure to initialize
 */
void avago_addr_init(struct avago_addr *addr_struct);

/**
 * @brief	Modifies the addr_struct passed in to be all broadcast addresses
 *		(chip, ring, SBus, etc)
 *
 * @param[out]	addr_struct	The modified broadcast addr_struct
 *
 * @return The address integer, and modifies the addr_struct passed in.
 */
uint32_t avago_addr_init_broadcast(struct avago_addr *addr_struct);

/**
 * Converts an address integer into an address structure
 *
 * @param	addr		Address to convert
 * @param[out]	addr_struct	Converted address
 *
 * @return	false if address is not a recognized value
 */
bool avago_addr_to_struct(uint32_t addr, struct avago_addr *addr_struct);

/**
 * Converts hardware address structure into an address integer
 *
 * @param[in]	Hardware address structure
 *
 * @return	The address value representation of the hardware address.
 */
uint32_t avago_struct_to_addr(const struct avago_addr *addr_struct);

/**
 * Combine the chip, ring and sbus values into an address integer
 *
 * @param	chip	chip number, range [0..15]
 * @param	ring	ring number, range [0..15]
 * @param	sbus	SerDes number, range [0..255]
 *
 * @return	Combined address
 */
uint32_t avago_make_addr3(int chip, int ring, int sbus);

/**
 * Combine the chip, ring, sbus and lane values into an address integer
 *
 * @param	chip	chip number, range [0..15]
 * @param	ring	ring number, range [0..15]
 * @param	sbus	SerDes number, range [0..255]
 * @param	lane	SerDes lane number, range [0..7,
 *						   AVAGO_ADDR_QUAD_LOW,
 *						   AVAGO_ADDR_QUAD_HIGH,
 *						   AVAGO_ADDR_QUAD_ALL,
 *						   AVAGO_ADDR_BROADCAST,
 *						   AVAGO_ADDR_IGNORE_LANE ]
 *
 * @return	Combined address
 */
uint32_t avago_make_addr4(int chip, int ring, int sbus, int lane);

/**
 * Sets the sbus portion of an input address to the new value and sets the lane
 * to be ignored.
 *
 * @param	addr	Chip/ring/sbus address
 * @param	sbus	new sbus value
 *
 * @return	modified addr value with new sbus value.
 */
uint32_t avago_make_sbus_addr(uint32_t addr, int sbus);

/**
 * @brief	Modifies the lane portion of an input address.
 *
 * @param	addr	Chip/ring/sbus address
 * @param	lane	New lane address
 *
 * @return The input address modified to reference the given lane.
 */
uint32_t avago_make_sbus_lane_addr(uint32_t addr, int lane);

/**
 * @brief  Returns the SBus Master address for the addressed chip and ring.
 *
 * @param	addr	chip/ring address
 *
 * @return The SBus Master address corresponding to given input address.
 */
uint32_t avago_make_sbus_master_addr(uint32_t addr);

/**
 * @brief	Returns the SBus Controller address for the addressed chip and
 *		ring.
 *
 * @param	addr	Chip/ring address
 *
 * @return The SBus Controller address corresponding to given input address.
 */
uint32_t avago_make_sbus_controller_addr(uint32_t addr);

/**
 * @brief	Returns the SerDes Broadcast address for the addressed chip and ring.
 *
 * @param	addr	Chip/ring address
 *
 * @return The SerDes Broadcast address for the addressed chip and ring.
 */
uint32_t avago_make_serdes_broadcast_addr(uint32_t addr);

#define AVAGO_BROADCAST_IGNORE_LANE     (0x01)
#define AVAGO_BROADCAST_NO_ITER_LANE    (0x02)
#define AVAGO_BROADCAST_LANE            (0x04)
#define AVAGO_BROADCAST_NO_ITER_SBUS    (0x08)

/**
 * @brief	Begins traversal of addresses selected by sbus_addr.
 * @details	start, stop and first are updated based on any
 *		chip, ring or sbus broadcast addresses.
 *		start and stop should be passed unchanged to aapl_broadcast_next(),
 *		and first should be passed as the addr field.
 *		Flags can be the logical OR of several values:
 *		+ AVAGO_BROADCAST_IGNORE_LANE - causes all returned lane values
 *		  to be AVAGO_ADDR_IGNORE_LANE.
 *		  Use when lanes are not applicable.
 *		+ AVAGO_BROADCAST_NO_ITER_LANE - causes addr lane value to be
 *		  passed through unchanged.
 *		  Use to send broadcast requests rather than iterate over the
 *		  lane range.
 *		+ AVAGO_BROADCAST_LANE - lane value AVAGO_ADDR_IGNORE_LANE is
 *		  treated as AVAGO_ADDR_BROADCAST.
 *		  Other values of lane are passed through unchanged.
 *		  Use to address all SerDes even when broadcast wasn't requested.
 *		+ AVAGO_BROADCAST_NO_ITER_SBUS - causes addr sbus value to be
 *		  passed through unchanged.
 *		  Use to send broadcast requests rather than iterate over the
 *		  sbus range.
 *
 * @param[in]	ah	Avago handle
 * @param[in]	addr	SBus slice (broadcast) address.
 * @param[out]	start	Start of hardware address range
 * @param[out]	end	End of hardware address range
 * @param[out]	first	First hardware address
 * @param	flags	Flags to control the iteration
 *
 * @return	Returns TRUE and first is initialized to the first address.
 */
bool aapl_broadcast_first(struct avago_hdl *ah, struct avago_addr *addr,
			  struct avago_addr *start, struct avago_addr *stop,
			  struct avago_addr *first, uint32_t flags);

/**
 * @brief	Continues traversal of addresses between start and stop.
 * @details	addr must contain the previous address on input and is updated
 *		to the next address on output.
 *
 * @param[in]		ah	Avago handle pointer
 * @param[in,out]	addr	Previous/next hardware address
 * @param[in]		start	Start of hardware address range
 * @param[in]		stop	End of hardware address range
 * @param		flags	Flags to control iteration
 *
 * @return	Updates addr and returns TRUE while traversing the selected
 *		addresses.
 *		Returns FALSE when all addresses have been traversed.
 */
bool avago_broadcast_next(struct avago_hdl *ah, struct avago_addr *addr,
			  struct avago_addr *start, struct avago_addr *stop,
			  uint32_t flags);

/**
 * Checks if the selected address matches a SerDes device.
 * @details Checks if addr_struct is a D6, M4 or P1 SerDes broadcast address
 *              and addr references an instance of the broadcast type
 *              and the SerDes is not ignoring broadcasts;
 *          OR if addr_struct is not a SerDes broadcast
 *              and addr references any SerDes type.
 *
 * @param[in]	ah		Avago handle
 * @param[in]	addr_struct	Original address struct
 * @param	sbus_addr	Device address
 * @param[out]	ignore		If address selected is broadcast, returns SerDes
 *				broadcast ignore bit.
 *
 * @return  Returns TRUE if addr references a matching SerDes device.
 *          The ignore flag is set if broadcast type and ignore bit is set.
 */
bool avago_addr_selects_fw_device(struct avago_hdl *ah,
				  struct avago_addr *addr_struct,
				  uint32_t sbus_addr,
				  bool *ignore);

enum avago_srdev {
	AVAGO_SDREV_UNKNOWN	= 0,
	AVAGO_SDREV_D6		= 1,
	AVAGO_SDREV_16		= 2,
	AVAGO_SDREV_P1		= 3,
	AVAGO_SDREV_HVD6	= 4,
	AVAGO_SDREV_OM4		= 5,
	AVAGO_SDREV_CM4		= 6,
	AVAGO_SDREV_CM4_16	= 7,
	AVAGO_SDREV_XXX
};

#ifdef __U_BOOT__
static inline void *avago_malloc(struct avago_hdl *ah, const size_t bytes,
				 const char *description)
{
	return malloc(bytes);
}

static inline void *avago_realloc(struct avago_hdl *ah, void *ptr,
				  const size_t bytes, const char *description)
{
	return realloc(ptr, bytes);
}

static inline void avago_free(struct avago_hdl *ah, void *ptr,
			      const char *description)
{
	return free(ptr);
}

#endif
/**
 * Returns an avago_cmp_rev type the comparator design of the SerDes.
 *
 * @param	ah	Avago handle
 * @param	addr	Device address
 *
 * @return	avago_cmp_rev enumation type the comparitor design of the SerDes
 *		If unrecognized, logs an error message and decrements
 *		ah->return_code and returns AVAGO_SDREV_UNKNOWN.
 */
int avago_get_sdrev(struct avago_hdl *ah, uint32_t addr);

/** Return revision number of the given interrupt. */
int avago_get_interrupt_rev(struct avago_hdl *ah, uint32_t addr, int int_num);


/**
 * @brief Converts data to a 32 character ASCII encoded binary string with
 * optional underscores every 8 bits.
 *
 * @param[out] 	str	Output string, also returned on success
 * @param	data	Data to convert.
 * @param	underscore_en	true to add underscores every 8 bits.
 * @param	bits	Number of bits to convert
 *
 * @return	str is returned; if bits requested is greater than 32, then
 *		NULL is returned.
 */

char *avago_hex_2_bin(char *str, uint32_t data, bool underscore_en, int bits);

struct avago_state_table_options {
	bool disable_tx;
	bool disable_rx;
	bool disable_dfe;
	bool disable_clk;
};


/** @brief Print information on the device(s) AAPL is connected to using the AVAGO_INFO log type (which is typically STDOUT). */
/** @details The addr field selects the device to print info on. */
/**          As ah->verbose levels increase, more information is printed */
/**          The type input will print info only for the IP type specified. */
/**          Provides additional options as compared with avago_device_info.\n */
/** @return void */
void avago_device_info_options(struct avago_hdl *ah,
			       struct avago_addr *addr_struct,
			       enum avago_ip_type type,
			       struct avago_state_table_options *options);

extern bool avago_is_i2c_communication_method(struct avago_hdl *ah);
extern bool avago_is_mdio_communication_method(struct avago_hdl *ah);

/** @brief  Get a string representation of the Process ID of the specified chip
 * *         and sbus ring
 ** @return The Process ID string of the sbus device for the current chip and
 **         sbus ring (specified by the sbus address)
 **/
const char *avago_get_process_id_str(struct avago_hdl *ah, uint32_t addr);


#endif /* __AVAGO_H__ */
