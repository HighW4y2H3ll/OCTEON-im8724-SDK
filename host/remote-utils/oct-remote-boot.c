/***********************license start************************************
 * Copyright (c) 2003-2015 Cavium Inc. (support@cavium.com). All rights
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
 * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
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
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **********************license end**************************************/

/**
 * @file
 *
 * Boot a remote Octeon
 *
 * $Id: oct-remote-boot.c 144517 2016-08-31 16:39:57Z cchavva $
 */
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>

#define CVMX_ADD_IO_SEG(a) (a)  /* There aren't any Mips IO segments when using PCI */

#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "octeon-model.h"
#include <octeon_eeprom_types.h>
#include <lib_octeon_shared.h>
#include <octeon_mem_map.h>
#include "cvmx-twsi.h"
#include "cvmx-bootloader.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-ocx-defs.h"


uint32_t  crc32(uint32_t crc, const uint8_t *buf, uint32_t len);

#define debug(format, ...) octeon_remote_debug(1, format, ##__VA_ARGS__);
#define ULL unsigned long long

#define U_BOOT_PCI_LOAD_ADDRESS     0x1000000
#define NAME_LEN                    (300)

/* Variables for use with getopt */
static int memonly_flag = 0;
static int twsi_scan_flag = 0;
static int dump_spd_flag = 0;
static int read_level_flag = 0;
static int load_cache_flag = 0;
static int twsi_id = 0;
static uint8_t dev_addr = 0;
static uint16_t intr_addr = 0;
static uint64_t data = 0;


/* provide several octeon_ functions for the DRAM controller config code
** shared with the bootloader */
uint64_t octeon_read64(uint64_t address)
{
    return(octeon_remote_read_csr(address));
}
void octeon_write64(uint64_t address, uint64_t val)
{
    octeon_remote_write_csr(address, val);
}
void octeon_delay_cycles(uint64_t cycles)
{
    cycles = cycles/10;
    while (cycles--)
        ;
}

int copy_file_to_octeon_dram(uint64_t address, const char *filename)
{
    char buffer[65536] = {0};
    uint64_t bytes_copied = 0;


    FILE *infile = fopen(filename, "r");
    if (infile == NULL)
    {
        perror("Unable to open file");
        return -1;
    }

    while (!feof(infile))
    {
        int count = fread(buffer, 1, sizeof(buffer), infile);
        if (count > 0)
        {
            octeon_remote_write_mem(address, buffer, count);
            address += count;
            bytes_copied += count;
        }
    }
    fclose(infile);

    /* Clear to end of cache line */
    if (load_cache_flag && (bytes_copied % 128)) {
        memset(buffer, 0, 128);
	octeon_remote_write_mem(address, buffer, 128 - ((bytes_copied + 127) % 128));
    }
    return bytes_copied;

}

/* Very simple and slow memtest. */
void memtest(uint32_t size)
{
    uint32_t addr, val, ref;
    int i, j;
    int ecnt = 0;


    printf("Testing %d Kbytes of memory\n", size/1024);

    for (j = 0; j < 10; j++)
    {
        for (i = 0; i < 1; i++)
        {
            ref = 0;
            for (addr = 0; addr < size; addr += 4)
            {
                octeon_remote_write_mem32(addr, ref);
                ref += 0x0000400001;
            }

            ref = 0;
            for (addr = 0; addr < size; addr += 4)
            {
                val = octeon_remote_read_mem32(addr);

                if (val != ref)
                {
                    printf("Mismatch: addr: 0x%llx, ref: 0x%llx, val: 0x%llx, xor: 0x%08llx\n",
                           (unsigned long long)addr,
                           (unsigned long long)ref,
                           (unsigned long long)val,
                           (unsigned long long)((unsigned long long)ref ^ (unsigned long long)val));
                    val = octeon_remote_read_mem32(addr);
                    if (val != ref)
                    {
                        printf("Reread: Mismatch: addr: 0x%llx, ref: 0x%llx, val: 0x%llx, xor: 0x%08llx\n",
                               (unsigned long long)addr,
                               (unsigned long long)ref,
                               (unsigned long long)val,
                               (unsigned long long)((unsigned long long)ref ^ (unsigned long long)val));
                    }
                    else
                    {
                        printf("Reread: MATCH: addr: 0x%llx, ref: 0x%llx, val: 0x%llx\n",
                               (unsigned long long)addr,
                               (unsigned long long)ref,
                               (unsigned long long)val);

                    }

                    if (ecnt++ > 50)
                        return;
                }
                ref += 0x0000400001;
            }
            printf(".");
            fflush(stdout);

        }
    }
    printf("Done\n");
}


/* Determine if the device at the given address is a dimm.
** If it is, we don't want to use 16 bit internal addresses on it (which the TLV code uses), as that will
** write to the spd eeprom.
** Return 1 if it looks like an SPD, 0 otherwise
*/
int is_spd(int twsi_addr)
{
    return validate_spd_checksum(twsi_addr, 1);
}

int determine_board_type(int forced_board)
{
    uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
    octeon_eeprom_board_desc_t *bd_ptr = (void *)ee_buf;
    int addr;
    int i;
    int detected_board_type = 0;
    uint8_t tuple_eeprom_addrs[] = {0x52, 0x53, 0x56, 0x54};  /* Possible addresses of eeprom with tuples */



    /* All of the eeproms used for tuples have 16 bit internal addresses, so
    ** we will ignore eeproms that look like DDR2 SPDs, as these only have 8 bit internal addresses */
    for (i = 0; i < (int)sizeof(tuple_eeprom_addrs); i++)
    {
        /* Read (carefully, using 8 bit internall addresses) some data from the eeprom and see if it looks like
        ** configuration tuples. */
        debug("Looking for non-SPD eeprom at TWSI address 0x%x\n", tuple_eeprom_addrs[i]);
        if (is_spd(tuple_eeprom_addrs[i]))
        {
            debug("Addr 0x%x looks like SPD, skipping\n", tuple_eeprom_addrs[i]);
            continue;
        }
        /* Now that we don't think this is an SPD, look for a board descriptor */
        addr = octeon_tlv_get_tuple_addr(tuple_eeprom_addrs[i], EEPROM_BOARD_DESC_TYPE, 0, ee_buf, OCTEON_EEPROM_MAX_TUPLE_LENGTH);
        if (addr >= 0)
        {
            /* Found valid board descriptor */
            debug("Read board descriptor: type: %d, rev_maj: %d, rev_min: %d\n",
                  ntohs(bd_ptr->board_type), bd_ptr->rev_major, bd_ptr->rev_minor);
            detected_board_type = ntohs(bd_ptr->board_type);
            break;
        }

    }

    if (forced_board)
    {
        if (detected_board_type && (forced_board != detected_board_type))
            printf("WARNING: detected board type %s (%d) does no match supplied board type %s (%d).\n",
                   cvmx_board_type_to_string(detected_board_type), detected_board_type, cvmx_board_type_to_string(forced_board), forced_board);
        return(forced_board);
    }
    if (detected_board_type)
        return(detected_board_type);

    printf("ERROR: board type detection failed, and no board type specified with --board option\n");
    return(-1);
}

