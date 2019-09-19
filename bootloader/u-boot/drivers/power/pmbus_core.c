/*
 * Copyright (C) 2015 Cavium, Inc.
 * <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <linux/types.h>
#include <linux/list.h>
#include <power/pmbus.h>

/** List of pmbus devices */
static LIST_HEAD(pmbus_list);

/** pmbus bus number to assign to the next device */
static int pmbus_bus_number = 0;

/**
 * Internal function to read a command code
 *
 * @param[in]	p		pmbus device
 * @param[in]	pmbus_code	pmbus command code data structure
 * @param[out]	val		value read from the command code
 *
 * @return	0 for success, otherwise error
 *
 * @INTERNAL
 */
static int pmbus_code_read_internal(const struct pmbus_dev *p,
				    const struct pmbus_code *pmbus_code,
				    uint32_t *val);

/**
 * Given a code, look up the pmbus_code entry for it
 *
 * @param[in]	p	pmbus device
 * @param	code	code to look up
 *
 * @return pointer to pmbus code data structure or NULL if not found.
 */
static struct pmbus_code *pmbus_lookup_code(const struct pmbus_dev *p,
					    uint32_t code)
{
	int i;

	for (i = 0; i < p->num_codes; i++)
		if (p->codes[i].code == code)
			return &(p->codes[i]);
	return NULL;
}

/**
 * Returns a string representation for the pmbus bus type
 *
 * @param[in]	p	pmbus device
 *
 * @return	string representation of pmbus interface type
 */
static const char *pmbus_get_iface_type_str(const struct pmbus_dev *p)
{
	switch (p->bus_type) {
	case PMBUS_I2C:
		return "I2C";
	case PMBUS_SPI:
		return "SPI";
	case PMBUS_NONE:
		return "NONE";
	default:
		return "UNKNOWN";
	}
}

/**
 * Returns if a code is valid or not.
 *
 * @param[in]	p	pmbus device
 * @param	code	pmbus command code
 *
 * @return	0 if code is OK, -1 if not.
 */
int pmbus_check_code(const struct pmbus_dev *p, uint32_t code)
{
	return pmbus_lookup_code(p, code) ? 0 : -1;
}

static void pmbus_show_info(const struct pmbus_dev *p)
{
	printf("PMBus: %s\n", p->name);
}


/**
 * Internal routine to print a pmbus code
 *
 * @param[in]	p	pmbus device
 * @param[in]	code	pmbus code data structure
 *
 * @return	0 for success, -1 on error
 *
 * @INTERNAL
 */
static int pmbus_print_code_internal(const struct pmbus_dev *p,
				    const struct pmbus_code *code)
{
	enum pmbus_code_data_format dat_form;
	uint32_t val;
	int ret_code;
	int64_t int_val;
	int mantissa;
	bool neg;
	int frac;
	union pmbus_linear lin;
	const char *format_str[] = {
		"NONE", "DIRECT", "VID", "LINEAR", "IEEE_HALF", "IEEE_SINGLE"
	};
	char format[64];

	dat_form = code->format;
	if ((code->num_bytes > 0) && (dat_form != NONE)) {
		ret_code = pmbus_code_read_internal(p, code, &val);
		if (ret_code) {
			printf("Unable to read PMBUS code 0x%x\n", code->code);
			return ret_code;
		}
	} else {
		dat_form = NONE;
	}

	if (code->print_fn)
		return code->print_fn(p, code, val);

	switch (dat_form) {
	case NONE:
		printf("%-32s: 0x%02x: (no data)\n", code->name, code->code);
		break;
	case LINEAR:
		lin.u16 = val;
		neg = lin.s.mantissa < 0;
		if (lin.s.exponent >= 0) {
			int_val = lin.s.mantissa << lin.s.exponent;
			frac = 0;
		} else {
			if (neg)
				mantissa = -lin.s.mantissa;
			else
				mantissa = lin.s.mantissa;
			int_val = mantissa >> -lin.s.exponent;
			frac = 10000 * mantissa >> -lin.s.exponent;
			frac = frac % 10000;
			debug("  int: %ld, frac: %d, exponent: %d, mantissa: %d\n",
			      int_val, frac, lin.s.exponent, lin.s.mantissa);
		}
		printf("%-32s: 0x%02x: (LINEAR) %s%lld.%04d (0x%04x)\n",
		       code->name, code->code, neg ? "-" : "", int_val, frac,
		       val);
		break;
	case VID:
	case DIRECT:
	case IEEE_HALF:
	case IEEE_SINGLE:
		snprintf(format, sizeof(format),
			 "%%-32s: 0x%%02x: (%%s) 0x%%0%dx (%d bytes)\n",
			 2 * code->num_bytes, code->num_bytes);
		printf(format, code->name, code->code,
		       format_str[code->format], val);
		break;
	default:
		printf("%-32s: 0x%02x: Unknown type, value: 0x%x\n",
		       code->name, code->code, val);
	}
	return 0;
}

