#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <getopt.h>
#include <dirent.h>
#include <unistd.h>

#include "sfp_info.h"
#include "board_common.h"

/* GLOBAL VARIABLES >>> */
int sfp_num = -1, pim_num = 0;
int info_enable = 1, diag_enable = 0, run_in_cycle = 0;
unsigned int const *ind;

char ptFileName[MAX_FILE_NAME+1] = {0};
char ptDiagName[MAX_FILE_NAME+1] = {0};
/* <<< GLOBAL VARIABLES */

typedef enum
{
	DIAG_UNKNOWN = 0,
	DIAG_DISABLED,
	DIAG_INT,
	DIAG_EXT
} diag_mode_t;

/* RETURN CODES >>> */
enum {
		RC_NO_ERROR=0,
		RC_SHOW_HELP,
		RC_I2C_PARSE_ERROR,
		RC_EEPROM_READ_ERROR,
		RC_EEPROM_SIZE_MISMATCH,
		RC_MODULE_NOT_SUPPORTED,
		RC_UNSPECIFIED
	};
/* <<< RETURN CODES */

/* MEDIA IDENTIFIERS >>> */
typedef enum {
	SFF_8024_ID_UNKNOWN		= 0x00,
	SFF_8024_ID_GBIC		= 0x01,
	SFF_8024_ID_SFF			= 0x02,
	SFF_8024_ID_SFP			= 0x03,
	SFF_8024_ID_XBI			= 0x04,
	SFF_8024_ID_XENPAK		= 0x05,
	SFF_8024_ID_XFP			= 0x06,
	SFF_8024_ID_XFF			= 0x07,
	SFF_8024_ID_XFPE		= 0x08,
	SFF_8024_ID_XPAK		= 0x09,
	SFF_8024_ID_X2			= 0x0A,
	SFF_8024_ID_DWDM_SFP		= 0x0B,
	SFF_8024_ID_QSFP		= 0x0C,
	SFF_8024_ID_QSFPPLUS		= 0x0D,
	SFF_8024_ID_CXP			= 0x0E,
	SFF_8024_ID_HD4X		= 0x0F,
	SFF_8024_ID_HD8X		= 0x10,
	SFF_8024_ID_QSFP28		= 0x11,
	SFF_8024_ID_CXP2		= 0x12,
	SFF_8024_ID_CDFP		= 0x13,
	SFF_8024_ID_HD4XFC		= 0x14,
	SFF_8024_ID_HD8XFC		= 0x15,
	SFF_8024_ID_CDFP3		= 0x16,
	SFF_8024_ID_MQSFP		= 0x17,
	SFF_8024_ID_QSFPDD		= 0x18
} sff_8024_id;
/* <<< MEDIA IDENTIFIERS */

struct st_supported_link_length {
	const char *message;
	const int	divider;
	const char *distance;
};

/* SFP and QSFP EEPROM ADDRESSING >>> */
enum {
	iIdentifier=0,
	iConnector,
	iTransceiver,
	iTransceiver_ext,
	iEncoding,
	iBR_Nominal,
	iBR_Nominal_ext,
	iLength,
	iVendor_Name,
	iVendor_OUI,
	iVendor_PN, // 10
	iVendor_Rev,
	iVendor_Rev_len,
	iWavelength,
	iVendor_SN,
	iDate_Code,
	iCC_Base_Start,
	iCC_Base,
	iCC_Base_Ext_Start,
	iCC_Base_Ext,
	iCC_Diag_Start, // 20
	iCC_Diag,
	iCompliance,
	iDiagType,
	iTxFault
};

// for ind_qsfp index is an offset in UPPER page, e.q. offset MINUS 128
const unsigned int ind_qsfp[] = {   0,  2,  3, 64, 11, 12, 94, 14, 20, 37,
					     40, 56,  2, 58, 68, 84,  0, 63, 64, 95,
					      0,  0,  3,  0, 67 };

const unsigned int ind_sfp[]  = {   0,  2,  3, 36, 11, 12, 12, 14, 20, 37,
					     40, 56,  4, 60, 68, 84,  0, 63, 64, 95,
					      0, 95, 94, 92, 65 };
/* <<< SFP and QSFP EEPROM ADDRESSING */

struct sfp_diagnostics
{
	float temp;
	float voltage;
	float current;
	float rx_power;
	float tx_power;
	unsigned int tx_fault;
};

enum { // for diag state machine
	DIAG_TEMPERATURE,
	DIAG_VOLTAGE,
	DIAG_CURRENT,
	DIAG_TX_POWER,
	DIAG_RX_POWER,
	DIAG_TX_FAULT,
	DIAG_FINISH
};

void help(char *myName) {
	printf("Usage:\n");
	printf("\t%s <eeprom_file_name>\n", myName);
	printf("or\n");
	printf("\t%s [-d|-D] [-c] [-v] -s <0-%d> | -a <0-%d> [-p <0-%d>]\n",
											myName,
											SFP_MAX_COUNT-1,
											RXAUI_MAX_COUNT-1,
											PIM_MAX_COUNT-1);
	printf("\n");
	printf("\t-a - rxaui port number (in this case -p param is not needed), or\n");
	printf("\t-p - IO bay number\n");
	printf("\t-s - sfp number in selected IO bay\n");
	printf("\t-d - enable diagnostics info output\n");
	printf("\t-D - ONLY diagnostics info output\n");
	printf("\t-c - run in cycle\n");
	printf("\t-v - show app version\n");
	printf("\ndefault pim is 0\n\n");
	printf("Ex:\n");
	printf("\t%s -s 4\n", myName);
	printf("\t%s -s 7 -p 1\n", myName);
	printf("\t%s -a 0\n", myName);
	printf("\t%s eeprom_dump.dat\n", myName);
	printf("\n");
}

