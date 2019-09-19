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


/** Doxygen File Header
 * * @file
 ** @brief Logging and core type--string conversion functions.
 **/

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_logging.h"

static int value_to_index(const struct avago_conv_table tbl[], int value)
{
	int i;
	for (i = 0; tbl[i].name; i++)
		if (value == tbl[i].value)
			return i;
		return -1;
}

static const struct avago_conv_table process_id_table[] = {
	{ "PROCESS_C",	AVAGO_PROCESS_C },
	{ "PROCESS_B",	AVAGO_PROCESS_B },
	{ "PROCESS_G",	AVAGO_PROCESS_G },
	{ "PROCESS_F",	AVAGO_PROCESS_F },
	{ "PROCESS_A",	AVAGO_PROCESS_A },
	{ "PROCESS_E",	AVAGO_PROCESS_E },
	{ "PROCESS_D",	AVAGO_PROCESS_D },
	{ NULL,		0 },
};

/**
 * Convert an avago_process_id value into a string.
 *
 * @param	value	Process ID value
 * @return	String representation or NULL if no match.
 */
const char *avago_process_id_to_str(enum avago_process_id value)
{
	const struct avago_conv_table *table = process_id_table;

	int index = value_to_index(table, value);
	return index >= 0 ? table[index].name : "UNKNOWN_PROCESS";
}

static struct avago_conv_table dma_type_table[] =
{
	{ "ESB",            AVAGO_ESB           },
	{ "ESB_DIRECT",     AVAGO_ESB_DIRECT    },
	{ "LSB",            AVAGO_LSB           },
	{ "LSB_DIRECT",     AVAGO_LSB_DIRECT    },
	{ "DMEM",           AVAGO_DMEM          },
	{ "DMEM_PREHALTED", AVAGO_DMEM_PREHALTED},
	{ "IMEM",           AVAGO_IMEM          },
	{ "IMEM_PREHALTED", AVAGO_IMEM_PREHALTED},
	{ 0,            0 }
};

/** @brief  Converts an Avago_serdes_mem_type_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_mem_type().
 **/
const char *avago_mem_type_to_str(enum avago_serdes_mem_type value)
{
	struct avago_conv_table *table = dma_type_table;
	int index = value_to_index(table,value);
	return index >= 0 ? table[index].name : "unknown";
}

/**
 * @brief  Converts a boolean value into a string.
 * @return Returns either "TRUE" or "FALSE".
 * @see    avago_enable_to_str(), avago_onoff_to_str().
 */
const char *avago_bool_to_str(int value)
{
	return value ? "TRUE" : "FALSE";
}

/**
 * @brief  Converts a boolean value into a string.
 * @return Returns either "ON" or "OFF".
 * @see    avago_bool_to_str(), avago_enable_to_str().
 */
const char *avago_onoff_to_str(int value)
{
	return value ? "ON" : "OFF";
}

/**
 * @brief  Converts a boolean value into a string.
 * @return Returns either "enabled" or "disabled".
 * @see    avago_bool_to_str(), avago_onoff_to_str().
 */
const char *avago_enable_to_str(int value)
{
	return value ? "enabled" : "disabled";
}

static const struct avago_conv_table data_sel_table[] = {
	{ "LOOPBACK",	AVAGO_SERDES_TX_DATA_SEL_LOOPBACK },
	{ "CORE",	AVAGO_SERDES_TX_DATA_SEL_CORE     },
	{ "PRBS7",	AVAGO_SERDES_TX_DATA_SEL_PRBS7    },
	{ "PRBS9",	AVAGO_SERDES_TX_DATA_SEL_PRBS9    },
	{ "PRBS11",	AVAGO_SERDES_TX_DATA_SEL_PRBS11   },
	{ "PRBS15",	AVAGO_SERDES_TX_DATA_SEL_PRBS15   },
	{ "PRBS23",	AVAGO_SERDES_TX_DATA_SEL_PRBS23   },
	{ "PRBS31",	AVAGO_SERDES_TX_DATA_SEL_PRBS31   },
	{ "USER",	AVAGO_SERDES_TX_DATA_SEL_USER     },
	{ 0,          0 }
};

/**
 * Convert an avago_serdes_tx_data_sel value into a string
 */
