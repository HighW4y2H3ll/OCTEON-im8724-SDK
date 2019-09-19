/***********************license start************************************
 * Copyright (c) 2015 Cavium Inc. (support@cavium.com). All rights
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
 * Utility to read or write memory over a remote interface
 *
 * $Id: oct-remote-eeprom.c 108152 2015-08-11 01:48:24Z cchavva $
 *
 *
 */
#include <stdint.h>
#include <stdio.h>
#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "cvmx-pcie.h"
#include "cvmx-pemx-defs.h"

/**
 * EEPROM entry struct
 */
typedef union octeon_remote_pcie_eeprom
{
	uint64_t u64;
#ifdef __BIG_ENDIAN_BITFIELD	/* A Linux compatible proxy for __BIG_ENDIAN */
	struct octeon_data_s {
		uint64_t preamble:16;  /* 0x9DA1 valid entry, 0x6A5D end of table
					  0xffff invalid access */
		uint64_t rsvd1:1;
		uint64_t pf:2;         /* specifies the physical function number
					  accessed by the write operation. */
		uint64_t cs2:1;        /* specifies bit<31> of the address written by
					  hardware. 1 = configuration mask register
					  0 = configuration register */
		uint64_t address:12;   /* specifies bits<11:0> of the address written
					  by hardware. bits<30:12> of this address are
					  all 0s. */
		uint64_t data:32;
	} s;
#else
	struct octeon_data_s {
		uint64_t data:32;
		uint64_t address:12;
		uint64_t cs2:1;
		uint64_t pf:2;
		uint64_t rsvd1:1;
		uint64_t preamble:16;
	} s;
#endif
} octeon_remote_pcie_eeprom_t;

static uint64_t value        = 0;
static uint64_t offset       = 0;
static int write_eeprom = 0;
static int read_eeprom = 0;

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

void usage(char * const *argv)
{
    printf("\n"
           "Usage: %s [--read] [--write value] offset\n"
           "    Read or write bytes PCIe EEPROM at a given offset.\n"
           "\n"
           "    read    Read bytes from a given offset.\n"
           "    write   Write value to a given offset.\n"
           "    value   If present, write this value to the address. Otherwise do a read.\n"
           "    offset  Offset to read/write PCIe EEPROM.\n"
           "\n"
           "\n", argv[0]);
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"read", 1, 0, 'r'},
        {"write", 2, 0, 'w'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "w:r:v", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'w':
		write_eeprom = 1;
                if (optarg) {
                    value = convert_number(optarg);
                    offset = convert_number(argv[optind]);
                } else {
                    value = convert_number(argv[optind]);
                    offset = convert_number(argv[optind+1]);
                }
                break;
            case 'r':
                read_eeprom = 1;
                if (optarg)
                    offset = convert_number(optarg);
                else
                    offset = convert_number(argv[optind]);
                break;
            default:
                usage(argv);
	    /* fallthrough */
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
                return -1;
        }
    }

    if (optind == 1) {
        usage(argv);
        return -1;
    }
    return 0;
}

/* Read data from PCIe EEPROM
 *
 * @param pcie_port    PCIe port
 * @param offset       offset to read the data from
 *
 * @return data.
 */
octeon_remote_pcie_eeprom_t __octeon_remote_pcie_eeprom_read(int pcie_port, int offset)
{
	cvmx_pemx_spi_ctl_t spi_ctl;
	octeon_remote_pcie_eeprom_t data;

	data.u64 = 0;

	if (!OCTEON_IS_OCTEON3())
            return data;

	if (CVMX_WAIT_FOR_FIELD64(CVMX_PEMX_SPI_CTL(pcie_port),
					  cvmx_pemx_spi_ctl_t, start_busy, ==,
					  0, 10000)) {
		cvmx_dprintf("PCIe EEPROM read: PEM%d_SPI_CTL[start_busy] is stuck on\n",
				     pcie_port);
		return data;
	}

	/* Issue the read */
	spi_ctl.u64 = 0;
	spi_ctl.s.start_busy = 1;
	spi_ctl.s.cmd = 0x3;	 /* Read */
	spi_ctl.s.adr = offset;
	cvmx_write_csr(CVMX_PEMX_SPI_CTL(pcie_port), spi_ctl.u64);

	/* Wait for complete, up to 100ms */
	if (CVMX_WAIT_FOR_FIELD64(CVMX_PEMX_SPI_CTL(pcie_port),
					  cvmx_pemx_spi_ctl_t, start_busy, ==,
					  0, 100000)) {
		cvmx_dprintf("PCIe EEPROM read: timeout\n");
		return data;
	}

	data.u64 = cvmx_read_csr(CVMX_PEMX_SPI_DATA(pcie_port));

	return data;
}


/* Write data in PCIe EEPROM
 *
 * @param pcie_port    PCIe port
 * @param offset       offset to read the data from
 *
 */
