/***********************license start************************************
 * Copyright (c) 2016 Cavium <support@cavium.com>. All rights
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
 * For any questions regarding licensing please contact support@cavium.com
 *
 ***********************license end**************************************/

/**
 * @file
 *
 * $Id: cmd_octeon.c 139702 2016-05-28 03:46:24Z awilliams $
 *
 */

#include <common.h>
#include <command.h>
#include <i2c.h>
#include <asm/arch/octeon_eeprom_types.h>
#include <compiler.h>

#ifndef CONFIG_SYS_SSD_VPD_MAX_POWER
# define CONFIG_SYS_SSD_VPD_MAX_POWER	0
#endif
#ifndef CONFIG_SYS_SSD_VPD_INIT_POWER
# define CONFIG_SYS_SSD_VPD_INIT_POWER	0
#endif
#ifndef CONFIG_SYS_SSD_VPD_PCIE0_GEN
# define CONFIG_SYS_SSD_VPD_PCIE0_GEN	1
#endif
#ifndef CONFIG_SYS_SSD_VPD_PCIE0_WIDTH
# define CONFIG_SYS_SSD_VPD_PCIE0_WIDTH	1
#endif
#ifndef CONFIG_SYS_SSD_VPD_PCIE1_GEN
# define CONFIG_SYS_SSD_VPD_PCIE1_GEN	0
#endif
#ifndef CONFIG_SYS_SSD_VPD_PCIE1_WIDTH
# define CONFIG_SYS_SSD_VPD_PCIE1_WIDTH	0
#endif

/** Default offset of temperature capability in EEPROM */
#define TEMP_CAP_OFFSET		0x50

/** Temperature capability ID */
#define TEMP_CAP		0xa2

struct vpd_header {
	uint8_t class_code[3];
	uint16_t pci_sig_vendor_id;
	uint8_t serial_no[20];
	uint8_t model_no[40];
	uint8_t p0_max_link_speed;
	uint8_t p0_max_link_width;
	uint8_t p1_max_link_speed;
	uint8_t p1_max_link_width;
	uint8_t initial_power_12v;
	uint8_t reserved[2];
	uint8_t max_power_12v;
	uint8_t reserved2[2];
	uint16_t cap_list_ptr;	/* little endian */
} __attribute__((packed));

struct capability_header {
	uint16_t capability_id;
	uint16_t next_capability_addr;
};

struct temp_capability {
	struct capability_header header;
	/**
	 * 0x00 = JC-42.4 TSE2004av compliant temperature sensor on 3.3Vaux
	 * 0x01 = JC-42.4 TSE2004av compliant temperature sensor on 12V
	 * 0x02 = JC-42.4 TSE2002av compliant temperature sensor on 3.3Vaux
	 * 0x03 = JC-42.4 TSE2002av compliant temperature sensor on 12V
	 */
	uint8_t sensor_type;
	/** SM-Bus slave address, bits 1-7. */
	uint8_t smbus_addr;

	uint16_t reserved;
	/**
	 * The specification appears to be wrong.  The specification says that
	 * the twos compliment centigrade temperature is in bits 2 through 12
	 * with bit 2 representing 0.125 and bit 11 representing 128C and bit
	 * 12 representing the 2s compliment sign.  I believe what the
	 * specification means is that bits 0-2 represent 0.125C and bits 3-11
	 * represent the integer temperature values with bit 12 representing
	 * the sign.  The value is stored in little-endian format.
	 */
	uint16_t warning_threshold;
	uint16_t overtemp_threshold;
};

#define VPD_HEADER_SIZE	(TEMP_CAP_OFFSET + sizeof(struct temp_capability))
enum sensor_type {
	TSE2004AV_3V_AUX = 0,
	TSE2004AV_12V = 1,
	TSE2002AV_3V_AUX = 2,
	TSE2002AV_12V = 3,
	/** SA56004X, ADM1032, MAX6657 */
	SA56004X = 0x44,
};

struct sensor_type_entry {
	enum sensor_type type;
	char *description;
};

const struct sensor_type_entry sensor_types[] = {
	{
		.type = TSE2004AV_3V_AUX,
		.description = "JC-42.4 TSE2004av compliant temperature sensor on 3.3Vaux",
	},
	{
		.type = TSE2004AV_12V,
		.description = "JC-42.4 TSE2004av compliant temperature sensor on 12V",
	},
	{
		.type = TSE2002AV_3V_AUX,
		.description = "JC-42.4 TSE2002av compliant temperature sensor on 3.3Vaux",
	},
	{
		.type = TSE2002AV_12V,
		.description = "JC-42.4 TSE2002av compliant temperature sensor on 12V",
	},
	{
		.type = SA56004X,
		.description = "SA56004X, ADM1032, MAX6657",
	}
};