/**
 * Print out a PMBus code
 *
 * @param[in]	p	pmbus device
 * @param	code	command code to read and print
 *
 * @return 0 for success, otherwise error
 */
int pmbus_print_code(const struct pmbus_dev *p, uint32_t code)
{
	const struct pmbus_code *pmbus_code;

	pmbus_code = pmbus_lookup_code(p, code);
	if (!pmbus_code) {
		printf("Invalid PMBUS code 0x%x\n", code);
		return -1;
	}

	return pmbus_print_code_internal(p, pmbus_code);
}

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
int pmbus_get_direct_coefficients(struct pmbus_dev *p, uint32_t code)
{
	uint8_t buffer[8];
	struct pmbus_code *pmbus_code = pmbus_lookup_code(p, code);
	int ret_code;

	if (!pmbus_code) {
		printf("Invalid PMBUS code 0x%x\n", code);
		return -1;
	}
	if (pmbus_code->format != DIRECT) {
		printf("%s: coefficients are only for DIRECT codes, code 0x%x not supported\n",
		       __func__, code);
		return -1;
	}
	buffer[0] = 2;	/* Coefficients command code */
	buffer[1] = code;
	buffer[2] = 1;
	ret_code = i2c_write(p->hw.i2c.addr, 0x30, 1, buffer, 3);
	if (ret_code)
		return -1;
	I2C_SET_BUS(p->hw.i2c.bus);
	ret_code = i2c_read(p->hw.i2c.addr, 0x30, 1, buffer, 7);
	if (ret_code)
		return -1;
	if (buffer[0] != 5) {
		printf("%s: Too few coeffients for DIRECT code 0x%x\n",
		       __func__, code);
		return -1;
	}
	pmbus_code->coeff.m = buffer[1] | (buffer[2] << 8);
	pmbus_code->coeff.b = buffer[3] | (buffer[4] << 8);
	pmbus_code->coeff.r = buffer[5];

	return 0;
}

/**
 * Internal function to read a command code
 *
 * @param[in]	p		pmbus device
 * @param[in]	pmbus_code	pmbus command code data structure
 * @param[out]	val		value read from the command code
 *
 * @return	0 for success, otherwise error
 *
 * @INTERNAL
 */
static int pmbus_code_read_internal(const struct pmbus_dev *p,
				    const struct pmbus_code *pmbus_code,
				    uint32_t *val)
{
	uint8_t buffer[4];
	int ret_code;
	int i;

	debug("%s(%s): Reading code 0x%x from i2c bus %d:%d\n",
	      __func__, p->name, pmbus_code->code,
	      pmbus_i2c_bus(p), pmbus_i2c_addr(p));
	I2C_SET_BUS(pmbus_i2c_bus(p));
	ret_code = i2c_read(pmbus_i2c_addr(p), pmbus_code->code,
			    pmbus_code->code > 255 ? 2 : 1, buffer,
			    pmbus_code->num_bytes);
	if (ret_code) {
		printf("%s: Error reading code 0x%x from i2c address %d:%d\n",
		       __func__, pmbus_code->code,
		       pmbus_i2c_bus(p), pmbus_i2c_addr(p));
		return -1;
	}
	*val = 0;
	for (i = 0; i < pmbus_code->num_bytes; i++)
		*val |= buffer[i] << (i * 8);
	return 0;
}

