
/*
 * (C) Copyright 2015-2016 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <command.h>
#include <common.h>
#include <net.h>
#include <sfp.h>
#include <malloc.h>
#include <i2c.h>

/**
 * Reads the SFP EEPROM using the i2c bus
 *
 * @param[out]	buffer		Buffer to store SFP EEPROM data in
 * @param	i2c_bus		i2c bus number to read from for SFP port
 * @param	i2c_addr	i2c address to use, 0 for default
 *
 * @return	-1 if invalid bus or i2c read error, 0 for success
 */
int phy_sfp_read_i2c_eeprom(uint8_t *buffer, int i2c_bus, int i2c_addr)
{
	int rc;
	int old_bus;

	if (!i2c_addr)
		i2c_addr = SFP_DEFAULT_I2C_ADDR;
	debug("%s: Reading sfp at %02x:%02x\n", __func__, i2c_bus, i2c_addr);
	memset(buffer, 0, SFP_MAX_EEPROM_SIZE);
	old_bus = i2c_get_bus_num();
	if (i2c_set_bus_num(i2c_bus)) {
		printf("Invalid i2c bus %d\n", i2c_bus);
		return -1;
	}
	rc = i2c_read(i2c_addr, 0, 1, buffer, SFP_MAX_EEPROM_SIZE);
	i2c_set_bus_num(old_bus);
#ifdef DEBUG
	if (rc == 0)
		print_buffer(0, buffer, 1, SFP_MAX_EEPROM_SIZE, 0);
	else
		printf("Error reading SFP EEPROM at %02x:%02x\n",
		       i2c_bus, i2c_addr);
#endif
	return rc == 0 ?  0 : -1;
}

