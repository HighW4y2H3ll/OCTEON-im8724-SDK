/***********************license start************************************
 * Copyright (c) 2003-2009 Cavium Inc. (support@cavium.com). All rights
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
 * Utility to display CSR of a remote Octeon
 *
 * $Id: oct-remote-csr.c 135351 2016-03-21 19:11:03Z cchavva $
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "cvmx-csr-db.h"
#include "cvmx-swap.h"
#undef CVMX_ADD_IO_SEG
#define CVMX_ADD_IO_SEG(a) (a)	/* There aren't any Mips IO segments when using PCI */
#include "cvmx-srio.h"

#define CVMX_SRIO_CONFIG_TIMEOUT        10000	/* 10ms */

int node_num = -1;

static uint64_t convert_number(const char *str) {
	unsigned long long result;
	if ((str[0] == '0') && (str[1] == 'x')) {
		if (sscanf(str + 2, "%llx", &result) != 1)
			result = 0;
	} else {
		if (sscanf(str, "%lli", &result) != 1)
			result = 0;
	}
	return result;
}

void usage(char * const *argv) {
	printf("\n" "Usage: %s CSR [value] [CSR [value] ...]\n"
	"    Read or write a CSR by name.\n\n"
	"    CSR     Name of CSR to read or write. Case is ignored.\n"
	"    value   If present, write this value to the CSR. Otherwise do a read.\n"
	"            The value can be in decimal of hex (0x...).\n\n"
	"    If CSR is a partial name, the list of CSRs starting with it are displayed.\n\n",
	argv[0]);
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"node", 1, 0, 'n'},
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "n:hv", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'n':
                node_num = convert_number(optarg);
                if (node_num > CVMX_MAX_NODES)
                {
                    printf("Illegal node number. Must be 0..3\n");
                    return -1;
                }
                break;
            default:
                usage(argv);
	    /* fallthrough */
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
                return -1;
        }
    }

    if (optind > argc)
    {
        usage(argv);
        return -1;
    }
    return 0;
}
static int check_csr_type(const char *csr_name, int csr_type, uint64_t csr_addr) {

	/* We don't support CSRs that aren't in IO space (bit 48 set) */
	if (((csr_addr & (1ull << 48)) == 0) &&
		(csr_type != CVMX_CSR_DB_TYPE_PCICONFIGEP) &&
		(csr_type != CVMX_CSR_DB_TYPE_PCICONFIGRC) &&
		(csr_type != CVMX_CSR_DB_TYPE_SRIOMAINT)) {
		printf("CSR %s: This utility can't access CSRs of this type\n", csr_name);
		return -1;
	}

	return 0;
}

