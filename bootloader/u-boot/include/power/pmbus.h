/*
 *  Copyright (C) 2015 Cavium, Inc.
 *  <support@cavium.com>
 *
 *  Copyright (C) 2011-2012 Samsung Electronics
 *  Lukasz Majewski <l.majewski@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __PMBUS_H_
#define __PMBUS_H_

#include <common.h>
#include <linux/list.h>
#include <i2c.h>

#define PMBUS_NAME_LEN				32

/* Low byte status summary bits */
#define PMBUS_STATUSS_NONE_OF_ABOVE		1
#define PMBUS_STATUSS_COMM_MEMORY_LOGIC_EVENT	2	/** See STATUS_CML */
#define PMBUS_STATUSS_TEMP_FAULT_OR_WARNING	4	/** See STATUS_TEMPERATURE */
#define PMBUS_STATUSS_VIN_UV_FAULT		8	/** See STATUS_INPUT */
#define PMBUS_STATUSS_IOUT_OC_FAULT		0x10	/** See STATUS_IOUT */
#define PMBUS_STATUSS_VOUT_OV_FAULT		0x20	/** See STATUS_VOUT */
#define PMBUS_STATUSS_UNIT_IS_OFF		0x40	/** Output power off */
#define PMBUS_STATUSS_UNIT_WAS_BUSY		0x80	/** Device busy, unable to respond */
/* High byte status summary bits */
#define PMBUS_STATUSS_UNKNOWN_FAULT_OR_WARNING	0x100
#define PMBUS_STATUSS_OTHER			0x200	/** See STATUS_OTHER */
#define PMBUS_STATUSS_FAN_FAULT_OR_WARNING	0x400	/** See STATUS_FAN_X_Y */
#define PMBUS_STATUSS_POWER_GOOD_NEGATED	0x800
#define PMBUS_STATUSS_MFR_SPECIFIC		0x1000	/** See STATUS_MFR */
#define PMBUS_STATUSS_INPUT_FAULT_OR_WARNING	0x2000	/** See STATUS_INPUT */
#define PMBUS_STATUSS_IOUT_POUT_FAULT_OR_WARNING	0x4000	/** See STATUS_IOUT */
#define PMBUS_STATUSS_VOUT_FAULT_OR_WARNING	0x8000	/** See STATUS_VOUT */

/** STATUS_VOUT bits */
#define PMBUS_STATUS_VOUT_TRACKING_ERROR	1
#define PMBUS_STATUS_VOUT_TOFF_MAX_WARNING	2
#define PMBUS_STATUS_VOUT_TON_MAX_FAULT		4
/** An attempt was made to set the output voltage above VOUT_MAX or under VOUT_MIN */
#define PMBUS_STATUS_VOUT_MAX_MIN_WARNING	8
#define PMBUS_STATUS_VOUT_UV_FAULT		0x10	/** Output undervoltage fault */
#define PMBUS_STATUS_VOUT_UV_WARNING		0x20	/** Output undervoltage warning */
#define PMBUS_STATUS_VOUT_OV_WARNING		0x40	/** Output overvoltage warning */
#define PMBUS_STATUS_VOUT_OV_FAULT		0x80	/** Output overvoltage fault */

/** STATUS_IOUT bits */
#define PMBUS_STATUS_IOUT_POUT_OP_WARNING		1
#define PMBUS_STATUS_IOUT_POUT_OP_FAULT			2
#define PMBUS_STATUS_IOUT_IN_POWER_LIMITING_MODE	4
#define PMBUS_STATUS_IOUT_CURRENT_SHARE_FAULT		8
#define PMBUS_STATUS_IOUT_UC_FAULT			0x10
#define PMBUS_STATUS_IOUT_OC_WARNING			0x20
#define PMBUS_STATUS_IOUT_OC_FAULT_WITH_LV_SHUTDOWN	0x40
#define PMBUS_STATUS_IOUT_OC_FAULT			0x80

#define PMBUS_STATUS_TEMP_UT_FAULT			0x10
#define PMBUS_STATUS_TEMP_UT_WARNING			0x20
#define PMBUS_STATUS_TEMP_OT_WARNING			0x40
#define PMBUS_STATUS_TEMP_OT_FAULT			0x80

/** Other memory or logic fault has occurred */
#define PMBUS_STATUS_CML_OTHER_MEM_OR_LOGIC		1
/** Other communications fault */
#define PMBUS_STATUS_CML_OTHER_COMM_FAULT		2
#define PMBUS_STATUS_CML_PROCESSOR_FAULT		8
#define PMBUS_STATUS_CML_MEMORY_FAULT			0x10
#define PMBUS_STATUS_CML_PACKET_ERROR_CHECK_FAILED	0x20
#define PMBUS_STATUS_CML_INVALID_DATA_RECEIVED		0x40
#define PMBUS_STATUS_CML_INVALID_COMMAND_RECEIVED	0x80