static int phy_sfp_parse_sfp(struct phy_sfp_info *sfp_info,
			     const uint8_t *buffer)
{
	uint8_t csum = 0;
	bool csum_good = false;
	int i;

	/* Validate the checksum */
	for (i = 0; i < 0x3f; i++)
		csum += buffer[i];
	csum_good = csum == buffer[0x3f];
	debug("%s: Lower checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0x3f]);
	if (!csum_good) {
		printf("Error: SFP EEPROM lower checksum information is incorrect.\n"
		       "Expected 0x%02x but checksum is 0x%02x\n",
		       csum, buffer[0x3f]);
		print_buffer(0, buffer, 1, 0x100, 0);
		return -1;
	}
	csum = 0;
	for (i = 0x40; i < 0x5f; i++)
		csum += buffer[i];
	debug("%s: Upper checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0x5f]);
	if (csum != buffer[0x5f]) {
		printf("Error: SFP EEPROM upper checksum information is incorrect.\n"
		       "Expected 0x%02x but checksum is 0x%02x\n",
		       csum, buffer[0x5f]);
		print_buffer(0, buffer, 1, 0x100, 0);
		return -1;
	}

	sfp_info->conn_type = buffer[0];
	if (buffer[1] < 1 || buffer[1] > 7) {	/* Extended ID */
		printf("Error: Unknown SFP extended identifier 0x%x\n",
		       buffer[1]);
		return -1;
	}
	if (buffer[1] != 4) {
		printf("Module is not SFP/SFP+/SFP28/QSFP+\n");
		return -1;
	}
	sfp_info->mod_type = buffer[2];
	sfp_info->eth_comp = buffer[3] & 0xf0;
	sfp_info->cable_comp = buffer[0x24];

	switch (sfp_info->mod_type) {
	case SFP_MOD_COPPER_PIGTAIL:
	case SFP_MOD_NO_SEP_CONN:
		debug("%s: copper pigtail or no separable cable\n", __func__);
		/* There are several ways a cable can be marked as active or
		 * passive.  8.[2-3] specify the SFP+ cable technology.  Some
		 * modules also use 3.[0-1] for Infiniband, though it's
		 * redundant.
		 */
		sfp_info->copper_cable = true;
		if ((buffer[8] & 0x0C) == 0x08) {
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
		} else if ((buffer[8] & 0xC) == 0x4) {
			sfp_info->limiting = false;
			sfp_info->active_cable = false;
		}
		if ((buffer[3] & 3) == 2) {
			sfp_info->active_cable = true;
			sfp_info->limiting = true;
		}
		break;
	case SFP_MOD_RJ45:
		debug("%s: RJ45 adapter\n", __func__);
		sfp_info->copper_cable = true;
		sfp_info->active_cable = true;
		sfp_info->limiting = true;
		break;
	case SFP_MOD_UNKNOWN:
		/* The Avago 1000Base-X to 1000Base-T module reports that it
		 * is an unknown module type but the Ethernet compliance code
		 * says it is 1000Base-T.  We'll change the reporting to RJ45.
		 */
		if (buffer[6] & 8) {
			debug("RJ45 gigabit module detected\n");
			sfp_info->mod_type = SFP_MOD_RJ45;
			sfp_info->copper_cable = false;
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
			sfp_info->max_copper_cable_len = buffer[0x12];
			sfp_info->rate = SFP_RATE_1G;
		} else {
			debug("Unknown module type 0x%x\n", sfp_info->mod_type);
		}

		/* fall through */
	default:
		sfp_info->limiting = true;
		break;
	}
	if (sfp_info->copper_cable)
		sfp_info->max_copper_cable_len = buffer[0x12];
	else
		sfp_info->max_50um_om4_cable_length = buffer[0x12] * 10;

	if (buffer[0xe])
		sfp_info->max_single_mode_cable_length = buffer[0xe] * 1000;
	else
		sfp_info->max_single_mode_cable_length = buffer[0xf] * 100000;

	sfp_info->max_50um_om2_cable_length = buffer[0x10] * 10;
	sfp_info->max_62_5um_om1_cable_length = buffer[0x11] * 10;
	sfp_info->max_50um_om3_cable_length = buffer[0x13] * 10;

	if (buffer[0xc] == 12) {
		sfp_info->rate = SFP_RATE_1G;
	} else if (buffer[0xc] == 103) {
		sfp_info->rate = SFP_RATE_10G;
	} else if (buffer[0xc] == 0xff) {
		if (buffer[0x42] == 103)
			sfp_info->rate = SFP_RATE_25G;
	}

	if (buffer[0xc] < 0xff)
		sfp_info->bitrate_max = buffer[0xc] * 100;
	else
		sfp_info->bitrate_max = buffer[0x42] * 250;

	if ((buffer[8] & 0xc) == 8) {
		if (buffer[0x3c] & 0x4)
			sfp_info->limiting = true;
	}

	/* Currently we only set this for 25G.  FEC is required for CA-S cables
	 * and for cable lengths >= 5M as of this writing.
	 */
	if (((sfp_info->rate == SFP_RATE_25G) && (sfp_info->copper_cable)) &&
		((sfp_info->cable_comp == SFP_CABLE_25G_CR_CA_S) ||
		(sfp_info->max_copper_cable_len >= 5)))
		sfp_info->fec_required = true;

	/* copy strings and vendor info, strings will be automatically NUL
	 * terminated.
	 */
	memcpy(sfp_info->vendor_name, &buffer[0x14], 16);
	memcpy(sfp_info->vendor_oui, &buffer[0x25], 3);
	memcpy(sfp_info->vendor_pn, &buffer[0x28], 16);
	memcpy(sfp_info->vendor_rev, &buffer[0x38], 4);
	memcpy(sfp_info->vendor_sn, &buffer[0x44], 16);
	memcpy(sfp_info->date_code, &buffer[0x54], 8);

	sfp_info->linear_rx_output = !!(buffer[0x40] & 1);
	sfp_info->cooled_laser = !!(buffer[0x40] & 4);
	sfp_info->internal_cdr = !!(buffer[0x40] & 8);

	if (buffer[0x40] & 0x20)
		sfp_info->power_level = 3;
	else
		sfp_info->power_level = (buffer[0x40] & 2) ? 2 : 1;

	sfp_info->diag_paging = !!(buffer[0x40] & 0x10);
	sfp_info->los_implemented = !!(buffer[0x41] & 2);
	sfp_info->los_inverted = !!(buffer[0x41] & 4);
	sfp_info->tx_fault_implemented = !!(buffer[0x41] & 8);
	sfp_info->tx_disable_implemented = !!(buffer[0x41] & 0x10);
	sfp_info->rate_select_implemented = !!(buffer[0x41] & 0x20);
	sfp_info->tuneable_transmitter = !!(buffer[0x41] & 0x40);
	sfp_info->rx_decision_threshold_implemented = !!(buffer[0x41] & 0x80);


	sfp_info->diag_monitoring = !!(buffer[0x5c] & 0x40);
	sfp_info->diag_rx_power_averaged = !!(buffer[0x5c] & 0x8);
	sfp_info->diag_externally_calibrated = !!(buffer[0x5c] & 0x10);
	sfp_info->diag_internally_calibrated = !!(buffer[0x5c] & 0x20);
	sfp_info->diag_addr_change_required = !!(buffer[0x5c] & 0x4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0x5d] & 2);
	sfp_info->diag_app_select_control = !!(buffer[0x5d] & 4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0x5d] & 8);
	sfp_info->diag_soft_rx_los_implemented = !!(buffer[0x5d] & 0x10);
	sfp_info->diag_soft_tx_fault_implemented = !!(buffer[0x5d] & 0x20);
	sfp_info->diag_soft_tx_disable_implemented = !!(buffer[0x5d] & 0x40);
	sfp_info->diag_alarm_warning_flags_implemented = !!(buffer[0x5d] & 0x80);
	sfp_info->diag_rev = buffer[0x5e];

	return 0;
}