DECLARE_GLOBAL_DATA_PTR;

extern void eeprom_init  (void);
extern int  eeprom_read  (unsigned dev_addr, unsigned offset,
			  uchar *buffer, unsigned cnt);
extern int  eeprom_write (unsigned dev_addr, unsigned offset,
			  uchar *buffer, unsigned cnt);

/**
 * Weak function called to make the VPD EEPROM available locally
 *
 * @param	enable	Set true for local access, false for remote access
 */
void board_enable_vpd(bool enable)
{
	return;
}
void board_enable_vpd(bool enable) __attribute__((weak));

/**
 * Gets the board model and serial number
 *
 * @param[out]	model_no	21 character \0 terminated model number
 * @param[out]	serial_no	41 character \0 terminated serial number
 *
 * @return	0 for success, -1 on error
 */
int board_get_ssd_vpd_model_serial(char *model_no, char *serial_no)
{
	return -1;
}
int board_get_ssd_vpd_model_serial(char *model_no, char *serial_no)
	__attribute__((weak));

int board_get_ssd_vpd_power(bool initial)
{
	if (initial)
		return CONFIG_SYS_SSD_VPD_INIT_POWER;
	else
		return CONFIG_SYS_SSD_VPD_MAX_POWER;
}
int board_get_ssd_vpd_power(bool initial) __attribute__((weak));

/**
 * Print out the SSD VPD data
 *
 * @param[in]	buffer		Buffer containing VPD data
 * @param	len		Length of buffer
 *
 * @return	0 for success, -1 on failure
 */
int ssd_smbus_vpd_print(const void *buffer, size_t len)
{
	struct vpd_header *vpd = (struct vpd_header *)buffer;
	struct temp_capability *temp;
	uint16_t offset;
	int temp_c, temp_frac;
	const struct sensor_type_entry *sensor;
	int i;

	if (len < VPD_HEADER_SIZE) {
		printf("Error: buffer length %u too small!  Must be at least %u bytes\n",
		       len, VPD_HEADER_SIZE);
		return -1;
	};

	printf("Class:         %02x:%02x:%02x\n", vpd->class_code[2],
	       vpd->class_code[1], vpd->class_code[0]);
	if ((vpd->class_code[0] != 0) || (vpd->class_code[1] != 0x80) ||
		(vpd->class_code[2] != 0x02)) {
		printf("Unknown VPC class code\n");
		return -1;
	}
	printf("Vendor ID:     %04x", le16_to_cpu(vpd->pci_sig_vendor_id));
	printf("Serial number: %.20s\n", vpd->serial_no);
	printf("Model:         %.20s\n", vpd->model_no);
	if (vpd->p0_max_link_speed) {
		printf("PCIe 0 gen:    %u\n", vpd->p0_max_link_speed);
		printf("PCIe 0 width:  %u\n", vpd->p0_max_link_width);
	}
	if (vpd->p1_max_link_speed) {
		printf("PCIe 1 gen:    %u\n", vpd->p1_max_link_speed);
		printf("PCIe 1 width:  %u\n", vpd->p1_max_link_width);
	}
	printf("Initial 12v power:     %u watts\n", vpd->initial_power_12v);
	printf("Maximum 12v power:     %u watts\n", vpd->max_power_12v);
	offset = le16_to_cpu(vpd->cap_list_ptr);
	if (offset >= len) {
		printf("Error: buffer length %u too small, offset: 0x%x!\n",
		       len, offset);
		return -1;
	}
	debug("%s: starting offset: 0x%x\n", __func__, offset);
	temp = (struct temp_capability *)((uint8_t *)buffer + offset);
	while (offset && le16_to_cpu(temp->header.capability_id) != TEMP_CAP) {
		debug("%s: capability id: 0x%x\n", __func__,
		      temp->header.capability_id);
		offset = le16_to_cpu(temp->header.next_capability_addr);
		if (offset >= len) {
			printf("Error: buffer length %u too small!  offset: %u, cap id: 0x%x\n",
			       len, offset,
			       le16_to_cpu(temp->header.capability_id));
			return -1;
		}
		debug("%s: next offset: 0x%x\n", __func__, offset);
		temp = (struct temp_capability *)((uint8_t *)buffer + offset);
	}

	if (offset) {
		sensor = NULL;
		for (i = 0; i < ARRAY_SIZE(sensor_types); i++)
			if (sensor_types[i].type == temp->sensor_type) {
				sensor = &sensor_types[i];
				break;
			}

		printf("Temperature SM-Bus Address: 0x%02x\n",
		       temp->smbus_addr);
		if (sensor)
			printf("Temperature Sensor Type: (%u) %s\n",
			       sensor->type, sensor->description);
		else
			printf("Unknown sensor type 0x%x\n",
			       temp->sensor_type);
		temp_c = le16_to_cpu(temp->warning_threshold) >> 2;
		temp_frac =
			((le16_to_cpu(temp->warning_threshold) & 3) * 250) % 1000;
		printf("Warning temperature: %u.%uC\n", temp_c, temp_frac);
		temp_c = le16_to_cpu(temp->overtemp_threshold) >> 2;
		temp_frac =
			((le16_to_cpu(temp->overtemp_threshold) & 3) * 250) % 1000;
		printf("Over temperature: %u.%uC\n", temp_c, temp_frac);
	}
	return 0;
}