void usage(const char *argv[])
{

    printf("\n"
           "Usage: %s [--memonly] [--board=board_name] [bootloader filename]\n"
           "    Boot Octeon and load bootloader over remote connection (PCI or EJTAG).\n"
           "    --help: Show this usage.\n"
           "    --memonly: If specified only the memory controller is configured.\n"
           "    --loadcache: If specified the memory controller is NOT configured and the\n"
           "                 bootloader is loaded into L2 cache where it begins executing.\n"
           "    --board=board_name:  Force a particular board type to be used.\n"
           "    --envfile=<environment filename>: Reads the u-boot environment data from\n"
           "              the specified file, loads it into DRAM for u-boot to use.  The u-boot\n"
           "              image must be configured to use this environment.  The format of the file\n"
           "              is one environment variable per line:\n"
           "              env_var=env_val\\n\n"
           "              each line must be ended with a single '\\n'\n"
           "    --scantwsi: Scan twsi bus and exit.\n"
           "    --twsiread: Read TWSI bus from a 7-bit device address using internal address, 8 bytes at a time\n"
           "    --twsiwrite: Write 2 bytes to a TWSI device using internal address\n"
           "    --dumpspd: Dump SPD contents when scanning twsi bus.\n"
           "\n"
           "    The following options are to support booting the generic ram based u-boot image, and\n"
           "    are not used for normal operation.\n"
           "    --ddr0spd=<twsi_addr[,twsi_addr]|filename[,filename]>: SPD addresses or files for DIMMS on interface 0.  Specify multiple times\n"
           "               if multiple dimms are present.  For 128 bit wide DDR interaces (CN38XX/CN58XX)\n"
           "               comma separated pairs off addresses must be used.(only required for generic board type.)\n"
           "    --ddr1spd=<twsi_addr|filename>: SPD addresses or files for DIMMS on DDR interface 1.  Only valid for Chips\n"
           "               with 2/4 DDR controllers.  Specify multiple times for multiple DIMMS.(only required for generic board type.)\n"
           "    --ddr2spd=<twsi_addr|filename>: SPD addresses or files for DIMMS on DDR interface 2.  Only valid for Chips\n"
           "               with 4 DDR controllers, such as CN68XX.  Specify multiple times for multiple DIMMS.(only required for generic board type.)\n"
           "    --ddr3spd=<twsi_addr|filename>: SPD addresses or files for DIMMS on DDR interface 3.  Only valid for Chips\n"
           "               with 4 DDR controllers, such as CN68XX.  Specify multiple times for multiple DIMMS.(Only required for generic board type.)\n"
           "               \n"
           "    --cpu_ref_hz=<NNN>: CPU reference clock in Hz.  (only requried for generic board type.)\n"
           "    --ddr_ref_hz=<NNN>: DDR reference clock in Hz.  (only requried for generic board type.)\n"
           "    --ddr_clock_hz=<NNN>: DDR clock in Hz.  (only requried for generic board type.)\n"
           "    --board_delay=<NNN>: board delay, in picoseconds. (only required for generic board type.)\n"
           "    --readlevel:  Enable read leveling (auto board delay), CN52XX/CN56XX only.\n"
           "    The bootloader (optionally) supplied must be built for\n"
           "     booting from DRAM.  If no bootloader is specified the standard one\n"
           "     for the detected or supplied board type will be chosen.\n"
           "\n", argv[0]);
    printf("For boards without SPD eeproms, filenames rather than SPD addresses may be specified.  The files\n"
           "must contain SPD values with 1 hex value per line, and a minimum of 64 values.\n\n");
}

int init_twsi(int scan_flag)
{
    /* Slow down the TWSI clock, as some boards (Thunder) seem to need it slower
     ** than the default, especially when a spi4000 is connected. */
     int max_twsi_bus = 0;  /* MAX twsi bus number */
     int bus;

     if (OCTEON_IS_MODEL(OCTEON_CNF75XX))
         max_twsi_bus = 2;
     else if (OCTEON_IS_MODEL(OCTEON_CN6XXX)
	 || OCTEON_IS_MODEL(OCTEON_CN52XX)
	 || OCTEON_IS_MODEL(OCTEON_CN56XX)
	 || OCTEON_IS_MODEL(OCTEON_CN7XXX))
         max_twsi_bus = 1;
     cvmx_mio_tws_sw_twsi_t sw_twsi;
     sw_twsi.u64 = 0;
     sw_twsi.s.v = 1;
     sw_twsi.s.op = 0x6;
     sw_twsi.s.eop_ia = 0x3;
     sw_twsi.s.d = (0xF << 3);  /* Slow down TWSI clock */
     for (bus = 0;bus <= max_twsi_bus;bus++)
         octeon_remote_write_csr(CVMX_MIO_TWSX_SW_TWSI(bus), sw_twsi.u64);

     if (scan_flag)
     {
         for (bus=0; bus <= max_twsi_bus; bus++)
         {
             printf("Scanning twsi bus %d\n", bus);

             /* Scan twsi bus, useful for new board debug/bringup */
             {
                 int i;
                 for (i = 0; i < 128; i++)
                 {
                     uint64_t data;
                     if (cvmx_twsix_read(bus, i, 1, &data) >= 0)
                     {
                         printf("Twsi addr %d (0x%x), bus %d responded", i, i, bus);
                         if (is_spd(i | (bus << OCTEON_TWSI_BUS_IN_ADDR_BIT)))
                         {
                             printf(", DIMM SPD");
                             if (dump_spd_flag)
                             {
                                 int j;
                                 uint8_t val;
                                 printf("\n");
                                 printf("SPD contents:\n");
                                 for (j = 0; j < 128; j++)
                                 {
                                     val = cvmx_twsix_read_ia8(bus, i, j, 1);
                                     printf("0x%02x: 0x%02x\n", j, val);
                                 }

                             }

                         }
                         printf("\n");
                     }
                 }
             }
         }
     }

     return 0;
}