/**
 * Main
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char * const *argv) {
	int index;
	const char *csr_name;
	uint64_t csr_addr;
	int csr_type, csr_widthbits, ret;
	uint32_t model;
	char t_name[30];
	int node = -1;

	/* Make sure we got the correct number of arguments */
	if (parse_options(argc, argv))
		return -1;

	/* Open a connection */
	if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
		return -1;

	model = octeon_remote_get_model();

	index = 1;
	while (index < argc) {

                if (argv[index][0] == '-')
                {
                    index += 2;
                    continue;
                }
		csr_name = argv[index];

		sscanf(csr_name, "%d:%s", &node, t_name);
		if (node != -1 && t_name[0])
			strcpy((char *)csr_name, t_name);

		ret = cvmx_csr_db_get_params(model, (char *) csr_name, &csr_addr, &csr_type, NULL, &csr_widthbits);

		if (node_num != -1)
                        node = node_num;
                else if (node == -1)
			node = 0;

		if (node != -1)
			/* Add node to csr_name */
			csr_addr |= ((uint64_t)node << 36);

		/* Determine if we're doing a read or a write */
		if ((index + 1 < argc) && (argv[index + 1][0] >= '0') && (argv[index + 1][0] <= '9')) {
			/* Write */
			uint64_t value = convert_number(argv[index + 1]);

			if (ret != 0) {
				fprintf(stderr, "Could not find exact match for csr name: %s, skipping...\n", csr_name);
				index += 2;
				continue;
			}

			ret = check_csr_type(csr_name, csr_type, csr_addr);
			if (ret != 0) {
				index += 2;
				continue;
			}

			if ((csr_type == CVMX_CSR_DB_TYPE_PCICONFIGEP) || (csr_type == CVMX_CSR_DB_TYPE_PCICONFIGRC)) {
				/* Names are of the format "PCIE??#_CFG???". The # is the pcie port number */
				int pcie_port = (csr_name[6] - '0');
				if (octeon_has_feature(OCTEON_FEATURE_NPEI)) {
					cvmx_pescx_cfg_wr_t pescx_cfg_wr;
					pescx_cfg_wr.u64 = 0;
					pescx_cfg_wr.s.addr = csr_addr;
					pescx_cfg_wr.s.data = value;
					octeon_remote_write_csr(CVMX_PESCX_CFG_WR(pcie_port), pescx_cfg_wr.u64);
				} else {
					cvmx_pemx_cfg_wr_t pemx_cfg_wr;
					pemx_cfg_wr.u64 = 0;
					pemx_cfg_wr.s.addr = csr_addr;
					pemx_cfg_wr.s.data = value;
					octeon_remote_write_csr(CVMX_PEMX_CFG_WR(pcie_port), pemx_cfg_wr.u64);
				}
			} else if (csr_type == CVMX_CSR_DB_TYPE_SRIOMAINT && octeon_has_feature(OCTEON_FEATURE_SRIO)) {
				/* Names are of the format "SRIOMAINT#_*". The # is the srio port number */
				int srio_port = (csr_name[9] - '0');
				assert (srio_port <= 9);
				if (cvmx_srio_config_write32(srio_port, 0, -1, 0, 0, csr_addr, value))
					fprintf(stderr, "SRIO access failed\n");
			} else if (csr_widthbits == 32) {
                                /* Do endian swapping for USB/SATA CSRs */
				if (csr_type == CVMX_CSR_DB_TYPE_NCB
                                    && !octeon_remote_is_le()
				    && octeon_remote_protocol())
				    octeon_remote_write_csr32(csr_addr, cvmx_swap32(value));
                                else
				    octeon_remote_write_csr32(csr_addr, value);
                        }
			else
				octeon_remote_write_csr(csr_addr, value);
			index += 2;
		} else {
			/* Read */
			uint64_t value;

			if (ret != 0) {
				/* This lists all csr names by partial or full basename */
				fprintf(stderr, "CSR not found in list. Looking for prefix \"%s\"\n", csr_name);
				cvmx_csr_db_print_decode_by_prefix_node(node, model, csr_name, 0);
				index++;
				ret = 0;
				continue;
			}

			ret = check_csr_type(csr_name, csr_type, csr_addr);
			if (ret != 0) {
				index++;
				continue;
			}

			if ((csr_type == CVMX_CSR_DB_TYPE_PCICONFIGEP) || (csr_type == CVMX_CSR_DB_TYPE_PCICONFIGRC)) {
				/* Names are of the format "PCIE??#_CFG???". The # is the pcie port number */
				int pcie_port = (csr_name[6] - '0');
				if (octeon_has_feature(OCTEON_FEATURE_NPEI)) {
					cvmx_pescx_cfg_rd_t pescx_cfg_rd;
					pescx_cfg_rd.u64 = 0;
					pescx_cfg_rd.s.addr = csr_addr;
					octeon_remote_write_csr(CVMX_PESCX_CFG_RD(pcie_port), pescx_cfg_rd.u64);
					pescx_cfg_rd.u64 = octeon_remote_read_csr(CVMX_PESCX_CFG_RD(pcie_port));
					value = 0xffffffffull & pescx_cfg_rd.s.data;
				} else {
					cvmx_pemx_cfg_rd_t pemx_cfg_rd;
					pemx_cfg_rd.u64 = 0;
					pemx_cfg_rd.s.addr = csr_addr;
					octeon_remote_write_csr(CVMX_PEMX_CFG_RD(pcie_port), pemx_cfg_rd.u64);
					pemx_cfg_rd.u64 = octeon_remote_read_csr(CVMX_PEMX_CFG_RD(pcie_port));
					value = 0xffffffffull & pemx_cfg_rd.s.data;
				}
			} else if (csr_type == CVMX_CSR_DB_TYPE_SRIOMAINT && octeon_has_feature(OCTEON_FEATURE_SRIO)) {
				/* Names are of the format "SRIOMAINT#_*". The # is the srio port number */
				int srio_port = (csr_name[9] - '0');
				uint32_t result;
				if (cvmx_srio_config_read32(srio_port, 0, -1, 0, 0, csr_addr, &result)) {
					fprintf(stderr, "SRIO access failed\n");
					result = 0;
				}
				value = result;
			} else if (csr_widthbits == 32) {
                                /* Do endian swapping for USB/SATA CSRs */
				if (csr_type == CVMX_CSR_DB_TYPE_NCB
                                    && !octeon_remote_is_le()
				    && octeon_remote_protocol()) {
				        value = octeon_remote_read_csr32(csr_addr);
                                        value = cvmx_swap32(value);
                                } else {
				    value = octeon_remote_read_csr32(csr_addr);
                                }
                        } else
				value = octeon_remote_read_csr(csr_addr);
			cvmx_csr_db_decode_by_name_node(node, model, (char *) csr_name, value);
			index++;
		}
	}

	octeon_remote_close();
	return ret;
}