const char *avago_data_sel_to_str(enum avago_serdes_tx_data_sel value)
{
	const struct avago_conv_table *table = process_id_table;

	int index = value_to_index(table, value);
	return index >= 0 ? table[index].name : "unknown";
}

static struct avago_conv_table cmp_data_table[] =
{
	{ "PRBS7",      AVAGO_SERDES_RX_CMP_DATA_PRBS7     },
	{ "PRBS9",      AVAGO_SERDES_RX_CMP_DATA_PRBS9     },
	{ "PRBS11",     AVAGO_SERDES_RX_CMP_DATA_PRBS11    },
	{ "PRBS15",     AVAGO_SERDES_RX_CMP_DATA_PRBS15    },
	{ "PRBS23",     AVAGO_SERDES_RX_CMP_DATA_PRBS23    },
	{ "PRBS31",     AVAGO_SERDES_RX_CMP_DATA_PRBS31    },
	{ "OFF",        AVAGO_SERDES_RX_CMP_DATA_OFF       },
	{ "SELF_SEED",  AVAGO_SERDES_RX_CMP_DATA_SELF_SEED },
	{ 0,            0 }
};
/** @brief  Converts an Avago_serdes_rx_cmp_data_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_cmp_data().
 **/
const char *avago_cmp_data_to_str(enum avago_serdes_rx_cmp_data value)
{
	struct avago_conv_table *table = cmp_data_table;
	int index = value_to_index(table,value);
	return index >= 0 ? table[index].name : "unknown";
}

static struct avago_conv_table dfe_tune_mode_table[] =
{
	{ "ICAL",           AVAGO_DFE_ICAL           },
	{ "PCAL",           AVAGO_DFE_PCAL           },
	{ "ICAL_ONLY",      AVAGO_DFE_ICAL_ONLY      },
	{ "START_ADAPTIVE", AVAGO_DFE_START_ADAPTIVE },
	{ "STOP_ADAPTIVE",  AVAGO_DFE_STOP_ADAPTIVE  },
	{ "ENABLE_RR",      AVAGO_DFE_ENABLE_RR      },
	{ "DISABLE_RR",     AVAGO_DFE_DISABLE_RR     },
	{ 0,                0 }
};

static struct avago_conv_table mode_table[] =
{
	{ "OFF",         AVAGO_SERDES_RX_CMP_MODE_OFF },
	{ "XOR",         AVAGO_SERDES_RX_CMP_MODE_XOR },
	{ "TEST_PATGEN", AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN },
	{ "MAIN_PATGEN", AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN },
	{ 0,             0 }
};
/** @brief  Converts an Avago_serdes_rx_cmp_mode_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_cmp_mode().
 **/
const char *avago_cmp_mode_to_str(enum avago_serdes_rx_cmp_mode value)
{
	static char buf[16];
	struct avago_conv_table *table = mode_table;
	int index = value_to_index(table,value);
	if( index >= 0 )
		return table[index].name;
	snprintf(buf,sizeof(buf),"0x%04x",value);
	return buf;
}

static struct avago_conv_table term_table[] =
{
	{ "AVDD",  AVAGO_SERDES_RX_TERM_AVDD  },
	{ "FLOAT", AVAGO_SERDES_RX_TERM_FLOAT },
	{ "AGND",  AVAGO_SERDES_RX_TERM_AGND  },
	{ 0,       0 }
};

/** @brief  Converts an Avago_serdes_rx_term_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_term().
 **/
const char *avago_term_to_str(enum avago_serdes_rx_term value)
{
	struct avago_conv_table *table = term_table;
	int index = value_to_index(table,value);
	return index >= 0 ? table[index].name : "unknown";
}

/** @brief  Converts an Avago_serdes_dfe_tune_mode_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_dfe_tune_mode().
 **/
const char *avago_dfe_tune_mode_to_str(enum avago_serdes_dfe_tune_mode value)
{
	struct avago_conv_table *table = dfe_tune_mode_table;
	int index = value_to_index(table,value);
	return index >= 0 ? table[index].name : "unknown";
}