static int phy_sfp_parse_qsfp(struct phy_sfp_info *sfp_info,
			      const uint8_t *buffer)
{
	uint8_t csum = 0;
	bool csum_good = false;
	int i;

	/* Validate the checksum */
	for (i = 0x80; i < 0xbf; i++)
		csum += buffer[i];
	csum_good = csum == buffer[0xbf];
	debug("%s: Lower checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0xbf]);
	csum = 0;
	for (i = 0xc0; i < 0xdf; i++)
		csum += buffer[i];
	debug("%s: Upper checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0xdf]);
	if (csum != buffer[0xdf] || !csum_good) {
		printf("Error: SFP EEPROM checksum information is incorrect\n");
		return -1;
	}

	sfp_info->conn_type = buffer[0x80];
	sfp_info->mod_type = buffer[0x82];
	sfp_info->eth_comp = buffer[0x83] & 0xf0;
	sfp_info->cable_comp = buffer[0xa4];

	switch (sfp_info->mod_type) {
	case SFP_MOD_COPPER_PIGTAIL:
	case SFP_MOD_NO_SEP_CONN:
		debug("%s: copper pigtail or no separable cable\n", __func__);
		/* There are several ways a cable can be marked as active or
		 * passive.  8.[2-3] specify the SFP+ cable technology.  Some
		 * modules also use 3.[0-1] for Infiniband, though it's
		 * redundant.
		 */
		sfp_info->copper_cable = true;
		if ((buffer[0x88] & 0x0C) == 0x08) {
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
		} else if ((buffer[0x88] & 0xC) == 0x4) {
			sfp_info->limiting = false;
			sfp_info->active_cable = false;
		}
		if ((buffer[0x83] & 3) == 2) {
			sfp_info->active_cable = true;
			sfp_info->limiting = true;
		}
		break;
	case SFP_MOD_RJ45:
		debug("%s: RJ45 adapter\n", __func__);
		sfp_info->copper_cable = true;
		sfp_info->active_cable = true;
		sfp_info->limiting = true;
		break;
	case SFP_MOD_UNKNOWN:
		debug("Unknown module type\n");
		/* The Avago 1000Base-X to 1000Base-T module reports that it
		 * is an unknown module type but the Ethernet compliance code
		 * says it is 1000Base-T.  We'll change the reporting to RJ45.
		 */
		if (buffer[0x86] & 8) {
			sfp_info->mod_type = SFP_MOD_RJ45;
			sfp_info->copper_cable = false;
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
			sfp_info->max_copper_cable_len = buffer[0x92];
			sfp_info->rate = SFP_RATE_1G;
		}
		/* fall through */
		default:
			sfp_info->limiting = true;
			break;
	}

	if (sfp_info->copper_cable)
		sfp_info->max_copper_cable_len = buffer[0x92];
	else
		sfp_info->max_50um_om4_cable_length = buffer[0x92] * 10;

	if (buffer[0xe])
		sfp_info->max_single_mode_cable_length = buffer[0x8e] * 1000;
	else
		sfp_info->max_single_mode_cable_length = buffer[0x8f] * 100000;

	sfp_info->max_50um_om2_cable_length = buffer[0x90] * 10;
	sfp_info->max_62_5um_om1_cable_length = buffer[0x91] * 10;
	sfp_info->max_50um_om3_cable_length = buffer[0x93] * 10;

	if (buffer[0x8c] == 12) {
		sfp_info->rate = SFP_RATE_1G;
	} else if (buffer[0x8c] == 103) {
		sfp_info->rate = SFP_RATE_10G;
	} else if (buffer[0x8c] == 0xff) {
		if (buffer[0xc2] == 103)
			sfp_info->rate = SFP_RATE_100G;
	}

	if (buffer[0x8c] < 0xff)
		sfp_info->bitrate_max = buffer[0x8c] * 100;
	else
		sfp_info->bitrate_max = buffer[0xc2] * 250;

	if ((buffer[0x88] & 0xc) == 8) {
		if (buffer[0xbc] & 0x4)
			sfp_info->limiting = true;
	}

	/* Currently we only set this for 25G.  FEC is required for CA-S cables
	 * and for cable lengths >= 5M as of this writing.
	 */
#if 0 /* TODO */
	if (((sfp_info->rate == SFP_RATE_100G) && (sfp_info->copper_cable)) &&
		((sfp_info->cable_comp == SFP_CABLE_25G_CR_CA_S) ||
		(sfp_info->max_copper_cable_len >= 5)))
		sfp_info->fec_required = true;
#endif
	/* copy strings and vendor info, strings will be automatically NUL
	 * terminated.
	 */
	memcpy(sfp_info->vendor_name, &buffer[0x94], 16);
	memcpy(sfp_info->vendor_oui, &buffer[0xa5], 3);
	memcpy(sfp_info->vendor_pn, &buffer[0xa8], 16);
	memcpy(sfp_info->vendor_rev, &buffer[0xb8], 4);
	memcpy(sfp_info->vendor_sn, &buffer[0xc4], 16);
	memcpy(sfp_info->date_code, &buffer[0xd4], 8);

	sfp_info->linear_rx_output = !!(buffer[0xc0] & 1);
	sfp_info->cooled_laser = !!(buffer[0xc0] & 4);
	sfp_info->internal_cdr = !!(buffer[0xc0] & 8);

	if (buffer[0xc0] & 0x20)
		sfp_info->power_level = 3;
	else
		sfp_info->power_level = (buffer[0xc0] & 2) ? 2 : 1;

	sfp_info->diag_paging = !!(buffer[0xc0] & 0x10);
	sfp_info->los_implemented = !!(buffer[0xc1] & 2);
	sfp_info->los_inverted = !!(buffer[0xc1] & 4);
	sfp_info->tx_fault_implemented = !!(buffer[0xc1] & 8);
	sfp_info->tx_disable_implemented = !!(buffer[0xc1] & 0x10);
	sfp_info->rate_select_implemented = !!(buffer[0xc1] & 0x20);
	sfp_info->tuneable_transmitter = !!(buffer[0xc1] & 0x40);
	sfp_info->rx_decision_threshold_implemented = !!(buffer[0xc1] & 0x80);

	sfp_info->diag_monitoring = !!(buffer[0xdc] & 0x40);
	sfp_info->diag_rx_power_averaged = !!(buffer[0xdc] & 0x8);
	sfp_info->diag_externally_calibrated = !!(buffer[0xdc] & 0x10);
	sfp_info->diag_internally_calibrated = !!(buffer[0xdc] & 0x20);
	sfp_info->diag_addr_change_required = !!(buffer[0xdc] & 0x4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0xdd] & 2);
	sfp_info->diag_app_select_control = !!(buffer[0xdd] & 4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0xdd] & 8);
	sfp_info->diag_soft_rx_los_implemented = !!(buffer[0xdd] & 0x10);
	sfp_info->diag_soft_tx_fault_implemented = !!(buffer[0xdd] & 0x20);
	sfp_info->diag_soft_tx_disable_implemented = !!(buffer[0xdd] & 0x40);
	sfp_info->diag_alarm_warning_flags_implemented = !!(buffer[0xdd] & 0x80);
	sfp_info->diag_rev = buffer[0xde];

	return 0;
}