/**
 * Read a command code
 *
 * @param[in]	p	pmbus device
 * @param	code	pmbus command code
 * @param[out]	val	value read from the command code
 *
 * @return	0 for success, otherwise error
 */
int pmbus_code_read(const struct pmbus_dev *p, uint32_t code, uint32_t *val)
{
	struct pmbus_code *pmbus_code = pmbus_lookup_code(p, code);

	if (!pmbus_code) {
		printf("Invalid PMBUS code 0x%x\n", code);
		return -1;
	}

	return pmbus_code_read_internal(p, pmbus_code, val);
}

/**
 * Writes a command code to a pmbus device
 *
 * @param[in]	p		pmbus device
 * @param[in]	pmbus_code	command code data structure
 * @param	val		value to write
 *
 * @return 0 for success, otherwise error
 *
 * @INTERNAL
 */
static int pmbus_code_write_internal(const struct pmbus_dev *p,
				     const struct pmbus_code *pmbus_code,
				     uint32_t val)
{
	uint8_t buffer[4];
	int ret_code;
	int i;


	for (i = 0; i < pmbus_code->num_bytes; i++)
		buffer[i] = ((val >> (i * 8)) & 0xff);
	I2C_SET_BUS(p->hw.i2c.bus);
	ret_code = i2c_write(p->hw.i2c.addr, pmbus_code->code,
			     pmbus_code->code > 255 ? 2 : 1, buffer,
			     pmbus_code->num_bytes);
	if (ret_code) {
		printf("%s: Error writing code 0x%x\n", __func__,
		       pmbus_code->code);
		return -1;
	}

	return 0;
}

/**
 * Writes a command code to a pmbus device
 *
 * @param[in]	p	pmbus device
 * @param	code	command code
 * @param	val	value to write
 *
 * @return 0 for success, otherwise error
 *
 * @INTERNAL
 */
int pmbus_code_write(const struct pmbus_dev *p, uint32_t code, uint32_t val)
{
	struct pmbus_code *pmbus_code = pmbus_lookup_code(p, code);
	if (!pmbus_code) {
		printf("Invalid PMBUS code 0x%x\n", code);
		return -1;
	}
	return pmbus_code_write_internal(p, pmbus_code, val);
}

/**
 * Read and print out all defined readable PMBus command codes
 *
 * @param[in]	p	pmbus device
 *
 * @return 0 for success, -1 on error
 */
int pmbus_dump(const struct pmbus_dev *p)
{
	int i, ret;
	u32 val;

	if (!p) {
		puts("Invalid PMBUS!\n");
		return -1;
	}

	pmbus_show_info(p);
	printf("%-32s: Code  Type   Value\n", "Code Name");
	for (i = 0; i < p->num_codes; i++) {
		if ((p->codes[i].num_bytes == 0) || (p->codes[i].format == NONE))
			continue;
		val = 0;
		ret = pmbus_code_read_internal(p, &p->codes[i], &val);
		if (ret)
			printf("PMBUS: Registers dump failed, index: %d, code: 0x%x, name: %s\n",
			       i, p->codes[i].code, p->codes[i].name);
		pmbus_print_code_internal(p, &p->codes[i]);
	}
	return 0;
}

/**
 * Probe to see if the device actually exist
 *
 * @param[in]	p	pmbus device to probe for
 *
 * @return 0 if device is valid, -1 if invalid.
 */