static const struct avago_conv_table ip_type_table[] = {
	{ "Unused",              AVAGO_UNKNOWN_IP           },
	{ "SerDes",              AVAGO_SERDES               },
	{ "SBus Controller",     AVAGO_SBUS_CONTROLLER      },
	{ "SBus Master SPICO",   AVAGO_SPICO                },
	{ "QPI",                 AVAGO_QPI                  },
	{ "FBD",                 AVAGO_FBD                  },
	{ "PCS64B66B",           AVAGO_PCS64B66B            },
	{ "Auto Negotiation",    AVAGO_AUTO_NEGOTIATION     },
	{ "PCS64B66B FEC",       AVAGO_PCS64B66B_FEC        },
	{ "PCIe PCS",            AVAGO_PCIE_PCS             },
	{ "Core PLL",            AVAGO_CORE_PLL             },
	{ "PMRO",                AVAGO_PMRO                 },
	{ "DDR address",         AVAGO_DDR_ADDRESS          },
	{ "DDR data",            AVAGO_DDR_DATA             },
	{ "DDR train",           AVAGO_DDR_TRAIN            },
	{ "DDR CTC",             AVAGO_DDR_CTC              },
	{ "DDR STOP",            AVAGO_DDR_STOP             },
	{ "Thermal Sensor",      AVAGO_THERMAL_SENSOR       },
	{ "RMON",                AVAGO_RMON                 },
	{ "Link Emulator",       AVAGO_LINK_EMULATOR        },
	{ "Link Emulator",       AVAGO_LINK_EMULATOR_2      },
	{ "AVSP Control Logic",  AVAGO_AVSP_CONTROL_LOGIC   },
	{ "RAM PMRO",            AVAGO_RAM_PMRO             },
	{ "SerDes Broadcast",    AVAGO_SERDES_BROADCAST     },
	{ "Pandora LSB",         AVAGO_PANDORA_LSB          },
	{ "Super Link Em Packet",AVAGO_SLE_PKT              },
	{ "SerDes M4",           AVAGO_M4                   },
	{ "SerDes P1",           AVAGO_P1                   },
	{ "Super Link Emulator", AVAGO_SLE                  },
	{ "Padframe Control",    AVAGO_PAD_CONTROL          },
	{ "MLD",                 AVAGO_MLD                  },
	{ "RS FEC Bridge",       AVAGO_RSFEC_BRIDGE         },
	{ "Crosspoint",          AVAGO_CROSSPOINT           },
	{ "SAPPH GearBox",       AVAGO_SAPPH_GBX            },
	{ "SAPPH GearBox Top",   AVAGO_SAPPH_GBX_TOP        },
	{ "RSFEC 528",           AVAGO_OPAL_RSFEC528        },
	{ "RSFEC 528 544",       AVAGO_OPAL_RSFEC528_544    },
	{ "Host Aligner",        AVAGO_OPAL_HOST_ALIGNER    },
	{ "Mod Aligner",         AVAGO_OPAL_MOD_ALIGNER     },
	{ "Control Logic",       AVAGO_OPAL_CONTROL         },
	{ "Opal RSFEC 528",      AVAGO_OPAL_RSFEC528        },
	{ "Opal RSFEC 528 544",  AVAGO_OPAL_RSFEC528_544    },
	{ "Opal Host Aligner",   AVAGO_OPAL_HOST_ALIGNER    },
	{ "Opal Mod Aligner",    AVAGO_OPAL_MOD_ALIGNER     },
	{ "Opal Control Logic",  AVAGO_OPAL_CONTROL         },
	{ "OSC/SSC PLL",         AVAGO_OSC_SSC_PLL          },
	{ "Garnet 25GE Logic",   AVAGO_GARNET_25GE_INTERFACE},
	{ "SGMII PCS",           AVAGO_SGMII                },
	{ 0,                     0                          }
};

/**
 * convert an avago_ip_type value into a string
 */
const char *avago_ip_type_to_str(enum avago_ip_type value)
{
	static char buf[32];
	const struct avago_conv_table *table = ip_type_table;
	int index = value_to_index(table, value);
	if (index >= 0)
		return table[index].name;

	snprintf(buf, sizeof(buf), "AVAGO_UNKNOWN_IP (0x%x)", value);
	return buf;
}