int load_l2(uint64_t address, const char *filename)
{
    int retcode;
    /* Enable all L2 ways */
    printf("Loading %s into L2 cache starting at address 0x%llx\n",
           filename, (unsigned long long)address);
    cvmx_write_csr(CVMX_L2C_WPAR_PPX(0), 0);
    /* Apply workaround for Errata L2C-24656 */
    if (OCTEON_IS_MODEL(OCTEON_CN73XX_PASS1_0))
    {
        cvmx_l2c_tad_ctl_t l2c_tad;
        l2c_tad.u64 = cvmx_read_csr(CVMX_L2C_TAD_CTL);
        l2c_tad.s.maxlfb = 0;
        cvmx_write_csr(CVMX_L2C_TAD_CTL, l2c_tad.u64);
    }
    retcode = copy_file_to_octeon_dram(address, filename);

    if (OCTEON_IS_OCTEON3())
    {
        union cvmx_rst_boot rst_boot;
        rst_boot.u64 = octeon_remote_read_csr(CVMX_RST_BOOT);
        rst_boot.s.romen = 0;
        cvmx_write_csr(CVMX_RST_BOOT, rst_boot.u64);
    }
    return retcode;
}

static uint64_t convert_number(const char *str)
{
    unsigned long long result;
    if ((str[0] == '0') && (str[1] == 'x'))
    {
        if (sscanf(str+2, "%llx", &result) != 1)
            result = 0;
    }
    else
    {
        if (sscanf(str, "%lli", &result) != 1)
            result = 0;
    }
    return result;
}