/**
 * Outputs the warning and over temperature values
 *
 * @param[out]	warn_temp	Integer portion of warning temperature
 * @param[out]	over_temp_frac	Fractional portion of warning temperature
 * @param[out]	over_temp	Integer portion of over temperature
 * @param[out]	over_temp_frac	Fractional portion of over temperature
 *
 * @return 0 for success, error otherwise
 */
int ssd_smbus_get_temps(int *warn_temp, int *warn_temp_frac,
			int *over_temp, int *over_temp_frac)
{
	uint8_t buffer[1024];
	struct vpd_header *vpd = (struct vpd_header *)buffer;
	struct temp_capability *temp;
	int rc;
	unsigned short offset;

	board_enable_vpd(true);
	i2c_set_bus_num(CONFIG_SYS_SSD_VPD_BUS);

	rc = eeprom_read(CONFIG_SYS_SSD_VPD_ADDR, 0, buffer,
			 sizeof(buffer));
	if (rc) {
		printf("%s: error reading VPD eeprom\n", __func__);
		goto error;
	}

	if (vpd->class_code[0] != 0x0 || vpd->class_code[1] != 0x80 ||
	    vpd->class_code[2] != 0x2) {
		debug("%s: invalid VPD header class\n", __func__);
		rc = -1;
		goto error;
	}

	offset = le16_to_cpu(vpd->cap_list_ptr);
	if (offset >= (sizeof(buffer) - sizeof(struct temp_capability))) {
		printf("%s: Error: capability offset out of range\n", __func__);
		rc = -1;
		goto error;
	}
	temp = (struct temp_capability *)&buffer[offset];
	while (offset && le16_to_cpu(temp->header.capability_id) != TEMP_CAP) {
		offset = le16_to_cpu(temp->header.next_capability_addr);
		if (offset > (sizeof(buffer) - sizeof(*temp))) {
			printf("%s: temp capability not found\n", __func__);
			rc = -1;
			goto error;
		}
		temp = (struct temp_capability *)&buffer[offset];
	}
	if (!offset) {
		printf("%s: temp capability not found\n", __func__);
		rc = -1;
		goto error;
	}
	*warn_temp = le16_to_cpu(temp->warning_threshold);
	*warn_temp_frac = (*warn_temp & 0x3) * 250;
	*warn_temp >>= 2;
	if (*warn_temp & 0x100)
		*warn_temp = -(*warn_temp);

	*over_temp = le16_to_cpu(temp->overtemp_threshold);
	*over_temp_frac = (*over_temp & 0x3) * 250;
	*over_temp >>= 2;
	if (*over_temp & 0x100)
		*over_temp = -(*over_temp);
	rc = 0;

error:
	board_enable_vpd(false);
	return rc;
}

/**
 * Fills in a buffer with the VPD structure
 *
 * @param[out]		buffer	Buffer to hold the structures
 * @param		len	Size of buffer
 * @param[in]		serial_no	Serial number
 * @param[in]		model_no	Model number
 * @param		initial_power	Initial power draw
 * @param		max_power	Maximum power draw
 * @param		warn_temp	Warning temperature in C
 * @param		warning_temp_frac	Fractional warning temp in
 *						multiples of 125
 * @param		over_temp	Over temperature in C
 * @param		over_temp_frac	Fractional over temp in .125
 * @param		sensor_type	Type of sensor
 * @param		sensor_addr	i2c address of sensor
 *
 * @return	0 for success, -1 on error;
 */