void f_0_type_of_transceiver(unsigned char c) {
	char *f0[] = { // according to enum sff_8024_id, be careful!
		"Unknown or unspecified",
		"GBIC",
		"Module/connector soldered to motherboard (using SFF-8472)",
		"SFP/SFP+/SFP28",
		"300 pin XBI",
		"XENPAK",
		"XFP",
		"XFF",
		"XFP-E",
		"XPAK",
		"X2",
		"DWDM-SFP/SFP+ (not using SFF-8472)",
		"QSFP (INF-8438)",
		"QSFP+ or later (SFF-8436, SFF-8635, SFF-8665, SFF-8685 et al)",
		"CXP or later",
		"Shielded Mini Multilane HD 4X",
		"Shielded Mini Multilane HD 8X",
		"QSFP28 or later (SFF-8665 et al)",
		"CXP2 (aka CXP28) or later",
		"CDFP (Style 1/Style2)",
		"Shielded Mini Multilane HD 4X Fanout Cable",
		"Shielded Mini Multilane HD 8X Fanout Cable",
		"CDFP (Style 3)",
		"microQSFP",
		"QSFP-DD Double Density 8X Pluggable Transceiver (INF-8628)"
	};

	printf("Description of physical device: %s\n",
		(c < (int)sizeof(f0)/sizeof(f0[0])) ? f0[c] : "Reserved");
}

void f_2_desc_of_connector(unsigned char c) {
	char *f2[] = {
		"Unknown or unspecified",
		"SC",
		"Fibre Channel Style 1 copper connector",
		"Fibre Channel Style 2 copper connector",
		"BNC/TNC",
		"Fibre Channel coaxial headers",
		"FiberJack",
		"LC",
		"MT-RJ",
		"MU",
		"SG",
		"Optical pigtail",
		"MPO Parallel Optic"
	};

	char *pt;
	
	if(c < (int)sizeof(f2)/sizeof(f2[0]))
		pt = f2[c];
	else if(c == 0x20)
		pt = "HSSDC II";
	else if(c == 0x21)
		pt = "Copper Pigtail";
	else if(c == 0x22)
		pt = "RJ45";
	else
		pt = "Reserved";

	printf("Description of connector: %s\n", pt);
}

// SFF-8024 Rev 4.1 r001
void f_36_ext_compliance(unsigned char c) {
	char *f36[] = {
		NULL, //"Unspecified",
		"100G AOC (Active Optical Cable) or 25GAUI C2M AOC. Providing a worst BER of 5 × 10^(-5)",
		"100GBASE-SR4 or 25GBASE-SR",
		"100GBASE-LR4 or 25GBASE-LR",
		"100GBASE-ER4 or 25GBASE-ER",
		"100GBASE-SR10",
		"100G CWDM4",
		"100G PSM4 Parallel SMF",
		"100G ACC (Active Copper Cable) or 25GAUI C2M ACC. Providing a worst BER of 5 × 10^(-5)",
		"Obsolete (assigned before 100G CWDM4 MSA required FEC)",
		NULL,
		"100GBASE-CR4 or 25GBASE-CR CA-L",
		"25GBASE-CR CA-S",
		"25GBASE-CR CA-N",
		NULL,
		NULL,
		"40GBASE-ER4",
		"4 x 10GBASE-SR",
		"40G PSM4 Parallel SMF",
		"G959.1 profile P1I1-2D1 (10709 MBd, 2km, 1310nm SM)",
		"G959.1 profile P1S1-2D2 (10709 MBd, 40km, 1550nm SM)",
		"G959.1 profile P1L1-2D2 (10709 MBd, 80km, 1550nm SM)",
		"10GBASE-T with SFI electrical interface",
		"100G CLR4",
		"100G AOC or 25GAUI C2M AOC. Providing a worst BER of 10^(-12) or below",
		"100G ACC or 25GAUI C2M ACC. Providing a worst BER of 10^(-12) or below",
		"100GE-DWDM2 (DWDM transceiver using 2 wavelengths on a 1550nm DWDM grid with a reach up to 80km)"
	};

	if((c < (int)sizeof(f36)/sizeof(f36[0])) && f36[c])
		printf("Description of Module Capability: %s\n", f36[c]);
}

void f_3_40G_compliance(unsigned char c) {
	int i = 0;

	printf("40G Ethernet Compliance Codes:");

	if(CHECK_BIT(c, 3)) {
		i=1;
		printf("\n\t40G BASE-CR4");
	}
	if(CHECK_BIT(c, 2)) {
		i=1;
		printf("\n\t40G BASE-SR4");
	}
	if(CHECK_BIT(c, 1)) {
		i=1;
		printf("\n\t40G BASE-LR4");
	}
	if(CHECK_BIT(c, 0)) {
		i=1;
		printf("\n\t40G ACTIVE");
	}

	if(!i) {
		printf(" none");
	}
	printf("\n");
}