/**
 * Reads the SFP/SFP+/QSFP EEPROM and outputs the type of module or cable
 * plugged in
 *
 * @param[out]	sfp_info	SFP module information
 * @param	ethdev		Ethernet device to read for
 * @param	i2c_bus		I2C bus number for SFP+ port
 * @param	i2c_addr	Usually 0x50
 *
 * @return	0 on success, -1 if error reading EEPROM or if EEPROM corrupt
 */
int phy_sfp_parse_eeprom(struct phy_sfp_info *sfp_info, const uint8_t *buffer)
{
	if (!sfp_info || !buffer) {
		printf("%s: Error: input NULL\n", __func__);
		return -1;
	}
	memset(sfp_info, 0, sizeof(*sfp_info));

	/* Check transceiver type.  See the SFF-8472 standard. */
	switch (buffer[0]) {
	case SFP_CONN_SFP:
		debug("%s: Parsing SFP/SFP+/SFP28 transceiver\n", __func__);
		return phy_sfp_parse_sfp(sfp_info, buffer);
	case SFP_CONN_QSFP:
	case SFP_CONN_QSFPP:
	case SFP_CONN_QSFP28:
	case SFP_CONN_MICRO_QSFP:
		debug("%s: Parsing QSFP/QSFP+/QSFP28/micro QSFP transceiver\n",
		      __func__);
		return phy_sfp_parse_qsfp(sfp_info, buffer);
	}
	printf("Unknown SFP transceiver type 0x%x\n", buffer[0]);
	return -1;
}