int pmbus_probe(const struct pmbus_dev *p)
{
	if (p->probe)
		return p->probe(p);

	I2C_SET_BUS(p->hw.i2c.bus);
	debug("%s: pmbus device %s probed\n", __func__, p->name);
	if (i2c_probe(pmbus_i2c_addr(p)))
		return -1;
	return 0;
}

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
			      struct pmbus_code *codes, int num_codes)
{
	struct pmbus_dev *p;

	p = calloc(sizeof(*p), 1);
	if (!p) {
		printf("%s: No available memory for allocation!\n", __func__);
		return NULL;
	}

	strncpy(p->name, name, PMBUS_NAME_LEN);
	p->bus = pmbus_bus_number++;
	p->codes = codes;
	p->num_codes = num_codes;
	list_add_tail(&p->list, &pmbus_list);

	debug("%s: new pmbus %s, struct: 0x%p, bus number: %d\n",
	      __func__, p->name, p, p->bus);

	return p;
}

/**
 * Given the name of a pmbus device, return the pmbus device data structure
 *
 * @param[in]	name	name of pmbus device
 *
 * @return pointer to pmbus device or NULL if not found.
 */
struct pmbus_dev *pmbus_get_by_name(const char *name)
{
	struct pmbus_dev *p;

	list_for_each_entry(p, &pmbus_list, list) {
		if (strcmp(p->name, name) == 0) {
			debug("%s: pmbus %s -> 0x%p\n", __func__, p->name, p);
			return p;
		}
	}

	return NULL;
}

/**
 * Print out a list of all pmbus devices and their interface bus
 */
void pmbus_list_names(void)
{
	struct pmbus_dev *p;

	puts("PMBus devices:\n");
	list_for_each_entry(p, &pmbus_list, list) {
		printf("name: %s bus: %s_%d\n", p->name,
		       pmbus_get_iface_type_str(p), p->bus);
	}
}

/**
 * Process pmbus command
 *
 * @INTERNAL
 */
static int do_pmbus(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 ret, reg, val;
	char *cmd, *name;
	struct pmbus_dev *p;

	/* at least two arguments please */
	if (argc < 2)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "list") == 0) {
		pmbus_list_names();
		return CMD_RET_SUCCESS;
	}

	name = argv[1];
	cmd = argv[2];

	debug("%s: name: %s cmd: %s\n", __func__, name, cmd);
	p = pmbus_get_by_name(name);
	if (!p) {
		printf("Could not find PMBus device %s\n", name);
		return CMD_RET_FAILURE;
	}

	if (strcmp(cmd, "dump") == 0) {
		debug("Dumping %s\n", name);
		if (pmbus_dump(p))
			return CMD_RET_FAILURE;
		return CMD_RET_SUCCESS;
	}

	if (strcmp(cmd, "read") == 0) {
		if (argc < 4)
			return CMD_RET_USAGE;

		reg = simple_strtoul(argv[3], NULL, 16);
		ret = pmbus_code_read(p, reg, &val);

		if (ret)
			puts("PMBUS: Register read failed\n");

		printf("\n0x%02x: 0x%08x\n", reg, val);

		return CMD_RET_SUCCESS;
	}

	if (strcmp(cmd, "write") == 0) {
		if (argc < 5)
			return CMD_RET_USAGE;

		reg = simple_strtoul(argv[3], NULL, 16);
		val = simple_strtoul(argv[4], NULL, 16);
		pmbus_code_write(p, reg, val);

		return CMD_RET_SUCCESS;
	}


	/* No subcommand found */
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	pmbus,	CONFIG_SYS_MAXARGS, 1, do_pmbus,
	"PMBus operations.  WARNING: Changing settings can damage the board.",
	"list - list available PMBUSes\n"
	"pmbus name dump - dump named PMIC registers\n"
	"pmbus name read <reg> - read register\n"
	"pmbus name write <reg> <value> - write register\n"
);