void f_3_10G_compliance(unsigned char c) {
	int i = 0;

	printf("10G Ethernet Compliance Codes:");

	if(CHECK_BIT(c, 7)) {
		i=1;
		printf("\n\t10G BASE-ER");
	}
	if(CHECK_BIT(c, 6)) {
		i=1;
		printf("\n\t10G BASE-LRM");
	}
	if(CHECK_BIT(c, 5)) {
		i=1;
		printf("\n\t10G BASE-LR");
	}
	if(CHECK_BIT(c, 4)) {
		i=1;
		printf("\n\t10G BASE-SR");
	}

	if(!i) {
		printf(" none");
	}
	printf("\n");
}

void f_6_GB_compliance(unsigned char c) {
	int i = 0;

	printf("Gigabit Ethernet Compliance Codes:");

	if(CHECK_BIT(c, 7)) {
		i=1;
		printf("\n\tBASE-PX");
	}
	if(CHECK_BIT(c, 6)) {
		i=1;
		printf("\n\tBASE-BX10");
	}
	if(CHECK_BIT(c, 5)) {
		i=1;
		printf("\n\t100BASE-FX");
	}
	if(CHECK_BIT(c, 4)) {
		i=1;
		printf("\n\t100BASE-LX/LX10");
	}
	if(CHECK_BIT(c, 3)) {
		i=1;
		printf("\n\t1000BASE-T");
	}
	if(CHECK_BIT(c, 2)) {
		i=1;
		printf("\n\t1000BASE-CX");
	}
	if(CHECK_BIT(c, 1)) {
		i=1;
		printf("\n\t1000BASE-LX");
	}
	if(CHECK_BIT(c, 0)) {
		i=1;
		printf("\n\t1000BASE-SX");
	}

	if(!i) {
		printf(" none");
	}
	printf("\n");
}

void f_11_serial_enc_algo(unsigned char c, int is_qsfp) {
	char *f11[] = {
		"Unspecified",
		"8B10B",
		"4B5B",
		"NRZ",
		"Manchester",
		"SONET Scrambled",
		"64B/66B"
	};

	char *f139[] = {
		"Unspecified",
		"8B10B",
		"4B5B",
		"NRZ",
		"SONET Scrambled",
		"64B/66B",
		"Manchester",
		"256B/257B (transcoded FEC-enabled data)",
		"PAM4"
	};

	char *pt = "Reserved";

	if(!is_qsfp) {
		if(c < (int)sizeof(f11)/sizeof(f11[0])) {
			pt = f11[c];
		}
	} else {
		if(c < (int)sizeof(f139)/sizeof(f139[0])) {
			pt = f139[c];
		}
	}

	printf("Description of encoding mechanism: %s\n", pt);
}

void f_12_nominal_bit_rate(unsigned char c, unsigned char d, int is_qsfp) {
	float fSpeed;
	char *ptSp = "Gbits";

	/*  SFF-8636 - only for QSFP's
		Nominal bit rate, units of 100
		Mbps. For BR > 25.4G, set this
		to FFh and use Byte 222.
		Byte 222 is a nominal bit rate per channel, in units of 250 Mbps.
      */
	if(is_qsfp) {
		if(c == 0xff && d)
			fSpeed = (float)d;
		else
			fSpeed = (float)c / 2.5;
	} else {
		if(c < 10) {
			fSpeed = (float)c * 100.0;
			ptSp = "MBits";
		} else
			fSpeed = (float)c / 10.0;
	}

	printf("Nominal bit rate: %.1f %s/sec\n", fSpeed, ptSp);
}

void f_14_19_length(const unsigned char *buf, int is_qsfp) {
	const struct st_supported_link_length f14[] = {
		{ "9/125 fiber",		1,		"km" },
		{ "9/125 fiber",		100,		"m"  },
		{ "50/125 OM2 fiber",		10,		"m"  },
		{ "62.5/125 OM1 fiber",		10,		"m"  },
		{ "copper and Active Cable",	1,		"m"  },
		{ "50/125 OM3 fiber",		10,		"m"  }
	};

	const struct st_supported_link_length f142[] = {
		{ "SMF fiber",			1,		"km" },
		{ "EBW 50/125 fiber",		2,		"m"  },
		{ "50/125 fiber",		1,		"m"  },
		{ "62.5/125 fiber",		1,		"m"  },
		{ "copper",			1,		"m"  }
	};

	struct st_supported_link_length const *pt_st_sup = is_qsfp ? f142 : f14;
	const int pt_st_sup_size = is_qsfp
						? (sizeof(f142) / sizeof(f142[0]))
						: (sizeof(f14) / sizeof(f14[0]));

	int i;

	for(i=0; i<pt_st_sup_size; ++i) {
		// A value of 255 means that the transceiver supports a greater link length
		if((buf[i] != 0) && (buf[i] < 255)) {
			printf("Link length supported for %s: %d%s\n",
				pt_st_sup[i].message, buf[i] * pt_st_sup[i].divider, pt_st_sup[i].distance);

			// not need to display 9/125 fiber distance in meters, already shown in kilometers
			if(!is_qsfp && (i == 0))
				++i;
		}
	}
}