uint64_t __octeon_remote_pcie_eeprom_write(int pcie_port, int offset, uint64_t value)
{
	cvmx_pemx_spi_ctl_t spi_ctl;

	if (!OCTEON_IS_OCTEON3())
            return -1;

	/* Make sure the interface is idle before we start */
	if (CVMX_WAIT_FOR_FIELD64(CVMX_PEMX_SPI_CTL(pcie_port),
					  cvmx_pemx_spi_ctl_t, start_busy, ==,
					  0, 10000)) {
		cvmx_dprintf("PCIe EEPROM write: PEM%d_SPI_CTL[start_busy] is stuck on\n",
				     pcie_port);
		return -1;
	}

	/* Enable writes to the EEPROM */
	spi_ctl.u64 = 0;
	spi_ctl.s.start_busy = 1;
	spi_ctl.s.cmd = 0x6;	 /* Write enable */
	cvmx_write_csr(CVMX_PEMX_SPI_CTL(pcie_port), spi_ctl.u64);

	/* Wait for complete, up to 10ms */
	if (CVMX_WAIT_FOR_FIELD64(CVMX_PEMX_SPI_CTL(pcie_port),
					  cvmx_pemx_spi_ctl_t, start_busy, ==,
					  0, 10000)) {
		cvmx_dprintf("PCIe EEPROM write: timeout on write enable\n");
		return -1;
	}

	/* Setup the data */
	cvmx_write_csr(CVMX_PEMX_SPI_DATA(pcie_port), value);
	cvmx_read_csr(CVMX_PEMX_SPI_DATA(pcie_port));

	/* Perform the write */
	spi_ctl.u64 = 0;
	spi_ctl.s.start_busy = 1;
	spi_ctl.s.cmd = 0x2;	 /* Write */
	spi_ctl.s.adr = offset;
	cvmx_write_csr(CVMX_PEMX_SPI_CTL(pcie_port), spi_ctl.u64);

	/* Wait for complete, up to 100ms */
	if (CVMX_WAIT_FOR_FIELD64(CVMX_PEMX_SPI_CTL(pcie_port),
					  cvmx_pemx_spi_ctl_t, start_busy, ==,
					  0, 10000)) {
		cvmx_dprintf("PCIe EEPROM write: timeout on write\n");
		return -1;
	}

	/* Disable writes to the EEPROM */
	spi_ctl.u64 = 0;
	spi_ctl.s.start_busy = 1;
	spi_ctl.s.cmd = 0x4;	 /* Write disable */
	cvmx_write_csr(CVMX_PEMX_SPI_CTL(pcie_port), spi_ctl.u64);

	/* Wait for complete, up to 10ms */
	if (CVMX_WAIT_FOR_FIELD64(CVMX_PEMX_SPI_CTL(pcie_port),
					  cvmx_pemx_spi_ctl_t, start_busy, ==,
					  0, 10000)) {
		cvmx_dprintf("PCIe EEPROM write: timeout on write disable\n");
		return -1;
	}
	return 0;
}

/*
 * PCIe EEPROM read
 * offset : read data from offset
 */
uint64_t octeon_remote_pcie_eeprom_read(int octeon_pcie_port, int offset)
{
    octeon_remote_pcie_eeprom_t data;
    data = __octeon_remote_pcie_eeprom_read(octeon_pcie_port, 0);
    if (data.s.preamble == 0xffff) {
        printf("WARNING: PCIe EEPROM is not available for reading\n");
        return -1;
    }
    data = __octeon_remote_pcie_eeprom_read(octeon_pcie_port, offset);
    return data.u64;
}

/*
 * PCIe EEPROM write
 * offset : write data at offset
 * value :  value to write
 */
uint64_t octeon_remote_pcie_eeprom_write(int octeon_pcie_port, int offset, uint64_t value)
{
    octeon_remote_pcie_eeprom_t data;
    data = __octeon_remote_pcie_eeprom_read(octeon_pcie_port, 0);
    if (data.s.preamble == 0xffff) {
        printf("WARNING: PCIe EEPROM is not available for writing\n");
        return -1;
    }
    return __octeon_remote_pcie_eeprom_write(octeon_pcie_port, offset, value);
}

/**
 * Main
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char * const *argv)
{
    int pcie_port = 0;
    const char *pcie_qlm_str;

    if (parse_options(argc, argv))
        return -1;

    /* Open a connection */
    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if ((pcie_qlm_str = getenv("OCTEON_PCIE_QLM")) != NULL)
        pcie_port = atoi(pcie_qlm_str);

    /* Determine if we're doing a read or a write */
    if (write_eeprom)
    {
        octeon_remote_pcie_eeprom_write(pcie_port, offset, value);
    }
    else
    {
        octeon_remote_pcie_eeprom_t data;
        data.u64 = octeon_remote_pcie_eeprom_read(pcie_port, offset);
        if (data.u64 != 1) {
            printf("%ld: Preamble = 0x%x, PF = %d, CS2 = %d, Address = 0x%x, Data = 0x%llx\n",
                (long int)offset, data.s.preamble, data.s.pf,
                data.s.cs2, data.s.address,
                (long long unsigned int)data.s.data);
        }
    }
    octeon_remote_close();
    return 0;
}