int main(int argc, const char *argv[])
{
    int retval = -1;
    uint32_t ddr_clock_hz = 0;
    uint32_t cpu_clock_hz = 0;
    uint32_t ddr_ref_hz = 0;
    uint32_t cpu_ref_hz = 0;
    int board_type = 0;
    int board_rev_maj = 0;
    int board_rev_min = 0;
    cvmx_ciu_pp_rst_t pp_rst;
    char bootloader_filename[NAME_LEN + 1] = {0};
    char *oct_root = getenv("OCTEON_ROOT");
    ddr_configuration_t ddr_configuration[4];
    uint32_t measured_ddr_hertz = 0;
    int c;
    int forced_board_type = 0;
    char env_buf[U_BOOT_RAM_ENV_SIZE] = {0};
    int ddr_int_valid_mask = 0;
    int board_delay = 0;
    int ddr0spd_index = 0;
    int ddr1spd_index = 0;
    int ddr2spd_index = 0;
    int ddr3spd_index = 0;
    int dram_size_mbytes = 0;
    const board_table_entry_t *board_entry;
    /* First index is for ddr interface number, and second is for dimm index on that interface. */
    dimm_config_t dimm_table[4][DDR_CFG_T_MAX_DIMMS];
    uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
    octeon_eeprom_board_desc_t *bd_ptr = (void *)ee_buf;
    octeon_eeprom_clock_desc_t *cd_ptr = (void *)ee_buf;
    octeon_eeprom_ddr_clock_desc_t *dcd_ptr = (void *)ee_buf;
    int addr = -1;
    char *board_type_str = NULL;
    int i;
    uint32_t crc;

    memset(&ddr_configuration, 0, sizeof(ddr_configuration));
    memset(&dimm_table, 0, sizeof(dimm_table));


    /* Parse command line options using getopt_long */
    while (1)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"memonly",   no_argument, &memonly_flag, 1},
            {"scantwsi",  no_argument, &twsi_scan_flag, 1},
            {"dumpspd",   no_argument, &dump_spd_flag, 1},
            {"readlevel", no_argument, &read_level_flag, 1},
	    {"loadcache", no_argument, &load_cache_flag, 1},
            /* These options don't set a flag.
               We distinguish them by their indices. */
            {"board",   required_argument, 0, 0},
	    {"board_type",required_argument, 0, 0},
            {"envfile",   required_argument, 0, 0},
            {"ddr0spd",   required_argument, 0, 0},
            {"ddr1spd",   required_argument, 0, 0},
            {"ddr2spd",   required_argument, 0, 0},
            {"ddr3spd",   required_argument, 0, 0},
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {"board_delay",   required_argument, 0, 0},
            {"ddr_ref_hz",   required_argument, 0, 0},
            {"ddr_clock_hz",   required_argument, 0, 0},
            {"cpu_ref_hz",   required_argument, 0, 0},
            {"twsiread",  required_argument, 0, 0},
            {"twsiwrite",  required_argument, 0, 0},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, (char * const *)argv, "hv",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            /* All long options handled in case 0 */
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
#ifdef DEBUG
            printf ("option(l) %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
#endif

            if (!strcmp(long_options[option_index].name, "board"))
            {
                int i = 0;
                const board_table_entry_t *board_entry;
                board_entry = lookup_board_table_entry(0, optarg);
                if (board_entry)
                    forced_board_type = board_entry->board_type;

                if (!forced_board_type)
                {
                    printf("unrecognized board name: %s, supported names are: ", optarg);
                    i = 0;
                    while (octeon_board_ddr_config_table[i].board_type)
                        printf(" %s", cvmx_board_type_to_string(octeon_board_ddr_config_table[i++].board_type));
                    printf("\n");
                    exit(-1);
                }
                printf("Using user supplied board name: %s, number: %d\n", optarg, forced_board_type);
            }
            else if (!strcmp(long_options[option_index].name, "board_type"))
            {
                const board_table_entry_t *board_entry;
                if (isdigit(*optarg))
                    board_type_str = strdup(optarg);
                else
                {
                    board_entry = lookup_board_table_entry(0, optarg);
                    if (board_entry) {
                        char *ptr;
                        board_type_str = strdup(optarg);
                        ptr = board_type_str;
                        /* Convert to upper case */
                        while (*ptr) {
                            *ptr = toupper(*ptr);
                            ptr++;
                        }
                    }
                }
            }
            else if (!strcmp(long_options[option_index].name, "envfile"))
            {
                /* Read environment from a file */
                int fd = open(optarg, O_RDONLY);
                if (fd < 0)
                {
                    printf("ERROR: unable to open env file: %s\n", optarg);
                    exit(-1);
                }
                if (read(fd, env_buf + U_BOOT_RAM_ENV_CRC_SIZE, U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE - 1) < 0)
                {
                    printf("ERROR: unable to read from env file: %s\n", optarg);
                    exit(-1);
                }
                close(fd);
                printf("Using environment from file: %s\n", optarg);
            }
            else if (!strcmp(long_options[option_index].name, "ddr0spd"))
            {
                printf("Using user supplied DDR 0 spd address(es)/file(s): %s\n", optarg);
                char *high_ptr = strstr(optarg, ",");
                if (high_ptr)
                {
                    *high_ptr = '\0';
                    high_ptr++;
                }

                process_spd_arg(&dimm_table[0][ddr0spd_index], optarg, 0);
                if (high_ptr)
                    process_spd_arg(&dimm_table[0][ddr0spd_index], high_ptr, 1);
                ++ddr0spd_index;
                ddr_int_valid_mask |= 1;
            }
            else if (!strcmp(long_options[option_index].name, "ddr1spd"))
            {
                printf("Using user supplied DDR 1 spd address/file: %s\n", optarg);
                process_spd_arg(&dimm_table[1][ddr1spd_index++], optarg, 0);
                ddr_int_valid_mask |= 2;
            }
            else if (!strcmp(long_options[option_index].name, "ddr2spd"))
            {
                printf("Using user supplied DDR 2 spd address/file: %s\n", optarg);
                process_spd_arg(&dimm_table[2][ddr2spd_index++], optarg, 0);
                ddr_int_valid_mask |= 4;
            }
            else if (!strcmp(long_options[option_index].name, "ddr3spd"))
            {
                printf("Using user supplied DDR 3 spd address/file: %s\n", optarg);
                process_spd_arg(&dimm_table[3][ddr3spd_index++], optarg, 0);
                ddr_int_valid_mask |= 8;
            }
            else if (!strcmp(long_options[option_index].name, "board_delay"))
            {
                board_delay = strtol(optarg, NULL, 0);
                printf("Using user supplied board delay: %d\n", board_delay);
            }
            else if (!strcmp(long_options[option_index].name, "ddr_ref_hz"))
            {
                printf("Using user supplied DDR ref Hz: %s\n", optarg);
                ddr_ref_hz = strtol(optarg, NULL, 0);
            }
            else if (!strcmp(long_options[option_index].name, "ddr_clock_hz"))
            {
                printf("Using user supplied DDR clock Hz: %s\n", optarg);
                ddr_clock_hz = strtol(optarg, NULL, 0);
            }
            else if (!strcmp(long_options[option_index].name, "cpu_ref_hz"))
            {
                printf("Using user supplied CPU ref Hz: %s\n", optarg);
                cpu_ref_hz = strtol(optarg, NULL, 0);
            }
            else if (!strcmp(long_options[option_index].name, "twsiread"))
            {
                if (argc != 5)
                	usage(argv);
                twsi_id = convert_number(argv[2]);
                dev_addr = convert_number(argv[3]);
                intr_addr = convert_number(argv[4]);

                if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
                    return -1;
                if (cvmx_twsix_read_ia(twsi_id, dev_addr, intr_addr, 2, 1, &data) == -1) {
                    printf("error reading 0x%x address\n", intr_addr);
                    return -1;
	        }
                printf("Read: bus %d addr %d(0x%x) reg %d(0x%x) = 0x%04x\n", twsi_id,
		        dev_addr, dev_addr, intr_addr, intr_addr, (unsigned int)data);
                octeon_remote_close();
                return 0;
            }
            else if (!strcmp(long_options[option_index].name, "twsiwrite"))
            {
                if (argc != 6)
                	usage(argv);
                twsi_id = convert_number(argv[2]);
                dev_addr = convert_number(argv[3]);
                intr_addr = convert_number(argv[4]);
                data = convert_number(argv[5]);
                if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
                    return -1;
                if (cvmx_twsix_write_ia(twsi_id, dev_addr, intr_addr, 2, 1, data) == -1) {
                    printf("Error writing 0x%x address with 0x%x data\n",
                            intr_addr, (unsigned int)data);
                    return -1;
	        }
	        printf("Wrote: bus %d addr %d(0x%x) reg %d(0x%x) <= 0x%04x\n", twsi_id,
		        dev_addr, dev_addr, intr_addr, intr_addr, (unsigned int)data);
                octeon_remote_close();
                return 0;
            }
            else
            {
                printf("Unrecognized option: %s\n", long_options[option_index].name);
                usage(argv);
                exit(-1);
            }
            break;


        case 'h':
        case '?':
            /* getopt_long already printed an error message. */
            usage(argv);

	   /* fallthrough */
        case 'v':
            printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING);
            return -1;
            break;

        default:
            abort ();
        }
    }

    if (optind < argc)
    {
        /* We only support one argument - an optional bootloader file name */
        if (argc - optind > 1)
        {
#ifdef DEBUG
            printf ("non-option ARGV-elements: ");
            while (optind < argc)
                printf ("%s ", argv[optind++]);
            putchar ('\n');
#endif
            usage(argv);
            return -1;
        }
        strncpy(bootloader_filename, argv[optind], NAME_LEN);
    }
    else if (!oct_root && !memonly_flag)
    {
        printf("Unable to determine OCTEON_ROOT\n");
        return -1;
    }


    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