void f_20_40_56_print_ascii(const unsigned char *ptBuf, char *ptMessage, const unsigned int len) {
	char buffer[VEN_ASCII_LEN+1] = { 0 };
	unsigned int i;

	if(len >= sizeof(buffer))
		return;

	if(ptBuf[0] != '\0') {
		for(i=0; i<len; ++i) {
			if(ptBuf[i] == '\0') {
				break;
			}
			if(!isascii(ptBuf[i])) {
				return;
			}
		}

		strncpy(buffer, (const char *)ptBuf, i);

		printf("%s: %s\n", ptMessage, buffer);
	}
}

void f_37_SFP_vendor_IEEE_company_ID(unsigned char c, unsigned char d, unsigned char e) {
	unsigned long uID = ((unsigned long)c & 0xff) << (sizeof(char) * CHAR_BIT * 2);
	uID += ((unsigned long)d & 0xff) << (sizeof(char) * CHAR_BIT);
	uID += (unsigned long)e & 0xff;
	printf("SFP vendor IEEE company ID: %lu (hex: 0x%06lx)\n", uID, uID);
}

void f_60_laser_wavelength(unsigned char c, unsigned char d) {
	unsigned int uLen = (((unsigned int)c & 0xff) << (sizeof(char) * CHAR_BIT))
		+ ((unsigned int)d & 0xff);
	
	printf("Laser wavelength: %u nm\n", uLen);
}

static int is_crc_error(const unsigned char *buf,
				unsigned int start_addr,
				unsigned int crc_addr,
				char *page_message) {
	int is_error = 0;

	if(crc_addr) {
		unsigned short int src=0;
		unsigned int i;
		for(i=start_addr; i<crc_addr; ++i) {
			src += buf[i];
		}
		is_error = ((src & 0xff) != buf[crc_addr]);
		printf("Check Code (%s) \t: %svalid\n", page_message, is_error ? "NOT " : "");
	}

	return is_error;
}

/*
 * calc and check main and extended eeprom src
 * return 0 if ok, >0 if error
 */
int f_63_calc_src(const unsigned char *buf, int is_qsfp, int is_diag_src) {
	int iRet = 0;

	if(is_diag_src == SRC_INFO) {
		iRet |= is_crc_error(buf,
						ind[iCC_Base_Start],
						ind[iCC_Base],
						"CRC");
		iRet |= is_crc_error(buf,
						ind[iCC_Base_Ext_Start],
						ind[iCC_Base_Ext],
						"extended");
	} else if((is_diag_src == SRC_DIAG) && !is_qsfp) {
		iRet |= is_crc_error(buf,
						ind[iCC_Diag_Start],
						ind[iCC_Diag],
						"diag.");
	}

	printf("\n");

	return iRet;
}

void f_66_67_bit_rate_margin(unsigned char c, unsigned char d) {
	printf("Upper bit rate margin: %u%%\n", c);
	printf("Lower bit rate margin: %u%%\n", d);
}

int two_ascii_to_int(const unsigned char buf[2], int max_value) {
	int iRet;
	
	if((iRet = (buf[0] - '0') * 10 + (buf[1] - '0')) > max_value)
		iRet = 0;

	return iRet;
}

/*
 * Date Code
 * 
 * DataAddress	Description of field
 * 0-1			ASCII code, two low order digits of year. (00 = 2000).
 * 2-3			ASCII code, digits of month (01 = Jan through 12 = Dec)
 * 4-5			ASCII code, day of month (01 - 31)
 * 6-7			ASCII code, vendor specific lot code, may be blank
 */