#define PMBUS_STATUS_OTHER_FIRST_TO_ASSERT_SMBALERT	1
#define PMBUS_STATUS_OTHER_OUTPUT_ORING_DEVICE_FAULT	2
#define PMBUS_STATUS_OTHER_INPUT_B_ORING_DEVICE_FAULT	4
#define PMBUS_STATUS_OTHER_INPUT_A_ORING_DEVICE_FAULT	8
#define PMBUS_STATUS_OTHER_INPUT_B_FUSE_OR_CIRCUIT_BREAK_FAULT	0x10
#define PMBUS_STATUS_OTHER_INPUT_A_FUSE_OR_CIRCUIT_BREAK_FAULT	0x20

#define PMBUS_STATUS_INPUT_PIN_OP_WARNING		1
#define PMBUS_STATUS_INPUT_IIN_OC_WARNING		2
#define PMBUS_STATUS_INPUT_IIN_OC_FAULT			4
#define PMBUS_STATUS_INPUT_UNIT_OFF_FOR_LOW_INPUT_VOLTAGE	8
#define PMBUS_STATUS_INPUT_VIN_UV_FAULT			0x10
#define PMBUS_STATUS_INPUT_VIN_UV_WARNING		0x20
#define PMBUS_STATUS_INPUT_VIN_OV_WARNING		0x40
#define PMBUS_STATUS_INPUT_VIN_OV_FAULT			0x80

#define PMBUS_STATUS_FANS12_AIR_FLOW_WARNING		1
#define PMBUS_STATUS_FANS12_AIR_FLOW_FAULT		2
#define PMBUS_STATUS_FANS12_FAN2_SPEED_OVERRIDE		4
#define PMBUS_STATUS_FANS12_FAN1_SPEED_OVERRIDE		8
#define PMBUS_STATUS_FANS12_FAN2_WARNING		0x10
#define PMBUS_STATUS_FANS12_FAN1_WARNING		0x20
#define PMBUS_STATUS_FANS12_FAN2_FAULT			0x40
#define PMBUS_STATUS_FANS12_FAN1_FAULT			0x80

#define PMBUS_STATUS_FANS34_AIR_FLOW_WARNING		1
#define PMBUS_STATUS_FANS34_AIR_FLOW_FAULT		2
#define PMBUS_STATUS_FANS34_FAN4_SPEED_OVERRIDE		4
#define PMBUS_STATUS_FANS34_FAN3_SPEED_OVERRIDE		8
#define PMBUS_STATUS_FANS34_FAN4_WARNING		0x10
#define PMBUS_STATUS_FANS34_FAN3_WARNING		0x20
#define PMBUS_STATUS_FANS34_FAN4_FAULT			0x40
#define PMBUS_STATUS_FANS34_FAN3_FAULT			0x80

/*
 * Unlike the pmic interface, the pmbus does not define support for batteries
 * and charging.
 */

enum pmbus_type { PMBUS_I2C, PMBUS_SPI, PMBUS_NONE};

/**
 * Various formats PMBus data can be in
 */
enum pmbus_code_data_format {
	NONE,
	DIRECT,
	VID,
	/**
	 * Linear mode uses a 5 bit signed exponent and 11 bit signed mantissa.
	 */
	LINEAR,
	/**
	 * Used by some MFG codes
	 */
	IEEE_HALF,
	/**
	 * Used by some MFG codes
	 */
	IEEE_SINGLE,
};

struct pmbus_i2c {
	unsigned char bus;
	unsigned char addr;
};

struct pmbus_spi {
	unsigned int cs;
	unsigned int mode;
	unsigned int bitlen;
	unsigned int clk;
	unsigned int flags;
	u32 (*prepare_tx)(u32 reg, u32 *val, u32 write);
};

union pmbus_linear {
	struct {
		int16_t exponent:5;
		int16_t mantissa:11;
	} s;
	uint16_t u16;
};

union pmbus_ieee_half {
	struct {
		uint16_t sign:1;
		uint16_t exponent:5;
		uint16_t mantissa:10;
	} s;
	uint16_t u16;
};

enum pmbus_vout_byte_mode {
	VOUT_MODE_LINEAR = 0,
	VOUT_MODE_VID = 1,
	VOUT_MODE_DIRECT = 2,
	VOUT_MODE_IEEE_HALF = 3
};

union pmbus_vout_mode {
	struct {
		uint8_t relative:1;	/** 0 = absolute, 1 = relative */
		uint8_t mode:2;		/** See pmbus_vout_byte_mode */
		/**
		 * LINEAR:	5-bit two's complement exponent for mantissa
		 * VID:		5-bit VID code
		 * DIRECT	Always 0
		 * IEEE_HALF	Always 0
		 */
		uint8_t parameter:5;
	} s;
	uint8_t u8;
};