const char *avago_get_ip_type_str(struct avago_hdl *ah, uint32_t addr)
{
	return avago_ip_type_to_str(avago_get_ip_type(ah, addr));
}

static int uint_to_hex(uint32_t val, char *buffer)
{
	int len = 0;
	int shift = 28;
	const char hexstr[16] = "0123456789abcdef";

	while (!((val >> shift) & 0xf) && shift > 0)
		shift -= 4;
	do {
		*(buffer++) = hexstr[(val >> shift) & 0xf];
		len++;
		shift -= 4;
	 } while (shift >= 0);
	 *buffer = '\0';
	 return len;
}

/**
 * Converts an address number into a printable string
 */
const char* avago_addr_to_str(uint32_t addr)
{
# define NUM_BUFS	(5)
	static int which = 0;
	static char num_bufs[NUM_BUFS][32];

	char *buf = num_bufs[which = (which + 1) % NUM_BUFS];
	char *ptr = buf;

	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);

	memset(ptr, 0, 20);
	if (addr_struct.chip > 0) {
		if (addr_struct.chip == AVAGO_ADDR_BROADCAST) {
			*ptr++ = '*';
		} else {
			ptr += uint_to_hex(addr_struct.chip, ptr);
		}
		*ptr++ = ':';
	}

	if (addr_struct.ring > 0) {
		if (addr_struct.ring == AVAGO_ADDR_BROADCAST) {
			*ptr++ = '*';
		} else {
			ptr += uint_to_hex(addr_struct.ring, ptr);
		}
	}

	*ptr++ = ':';
	if (addr_struct.sbus == AVAGO_ADDR_BROADCAST) {
		*ptr++ = '*';
	} else {
		ptr += uint_to_hex(addr_struct.sbus, ptr);
	}
	if (addr_struct.lane < 8) {
		*ptr++ = '.';
		ptr += uint_to_hex(addr_struct.lane, ptr);
	} else if (addr_struct.lane == AVAGO_ADDR_QUAD_ALL) {
		*ptr++ = '.';
		*ptr++ = '*';
	} else if (addr_struct.lane == AVAGO_ADDR_QUAD_LOW) {
		*ptr++ = '.';
		*ptr++ = 'L';
	} else if (addr_struct.lane == AVAGO_ADDR_QUAD_HIGH) {
		*ptr++ = '.';
		*ptr++ = 'H';
	}
	*ptr = '\0';
# undef NUM_BUFS
	return buf;
}

static const struct avago_conv_table log_type_table[] =
{
	{ "DEBUG0",         AVAGO_DEBUG0          },
	{ "DEBUG1",         AVAGO_DEBUG1          },
	{ "DEBUG2",         AVAGO_DEBUG2          },
	{ "DEBUG3",         AVAGO_DEBUG3          },
	{ "DEBUG4",         AVAGO_DEBUG4          },
	{ "DEBUG5",         AVAGO_DEBUG5          },
	{ "DEBUG6",         AVAGO_DEBUG6          },
	{ "DEBUG7",         AVAGO_DEBUG7          },
	{ "DEBUG8",         AVAGO_DEBUG8          },
	{ "DEBUG9",         AVAGO_DEBUG9          },
	{ "MEM_LOG",        AVAGO_MEM_LOG         },
	{ "DATA_CHAR",      AVAGO_DATA_CHAR       },
	{ "DATA_CHAR_ADD",  AVAGO_DATA_CHAR_ADD   },
	{ "ERROR",          AVAGO_ERR             },
	{ "WARNING",        AVAGO_WARNING         },
	{ "INFO",           AVAGO_INFO            },
	{ 0,                0 }
};

/**
 * Converts the logging type to a string value
 */
const char *avago_log_type_to_str(enum avago_log_type value)
{
	const struct avago_conv_table *table = log_type_table;
	int index = value_to_index(table, value);
	return (index >= 0) ? table[index].name : "INVALID";
}

