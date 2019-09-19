/*
 *  Copyright (C) 2015 Cavium, Inc.
 *  support@cavium.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <fdtdec.h>
#include <i2c.h>
#include <power/pmbus.h>
#include <power/mp2953_pmbus.h>
#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

static int mp2953_dev_no = 0;

/**
 * The MP2953 supports many of the standard PMBus commands including some that
 * are not documented in the data sheet.
 */
enum mp2953_command_codes {
	OPERATION = 1,
	CLEAR_FAULTS = 3,
	STORE_USER_ALL = 0x15,
	RESTORE_USER_ALL = 0x16,
	VOUT_COMMAND = 0x21,
	VOUT_MAX = 0x24,
	VOUT_MARGIN_HIGH = 0x25,
	VOUT_MARGIN_LOW = 0x26,
	VIN_ON = 0x35,
	VIN_OFF = 0x36,
	IOUT_CAL_GAIN = 0x38,	/* Not in datasheet */
	IOUT_CAL_OFFSET = 0x39,	/* Not in datasheet */
	VIN_OV_FAULT_LIMIT = 0x55,
	VIN_UV_WARN_LIMIT = 0x58,
	STATUS_BYTE = 0x78,
	STATUS_WORD = 0x79,
	STATUS_VOUT = 0x7a,
	STATUS_IOUT = 0x7b,
	STATUS_INPUT = 0x7c,
	STATUS_TEMPERATURE = 0x7d,
	STATUS_CML = 0x7e,
	READ_VIN = 0x88,
	READ_IIN = 0x89,
	READ_VOUT = 0x8b,
	READ_IOUT = 0x8c,
	READ_TEMPERATURE = 0x8d,
	READ_POUT = 0x96,
	READ_PIN = 0x97,
	USER_DATA_00 = 0xb0,
	USER_DATA_01 = 0xb1,
	USER_DATA_02 = 0xb2,
	USER_DATA_03 = 0xb3,
	USER_DATA_04 = 0xb4,
	USER_DATA_05 = 0xb5,
	USER_DATA_06 = 0xb6,
	USER_DATA_07 = 0xb7,
	USER_DATA_10 = 0xba,
	USER_DATA_11 = 0xbb,
	MFR_MP2953_VR_CONFIG = 0xd0,
	MFR_MP2953_TEMPERATURE = 0xd1,
	MFR_MP2953_FS_SET = 0xd2,
	MFR_SPECIFIC_03 = 0xd3,
	MFR_SPECIFIC_04 = 0xd4,
	MFR_SPECIFIC_05 = 0xd5,
	MFR_SPECIFIC_06 = 0xd6,
	MFR_SPECIFIC_07 = 0xd7,
	MFR_MP2953_ADDR_PMBUS = 0xd8,
	MFR_MP2953_ADDR_SVID = 0xd9,
	MFR_SPECIFIC_10 = 0xda,
	MFR_MP2953_OVER_TEMP_LIMIT = 0xdb,
	MFR_SPECIFIC_12 = 0xdc,
	MFR_MP2953_CUR_OFFSET = 0xdd,
	MFR_SPECIFIC_14 = 0xde,
	MFR_SPECIFIC_15 = 0xdf,
	MFR_SPECIFIC_16 = 0xe0,
	MFR_MP2953_CS_TEMP1 = 0xe1,
	MFR_MP2953_CS_TEMP2 = 0xe2,
	MFR_MP2953_CS_TEMP3 = 0xe3,
	MFR_MP2953_CS_TEMP4 = 0xe4,
	MFR_MP2953_CS_TEMP5 = 0xe5,
	MFR_MP2953_CS_TEMP6 = 0xe6,
	MFR_SPECIFIC_23 = 0xe7,
	MFR_SPECIFIC_24 = 0xe8,
	MFR_SPECIFIC_25 = 0xe9,
	MFR_MP2953_VBOOT = 0xea,
	MFR_MP2953_ICC_MAX = 0xeb,
	MFR_MP2953_TEMP_MAX = 0xec,
	MFR_SPECIFIC_29 = 0xed,
	MFR_MP2953_VR_TOLERANCE = 0xee,
	MFR_MP2953_VENDOR_ID_VR = 0xef,
	MFR_MP2953_PRODUCT_ID_VR = 0xf0,
	MFR_MP2953_PRODUCT_REV_VR = 0xf1,
	MFR_MP2953_PRODUCT_DATA_CODE_VR = 0xf2,
	MFR_MP2953_LOT_CODE_VR = 0xf3,
	MFR_MP2953_PROTOCOL_ID_VR = 0xf4,
	MFR_MP2953_CAPABILITY_VR = 0xf5,
	MFR_SPECIFIC_38 = 0xf6,
	MFR_SPECIFIC_39 = 0xf7,
	MFR_SPECIFIC_40 = 0xf8,
	MFR_SPECIFIC_41 = 0xf9,
	MFR_SPECIFIC_42 = 0xfa,
	MFR_SPECIFIC_43 = 0xfb,
	MFR_SPECIFIC_44 = 0xfc,
	MFR_SPECIFIC_45 = 0xfd,
	MFR_SPECIFIC_46 = 0xfe,
};