#ifndef CONFIG_OCTEON_CN70XX
    if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
        printf("oct-remote-boot is depreciated for cn70xx, instead use oct-remote-boot-cn70xx.\n");
        return -1;
    }
#endif

    if (OCTEON_IS_OCTEON3() && !OCTEON_IS_MODEL(OCTEON_CN70XX)) {
        load_cache_flag = 1;
	printf("CN7XXX: Loading from cache automatically\n");
    }
    if (load_cache_flag)
    {
        if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
        {
            printf("Warning: --loadcache is only supported on Octeon II and later\n");
            load_cache_flag = 0;
        } else if (OCTEON_IS_MODEL(OCTEON_CN60XX) || OCTEON_IS_MODEL(OCTEON_CNF71XX) ||
                   OCTEON_IS_MODEL(OCTEON_CN70XX))
        {
            printf("Warning: The L2 cache on the Octeon model is likely too small for --loadcache\n");
            load_cache_flag = 0;
        }
    }
    /* Explicitly lock access to the bus, otherwise this is done for every
    ** read/write, which allows other accesses in while we are booting the chip.
    ** We want to prevent all access until we are done.
    */
    octeon_remote_lock();

    if (OCTEON_IS_MODEL(OCTEON_CN76XX))
    {
        cvmx_l2c_oci_ctl_t l2c_oci;
        cvmx_ocx_com_node_t ocx_node;
        uint64_t node_mask = 0x3ull << 36;

        l2c_oci.u64 = octeon_remote_read_csr(CVMX_L2C_OCI_CTL + node_mask);
        l2c_oci.s.gksegnode = 0;
        octeon_remote_write_csr(CVMX_L2C_OCI_CTL + node_mask, l2c_oci.u64);

        ocx_node.u64 = octeon_remote_read_csr(CVMX_OCX_COM_NODE + node_mask);
        ocx_node.s.id = 0;
        octeon_remote_write_csr(CVMX_OCX_COM_NODE + node_mask, ocx_node.u64);

    }

    int retry_count=0;