/**
 * Prints out information about a SFP/QSFP device
 *
 * @param[in]	sfp_info	data structure to print
 */
void phy_sfp_print_info(const struct phy_sfp_info *sfp_info)
{
	const char *conn_type;
	const char *mod_type;
	const char *rate_str;
	const char *cable_comp;

	if (!sfp_info) {
		printf("Error: NULL input\n");
		return;
	}

	/* Please refer to the SFF-8024 and SFF-8472 documents */
	switch (sfp_info->conn_type) {
	case SFP_CONN_SFP:
		conn_type = "SFP/SFP+/SFP28";
		break;
	case SFP_CONN_QSFP:
		conn_type = "QSFP";
		break;
	case SFP_CONN_QSFPP:
		conn_type = "QSFP+";
		break;
	case SFP_CONN_QSFP28:
		conn_type = "QSFP28";
		break;
	case SFP_CONN_MICRO_QSFP:
		conn_type = "Micro QSFP";
		break;
	default:
		conn_type = "Unknown";
		break;
	}

	switch (sfp_info->mod_type) {
	case SFP_MOD_UNKNOWN:
		mod_type = "Unknown";
		break;
	case SFP_MOD_OPTICAL_LC:
		mod_type = "Optical LC";
		break;
	case SFP_MOD_OPTICAL_PIGTAIL:
		mod_type = "Optical Pigtail";
		break;
	case SFP_MOD_COPPER_PIGTAIL:
		mod_type = "Copper Pigtail";
		break;
	case SFP_MOD_RJ45:
		mod_type = "Copper RJ45";
		break;
	case SFP_MOD_NO_SEP_CONN:
		mod_type = "No Separable Connector";
		break;
	case SFP_MOD_OTHER:
		mod_type = "Unknow/Other";
		break;
	default:
		mod_type = "Undefined";
		break;
	}

	switch (sfp_info->cable_comp) {
	case SFP_CABLE_UNSPEC:
		cable_comp = "";
		break;
	case SFP_CABLE_25G_CR_CA_S:
		cable_comp = " 25GBase-CR CA-S";
		break;
	case SFP_CABLE_25G_CR_CA_N:
		cable_comp = " 25GBase-CR CA-N";
		break;
	default:
		cable_comp = "Unknown or Unsupported";
		break;
	}

	switch (sfp_info->rate) {
	case SFP_RATE_UNKNOWN:
	default:
		rate_str = "Unknown";
		break;
	case SFP_RATE_1G:
		rate_str = "1G";
		break;
	case SFP_RATE_10G:
		rate_str = "10G";
		break;
	case SFP_RATE_25G:
		rate_str = "25G";
		break;
	case SFP_RATE_40G:
		rate_str = "40G";
		break;
	case SFP_RATE_100G:
		rate_str = "100G";
		break;
	}

	printf("%s %s%s module detected\n", mod_type, conn_type, cable_comp);
	printf("Vendor: %s\n", sfp_info->vendor_name);
	printf("Vendor OUI: %02X:%02X:%02X\n",
	       sfp_info->vendor_oui[0], sfp_info->vendor_oui[1],
	       sfp_info->vendor_oui[2]);
	printf("Vendor part number: %s Revision: %s\n", sfp_info->vendor_pn,
	       sfp_info->vendor_rev);
	printf("Manufacturing date code: %s\n", sfp_info->date_code);
	printf("Rate: %s\n", rate_str);
	if (sfp_info->copper_cable) {
		printf("Copper cable type: %s\n",
		       sfp_info->active_cable ? "Active" : "Passive");
		printf("Cable length: %u meters\n",
		       sfp_info->max_copper_cable_len);
		if (sfp_info->rate == SFP_RATE_25G)
			printf("Forward error correction is%s %s\n",
			       sfp_info->fec_required ? "" : " not",
			       (sfp_info->max_copper_cable_len >= 5 ||
			        !sfp_info->fec_required) ?
					"required" : "suggested");
	} else {
		bool more = false;
		printf("Supported optical types: ");
		if (sfp_info->eth_comp & SFP_CABLE_10GBASE_ER) {
			more = !!(sfp_info->eth_comp & ~SFP_CABLE_10GBASE_ER);
			printf("10GBase-ER%s", more ? ", " : "");
		}
		if (sfp_info->eth_comp & SFP_CABLE_10GBASE_LRM) {
			more = !!(sfp_info->eth_comp & ~(SFP_CABLE_10GBASE_ER |
							 SFP_CABLE_10GBASE_LRM));
			printf("10GBase-LRM%s", more ? ", " : "");
		}
		if (sfp_info->eth_comp & SFP_CABLE_10GBASE_LR) {
			more = !!(sfp_info->eth_comp & ~(SFP_CABLE_10GBASE_ER |
							 SFP_CABLE_10GBASE_LRM |
							 SFP_CABLE_10GBASE_LR));
			printf("10GBase-LR%s", more ? ", " : "");
		}
		if (sfp_info->eth_comp & SFP_CABLE_10GBASE_SR)
			printf("10GBase-SR");
		putc('\n');
		printf("Maximum single mode cable length: %d meters\n",
		       sfp_info->max_single_mode_cable_length);
		printf("Maximum 62.5um OM1 cable length:  %d meters\n",
		       sfp_info->max_62_5um_om1_cable_length);
		printf("Maximum 50um OM2 cable length:    %d meters\n",
		       sfp_info->max_50um_om2_cable_length);
		printf("Maximum 50um OM3 cable length:    %d meters\n",
		       sfp_info->max_50um_om3_cable_length);
		printf("Maximum 50um OM4 cable length:    %d meters\n",
		       sfp_info->max_50um_om4_cable_length);
		printf("Laser is%s cooled\n",
		       sfp_info->cooled_laser ? "" : " not");
	}
	printf("Limiting is %sabled\n", sfp_info->limiting ? "en" : "dis");
	printf("Power level: %d\n", sfp_info->power_level);
	printf("RX LoS is%s implemented and is%s inverted\n",
	       sfp_info->los_implemented ? "" : " not",
	       sfp_info->los_inverted ? "" : " not");
	printf("TX fault is%s implemented\n",
	       sfp_info->tx_fault_implemented ? "" : " not");
	printf("TX disable is%s implemented\n",
	       sfp_info->tx_disable_implemented ? "" : " not");
	printf("Rate select is%s implemented\n",
	       sfp_info->rate_select_implemented ? "" : " not");
	printf("RX output is %s\n",
	       sfp_info->linear_rx_output ? "linear" : "limiting");
	printf("Diagnostic monitoring is%s implemented\n",
	       sfp_info->diag_monitoring ? "" : " not");
	if (sfp_info->diag_monitoring) {
		const char *diag_rev;
		switch (sfp_info->diag_rev) {
		case SFP_SFF_8472_NO_DIAG:
			diag_rev = "none";
			break;
		case SFP_SFF_8472_REV_9_3:
			diag_rev = "9.3";
			break;
		case SFP_SFF_8472_REV_9_5:
			diag_rev = "9.5";
			break;
		case SFP_SFF_8472_REV_10_2:
			diag_rev = "10.2";
			break;
		case SFP_SFF_8472_REV_10_4:
			diag_rev = "10.4";
			break;
		case SFP_SFF_8472_REV_11_0:
			diag_rev = "11.0";
			break;
		case SFP_SFF_8472_REV_11_3:
			diag_rev = "11.3";
			break;
		case SFP_SFF_8472_REV_11_4:
			diag_rev = "11.4";
			break;
		case SFP_SFF_8472_REV_12_0:
			diag_rev = "12.0";
			break;
		case SFP_SFF_8472_REV_UNALLOCATED:
			diag_rev = "Unallocated";
			break;
		default:
			diag_rev = "Unknown";
			break;
		}
		printf("Diagnostics revision: %s\n", diag_rev);
		printf("Diagnostic address change is%s required\n",
		       sfp_info->diag_addr_change_required ? "" : " not");
		printf("Diagnostics are%s internally calibrated\n",
		       sfp_info->diag_internally_calibrated ? "" : " not");
		printf("Diagnostics are%s externally calibrated\n",
		       sfp_info->diag_externally_calibrated ? "" : " not");
		printf("Receive power measurement type: %s\n",
		       sfp_info->diag_rx_power_averaged ? "Averaged" : "OMA");
	}
}