int ssd_smbus_set_vpd(void *buffer, size_t len,
		      const char *serial_no, const char *model_no,
		      uint8_t initial_power, uint8_t max_power,
		      int warning_temp, int warning_temp_frac,
		      int over_temp, int over_temp_frac,
		      uint8_t sensor_type, uint8_t sensor_addr)
{
	struct vpd_header *header = (struct vpd_header *)buffer;
	struct temp_capability *temp =
		(struct temp_capability *)(buffer + TEMP_CAP_OFFSET);
	int i;

	debug("%s: warning_temp: %d, warning_temp_frac: %d\n", __func__,
	      warning_temp, warning_temp_frac);
	debug("%s(%p, %u, %s, %s, %u, %u, %d.%d, %d.%d, 0x%x, 0x%x)\n",
	      __func__, buffer, len, serial_no, model_no,
	      initial_power, max_power,
	      warning_temp, warning_temp_frac, over_temp, over_temp_frac,
	      sensor_type, sensor_addr);
	if (len < VPD_HEADER_SIZE) {
		printf("%s: Error: buffer length %u too small, must be %u bytes\n",
		       __func__, len, VPD_HEADER_SIZE);
		return -1;
	}

	memset(buffer, 0, VPD_HEADER_SIZE);
	header->class_code[0] = 0x00;
	header->class_code[1] = 0x80;	/* Ethernet controller */
	header->class_code[2] = 0x02;
	header->pci_sig_vendor_id = cpu_to_le16(CONFIG_SYS_SSD_VPD_VEND_ID);
	strncpy((char *)header->serial_no, serial_no, sizeof(header->serial_no));
	for (i = strlen((char *)header->serial_no);
	     i < sizeof(header->serial_no); i++)
		header->serial_no[i] = ' ';
	strncpy((char *)header->model_no, model_no, sizeof(header->model_no));
	for (i = strlen((char *)header->model_no);
	     i < sizeof(header->model_no); i++)
		header->model_no[i] = ' ';
	header->p0_max_link_speed = CONFIG_SYS_SSD_VPD_PCIE0_GEN;
	header->p0_max_link_width = CONFIG_SYS_SSD_VPD_PCIE0_WIDTH;
	header->p1_max_link_speed = CONFIG_SYS_SSD_VPD_PCIE1_GEN;
	header->p1_max_link_width = CONFIG_SYS_SSD_VPD_PCIE1_WIDTH;
	header->initial_power_12v = initial_power;
	header->max_power_12v = max_power;
	header->cap_list_ptr = cpu_to_le16(TEMP_CAP_OFFSET);

	temp->header.capability_id = cpu_to_le16(TEMP_CAP);
	temp->header.next_capability_addr = cpu_to_le16(0);
	temp->sensor_type = sensor_type;
	temp->smbus_addr = sensor_addr << 1;
	warning_temp_frac /= 250;
	debug("%s: scaled warning temp frac: %d\n", __func__, warning_temp_frac);
	if (warning_temp < 0)
		warning_temp |= 1 << 9;
	temp->warning_threshold =
		cpu_to_le16(((warning_temp << 2) |
			     (warning_temp_frac & 3)) & 0x1fff);
	over_temp_frac /= 250;
	if (over_temp < 0)
		over_temp |= 1 << 9;
	temp->overtemp_threshold =
		cpu_to_le16(((over_temp << 2) | (over_temp_frac & 3)) & 0x1fff);

	return 0;
}

/**
 * Parses a temperature string in the form of int.frac
 *
 * @param[in]	buffer		String buffer to parse
 * @param[out]	temp		Integer temperature value
 * @param[out]	frac		Fractional temperature value
 *
 * @return	0 for success, -1 on error
 */
static int parse_temp(const char *buffer, int *temp, int *frac)
{
	char *end;
	int exp = 0;
	int len = 0;

	*temp = simple_strtol(buffer, &end, 10);
	*frac = 0;
	/* See if there's a fractional part */
	if (*end == '.') {
		/* Yes, read it */
		end++;
		*frac = simple_strtol(end, NULL, 10);
		len = strlen(end);
		exp = 3 - len;
	}

	/* Make fractional part 3 digits, add or remove digits as needed */
	while (exp) {
		if (exp > 0) {
			*frac *= 10;
			exp--;
		} else {
			*frac /= 10;
			exp++;
		}
	}

	debug("%s(%s, %d, %d)\n", __func__, buffer, *temp, *frac);

	if (*temp > 127 || *temp < -127) {
		printf("Error: temperature %d out of range\n", *temp);
		return -1;
	}
	return 0;
}