retry:
    /* Check to see if all cores in reset, and if so, don't do a soft
    ** reset so we cooperate with the device driver. */
    pp_rst.u64 = octeon_remote_read_csr(CVMX_CIU_PP_RST);
    uint64_t mask = (1ull<<cvmx_pop(octeon_remote_read_csr(CVMX_CIU_FUSE)))-1;
    if ((pp_rst.u64 & mask) != mask)
    {
        octeon_remote_reset(1);
        usleep(10000);

        if (OCTEON_IS_MODEL(OCTEON_CN38XX) || OCTEON_IS_MODEL(OCTEON_CN58XX))
        {
            /* Do a second reset for the LLM workaround only if in pciboot mode. */
            pp_rst.u64 = octeon_remote_read_csr(CVMX_CIU_PP_RST);
            if ((pp_rst.u64 & mask) == mask)
            {
                octeon_remote_reset(1);
                usleep(10000);
            }
        }
    }
    else
    {
        printf("All cores in reset, skipping soft reset.\n");
    }



    init_twsi(twsi_scan_flag);
    if (twsi_scan_flag)
        goto cleanup;



    debug("Octeon write buffer base: 0x%x\n", BOOTLOADER_PCI_READ_BUFFER_BASE);

    debug("Octeon device id: 0x%x\n", octeon_remote_get_model());

    board_type = determine_board_type(forced_board_type);
    if (board_type < 0)
        goto cleanup;

    uint32_t chip_id;
    uint32_t ddr_interface_mask = 1;

    chip_id = octeon_remote_get_model();

    /* Look up board information from table */
    board_entry = lookup_board_table_entry(board_type, NULL);
    if (!board_entry)
    {
        printf("ERROR: Unsupported board type detected: %s (%d)\n", cvmx_board_type_to_string(board_type), board_type);
        return(-1);
    }

    if (!forced_board_type && board_entry->eeprom_addr)
    {

        addr = octeon_tlv_get_tuple_addr(board_entry->eeprom_addr, EEPROM_BOARD_DESC_TYPE, 0, ee_buf, OCTEON_EEPROM_MAX_TUPLE_LENGTH);
        if (addr >= 0)
        {
            /* Found valid board descriptor */
            board_type = ntohs(bd_ptr->board_type);
            board_rev_maj = bd_ptr->rev_major;
            board_rev_min = bd_ptr->rev_minor;
            debug("Read board descriptor: type: %d, rev_maj: %d, rev_min: %d\n", ntohs(bd_ptr->board_type), board_rev_maj, board_rev_min);
        }
        else
        {
            printf("Warning: No board descriptor tuple found in eeprom at address 0x%x\n", board_entry->eeprom_addr);
        }
    }

    if (!load_cache_flag)
    {
        /* Look up the (read only_ ddr configuration.  In some cases (generic
         * board for sure) we want to modify what is returned, so we make a
         * local copy of it.
         */
        const ddr_configuration_t *__ddr_config_ptr;
        __ddr_config_ptr = lookup_ddr_config_structure(chip_id, board_type,
                                                       board_rev_maj, board_rev_min, &ddr_interface_mask);
        if (!__ddr_config_ptr)
        {
            printf("ERROR: unable to determine DDR configuration for board type: %s (%d)\n",
                   cvmx_board_type_to_string(board_type), board_type);
            return(-1);
        }
        /* Make a local copy because we want to be able to modify it */
        memcpy(ddr_configuration, __ddr_config_ptr, sizeof(ddr_configuration));


        /* Some (should be a small minority) of boards need special handling on
         * a per-board basis
         */
        switch (board_type)
        {
            case CVMX_BOARD_TYPE_GENERIC:
                /* Generic board, so we don't do much here.  All settings need
                 * to come from the command line
                 */
                board_rev_maj = 0;   /* Set default board rev in case of no eeprom present */
                board_rev_min = 0;
                if (!ddr0spd_index && !ddr1spd_index)
                {
                    printf("ERROR: Must specify DIMM SPD addresses for generic board type.\n");
                    exit(-1);
                }
                if (!board_delay)
                {
                    board_delay = 4500;
                    printf("NOTICE: using default %d board delay.\n", board_delay);
                }
                ddr_configuration[0].registered.ddr_board_delay = board_delay;
                ddr_configuration[0].unbuffered.ddr_board_delay = board_delay;
                ddr_configuration[1].registered.ddr_board_delay = board_delay;
                ddr_configuration[1].unbuffered.ddr_board_delay = board_delay;
                ddr_configuration[2].registered.ddr_board_delay = board_delay;
                ddr_configuration[2].unbuffered.ddr_board_delay = board_delay;
                ddr_configuration[3].registered.ddr_board_delay = board_delay;
                ddr_configuration[3].unbuffered.ddr_board_delay = board_delay;

                memcpy(ddr_configuration[0].dimm_config_table, dimm_table[0],
                       sizeof(ddr_configuration[0].dimm_config_table));
                memcpy(ddr_configuration[1].dimm_config_table, dimm_table[1],
                       sizeof(ddr_configuration[1].dimm_config_table));
                memcpy(ddr_configuration[2].dimm_config_table, dimm_table[2],
                       sizeof(ddr_configuration[2].dimm_config_table));
                memcpy(ddr_configuration[3].dimm_config_table, dimm_table[3],
                       sizeof(ddr_configuration[3].dimm_config_table));

                /* Set ddr_interface_mask based on DIMM SPDs provided on command line. */
                if (!ddr_int_valid_mask)
                {
                    printf("ERROR: No DIMM SPDs specified for generic board type.\n");
                    exit(-1);
                }
                ddr_interface_mask = ddr_int_valid_mask;
                break;
        }

        if ((dimm_table[0][0].spd_addrs[0] || dimm_table[0][0].spd_ptrs[0]
            || dimm_table[1][0].spd_addrs[0] || dimm_table[1][0].spd_ptrs[0]
            || dimm_table[2][0].spd_addrs[0] || dimm_table[2][0].spd_ptrs[0]
            || dimm_table[3][0].spd_addrs[0] || dimm_table[3][0].spd_ptrs[0])
            && board_type != CVMX_BOARD_TYPE_GENERIC)
        {
            /* The u-boot image that boots for a specific board will look up the
             * SPD configuration, and use that to determine available memory.
             * If that configuration mismatches, then bad things happen.  This
             * restriction can be removed when u-boot is more flexible in this
             * regard.
             */
            printf("WARNING: Supplying SPD addresses/contents for non-generic board types not supported, ignoring SPD arguments.\n");
        }

        if (read_level_flag)
        {
            if (OCTEON_IS_MODEL(OCTEON_CN52XX) || OCTEON_IS_MODEL(OCTEON_CN56XX))
            {
                printf("Using read leveling for automatic board delay.\n");
                /* Board delay of 0 triggers use of read leveling */
                board_delay = 0;
                ddr_configuration[0].registered.ddr_board_delay = board_delay;
                ddr_configuration[0].unbuffered.ddr_board_delay = board_delay;
                ddr_configuration[1].registered.ddr_board_delay = board_delay;
                ddr_configuration[1].unbuffered.ddr_board_delay = board_delay;
            }
            else
            {
                printf("ERROR: read leveling not supported on this chip.\n");
                exit(-1);
            }
        }


        /* Read the contents of the onboard EEPROM to determine board characteristics */
        if (board_entry->eeprom_addr > 0)
            addr = octeon_tlv_get_tuple_addr(board_entry->eeprom_addr,
                                             EEPROM_CLOCK_DESC_TYPE, 0,
					     ee_buf, OCTEON_EEPROM_MAX_TUPLE_LENGTH);
        if (addr >= 0)
        {
            int cpu_ref_mhz_x_8 = ntohs(cd_ptr->cpu_ref_clock_mhz_x_8);

            cpu_ref_hz = (cpu_ref_mhz_x_8 * 1000000)/8;
            if (!ddr_clock_hz)
                ddr_clock_hz = ntohs(cd_ptr->ddr_clock_mhz) * 1000000;
            debug("Read clock descriptor: ddr_hz: %d, cpu_ref_mhz_x_8: %d (cpu_ref_hz: %d)\n",
                  ntohs(cd_ptr->ddr_clock_mhz) * 1000000, cpu_ref_mhz_x_8, cpu_ref_hz);
        }


        /* Read DDR specific configuration.  If this and the older CLOCK_DESC are
         * both present, the DDR_CLOCK_DESC supercedes the other values.
         */
        if (board_entry->eeprom_addr > 0)
            addr = octeon_tlv_get_tuple_addr(board_entry->eeprom_addr,
                                             EEPROM_DDR_CLOCK_DESC_TYPE, 0, ee_buf,
					     OCTEON_EEPROM_MAX_TUPLE_LENGTH);
        if (addr >= 0)
        {
            if (!ddr_clock_hz)
                ddr_clock_hz = ntohl(dcd_ptr->ddr_clock_hz);
            if (!ddr_ref_hz)
                ddr_ref_hz = ntohl(dcd_ptr->ddr_ref_hz);
            debug("Read ddr clock descriptor: ddr_clock_hz: %d, ddr_ref_hz: %d\n",
                  ntohl(dcd_ptr->ddr_clock_hz), ntohl(dcd_ptr->ddr_ref_hz));
        }

    }

    if (strlen(bootloader_filename) == 0 && !memonly_flag)
    {
        strncpy(bootloader_filename, oct_root, NAME_LEN);
        strncat(bootloader_filename, "/target/bin/", NAME_LEN - strlen(oct_root));
        if (strlen(board_entry->bootloader_name))
        {
            /* Name in structure present, so use */
            strncat(bootloader_filename, board_entry->bootloader_name,
                    NAME_LEN - strlen(bootloader_filename));
        }
        else
        {
            unsigned int i;
            /* Use default name based on board type */
            strncat(bootloader_filename, "u-boot-octeon_", NAME_LEN - strlen(bootloader_filename));
            strncat(bootloader_filename, cvmx_board_type_to_string(board_type),
                    NAME_LEN - strlen(bootloader_filename));
            /* Convert the board_type to lower case before appending to bootloader filename. */
            for (i = strlen(bootloader_filename) -
                     strlen( cvmx_board_type_to_string(board_type)) ;
                 i < strlen(bootloader_filename); i++)
                bootloader_filename[i] = tolower((unsigned char)bootloader_filename[i]);
            strncat(bootloader_filename, ".bin", NAME_LEN - strlen(bootloader_filename));
        }
    }
    if (memonly_flag)
        printf("Configuring memory controller only, not loading bootloader\n");
    else
        printf("Using bootloader image: %s\n", bootloader_filename);


    if (!load_cache_flag)
    {
        /* The board defaults will only be used if there is a non-standard (and fixed)
        ** default frequency.  These will likely be rarely used. */
        if (!ddr_clock_hz)/* May have been specified on command line */
        {
            ddr_clock_hz = board_entry->default_ddr_clock_hz;
            printf("Notice: Using board default DDR clock of: %d hertz.\n", ddr_clock_hz);
        }
        if (!ddr_ref_hz)/* May have been specified on command line */
        {
            /* CN52XX/CN56XX and CN6XXX all have fixed 50 MHz DDR reference clocks,
             * so this is the normal case.
             */
            if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN58XX) ||
                OCTEON_IS_MODEL(OCTEON_CN50XX))
                printf("Notice: Using board default DDR reference clock of %d hertz.\n",
                       ddr_ref_hz);
            ddr_ref_hz = board_entry->default_ddr_ref_hz;
        }

        /* Set some defaults for frequencies based on Octeon model.
        ** Some models don't have a good default, so those will need to be set per-board.*/
        if (OCTEON_IS_MODEL(OCTEON_CN3XXX))
        {
            if (!cpu_ref_hz)
                cpu_ref_hz = 50000000;
            if (!ddr_ref_hz)
                ddr_ref_hz = 1;  /* For 3XXX this isn't used, but needs to be non-zero */
        }
        else
        {
            /* These frequencies are required by the 5XXX chips. */
            if (!cpu_ref_hz)
                cpu_ref_hz = 50000000;
            if (!ddr_ref_hz)
            {
                switch (board_type)
                {
                    /* The ebt5800 and derivative boards use a atypical DDR clocking
                    ** scheme.  The reference is controlled by the MCU based on the
                    ** eeprom values, as early in their development the DDR PLL was
                    ** not used.  This variable reference now remains, even though
                    ** the PLL is used to generate the final DDR clock */
                    case CVMX_BOARD_TYPE_EBT5800:
                    case CVMX_BOARD_TYPE_EBT5810:
                        ddr_ref_hz = ddr_clock_hz/4;
                        break;
                    default:
                        ddr_ref_hz = 50000000;
                        break;

                }
            }
        }


        if (!ddr_clock_hz)
        {
            if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
                ddr_clock_hz  = 200000000;  /* DDR2 default */
            else
                ddr_clock_hz  = 533000000;  /* DDR# default */
            printf("Warning: Using generic default DDR clock of %d hertz.\n", ddr_clock_hz);
        }

        cpu_clock_hz = cvmx_clock_get_rate(CVMX_CLOCK_CORE);

        /* If the ddr reference clock is not explicitely set, and a fixed default
        ** is not valid, we determine the ddr reference clock frequency here.
        ** Only boards that have a _variable_ ddr reference clock will need handling here.
        */
        if (!ddr_ref_hz)
        {
            switch (board_type)
            {
                /* The EBT5800 is a special case.  The MCU on board sets the
                ** reference to 1/4 of the desired frequency, which is the same
                ** thing that was done on the ebt3000. */
                case CVMX_BOARD_TYPE_EBT5800:
                case CVMX_BOARD_TYPE_EBT5810:
                    ddr_ref_hz = ddr_clock_hz/4;
                    break;
            }

        }
        if (!ddr_ref_hz)/* May have been specified on command line */
            ddr_ref_hz = board_entry->default_ddr_ref_hz;

        if (!ddr_ref_hz)
        {
            printf("DDR reference clock not set (and no default) for board type %s (%d), exiting.\n",
                   cvmx_board_type_to_string(board_type), board_type);
            goto cleanup;
        }
        if (!cpu_ref_hz)
        {
            printf("CPU reference clock not set (and no default) for board type %s (%d), exiting.\n",
                   cvmx_board_type_to_string(board_type), board_type);
            goto cleanup;
        }

        if (!cpu_clock_hz)
        {
            printf("ERROR determining cpu(%d Hz) clock, exiting\n", cpu_clock_hz);
            goto cleanup;
        }

        dram_size_mbytes = octeon_ddr_initialize(chip_id,
                                                 cpu_clock_hz,
                                                 ddr_clock_hz,
                                                 ddr_ref_hz,
                                                 ddr_interface_mask,
                                                 ddr_configuration,
                                                 &measured_ddr_hertz,
                                                 board_type,
                                                 board_rev_maj,
                                                 board_rev_min,
                                                 0
                                                 );

        if (!measured_ddr_hertz)
        {
            printf("ERROR determining ddr(%d Hz) clock, exiting\n", measured_ddr_hertz);
            goto cleanup;
        }

        debug("Measured DDR clock %3.3f MHz\n", (double) measured_ddr_hertz / 1000000);

        if (dram_size_mbytes < 0)
        {
            printf("ERROR: Unable to configure DRAM controller, exiting\n");
            goto cleanup;
        }

        printf("Initialized %d MBytes of DRAM\n", dram_size_mbytes);

        if (getenv("ddr_memtest") != NULL)
            memtest(4 * 1024 * 1024);

        if (memonly_flag)
        {
            printf("Configured Octeon for %d Mbytes of DRAM.\n", dram_size_mbytes);
            retval = 0;
            goto cleanup;
        }

        /* Copy the u-boot binary to DRAM */
        if (0 > copy_file_to_octeon_dram(U_BOOT_PCI_LOAD_ADDRESS, bootloader_filename))
        {
            printf("ERROR copying file %s to Octeon DRAM\n", bootloader_filename);
            goto cleanup;
        }
    }
    else
    {
        if (load_l2(U_BOOT_PCI_LOAD_ADDRESS, bootloader_filename) < 0)
        {
            printf("ERROR copying file %s to Octeon L2 cache\n", bootloader_filename);
            goto cleanup;
        }
    }
    /* Load the environment for the bootloader into DRAM.  At a minimum, it
     * will have the dram size in it, and possibly more from the user supplied
     * file.
     */

    /* Add dram size to environment to load, unless it is already specified in the file */
    if (!load_cache_flag)
    {
        if (!strstr(env_buf + U_BOOT_RAM_ENV_CRC_SIZE, "dram_size_mbytes"))
        {
            /* dram_size_mbytes is not present in file, so add it */
            char *ptr = env_buf + U_BOOT_RAM_ENV_CRC_SIZE +
                        strlen(env_buf + U_BOOT_RAM_ENV_CRC_SIZE);
            sprintf(ptr, "dram_size_mbytes=%d\n", dram_size_mbytes);
            printf("Setting dram_size in env\n");
        }
    }

    if (board_type_str && !strstr(env_buf + U_BOOT_RAM_ENV_CRC_SIZE, "board_type"))
    {
        char *ptr = env_buf + U_BOOT_RAM_ENV_CRC_SIZE + strlen(env_buf + U_BOOT_RAM_ENV_CRC_SIZE);
        snprintf(ptr,U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE - strlen(env_buf + U_BOOT_RAM_ENV_CRC_SIZE) - 1,
                 "board_type=%.100s\n", board_type_str);
        ptr[U_BOOT_RAM_ENV_SIZE - 1] = 0;
        printf("Passing board type %s in env\n", board_type_str);
	free(board_type_str);
    }
    /* Now convert \n's to nulls */
    for (i = U_BOOT_RAM_ENV_CRC_SIZE; i < U_BOOT_RAM_ENV_SIZE - 1; i++)
    {
        if (env_buf[i] == '\n')
            env_buf[i] = '\0';
    }

    /* Compute crc */
    crc = crc32(0, (uint8_t *)env_buf + U_BOOT_RAM_ENV_CRC_SIZE, U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE);
    crc = htonl(crc);
    memcpy(env_buf, &crc, sizeof(crc));
    if (!load_cache_flag)
    {
        octeon_remote_write_mem(U_BOOT_RAM_ENV_ADDR, env_buf, sizeof(env_buf));
        printf("Wrote environment to RAM address 0x%llx\n",
               (unsigned long long)U_BOOT_RAM_ENV_ADDR);
    }
    else
    {
        octeon_remote_write_mem(U_BOOT_CACHE_ENV_ADDR, env_buf, sizeof(env_buf));
        printf("Wrote environment to L2 cache address 0x%llx\n",
               (unsigned long long)U_BOOT_CACHE_ENV_ADDR);
    }

    /* Program on board boot bus segment to be located at reset vector and
     * contain code to jump to u-boot entry point in DRAM.
     * Set up MMIO_BOOT_* to decode boot vector
     */
   uint64_t reg_val = (1ull << 31) | (0x1fc00000 >> 4);
   octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_CFGX(0), reg_val);

   debug("Writing boot stub into local memory block\n");
   octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0x0);  /* Auto increments after write */