static const struct pmbus_code code_info[] = {
	{ OPERATION, DIRECT, 1, false, "OPERATION", NULL },
	{ CLEAR_FAULTS, NONE, 0, false, "CLEAR_FAULTS", NULL },
	{ STORE_USER_ALL, NONE, 0, false, "STORE_USER_ALL", NULL },
	{ RESTORE_USER_ALL, NONE, 0, false, "RESTORE_USER_ALL", NULL },
	{ VOUT_COMMAND, VID, 2, false, "VOUT_COMMAND", NULL },
	{ VOUT_MAX, VID, 2, false, "VOUT_MAX", NULL },
	{ VOUT_MARGIN_HIGH, VID, 2, false, "VOUT_MARGIN_HIGH", NULL },
	{ VOUT_MARGIN_LOW, VID, 2, false, "VOUT_MARGIN_LOW", NULL },
	{ VIN_ON, LINEAR, 2, false, "VIN_ON", NULL },
	{ VIN_OFF, LINEAR, 2, false, "VIN_OFF", NULL },
	{ IOUT_CAL_GAIN, LINEAR, 2, false, "IOUT_CAL_GAIN", NULL },
	{ IOUT_CAL_OFFSET, LINEAR, 2, false, "IOUT_CAL_OFFSET", NULL },
	{ VIN_OV_FAULT_LIMIT, LINEAR, 2, false, "VIN_OV_FAULT_LIMIT", NULL },
	{ VIN_UV_WARN_LIMIT, LINEAR, 2, false, "VIN_UV_WARN_LIMIT", NULL },
	{ STATUS_BYTE, DIRECT, 1, true, "STATUS_BYTE", NULL },
	{ STATUS_WORD, DIRECT, 2, true, "STATUS_WORD", NULL },
	{ STATUS_VOUT, VID, 1, true, "STATUS_VOUT", NULL },
	{ STATUS_IOUT, DIRECT, 1, true, "STATUS_IOUT", NULL },
	{ STATUS_INPUT, DIRECT, 1, true, "STATUS_INPUT", NULL },
	{ STATUS_TEMPERATURE, DIRECT, 1, true, "STATUS_TEMPERATURE", NULL },
	{ STATUS_CML, DIRECT, 1, true, "STATUS_CML", NULL },
	{ READ_VIN, LINEAR, 2, true, "READ_VIN", NULL },
	{ READ_IIN, LINEAR, 2, true, "READ_IIN", NULL },
	{ READ_VOUT, VID, 2, true, "READ_VOUT", NULL },
	{ READ_IOUT, LINEAR, 2, true, "READ_IOUT", NULL },
	{ READ_TEMPERATURE, LINEAR, 2, true, "READ_TEMPERATURE", NULL },
	{ READ_POUT, LINEAR, 2, true, "READ_POUT", NULL },
	{ READ_PIN, LINEAR, 2, true, "READ_PIN", NULL },
	{ USER_DATA_00, DIRECT, 1, false, "USER_DATA_00", NULL },
	{ USER_DATA_01, DIRECT, 1, false, "USER_DATA_01", NULL },
	{ USER_DATA_02, DIRECT, 1, false, "USER_DATA_02", NULL },
	{ USER_DATA_03, DIRECT, 1, false, "USER_DATA_03", NULL },
	{ USER_DATA_04, DIRECT, 1, false, "USER_DATA_04", NULL },
	{ USER_DATA_05, DIRECT, 1, false, "USER_DATA_05", NULL },
	{ USER_DATA_06, DIRECT, 2, false, "USER_DATA_06", NULL },
	{ USER_DATA_07, DIRECT, 1, false, "USER_DATA_07", NULL },
	{ USER_DATA_10, DIRECT, 2, false, "USER_DATA_10", NULL },
	{ USER_DATA_11, DIRECT, 2, false, "USER_DATA_11", NULL },
	{ MFR_MP2953_VR_CONFIG, DIRECT, 2, false, "MFR_MP2953_VR_CONFIG", NULL },
	{ MFR_MP2953_TEMPERATURE, DIRECT, 2, false, "MFR_MP2953_TEMPERATURE", NULL },
	{ MFR_MP2953_FS_SET, DIRECT, 1, false, "MFR_MP2953_FS_SET", NULL },
	{ MFR_SPECIFIC_03, DIRECT, 2, false, "MFR_SPECIFIC_03", NULL },
	{ MFR_SPECIFIC_04, DIRECT, 2, false, "MFR_SPECIFIC_04", NULL },
	{ MFR_SPECIFIC_05, DIRECT, 2, false, "MFR_SPECIFIC_05", NULL },
	{ MFR_SPECIFIC_06, DIRECT, 1, false, "MFR_SPECIFIC_06", NULL },
	{ MFR_SPECIFIC_07, DIRECT, 1, false, "MFR_SPECIFIC_07", NULL },
	{ MFR_MP2953_ADDR_PMBUS, DIRECT, 1, false, "MFR_MP2953_ADDR_PMBUS", NULL },
	{ MFR_MP2953_ADDR_SVID, DIRECT, 1, false, "MFR_MP2953_ADDR_SVID", NULL },
	{ MFR_SPECIFIC_10, DIRECT, 1, false, "MFR_SPECIFIC_10", NULL },
	{ MFR_MP2953_OVER_TEMP_LIMIT, DIRECT, 2, false, "MFR_MP2953_OVER_TEMP_LIMIT", NULL },
	{ MFR_SPECIFIC_12, DIRECT, 2, false, "MFR_SPECIFIC_12", NULL },
	{ MFR_SPECIFIC_14, DIRECT, 2, false, "MFR_SPECIFIC_14", NULL },
	{ MFR_SPECIFIC_15, DIRECT, 2, false, "MFR_SPECIFIC_15", NULL },
	{ MFR_SPECIFIC_16, DIRECT, 2, false, "MFR_SPECIFIC_16", NULL },
	{ MFR_MP2953_CUR_OFFSET, DIRECT, 1, false, "MFR_MP2953_CUR_OFFSET", NULL },
	{ MFR_MP2953_CS_TEMP1, DIRECT, 2, true, "MFR_MP2953_CS_TEMP1", NULL },
	{ MFR_MP2953_CS_TEMP2, DIRECT, 2, true, "MFR_MP2953_CS_TEMP2", NULL },
	{ MFR_MP2953_CS_TEMP3, DIRECT, 2, true, "MFR_MP2953_CS_TEMP3", NULL },
	{ MFR_MP2953_CS_TEMP4, DIRECT, 2, true, "MFR_MP2953_CS_TEMP4", NULL },
	{ MFR_MP2953_CS_TEMP5, DIRECT, 2, true, "MFR_MP2953_CS_TEMP5", NULL },
	{ MFR_MP2953_CS_TEMP6, DIRECT, 2, true, "MFR_MP2953_CS_TEMP6", NULL },
	{ MFR_SPECIFIC_23, DIRECT, 1, false, "MFR_SPECIFIC_23", NULL },
	{ MFR_SPECIFIC_24, DIRECT, 1, false, "MFR_SPECIFIC_24", NULL },
	{ MFR_SPECIFIC_25, DIRECT, 1, false, "MFR_SPECIFIC_25", NULL },
	{ MFR_MP2953_VBOOT, DIRECT, 2, false, "MFR_MP2953_VBOOT", NULL },
	{ MFR_MP2953_ICC_MAX, DIRECT, 1, false, "MFR_MP2953_ICC_MAX", NULL },
	{ MFR_MP2953_TEMP_MAX, DIRECT, 1, false, "MFR_MP2953_TEMP_MAX", NULL },
	{ MFR_SPECIFIC_29, DIRECT, 2, false, "MFR_SPECIFIC_29", NULL },
	{ MFR_MP2953_VR_TOLERANCE, DIRECT, 1, false, "MFR_MP2953_VR_TOLERANCE", NULL },
	{ MFR_MP2953_VENDOR_ID_VR, DIRECT, 1, false, "MFR_MP2953_VENDOR_ID_VR", NULL },
	{ MFR_MP2953_PRODUCT_ID_VR, DIRECT, 1, false, "MFR_MP2953_PRODUCT_ID_VR", NULL },
	{ MFR_MP2953_PRODUCT_REV_VR, DIRECT, 1, false, "MFR_MP2953_PRODUCT_REV_VR", NULL },
	{ MFR_MP2953_PRODUCT_DATA_CODE_VR, DIRECT, 1, false, "MFR_MP2953_PRODUCT_DATA_CODE_VR", NULL },
	{ MFR_MP2953_LOT_CODE_VR, DIRECT, 1, false, "MFR_MP2953_LOT_CODE_VR", NULL },
	{ MFR_MP2953_PROTOCOL_ID_VR, DIRECT, 1, false, "MFR_MP2953_PROTOCOL_ID_VR", NULL },
	{ MFR_MP2953_CAPABILITY_VR, DIRECT, 1, false, "MFR_MP2953_CAPABILITY_VR", NULL },
	{ MFR_SPECIFIC_38, DIRECT, 1, false, "MFR_SPECIFIC_38", NULL },
	{ MFR_SPECIFIC_39, DIRECT, 1, false, "MFR_SPECIFIC_39", NULL },
	{ MFR_SPECIFIC_40, DIRECT, 1, false, "MFR_SPECIFIC_40", NULL },
	{ MFR_SPECIFIC_41, DIRECT, 1, false, "MFR_SPECIFIC_41", NULL },
	{ MFR_SPECIFIC_42, DIRECT, 2, false, "MFR_SPECIFIC_42", NULL },
	{ MFR_SPECIFIC_43, DIRECT, 1, false, "MFR_SPECIFIC_43", NULL },
	{ MFR_SPECIFIC_44, DIRECT, 2, false, "MFR_SPECIFIC_44", NULL },
	{ MFR_SPECIFIC_45, DIRECT, 2, false, "MFR_SPECIFIC_45", NULL },
	{ MFR_SPECIFIC_46, DIRECT, 2, false, "MFR_SPECIFIC_46", NULL },
};