static struct avago_conv_table spico_clk_table[] =
{
	{ "REFCLK",                   AVAGO_SERDES_SPICO_REFCLK                },
	{ "CORE_CLK",                 AVAGO_SERDES_SPICO_PCIE_CORE_CLK         },
	{ "PCIE_CORE_CLK",            AVAGO_SERDES_SPICO_PCIE_CORE_CLK         },
	{ "TX_F10",                   AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED      },
	{ "TX_F40",                   AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED      },
	{ "REFCLK_DIV2",              AVAGO_SERDES_SPICO_REFCLK_DIV2           },
	{ "CORE_CLK_DIV2",            AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2    },
	{ "PCIE_CORE_CLK_DIV2",       AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2    },
	{ "TX_F10_DIV2",              AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED_DIV2 },
	{ "TX_F40_DIV2",              AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED_DIV2 },
	{ "TX_F20",                   AVAGO_SERDES_SPICO_TX_F20_CLK_FIXED      },
	{ "TX_F80",                   AVAGO_SERDES_SPICO_TX_F80_CLK_FIXED      },
	{ 0,                0 }
};
/** @brief  Converts an Avago_serdes_spico_clk_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_spico_clk().
 **/
const char *avago_spico_clk_to_str(enum avago_serdes_spico_clk value)
{
	struct avago_conv_table *table = spico_clk_table;
	int index = value_to_index(table, value);
	return index >= 0 ? table[index].name : "unknown";
}


static struct avago_conv_table line_encoding_table[] =
{
	{ "NRZ",  AVAGO_SERDES_PAM2 },
	{ "PAM4", AVAGO_SERDES_PAM4 },
	{ "PAM2", AVAGO_SERDES_PAM2 },
	{ 0,      0 }
};

/** @brief  Converts an Avago_serdes_line_encoding_t value into a string.
 ** @return Returns a string representing the type.
 ** @see    aapl_str_to_line_encoding().
 **/
const char *avago_line_encoding_to_str(enum avago_serdes_line_encoding value)
{
	struct avago_conv_table *table = line_encoding_table;
	int index = value_to_index(table,value);
	return index >= 0 ? table[index].name : "unknown";
}

/**
 * Returns true if the logging type is suitable for the debug level
 */
static bool check_debug_level(enum avago_log_type log_sel, int debug_level)
{
	return ((int)log_sel > debug_level && log_sel < AVAGO_MEM_LOG);
}

#if 0
static void print_uint(unsigned int val)
{
	char buffer[16];
	char *ptr = buffer;

	do {
		*ptr++ = (val % 10) + '0';
		val /= 10;
	} while (val);

	do {
		putc(*(--ptr));
	} while (ptr > buffer);
}
#endif

/**
 * Prints if the log output is valid.
 *
 * @param	ah	Avago handle
 * @param	log_sel	Log selector
 * @param	caller	Calling function name
 * @param	line	Calling line number
 * @param	fmt	Format string
 * @param	...	arguments for format string
 */
static void avago_log_printf_va(struct avago_hdl *ah, enum avago_log_type log_sel,
				const char *caller, int line, const char *fmt,
				va_list ap)
{
	if (ah->suppress_errors && (log_sel == AVAGO_WARNING ||
				    log_sel == AVAGO_ERR))
		log_sel = AVAGO_DEBUG1;

	if (check_debug_level(log_sel, ah->debug))
		return;
	printf("%s: (%s:%d): ", avago_log_type_to_str(log_sel),
	       caller ? caller : "", line);
	vprintf(fmt, ap);
}

/**
 * Prints if the log output is valid.
 *
 * @param	ah	Avago handle
 * @param	log_sel	Log selector
 * @param	caller	Calling function name
 * @param	line	Calling line number
 * @param	fmt	Format string
 * @param	...	arguments for format string
 */
void avago_log_printf(struct avago_hdl *ah, enum avago_log_type log_sel,
		      const char *caller, int line, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	avago_log_printf_va(ah, log_sel, caller, line, fmt, ap);
	va_end(ap);
}


/**
 * Prints an error message
 *
 * @param	ah	Avago handle
 * @param	caller	Calling function name
 * @param	line	Calling line number
 * @param	fmt	Format string
 * @param	...	arguments for format string
 */
int avago_fail(struct avago_hdl *ah, const char *caller, int line,
	       const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	avago_log_printf_va(ah, AVAGO_ERR, caller, line, fmt, ap);
	va_end(ap);

	if (ah->return_code > 0)
		ah->return_code = 0;

	ah->return_code -= 1;

	return -1;
}