void f_84_date_code(const unsigned char *buf) {
	int iYear, iDay;
	char *ptMonth;

	char *months[] = { "Unk", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	iYear = two_ascii_to_int(&buf[0], 100) + 2000;
	ptMonth = months[two_ascii_to_int(&buf[2], 12)];
	iDay = two_ascii_to_int(&buf[4], 31);

	printf("Vendors manufacturing date code: %s %02d %d\n", ptMonth, iDay, iYear);
}

static int get_diag_16bit(const unsigned char *buf, int reg)
{
	short int value;
	value = (short int)((((unsigned short int) buf[reg]) << 8)
	                    | ((unsigned short int) buf[reg + 1]));
	return (int)value;
}

static unsigned int get_diag_u16bit(const unsigned char *buf, int reg)
{
	unsigned short int value;
	value = (((unsigned short int) buf[reg]) << 8)
		| ((unsigned short int) buf[reg + 1]);
	return (unsigned int)value;
}

static unsigned int get_diag_quad_u16bit(const unsigned char *buf, int reg)
{
	int i;
	unsigned short int value;
	unsigned int retval;

	for(i = 0, retval = 0; i < 4; i++, reg += 2)
	{
		value = (((unsigned short int) buf[reg]) << 8)
			| ((unsigned short int) buf[reg + 1]);
		retval += (unsigned int)value;
	}
	return retval / 4;
}

static unsigned int convert_diag_linear_16bit(const unsigned char *buf,
                                              int reg,
                                              int reg_slope,
                                              int reg_offset)
{
	short int value, value_slope, value_offset;

	value = (short int)((((unsigned short int) buf[reg]) << 8)
		| ((unsigned short int) buf[reg + 1]));

	if(!value) return 0;

	value_slope = (short int)((((unsigned short int) buf[reg_slope]) << 8)
		| ((unsigned short int) buf[reg_slope + 1]));
	value_offset = (short int)((((unsigned short int) buf[reg_offset]) << 8)
		| ((unsigned short int) buf[reg_offset + 1]));
	return ((int) value * (int) value_slope) / 256
		+ (int) value_offset;
}

static unsigned int convert_diag_linear_u16bit(const unsigned char *buf,
                                               int reg,
                                               int reg_slope,
                                               int reg_offset)
{
	unsigned short int value, value_slope, value_offset;

	value = (((unsigned short int) buf[reg]) << 8)
		| ((unsigned short int) buf[reg + 1]);

	if(!value) return 0;

	value_slope = (((unsigned short int) buf[reg_slope]) << 8)
		| ((unsigned short int) buf[reg_slope + 1]);
	value_offset = (((unsigned short int) buf[reg_offset]) << 8)
		| ((unsigned short int) buf[reg_offset + 1]);
	return ((unsigned int)value * (unsigned int)value_slope) / 256
		+ (unsigned int)value_offset;
}

static double convert_nonlinear_double(const unsigned char buf[4])
{
	union
	{
		unsigned int u;
		float f;
	} uvalue;

	/* float */
	uvalue.u = (((unsigned int) buf[0]) << 24)
			 | (((unsigned int) buf[1]) << 16)
			 | (((unsigned int) buf[2]) << 8)
			 |  ((unsigned int) buf[3]);
	return uvalue.f;
}

int roundNo(float num) 
{ 
    return num < 0 ? num - 0.5 : num + 0.5; 
}

static int convert_diag_nonlinear_16bit(const unsigned char *buf,
                                        int reg,
                                        int reg_0,
                                        int reg_1,
                                        int reg_2,
                                        int reg_3,
                                        int reg_4)
{
	short int value;

	double dvalue, dvalue_0, dvalue_1, dvalue_2, dvalue_3, dvalue_4;

	/*  integer */
	value = (short int)((((unsigned short int) buf[reg]) << 8)
	                   | ((unsigned short int) buf[reg + 1]));
	/* double */
	dvalue = (double)value;
	dvalue_0 = convert_nonlinear_double(&buf[reg_0]);
	dvalue_1 = convert_nonlinear_double(&buf[reg_1]);
	dvalue_2 = convert_nonlinear_double(&buf[reg_2]);
	dvalue_3 = convert_nonlinear_double(&buf[reg_3]);
	dvalue_4 = convert_nonlinear_double(&buf[reg_4]);

	return (int)roundNo(
		  dvalue * dvalue * dvalue * dvalue * dvalue_4
		+ dvalue * dvalue * dvalue * dvalue_3
		+ dvalue * dvalue * dvalue_2
		+ dvalue * dvalue_1
		+ dvalue_0);
}

int d_read_qsfp_diag_info(const unsigned char *buf,
					struct sfp_diagnostics *sfp_diag,
					unsigned char tx_fault) {
	int mode = DIAG_TEMPERATURE; // start from sfp temperature displaying

	int iRet = RC_NO_ERROR;

	while(mode < DIAG_FINISH) {
		switch (mode) {
			case DIAG_TEMPERATURE:
				sfp_diag->temp = get_diag_16bit(buf,
									QSFP_TEMPERATURE_ADDR) / 256.0;
				mode = DIAG_VOLTAGE;
				break;
			case DIAG_VOLTAGE:
				sfp_diag->voltage = get_diag_quad_u16bit(buf,
									QSFP_VOLTAGE_ADDR) / 10000.0;
				mode = DIAG_CURRENT;
				break;
			case DIAG_CURRENT:
				sfp_diag->current = get_diag_quad_u16bit(buf,
									QSFP_TX_BIAS_CURRENT_ADDR) / 1000.0;
				mode = DIAG_TX_POWER;
				break;
			case DIAG_TX_POWER:
				sfp_diag->tx_power = get_diag_quad_u16bit(buf,
									QSFP_TX_POWER_ADDR) / 10000.0;
				mode = DIAG_RX_POWER;
				break;
			case DIAG_RX_POWER:
				sfp_diag->rx_power = get_diag_quad_u16bit(buf,
									QSFP_RX_POWER_ADDR) / 10000.0;
				mode = DIAG_TX_FAULT;
				break;
			case DIAG_TX_FAULT:
				if(tx_fault & 0x08)
					sfp_diag->tx_fault = (buf[QSFP_TX_FAULT_ADDR] & 0x0f) != 0;
				else
					sfp_diag->tx_fault = 0;
				mode = DIAG_FINISH;
				break;
			default:
				return RC_UNSPECIFIED;
		}
	}

	return iRet;
}

void d_read_sfp_diag_info_ext(const unsigned char *buf,
					struct sfp_diagnostics *sfp_diag,
					unsigned char tx_fault) {
	int mode = DIAG_TEMPERATURE; // start from sfp temperature displaying

	while(mode < DIAG_FINISH) {
		switch (mode) {
			case DIAG_TEMPERATURE:
				sfp_diag->temp = convert_diag_linear_16bit(buf,
									SFP_XFP_TEMPERATURE_ADDR,
									SFP_T_SLOPE_ADDR,
									SFP_T_OFFSET_ADDR) / 256.0;
				mode = DIAG_VOLTAGE;
				break;
			case DIAG_VOLTAGE:
				sfp_diag->voltage = convert_diag_linear_u16bit(buf,
									SFP_XFP_VOLTAGE_ADDR,
									SFP_V_SLOPE_ADDR,
									SFP_V_OFFSET_ADDR) / 10000.0;
				mode = DIAG_CURRENT;
				break;
			case DIAG_CURRENT:
				sfp_diag->current = convert_diag_linear_u16bit(buf,
									SFP_XFP_CURRENT_ADDR,
									SFP_TX_CURRENT_SLOPE_ADDR,
									SFP_TX_CURRENT_OFFSET_ADDR) * 2/1000.0;
				mode = DIAG_TX_POWER;
				break;
			case DIAG_TX_POWER:
				sfp_diag->tx_power = convert_diag_linear_u16bit(buf,
									SFP_XFP_TX_POWER_ADDR,
									SFP_TX_POWER_SLOPE_ADDR,
									SFP_TX_POWER_OFFSET_ADDR) / 10000.0;
				mode = DIAG_RX_POWER;
				break;
			case DIAG_RX_POWER:
				sfp_diag->rx_power = convert_diag_nonlinear_16bit(buf,
									SFP_XFP_RX_POWER_ADDR,
									SFP_RX_PWR0_ADDR,
									SFP_RX_PWR1_ADDR,
									SFP_RX_PWR2_ADDR,
									SFP_RX_PWR3_ADDR,
									SFP_RX_PWR4_ADDR) / 10000.0;
				mode = DIAG_TX_FAULT;
				break;
			case DIAG_TX_FAULT:
				if(tx_fault & 0x08)
					sfp_diag->tx_fault = ((buf[110] & 0x04) != 0);
				else
					sfp_diag->tx_fault = 0;

				mode = DIAG_FINISH;
				break;
			default:
				return;
		}
	}
}

void d_read_sfp_diag_info_int(const unsigned char *buf,
					struct sfp_diagnostics *sfp_diag,
					unsigned char tx_fault) {
	int mode = DIAG_TEMPERATURE; // start from sfp temperature displaying

	while(mode < DIAG_FINISH) {
		switch (mode) {
			case DIAG_TEMPERATURE:
				sfp_diag->temp = get_diag_16bit(buf,
									SFP_XFP_TEMPERATURE_ADDR) / 256.0;
				mode = DIAG_VOLTAGE;
				break;
			case DIAG_VOLTAGE:
				sfp_diag->voltage = get_diag_u16bit(buf,
									SFP_XFP_VOLTAGE_ADDR) / 10000.0;
				mode = DIAG_CURRENT;
				break;
			case DIAG_CURRENT:
				sfp_diag->current = get_diag_u16bit(buf,
									SFP_XFP_CURRENT_ADDR) * 2/1000.0;
				mode = DIAG_TX_POWER;
				break;
			case DIAG_TX_POWER:
				sfp_diag->tx_power = get_diag_u16bit(buf,
									SFP_XFP_TX_POWER_ADDR) / 10000.0;
				mode = DIAG_RX_POWER;
				break;
			case DIAG_RX_POWER:
				sfp_diag->rx_power = get_diag_u16bit(buf,
									SFP_XFP_RX_POWER_ADDR) / 10000.0;
				mode = DIAG_TX_FAULT;
				break;
			case DIAG_TX_FAULT:
				if(tx_fault & 0x08)
					sfp_diag->tx_fault = ((buf[110] & 0x04) != 0);
				else
					sfp_diag->tx_fault = 0;

				mode = DIAG_FINISH;
				break;
			default:
				return;
		}
	}
}

/**
 * Options parsing.
 * 
 * bus range: 0-1
 * SFP range: 0-7
 * rxaui range: 0-1
**/
int parse_opts(int argc, char *argv[]) {
	int opt, iTmp, iRet = RC_NO_ERROR;

	while ((opt = getopt(argc, argv, "s:p:a:dDcvh")) != -1) {
		switch (opt) {
		case 's': // parse digit from 0 to 7, SFP number
			iTmp = optarg[0] - '0'; // convert ascii digit to int
			if((iTmp >= 0) && (iTmp < SFP_MAX_COUNT))
				sfp_num = iTmp;
			break;
		case 'p': // parse digit from 0 to 1, PIM number
			iTmp = optarg[0] - '0';
			if((iTmp >= 0) && (iTmp < PIM_MAX_COUNT))
				pim_num = iTmp;
			break;
		case 'a': // parse digit from 0 to 1, PIM number
			iTmp = optarg[0] - '0';
			if((iTmp >= 0) && (iTmp < RXAUI_MAX_COUNT)) {
				sfp_num = iTmp;
				pim_num = 2;
			}
			break;
		case 'D':
			info_enable = 0;
		case 'd':
			diag_enable = 1;
			break;
		case 'c':
			run_in_cycle = 1;
			break;
		case 'v':
			printf("version %d.%d\n\n", MAJOR_VER, MINOR_VER);
			break;
		case 'h':
		default: /* '?' */
			return RC_SHOW_HELP;
		    break;
		}
	}

	return iRet;
}

int compose_eeprom_name(char *buf, int buflen, int addr) {
	char devname[MAX_FILE_NAME+1];
	char local_buf[256+1] = "";

	if(sfp_num == (-1))
		return 0;

	if(2 == pim_num)
		snprintf(devname, MAX_FILE_NAME, "rxaui_sfp%d", sfp_num);
	else
		snprintf(devname, MAX_FILE_NAME, "pim%d_sfp%d", pim_num, sfp_num);

	if(find_i2c_device(devname, local_buf, sizeof(local_buf)))
		snprintf(buf, buflen, "%s-00%d/eeprom",	local_buf, addr);
	else if(pim_num < 2) {
		snprintf(devname, MAX_FILE_NAME, "pim%d_qsfp%d", pim_num, sfp_num);
		if(find_i2c_device(devname, local_buf, sizeof(local_buf)))
			snprintf(buf, buflen, "%s-00%d/eeprom",	local_buf, addr);
	}

	if(*buf)
		return 1;
	else
		return 0;
}

#define IS_SECOND_EEPROM	(1 << 0)
#define NO_CLOSE_FILE		(1 << 1)

int read_sysfs_file(unsigned char *buf, int offset, unsigned int size, int flags) {
	static FILE *file50 = NULL, *file51 = NULL;
	FILE *file = (flags & IS_SECOND_EEPROM) ? file51 : file50;
	char *filename = (flags & IS_SECOND_EEPROM) ? ptDiagName : ptFileName;

	if(file == NULL) {
		if((flags & IS_SECOND_EEPROM) && !ptDiagName[0] && !compose_eeprom_name(ptDiagName, MAX_FILE_NAME, 51))
			return RC_I2C_PARSE_ERROR;

		if(!(file = fopen(filename, "r"))) {
			printf("cant open file %s for reading\n", filename);
			return RC_EEPROM_READ_ERROR;
		}
	}

	if(fseek(file, offset, SEEK_SET) || (fread(buf, 1, size, file) != size)) {
		fclose(file);
		printf("input file read error\n");
		return RC_EEPROM_SIZE_MISMATCH;
	} else if(!(flags & NO_CLOSE_FILE)) {
		fclose(file);
	}

	return RC_NO_ERROR;
}

int read_current_sfp_speed(int is_qsfp) {
	FILE *pFile;
	size_t iResult;
	char devname[MAX_FILE_NAME+1];
	char ptSpeed[MAX_FILE_NAME+1];
	int iRet = 0;

	if(2 == pim_num) {
		snprintf(devname, MAX_FILE_NAME, SYS_DIAG_FOLDER, sfp_num);
	} else {
		snprintf(devname, MAX_FILE_NAME, PIM_DIAG_FOLDER, pim_num, is_qsfp ? "q" : "", sfp_num);
	}

	pFile = fopen(devname, "r");
	if(pFile) {
		iResult = fread(ptSpeed, 1, MAX_FILE_NAME, pFile);
		if((iResult > 2) && (ptSpeed[0] == '0') && ((ptSpeed[1] == 'x') || (ptSpeed[1] == 'X')))
			sscanf(ptSpeed, "%x", &iRet);
		fclose(pFile);
	}

	return iRet;
}

diag_mode_t check_diag_mode(unsigned char sfp_type) //unsigned char compliance, unsigned char diag_type)
{
	diag_mode_t diag_mode = DIAG_UNKNOWN;
	unsigned char diagMode[2];

	read_sysfs_file(&diagMode[0], ind[iCompliance], 1, NO_CLOSE_FILE);
	read_sysfs_file(&diagMode[1], ind[iDiagType], 1, NO_CLOSE_FILE);

	switch(sfp_type) {
		case SFF_8024_ID_QSFP28:
		case SFF_8024_ID_QSFP:
		case SFF_8024_ID_QSFPPLUS:
				diag_mode =	(diagMode[0] & (1 << 3))? DIAG_DISABLED : DIAG_INT;
			break;
		case SFF_8024_ID_SFP:
			/* SFF-8472
				* Note that if bit 6, address 92 is set
				* indicating that digital diagnostic monitoring
				* has been implemented
				*/
			if((diagMode[1] & 0x40) && diagMode[0])
				diag_mode = (diagMode[1] & 0x10) ? DIAG_EXT : DIAG_INT;
			else
				diag_mode = DIAG_DISABLED;
			break;
		default:
			diag_mode = DIAG_INT;
	}

	return diag_mode;
}

/**
 * file - handle of opened EEPROM
 */
int info_print(int is_qsfp) {
	unsigned char map[PAGE_SIZE];
	int iRet = RC_NO_ERROR;

	if(!(iRet = read_sysfs_file(map, is_qsfp ? PAGE_SIZE : 0, PAGE_SIZE, 0))) {
		printf("\n=> Information <=\n\n");
		f_63_calc_src(map, is_qsfp, SRC_INFO);
		f_0_type_of_transceiver(map[ind[iIdentifier]]);
		f_2_desc_of_connector(map[ind[iConnector]]);
		f_3_40G_compliance(map[ind[iTransceiver]]);
		f_3_10G_compliance(map[ind[iTransceiver]]);
		f_36_ext_compliance(map[ind[iTransceiver_ext]]);
		f_6_GB_compliance(map[ind[iTransceiver]+3]);
		f_11_serial_enc_algo(map[ind[iEncoding]], is_qsfp);
		f_12_nominal_bit_rate(map[ind[iBR_Nominal]], map[ind[iBR_Nominal_ext]], is_qsfp);
		f_14_19_length(&map[ind[iLength]], is_qsfp);
		f_20_40_56_print_ascii(&map[ind[iVendor_Name]],
			"SFP vendor name",
			VEN_ASCII_LEN);
		f_37_SFP_vendor_IEEE_company_ID(map[ind[iVendor_OUI]],
			map[ind[iVendor_OUI]+1],
			map[ind[iVendor_OUI]+2]);
		f_20_40_56_print_ascii(&map[ind[iVendor_PN]],
			"Part number",
			VEN_ASCII_LEN);
		f_20_40_56_print_ascii(&map[ind[iVendor_Rev]],
			"Revision level for part number provided by vendor",
			ind[iVendor_Rev_len]);
		f_60_laser_wavelength(map[ind[iWavelength]],
			map[ind[iWavelength]+1]);
		// if(is_qsfp) f_66_67_bit_rate_margin(map0[66], map0[67]);
		f_20_40_56_print_ascii(&map[ind[iVendor_SN]],
			"Serial number provided by vendor (ASCII)",
			VEN_ASCII_LEN);
		f_84_date_code(&map[ind[iDate_Code]]);
	}

	return iRet;
}

int diag_print(int is_qsfp, unsigned char sfpType) {
	unsigned char map[PAGE_SIZE];

	struct sfp_diagnostics sfp_diag = {
		.temp		= -1,
		.voltage	= -1,
		.current	= -1,
		.tx_power	= -1,
		.rx_power	= -1,
		.tx_fault	=  0
	};

	diag_mode_t diag_mode = check_diag_mode(sfpType);

	printf("\n=> Diagnostics <=\n\n");

	if(diag_mode == DIAG_UNKNOWN || diag_mode == DIAG_DISABLED)
		printf("Digital diagnostics not implemented...\n");
	else {
		unsigned char txFault;

		if(read_sysfs_file(&txFault, ind[iTxFault] + (is_qsfp ? PAGE_SIZE : 0), 1, NO_CLOSE_FILE)
			|| read_sysfs_file(map, 0, PAGE_SIZE, !is_qsfp)) {
				printf("input file read error\n");
				return RC_EEPROM_SIZE_MISMATCH;
		}

		if(is_qsfp)
			d_read_qsfp_diag_info(map, &sfp_diag, txFault);
		else {
			f_63_calc_src(map, is_qsfp, SRC_DIAG);
			if(diag_mode == DIAG_EXT)
				d_read_sfp_diag_info_ext(map, &sfp_diag, txFault);
			else
				d_read_sfp_diag_info_int(map, &sfp_diag, txFault);
		}

		if(sfp_diag.temp >= 0)
			printf("Internally measured module temperature: %f\n", sfp_diag.temp);
		if(sfp_diag.voltage >= 0)
			printf("Internally measured supply voltage in transceiver: %f\n", sfp_diag.voltage);
		if(sfp_diag.current >= 0)
			printf("Internally measured TX Bias Current: %f\n", sfp_diag.current);
		if(sfp_diag.tx_power >= 0)
			printf("Measured TX output power: %f\n", sfp_diag.tx_power);
		if(sfp_diag.rx_power >= 0)
			printf("Measured RX input power: %f\n", sfp_diag.rx_power);
		if(sfp_diag.tx_fault)
			printf("TX fault!\n");
	}

	return RC_NO_ERROR;
}

int main(int argc, char *argv[]) {
	int iRet = RC_SHOW_HELP;
	int is_qsfp = 0;

	if(argc != 1) {
		if((iRet = parse_opts(argc, argv)) == RC_NO_ERROR) {
			if(argc-optind == 1) {
				if(diag_enable)
					diag_enable = 0;
				strncpy(ptFileName, argv[optind], MAX_FILE_NAME);
			} else if((argc) > 2) {
				if(!compose_eeprom_name(ptFileName, MAX_FILE_NAME, 50)) {
					printf("\ncan't find device!\n\n");
					iRet = RC_I2C_PARSE_ERROR;
				}
			} else {
				iRet = RC_SHOW_HELP;
			}
		}
	}

	if(iRet == RC_SHOW_HELP) {
		help(argv[0]);
	} else if(iRet == RC_NO_ERROR) {
		unsigned char sfpType;
		int info_err = 0, diag_err = 0;

		printf("Opening %s ...\n", ptFileName);
		if((iRet = read_sysfs_file(&sfpType, 0, 1, 0)) == RC_NO_ERROR) {
			switch(sfpType) {
				case SFF_8024_ID_QSFP28:
				case SFF_8024_ID_QSFP:
				case SFF_8024_ID_QSFPPLUS:
					ind = ind_qsfp;
					is_qsfp = 1;
					break;
				case SFF_8024_ID_SFP: // SFP, SFP+
					ind = ind_sfp;
					break;
				default:
					iRet = RC_MODULE_NOT_SUPPORTED;
			}
		}

		if(iRet == RC_NO_ERROR) {
			do {
				if(info_enable)
					info_err = info_print(is_qsfp);
				if(diag_enable)
					diag_err = diag_print(is_qsfp, sfpType);
				if(run_in_cycle)
					sleep(3);
			} while (run_in_cycle && !info_err && !diag_err);

			{
				int curr_speed = read_current_sfp_speed(is_qsfp);
				if(curr_speed) {
					printf("\nCurrent SFP speed: %dG\n", curr_speed);
				}
			}
		}
	}

	return iRet;
}