int mp2953_init(unsigned char bus)
{
	char name[PMBUS_NAME_LEN];
	struct pmbus_dev *p;
#ifdef CONFIG_OF_CONTROL
	const void *blob = gd->fdt_blob;
	int node, parent;
#endif

	snprintf(name, PMBUS_NAME_LEN, "MP2953A:%d", mp2953_dev_no++);

	p = pmbus_alloc(name, code_info, ARRAY_SIZE(code_info));
	if (!p) {
		printf("%s: POWER allocation error!\n", __func__);
		return -ENOMEM;
	}

#ifdef CONFIG_OF_CONTROL
	node = fdt_node_offset_by_compatible(blob, -1, "mpc,mp2953a");
	if (node < 0) {
		debug("PMIC: No node for PMIC Chip in device tree\n");
		debug("node = %d\n", node);
		return -1;
	}

	parent = fdt_parent_offset(blob, node);
	if (parent < 0) {
		debug("%s: Cannot find node parent\n", __func__);
		return -1;
	}

	p->hw.i2c.bus = i2c_get_bus_num_fdt(parent);
	if (p->hw.i2c.bus < 0) {
		debug("%s: Cannot find I2C bus\n", __func__);
		return -1;
	}
	p->hw.i2c.addr = fdtdec_get_int(blob, node, "reg", MP2953_I2C_ADDR);
	debug("%s(%d): i2c address: %d:0x%x\n", __func__, bus,
	      pmbus_i2c_bus(p), pmbus_i2c_addr(p));
#else
	p->bus = bus;
	p->hw.i2c.bus = bus;
	p->hw.i2c.addr = MP2953_I2C_ADDR;
#endif
	p->bus_type = PMBUS_I2C;

	return 0;
}