/**
 * Handles the ssdvpd command
 *
 * @param	cmdtp		Command data structure
 * @param	flag		Command flags
 * @param	argc		Argument count
 * @param	argv		Array of arguments
 *
 * @return	CMD_RET_SUCCESS, CMD_RET_USAGE or CMD_RET_FAILURE
 */
int do_ssd_smbus(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint8_t eeprom_buf[1024];
	int rc;
	int warn_temp, warn_frac = 0, over_temp, over_frac = 0;
	char serial_no[21], model_no[41];
	int initial_power, max_power;

	if (argc < 2)
		return CMD_RET_USAGE;

	board_enable_vpd(true);
	eeprom_init();

	if (!strcmp(argv[1], "print")) {
		i2c_set_bus_num(CONFIG_SYS_SSD_VPD_BUS);
		rc = eeprom_read(CONFIG_SYS_SSD_VPD_ADDR, 0, eeprom_buf,
				 sizeof(eeprom_buf));
		if (rc) {
			printf("Error reading SSD VPD EEPROM\n");
			rc = CMD_RET_FAILURE;
			goto error;
		}
		ssd_smbus_vpd_print(eeprom_buf, sizeof(eeprom_buf));
		rc = CMD_RET_SUCCESS;
	} else if (!strcmp(argv[1], "set")) {
		if (argc < 4)
			return CMD_RET_USAGE;

		i2c_set_bus_num(CONFIG_SYS_SSD_VPD_BUS);
		memset(eeprom_buf, 0x00, VPD_HEADER_SIZE);
		rc = board_get_ssd_vpd_model_serial(model_no, serial_no);
		if (rc) {
			puts("Error: could not get board model number and/or serial number\n");
			rc = CMD_RET_FAILURE;
			goto error;
		}

		rc = parse_temp(argv[2], &warn_temp, &warn_frac);
		if (rc) {
			printf("Invalid temperature %s\n", argv[2]);
			rc = CMD_RET_FAILURE;
			goto error;
		}
		rc = parse_temp(argv[3], &over_temp, &over_frac);
		if (rc) {
			printf("Invalid temperature %s\n", argv[3]);
			rc = CMD_RET_FAILURE;
			goto error;
		}
		initial_power = board_get_ssd_vpd_power(true);
		max_power = board_get_ssd_vpd_power(false);
		if (argc > 4) {
			printf("Overriding model number with %s instead of %s\n",
			       argv[4], model_no);
			strncpy(model_no, argv[4], sizeof(model_no));
		}
		if (argc > 6) {
			initial_power = simple_strtol(argv[5], NULL, 10);
			max_power = simple_strtol(argv[6], NULL, 10);
			printf("Overriding initial power to %d watts and maximum power to %d watts\n",
			       initial_power, max_power);
		}

		rc = ssd_smbus_set_vpd(eeprom_buf, sizeof(eeprom_buf),
				       serial_no, model_no,
				       initial_power, max_power,
				       warn_temp, warn_frac,
				       over_temp, over_frac,
				       CONFIG_SYS_SSD_VPD_SENSOR_TYPE,
				       CONFIG_SYS_SSD_VPD_SENSOR_ADDR);
		if (rc) {
			puts("Error setting VPD values");
			rc = CMD_RET_FAILURE;
			goto error;
		}
#ifdef DEBUG
		print_buffer(0, eeprom_buf, 1, VPD_HEADER_SIZE, 0);
#endif
		debug("%s: Writing to EEPROM at 0x%x\n", __func__,
		      CONFIG_SYS_SSD_VPD_ADDR);
		rc = eeprom_write(CONFIG_SYS_SSD_VPD_ADDR, 0, eeprom_buf,
				  min(VPD_HEADER_SIZE, sizeof(eeprom_buf)));
		if (rc) {
			printf("Error %d writing to VPD EEPROM\n", rc);
			rc = CMD_RET_FAILURE;
			goto error;
		}
		rc = CMD_RET_SUCCESS;
	} else {
		printf("Unknown command %s\n", argv[1]);
		rc = CMD_RET_USAGE;
	}

error:
	board_enable_vpd(false);

	return rc;
}

U_BOOT_CMD(ssdvpd, 8, 0, do_ssd_smbus,
	   "Prints or sets the SSD/NVME VPD EEPROM temperature values",
	   "print - prints out the current values\n"
	   "set [warning temp[.frac]] [over temp[.frac]] where frac is the\n"
	   "       fractional value in 0.125 increments.\n");