int phy_sfp_parse_diag(struct phy_sfp_info *sfp_info, uint8_t *buffer)
{
	return 0;
}
#ifdef CONFIG_CMD_SFP
/**
 * Processes the sfpinfo command.
 */
enum command_ret_t do_sfpinfo(cmd_tbl_t *cmdtp, int flag, int argc,
			      char * const argv[])
{
	uint8_t buffer[SFP_MAX_EEPROM_SIZE];
	int i2c_bus = 0;
	int i2c_addr = SFP_DEFAULT_I2C_ADDR;
	ulong addr = (unsigned long)-1;
	int rc;
	struct phy_sfp_info sfp_info;

	if (!strcmp(argv[1], "i2c")) {
		if (argc > 2)
			i2c_addr = simple_strtoul(argv[2], NULL, 0);
		if (argc > 3) {
			i2c_bus = simple_strtoul(argv[2], NULL, 0);
			i2c_addr = simple_strtoul(argv[3], NULL, 16);
		}
		rc = phy_sfp_read_i2c_eeprom(buffer, i2c_bus, i2c_addr);
		if (rc < 0) {
			printf("Could not read SFP EEPROM from 0x%x:0x%x\n",
			       i2c_bus, i2c_addr);
			return CMD_RET_FAILURE;
		}
	} else if (!strncmp(argv[1], "addr", 4)) {
		if (argc < 3)
			return CMD_RET_USAGE;
		addr = simple_strtoul(argv[2], NULL, 16);
		memcpy(buffer, (void *)addr, sizeof(buffer));
	} else if (!strncmp(argv[1], "reg", 3)) {
		int old_bus;
		debug("%s reg: argc: %d\n", __func__, argc);
		old_bus = i2c_get_bus_num();
		if (argc == 4) {
			i2c_addr = simple_strtoul(argv[2], NULL, 16);
			addr = simple_strtoul(argv[3], NULL, 0);
		} else if (argc == 5) {
			i2c_bus = simple_strtoul(argv[2], NULL, 0);
			i2c_addr = simple_strtoul(argv[3], NULL, 0);
			addr = simple_strtoul(argv[4], NULL, 0);
		} else {
			return CMD_RET_USAGE;
		}
		i2c_set_bus_num(i2c_bus);
		i2c_read(i2c_addr, addr, 1, buffer, 2);
		i2c_set_bus_num(old_bus);
		printf("r 0x%x:0x%x reg: 0x%lx => 0x%02x%02x\n",
		       i2c_bus, i2c_addr, addr, buffer[0], buffer[1]);
		return CMD_RET_SUCCESS;
	} else {
		return CMD_RET_USAGE;
	}
	rc = phy_sfp_parse_eeprom(&sfp_info, buffer);
	if (rc) {
		printf("Error parsing SFP EEPROM\n");
		return CMD_RET_FAILURE;
	}
	phy_sfp_print_info(&sfp_info);

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(sfpinfo, 5, 1, do_sfpinfo,
	   "Displays information about a SFP/QSFP module",
	   "i2c [bus [address]] - Reads the SFP data from the specified i2c bus and optional address\n"
	   "addr [address]      - Displays the SFP data from the specified memory address\n"
	   "reg [bus [address]] [reg] - read a 16-bit value over i2c\n");

#endif /* CONFIG_CMD_SFP */