union pmbus_vout_linear_mode {
	struct {
		uint32_t	:8;
		uint32_t	mode:3;
		uint32_t	exponent:5;
		uint32_t	mantissa:16;
	} s;
	uint32_t u32;
};

struct pmbus_code_coefficients {
	uint16_t m;
	uint16_t b;
	uint8_t r;
	uint8_t pec;
};

struct pmbus_dev;

/** Data structure to describe each PMBus code */
struct pmbus_code {
	uint32_t			code;		/** Code number */
	enum pmbus_code_data_format	format;		/** Format of code */
	uint8_t				num_bytes;	/** Size of code data */
	bool				read_only;	/** True if read-only */
	const char 			*name;		/** Name of code */
	/** Optional function to print the value */
	int (*print_fn)(const struct pmbus_dev *p, const struct pmbus_code *code,
			uint32_t value);
	/** Optional PMBUS command coefficients for DIRECT codes */
	struct pmbus_code_coefficients coeff;
};

struct pmbus_dev {
	char name[PMBUS_NAME_LEN];	/** Name of device */
	int bus;			/** PMBus number */
	enum pmbus_type	bus_type;	/** Type of interface */
	union hw {
		struct pmbus_i2c i2c;	/** I2C bus info */
		struct pmbus_spi spi;	/** SPI bus info */
	} hw;

	unsigned int num_codes;		/** Number of codes in the table */
	struct pmbus_code *codes;	/** Table of supported codes */

	int (*probe)(const struct pmbus_dev *);	/** Probe function */
	/** Optional print function */
	void (*print_reg)(const struct pmbus_dev *,
			  const struct pmbus_code *, uint32_t);

	struct pmbus_dev *parent;
	struct list_head list;
};

/**
 * Returns if a code is valid or not.
 *
 * @param[in]	p	pmbus device
 * @param	code	pmbus command code
 *
 * @return	0 if code is OK, -1 if not.
 */
int pmbus_check_code(const struct pmbus_dev *p, uint32_t code);

/**
 * Print out a PMBus code
 *
 * @param[in]	p	pmbus device
 * @param	code	command code to read and print
 *
 * @return 0 for success, otherwise error
 */
int pmbus_print_code(const struct pmbus_dev *p, uint32_t code);

/**
 * Obtains the direct coefficients in order to convert the value
 * @param[in,out]	p	pmbus device
 * @param		code	command code to read coefficients for
 *
 * @return 0 for success, otherwise error
 *
 * NOTE: This function will update the coefficients stored in the command code
 *	 table.
 */
int pmbus_get_direct_coefficients(struct pmbus_dev *p, uint32_t code);

/**
 * Read a command code
 *
 * @param[in]	p	pmbus device
 * @param	code	pmbus command code
 * @param[out]	val	value read from the command code
 *
 * @return	0 for success, otherwise error
 */
int pmbus_code_read(const struct pmbus_dev *p, uint32_t code, uint32_t *val);

/**
 * Writes a command code to a pmbus device
 *
 * @param[in]	p	pmbus device
 * @param	code	command code
 * @param	val	value to write
 *
 * @return 0 for success, otherwise error
 */
int pmbus_code_write(const struct pmbus_dev *p, uint32_t code, uint32_t val);

/**
 * Read and print out all defined readable PMBus command codes
 *
 * @param[in]	p	pmbus device
 *
 * @return 0 for success, -1 on error
 */
int pmbus_dump(const struct pmbus_dev *p);

/**
 * Probe to see if the device actually exist
 *
 * @param[in]	p	pmbus device to probe for
 *
 * @return 0 if device is valid, -1 if invalid.
 */
int pmbus_probe(const struct pmbus_dev *p);

/**
 * Allocates a new pmbus device
 *
 * @param[in]	name	Unique name associated with pmbus device, will be copied
 * @param[in]	codes	array of pmbus codes
 * @param	num_codes	size of the array
 *
 * @return pointer to new pmbus device or NULL if out of memory.
 */
struct pmbus_dev *pmbus_alloc(const char *name,
			      struct pmbus_code *codes, int num_codes);

/**
 * Given the name of a pmbus device, return the pmbus device data structure
 *
 * @param[in]	name	name of pmbus device
 *
 * @return pointer to pmbus device or NULL if not found.
 */
struct pmbus_dev *pmbus_get_by_name(const char *name);

/**
 * Print out a list of all pmbus devices and their interface bus
 */
void pmbus_list_names(void);

#define pmbus_i2c_addr(p) (p->hw.i2c.addr)
#define pmbus_i2c_bus(p) (p->hw.i2c.bus)

#define pmbus_spi_bitlen(p) (p->hw.spi.bitlen)
#define pmbus_spi_flags(p) (p->hw.spi.flags)

#endif /* __CORE_PMIC_H_ */