/*
 000004f0 <llm_workaround_end>:
  4f0:   40047801        mfc0    a0,c0_ebase
  4f4:   308400ff        andi    a0,a0,0xff
  4f8:   3c098040        lui     t1,0x8100
  4fc:   14800003        bnez    a0,50c <later_cores_ram_boot>
  500:   00000000        nop
  504:   01200008        jr      t1
  508:   00000000        nop

 0000050c <later_cores_ram_boot>:
  50c:   24050008        li      a1,8
  510:   00052880        sll     a1,a1,0x2
  514:   70852002        mul     a0,a0,a1
  518:   3c058000        lui     a1,0x8000
  51c:   34a50800        ori     a1,a1,0x800
  520:   00a42820        add     a1,a1,a0
  524:   8cbd0000        lw      sp,0(a1)
  528:   8ca40004        lw      a0,4(a1)
  52c:   00800008        jr      a0
  530:   00000000        nop
 */

    /* Most sig. word executes first */
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x40047801ull << 32) | 0x308400ffull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x3c098100ull << 32) | 0x14800003ull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x00000000ull << 32) | 0x01200008ull);

    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x00000000ull << 32) | 0x24050008ull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x00052880ull << 32) | 0x70852002ull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x3c058000ull << 32) | 0x34a50800ull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x00a42820ull << 32) | 0x8cbd0000ull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x8ca40004ull << 32) | 0x00800008ull);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x00000000ull << 32) | 0x00000000ull);

    debug("Done writing boot stub into local memory block\n");


    /* Make sure read buffer owner is cleared. */
    octeon_remote_write_mem32(BOOTLOADER_PCI_READ_BUFFER_OWNER_ADDR, 0x0);

    /* Take core 0 out of reset */
    pp_rst.u64 = octeon_remote_read_csr(CVMX_CIU_PP_RST);
    if (pp_rst.s.rst0)
    {
        debug("Taking core 0 out of reset!\n");
    }
    else
    {
        debug("Starting core 0! (Core 0 already running.)\n");
    }
    octeon_remote_start_cores(1);

    sleep(2);
    if ((octeon_remote_read_csr(CVMX_CIU_PP_RST)&1) && (retry_count < 2))
    {
        printf("Warning: Octeon performed a soft reset, trying again!\n");
        retry_count++;
        /* Tell the DDR code to init the clock again. I don't know why this
            variable is global */
        extern int global_ddr_clock_initialized;
        global_ddr_clock_initialized = 0;
        goto retry;
    }
    /* Wait for boot to complete. Read first word of the bootloader read buffer, when
    ** this is set to be owned by the host, then the boot is complete. */
    if (octeon_remote_wait_for_bootloader(600) != 0)
    {

        printf("Timeout waiting for boot completion!\n");
        goto cleanup;
    }

    retval = 0;
cleanup:
    octeon_remote_unlock();
    octeon_remote_close();
    return retval;
}

