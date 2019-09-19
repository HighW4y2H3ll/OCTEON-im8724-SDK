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
 * Interface to Octeon remotely using PCI/PCIe
 *
 * $Id: octeon-remote-pci.c 163638 2017-07-26 02:24:50Z cchavva $
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "octeon-remote.h"
#include "octeon-remote-map.h"
#include "octeon-remote-debug-handler.h"
#define CVMX_ADD_IO_SEG(a) (a)
#include "cvmx.h"
#include "cvmx-swap.h"
#include "cvmx-core.h"
#include "cvmx-pexp-defs.h"


/* This is only supported for GCC 4.7 and above.  */
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)

/* Include stdatomic if we are using C11 toolchain.  */
#if defined(__GNUC__) && !defined(__HAS_ATOMIC)
# if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#  include <stdatomic.h>
#  define __HAS_ATOMIC
# endif
#endif

#ifndef __HAS_ATOMIC
/* Used for readability from C11, define our own versions.
 * Documentation on the atomic functions built into GCC 4.7 found here:
 * http://gcc.gnu.org/onlinedocs/gcc-4.7.0/gcc/_005f_005fatomic-Builtins.html
 */
#undef _Atomic
#define	_Atomic
#define atomic_fetch_sub_explicit(PTR, VAL, AMM) \
	(__atomic_fetch_sub((PTR), (VAL), (AMM)))
#define atomic_fetch_add_explicit(PTR, VAL, AMM) \
	(__atomic_fetch_add((PTR), (VAL), (AMM)))
#define atomic_store_explicit(PTR, VAL, AMM) \
	__atomic_store_n((PTR), (VAL), (AMM))
#define atomic_exchange_explicit(PTR, VAL, AMM) \
	__atomic_exchange_n((PTR), (VAL), (AMM))
#define atomic_compare_exchange_strong_explicit(PTR, EXP, DES, SUCC, FAIL) \
	(__atomic_compare_exchange_n((PTR), (EXP), (DES), 1, (SUCC), FAIL))
#define atomic_load_explicit(PTR, AMM) __atomic_load_n((PTR), (AMM))

#define memory_order_seq_cst __ATOMIC_SEQ_CST
#define memory_order_acquire __ATOMIC_ACQUIRE
#define memory_order_release __ATOMIC_RELEASE
#define memory_order_relaxed __ATOMIC_RELAXED

#endif /* !__HAS_ATOMIC */

#endif

#define ULL unsigned long long

#define MAX_PCIE_PORTS 4

#define OCTEON_SLI_ADDR 0x11f0000010000ull
#define OCTEON_SLI3_ADDR 0x11f0000020000ull

static const char *PCI_DEVICE_FILENAME = "/proc/bus/pci/devices";
static const uint32_t  OCTEON_PCI_IDS[] = { 0x177d0003, /* CN38XX Pass 1 */
                                            0x177d0004, /* CN38XX Pass 2 */
                                            0x177d0005, /* CN38XX Pass 3 */
                                            0x177d1001, /* Thunder */
                                            0x177d0020, /* CN31XX */
                                            0x177d0030, /* CN30XX */
                                            0x177d0040, /* CN58XX */
                                            0x177d0050, /* CN56XX */
                                            0x177d0080, /* CN52XX */
                                            0x177d0090, /* CN63XX */
                                            0x177d0093, /* CN61XX */
                                            0x177d0092, /* CN66XX */
                                            0x177d0091, /* CN68XX */
                                            0x177d0094, /* CNF71XX */
                                            0x177d0095, /* CN78XX */
                                            0x177d0096, /* CN70XX */
                                            0x177d9700, /* CN73XX */
                                            0x177d9800, /* CNF75XX */
                                            0 };

static uint32_t octeon_pci_bar0_size    = 0;    /* Size of the BAR0 memory mapped region */
static uint64_t octeon_pci_bar0_address = 0;    /* Physical address of the BAR0 region */
static char *   octeon_pci_bar0_ptr     = NULL; /* Virtual address of the BAR0 region */
static uint32_t octeon_pci_bar1_size    = 0;    /* Size of the BAR1 memory mapped region */
static uint64_t octeon_pci_bar1_address = 0;    /* Physical address of the BAR1 region */
static char *   octeon_pci_bar1_ptr     = NULL; /* Virtual address of the BAR1 region */
static uint32_t octeon_pci_model        = 0;    /* Octeon model we're connected to */
static uint32_t octeon_pcie_port        = 0;    /* Octeon PCIe port to use */
static octeon_remote_map_cookie_t bar0_cookie;
static octeon_remote_map_cookie_t bar1_cookie;
static uint64_t octeon_pci_bar0_bar1_index;
static int octeon_pci_bar0_win_rd_addr;
static int octeon_pci_bar0_win_rd_data;
static int octeon_pci_bar0_win_wr_addr;
static int octeon_pci_bar0_win_wr_data;
static int octeon_pci_bar0_win_wr_data;
static int octeon_pci_bar0_win_wr_mask;
static int octeon_pci_sli_last_win_rdata0;
static int octeon_pci_sli_last_win_rdata1;


/**
 * Find an Octeon on the PCI bus and setup the BAR0 and BAR1
 * addresses. Use the parameter device to control which Octeon
 * is used.
 *
 * @param device Octeon device to use. Zero for the first device, one for
 *               the second, etc.
 * @return Zero on success
 */
static int pci_get_device(int device)
{
    FILE *in;
    unsigned int bus;
    unsigned int devfn;
    unsigned int pci_id;
    int num_pci_devices = 0;

    /* Open the list of all PCI/PCIe devices */
    in = fopen(PCI_DEVICE_FILENAME, "r");
    if (in == NULL)
    {
        octeon_remote_debug(1, "Unable to open %s\n", PCI_DEVICE_FILENAME);
        return -1;
    }

    /* Each line in this file represents a PCI/PCIe device */
    while (!feof(in))
    {
        int index;
        unsigned int irq;
        unsigned long long bar0;
        unsigned long long bar1;
        unsigned long long siz0;
        unsigned long long siz1;
        unsigned long long unused;
        char rest_of_line[256];

        int count = fscanf(in, "%2x%2x %8x %x %Lx %Lx %Lx %Lx %Lx %Lx %Lx %Lx %Lx %Lx",
                           &bus, &devfn, &pci_id, &irq,
                           &bar0, &unused, &bar1, &unused, &unused, &unused, &unused,
                           &siz0, &unused, &siz1);
        if (count != 14)
        {
            if (count == -1)
                break;
            octeon_remote_debug(1, "fscanf returned %d instead of 14\n", count);
            octeon_remote_perror(-1, "fscanf failed to read the format");
            continue;
        }

        /* The fscanf doesn't read the whole line. Read the rest and throw it away */
        if (fgets(rest_of_line, sizeof(rest_of_line), in) == NULL)
        {
            octeon_remote_perror(-1, "fgets failed to read the rest of the line");
            fclose(in);
            return -1;
        }
        num_pci_devices++;

        /* Loop through our PCI ID list and look for a match */
        index = 0;
        while (OCTEON_PCI_IDS[index] && ((pci_id & 0xfffffff0) != OCTEON_PCI_IDS[index]))
            index++;
        /* If no match was found go read a new line */
        if (OCTEON_PCI_IDS[index] == 0)
        {
            octeon_remote_debug(1, "PCI ID 0x%08x not recognized\n", pci_id);
            continue;
        }

        octeon_remote_debug(1, "%d:%d.%d PCI ID 0x%08x is an Octeon. BAR0=0x%llx[0x%llx] BAR1=0x%llx[0x%llx]\n", bus, devfn>>3, devfn&7, pci_id, bar0, siz0, bar1, siz1);

        /* We found an Octeon and read the location of its bar registers. If
            this is the device number we want we can stop searching */
        if (device == 0)
        {
            /* We need to strip off the lower 3 flag bits */
            octeon_pci_bar0_address = bar0 & 0xfffffffffffffff0ull;
            octeon_pci_bar1_address = bar1 & 0xfffffffffffffff0ull;
            octeon_pci_bar0_size = siz0;
            octeon_pci_bar1_size = siz1;
            break;
        }
        else
        {
            octeon_remote_debug(1, "Looking for the next Octeon device\n");
            device--;
        }
    }
    fclose(in);

    /* If no Octeon device was found the print a message and fail */
    if (!(octeon_pci_bar0_address && octeon_pci_bar1_address && octeon_pci_bar0_size && octeon_pci_bar1_size))
    {
        octeon_remote_debug(-1, "Octeon device not found\n");
        return -1;
    }

    char proc_name[64];
    snprintf(proc_name, sizeof(proc_name), "/proc/bus/pci/%02x/%02x.0", bus, devfn>>3);
    in = fopen(proc_name, "r+");
    if (in == NULL)
    {
        int i = 0;
        bool found = 0;

        snprintf(proc_name, sizeof(proc_name), "/proc/bus/pci/%04x:%02x/%02x.0", i, bus, devfn>>3);
        in = fopen(proc_name, "r+");
        /* Check if domain name is present */
        for (i = 1; i < MAX_PCIE_PORTS; i++)
        {
            snprintf(proc_name, sizeof(proc_name), "/proc/bus/pci/%04x:%02x/%02x.0", i, bus, devfn>>3);
            in = fopen(proc_name, "r+");
            if (in != NULL)
            {
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            octeon_remote_perror(-1, "%s", proc_name);
            return -1;
        }
    }

    /* Read CVMX_PCI_CFG01 and make sure bus master and memory space are enabled */
    uint32_t octeon_cfg1;
    if (fseek(in, CVMX_PCI_CFG01, SEEK_SET))
    {
        octeon_remote_perror(-1, "fseek CVMX_PCI_CFG01");
        fclose(in);
        return -1;
    }
    if (fread(&octeon_cfg1, sizeof(octeon_cfg1), 1, in) != 1)
    {
        octeon_remote_perror(-1, "fread CVMX_PCI_CFG01");
        fclose(in);
        return -1;
    }
    if ((cvmx_le32_to_cpu(octeon_cfg1) & 0x7) != 0x6)
    {
        octeon_remote_debug(1, "Device is not enabled. Enabling it (0x%08x)\n", cvmx_le32_to_cpu(octeon_cfg1));
        /* Bus master and memory space are not enabled. Force them */
        octeon_cfg1 = cvmx_cpu_to_le32(cvmx_le32_to_cpu(octeon_cfg1) | 0x6);
        if (fseek(in, CVMX_PCI_CFG01, SEEK_SET))
        {
            octeon_remote_perror(-1, "fread CVMX_PCI_CFG01");
            fclose(in);
            return -1;
        }
        if (fwrite(&octeon_cfg1, sizeof(octeon_cfg1), 1, in) != 1)
        {
            octeon_remote_perror(-1, "fwrite CVMX_PCI_CFG01");
            fclose(in);
            return -1;
        }
    }
    fclose(in);

    /* Use the PCI ID to figure out which Octeon we are talking to. Note that
        the actual pass number will be read later after the mmaps are setup */
    switch ((pci_id>>4) & 0xf)
    {
        case 0: /* CN38XX or CN73XX onwards */
            switch ((pci_id>>8) & 0xff)
            {
                case 0x97:
                    octeon_pci_model = OCTEON_CN73XX_PASS1_0;
                    break;
                case 0x98:
                    octeon_pci_model = OCTEON_CNF75XX_PASS1_0;
                    break;
                default:
                    octeon_pci_model = OCTEON_CN38XX_PASS2;
                    break;
            }
            break;
        case 2: /* CN31XX */
            octeon_pci_model = OCTEON_CN31XX_PASS1;
            break;
        case 3: /* CN30XX */
            octeon_pci_model = OCTEON_CN30XX_PASS1;
            break;
        case 4: /* CN58XX */
            octeon_pci_model = OCTEON_CN58XX_PASS1_0;
            break;
        case 5: /* CN56XX */
            octeon_pci_model = OCTEON_CN56XX_PASS1_0;
            break;
        case 7: /* CN50XX */
            octeon_pci_model = OCTEON_CN50XX_PASS1_0;
            break;
        case 8: /* CN52XX */
            octeon_pci_model = OCTEON_CN52XX_PASS1_0;
            break;
        case 9: /* CN6XXX */
            switch (pci_id & 0xf)
            {
                case 0: /* CN63XX */
                default:
                    octeon_pci_model = OCTEON_CN63XX_PASS1_0;
                    break;
                case 1: /* CN68XX */
                    octeon_pci_model = OCTEON_CN68XX_PASS1_0;
                    break;
                case 2: /* CN66XX */
                    octeon_pci_model = OCTEON_CN66XX_PASS1_0;
                    break;
                case 3: /* CN61XX */
                    octeon_pci_model = OCTEON_CN61XX_PASS1_0;
                    break;
                case 4: /* CNF71XX */
                    octeon_pci_model = OCTEON_CNF71XX_PASS1_0;
                    break;
                case 5: /* CN78XX */
                    /* The SLI CSR are different between pass1.x and pass2.x,
                       set the model based on the BAR0 size.
                            pass1.x - 32KB
                            pass2.x - 8MB
                     */
                    if (octeon_pci_bar0_size == 0x8000)
                    	octeon_pci_model = OCTEON_CN78XX_PASS1_0;
                    else
                    	octeon_pci_model = OCTEON_CN78XX_PASS2_0;
                    break;
                case 6: /* CN70XX */
                    octeon_pci_model = OCTEON_CN70XX_PASS1_0;
                    break;
            }
            break;
        default:
            octeon_pci_model = OCTEON_CN58XX_PASS1_0;
            break;
    }
    octeon_remote_debug(1, "Found Octeon on bus %d in slot %d. BAR0=0x%08llx[0x%x], BAR1=0x%08llx[0x%x]\n",
           bus, devfn>>3,
           (unsigned long long)octeon_pci_bar0_address, octeon_pci_bar0_size,
           (unsigned long long)octeon_pci_bar1_address, octeon_pci_bar1_size);
    return 0;
}


/**
 * The normal memcpy() tends to do 1 byte at a time for 4 byte
 * and 8 byte sizes on some machines. Since we are copying over
 * the PCI bus, optimized size accesses can be much more
 * important. This function attempts to use larger transfers
 * whenever possible.
 *
 * @param dest   Destination of copy
 * @param src    Source of copy
 * @param length Number of bytes to transfer
 */
static void fast_memcpy(void *dest, const void *src, unsigned long length)
{
    /* Cast void pointers to chars so compiler won't complain about
        incrementing void pointers */
    char *dptr = (char*)dest;
    char *sptr = (char*)src;

    /* Only attempt 128bit copies if everything is aligned. This copy
        works well on Octeon as a host, firing 2 IO loads at once */
    if (!((unsigned long)dptr&0xf) && !((unsigned long)sptr&0xf))
    {
        while (length >= 16)
        {
            uint64_t tmp1 = *(volatile uint64_t *)sptr;
            uint64_t tmp2 = *(volatile uint64_t *)(sptr+8);
            *(volatile uint64_t*)dptr = tmp1;
            *(volatile uint64_t*)(dptr+8) = tmp2;
            dptr+=16;
            sptr+=16;
            length-=16;
        }
    }

    /* Only attempt 64bit copies if everything is aligned */
    if (!((unsigned long)dptr&7) && !((unsigned long)sptr&7))
    {
        while (length >= 8)
        {
            *(uint64_t*)dptr = *(uint64_t *)sptr;
            dptr+=8;
            sptr+=8;
            length-=8;
        }
    }
    /* Only attempt 32bit copies if everything is aligned */
    if (!((unsigned long)dptr&3) && !((unsigned long)sptr&3))
    {
        while (length >= 4)
        {
            *(uint32_t*)dptr = *(uint32_t *)sptr;
            dptr+=4;
            sptr+=4;
            length-=4;
        }
    }
    /* Copy everything that is left using the normal memcpy */
    if (length)
        memcpy(dptr, sptr, length);
}


/**
 * Setup BAR1 index register zero to access the supplied
 * address.
 *
 * @param address Octeon physical memory address to access.
 */
static void pci_bar1_setup(uint64_t address)
{
    if (octeon_pci_bar0_bar1_index & (1ull<<48))
    {
        cvmx_pemx_bar1_indexx_t bar1_entry;
        bar1_entry.u64          = 0;            /* Unused fields should be zero */
        bar1_entry.s.addr_idx   = address>>22;  /* Physical memory address in 4MB pages */
        bar1_entry.s.end_swp    = 1;            /* 1=little endian order */
        bar1_entry.s.addr_v     = 1;            /* Valid */
        octeon_remote_write_csr(octeon_pci_bar0_bar1_index, bar1_entry.u64);
        octeon_remote_read_csr(octeon_pci_bar0_bar1_index);
    }
    else
    {
        cvmx_pci_bar1_indexx_t bar1_entry;
        volatile uint32_t *bar1 = (uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_bar1_index);

        bar1_entry.u32          = 0;            /* Unused fields should be zero */
        bar1_entry.s.addr_idx   = address>>22;  /* Physical memory address in 4MB pages */
        bar1_entry.s.end_swp    = 1;            /* 1=little endian order */
        bar1_entry.s.addr_v     = 1;            /* Valid */

        *bar1 = cvmx_cpu_to_le32(bar1_entry.u32);
        *bar1; /* This read is needed to enforce ordering on PowerPC */
    }
}


static void setup_globals(void)
{
    /* Warning: This function can't use OCTEON_IS_MODEL as octeon_pci_model
        isn't fully setup yet. A call to octeon_remote_get_model() will cause
        it to cache the wrong value */
    switch (octeon_pci_model)
    {
        case OCTEON_CN38XX_PASS2:
        case OCTEON_CN31XX_PASS1:
        case OCTEON_CN30XX_PASS1:
        case OCTEON_CN58XX_PASS1_0:
        case OCTEON_CN50XX_PASS1_0:
        {
            octeon_pci_bar0_bar1_index = CVMX_PCI_BAR1_INDEXX(0);
            octeon_pci_bar0_win_rd_addr = CVMX_PCI_WIN_RD_ADDR;
            octeon_pci_bar0_win_rd_data = CVMX_PCI_WIN_RD_DATA;
            octeon_pci_bar0_win_wr_addr = CVMX_PCI_WIN_WR_ADDR;
            octeon_pci_bar0_win_wr_data = CVMX_PCI_WIN_WR_DATA;
            octeon_pci_bar0_win_wr_mask = CVMX_PCI_WIN_WR_MASK;

            cvmx_npi_ctl_status_t npi_ctl_status;
            npi_ctl_status.u64 = octeon_remote_read_csr(CVMX_NPI_CTL_STATUS);
            octeon_pci_model |= npi_ctl_status.s.chip_rev;
            break;
        }
        case OCTEON_CN56XX_PASS1_0:
        case OCTEON_CN52XX_PASS1_0:
        {
            octeon_pci_bar0_bar1_index = CVMX_NPEI_BAR1_INDEXX(0);
            octeon_pci_bar0_win_rd_addr = CVMX_NPEI_WIN_RD_ADDR;
            octeon_pci_bar0_win_rd_data = CVMX_NPEI_WIN_RD_DATA;
            octeon_pci_bar0_win_wr_addr = CVMX_NPEI_WIN_WR_ADDR;
            octeon_pci_bar0_win_wr_data = CVMX_NPEI_WIN_WR_DATA;
            octeon_pci_bar0_win_wr_mask = CVMX_NPEI_WIN_WR_MASK;

            cvmx_npei_ctl_status_t npei_ctl_status;
            cvmx_npei_window_ctl_t npei_window_ctl;
            /* Set access to register space on NCB to timeout as per the
               suggested value. */
            npei_window_ctl.u64 = octeon_remote_read_csr(CVMX_PEXP_NPEI_WINDOW_CTL);
            npei_window_ctl.s.time = 0x200000;
            octeon_remote_write_csr(CVMX_PEXP_NPEI_WINDOW_CTL, npei_window_ctl.u64);

            npei_ctl_status.u64 = octeon_remote_read_csr(CVMX_PEXP_NPEI_CTL_STATUS);
            octeon_pci_model |= npei_ctl_status.s.chip_rev;
            break;
        }
        case OCTEON_CN63XX_PASS1_0:
        case OCTEON_CN68XX_PASS1_0:
        case OCTEON_CN66XX_PASS1_0:
        case OCTEON_CN61XX_PASS1_0:
        case OCTEON_CNF71XX_PASS1_0:
        case OCTEON_CN78XX_PASS1_0:
        case OCTEON_CN78XX_PASS2_0:
        case OCTEON_CN70XX_PASS1_0:
        case OCTEON_CN73XX_PASS1_0:
        case OCTEON_CNF75XX_PASS1_0:
        default:
        {
            const char *pcie_qlm_str;
            cvmx_sli_ctl_status_t sli_ctl_status;
            cvmx_sli_window_ctl_t sli_window_ctl;
            uint64_t sli_mac_number_csr, sli_ctl_status_csr;
            if ((octeon_pci_model >= OCTEON_CN78XX_PASS2_0)
                && ((octeon_pci_model & 0xfff00) != OCTEON_CN70XX_PASS1_0)) {
                sli_mac_number_csr = 0x20050;
                sli_ctl_status_csr = 0x28570;
            } else {
                sli_mac_number_csr = 0x3e00;
                sli_ctl_status_csr = 0x570;
            }
            if (octeon_pci_model != OCTEON_CN63XX_PASS1_0)
                octeon_pcie_port = (cvmx_le32_to_cpu(*(uint32_t*)(octeon_pci_bar0_ptr + sli_mac_number_csr)) & 0xff);
            if ((pcie_qlm_str = getenv("OCTEON_PCIE_QLM")) != NULL)
                octeon_pcie_port = atoi(pcie_qlm_str);
            printf("PCIE port %d\n", octeon_pcie_port);
            sli_ctl_status.u64 = cvmx_le32_to_cpu(*(uint32_t*)(octeon_pci_bar0_ptr + (sli_ctl_status_csr)));
            octeon_pci_model |= sli_ctl_status.s.chip_rev;
            octeon_pci_bar0_bar1_index = CVMX_PEMX_BAR1_INDEXX(0, octeon_pcie_port);
            octeon_pci_bar0_win_rd_addr = CVMX_SLI_WIN_RD_ADDR;
            octeon_pci_bar0_win_rd_data = CVMX_SLI_WIN_RD_DATA;
            octeon_pci_bar0_win_wr_addr = CVMX_SLI_WIN_WR_ADDR;
            octeon_pci_bar0_win_wr_data = CVMX_SLI_WIN_WR_DATA;
            octeon_pci_bar0_win_wr_mask = CVMX_SLI_WIN_WR_MASK;
            octeon_pci_sli_last_win_rdata0 = CVMX_SLI_LAST_WIN_RDATA0;
            octeon_pci_sli_last_win_rdata1 = CVMX_SLI_LAST_WIN_RDATA1;

            /* Set access to register space on NCB to timeout as per the
               suggested value. */
            sli_window_ctl.u64 = octeon_remote_read_csr(CVMX_PEXP_SLI_WINDOW_CTL);
            sli_window_ctl.s.time = 0x200000;
            octeon_remote_write_csr(CVMX_PEXP_SLI_WINDOW_CTL, sli_window_ctl.u64);

            break;
        }
    }
}


/**
 * Called when octeon_remote_open() is called using a remote spec
 * for PCI.
 *
 * @param remote_spec
 *               String specification of the remote to connect to.
 *
 * @return Zero on success, negative on failure.
 */
static int pci_open(const char *remote_spec)
{
    int device = 0;

    /* If the spec contains a device number, then read it */
    if (remote_spec[3] == ':')
        device = atoi(remote_spec + 4);

    /* Check if the environment variable OCTEON_PCI_DEVICE is set. It is
        treated as an override for the passed device number. This is to
        maintain backwards compatability with the old PCI utilites. The new
        remote spec should be used instead */
    const char *pci_device_str = getenv("OCTEON_PCI_DEVICE");
    if (pci_device_str)
    {
        device = atoi(pci_device_str);
        if (device < 0) device = 0;
        if (device > 20) device = 20;
    }
    octeon_remote_debug(3, "Searching for device %d\n", device);

    /* Find the Octeon and get its memory mapped region addresses */
    if (pci_get_device(device))
        return -1;

    /* Map BAR0 */
    octeon_pci_bar0_ptr = octeon_remote_map(octeon_pci_bar0_address, octeon_pci_bar0_size, &bar0_cookie);
    if (!octeon_pci_bar0_ptr)
        return -1;

    /* Map BAR1 */
    octeon_pci_bar1_ptr = octeon_remote_map(octeon_pci_bar1_address, octeon_pci_bar1_size, &bar1_cookie);
    if (!octeon_pci_bar1_ptr)
        return -1;

    setup_globals();
    return 0;
}


/**
 * Called when octeon_remote_close() is called.
 */
static void pci_close(void)
{
    if (octeon_remote_debug_handler_is_installed() &&
        octeon_remote_mem_access_ok())
        octeon_remote_debug_handler_remove();
    octeon_remote_unmap(&bar0_cookie);
    octeon_pci_bar0_ptr = NULL;
    octeon_remote_unmap(&bar1_cookie);
    octeon_pci_bar1_ptr = NULL;
}


/**
 * Wrapper function to read a 32bit/64bit CSR over PCI/PCIe
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 64bit value of the CSR
 */
static uint64_t __pci_read_csr_wrapper(uint64_t physical_address, int size)
{
    if ((size == 4) && (octeon_pci_bar0_size == 4096)) {
        octeon_remote_debug(-1, "PCI does not support accessing 32bit CSRs");
        return -1;
    }

    if (OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_OCTEON3())
    {
	uint64_t addr = physical_address & (octeon_pci_bar0_size - 1);
	/* The SLI CSRs are accessed directly using external address. */
        if ((((physical_address & OCTEON_SLI_ADDR) == OCTEON_SLI_ADDR) ||
             ((physical_address & OCTEON_SLI3_ADDR) == OCTEON_SLI3_ADDR)) &&
            (addr < octeon_pci_bar0_size))
        {
            if (size == 4) {
                return cvmx_le32_to_cpu(*(uint32_t*)(octeon_pci_bar0_ptr + (physical_address & 0xffff)));
            } else {
                uint64_t upper = cvmx_le32_to_cpu(*(uint32_t*)(octeon_pci_bar0_ptr + addr + 4));
                uint64_t lower = cvmx_le32_to_cpu(*(uint32_t*)(octeon_pci_bar0_ptr + addr));
                return (uint64_t)((upper << 32) | lower);
            }
        }
        else if (OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_MODEL(OCTEON_CN70XX))
        {
            uint64_t result, reg_addr;

            /* Octeon II needs the read width to be set */
            if (size == 4)
                physical_address |= 2ull<<49;
            else
                physical_address |= 3ull<<49;
            *(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr + 4) = cvmx_cpu_to_le32(physical_address>>32);
            *(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr) = cvmx_cpu_to_le32(physical_address);

            /* This read is needed to enforce ordering on PowerPC */
            cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr));

            result = (uint64_t)(cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_data)));
            reg_addr = octeon_pcie_port ? octeon_pci_sli_last_win_rdata1 : octeon_pci_sli_last_win_rdata0;
            result |= ((uint64_t)(cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + reg_addr+4))))<<32;
            return result;
        }
        else
        {
            uint64_t result;

            physical_address |= 3ull<<49;
            *(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr + 4) = cvmx_cpu_to_le32(physical_address>>32);

            /* This read is needed to enforce ordering on PowerPC */
            cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr + 4));

            *(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr) = cvmx_cpu_to_le32(physical_address);

            /* This read is needed to enforce ordering on PowerPC */
            cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr));

            result = (uint64_t)(cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_data)));
            result |= ((uint64_t)(cvmx_le32_to_cpu(*(volatile uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_data+4))))<<32;
            return result;
        }
    }
    else
    {

        uint64_t result;
        volatile uint32_t *read_addr_lo = (uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_addr);
        volatile uint32_t *read_addr_hi = read_addr_lo+1;
        volatile uint32_t *read_data_lo = (uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_rd_data);
        volatile uint32_t *read_data_hi = read_data_lo+1;

        /* Writing the lo part of the address actually triggers the read. That
            means the high part must be written first */
        *read_addr_hi = cvmx_cpu_to_le32(physical_address>>32);
        *read_addr_hi; /* This read is needed to enforce ordering on PowerPC */
        *read_addr_lo = cvmx_cpu_to_le32(physical_address);
        *read_addr_lo; /* This read is needed to enforce ordering on Freescale PowerPC */

        if (size == 4)
            result = (uint64_t)cvmx_le32_to_cpu(*read_data_lo);
        else
            result = (((uint64_t)cvmx_le32_to_cpu(*read_data_hi))<<32) | (uint64_t)cvmx_le32_to_cpu(*read_data_lo);
        return result;
    }
}

/**
 * Wrapper function to write 32bit/64bit CSR over PCI/PCIe
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void __pci_write_csr_wrapper(uint64_t physical_address, uint64_t value, int size)
{
    volatile uint32_t *write_addr_lo;
    volatile uint32_t *write_addr_hi;
    volatile uint32_t *write_data_lo;
    volatile uint32_t *write_data_hi;
    volatile uint32_t *write_mask_lo;
    volatile uint32_t *write_mask_hi;
    uint64_t addr;

    if ((size == 4) && (octeon_pci_bar0_size == 4096)) {
        octeon_remote_debug(-1, "PCI does not support accessing 32bit CSRs");
        return;
    }

    write_addr_lo = (uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_wr_addr);
    write_addr_hi = write_addr_lo+1;
    write_data_lo = (uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_wr_data);
    write_data_hi = write_data_lo+1;
    write_mask_lo = (uint32_t*)(octeon_pci_bar0_ptr + octeon_pci_bar0_win_wr_mask);
    write_mask_hi = write_mask_lo+1;

    addr = physical_address & (octeon_pci_bar0_size - 1);
    /* In Octeon II, the SLI CSRs are accessed directly using external address. */
    if ((OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_MODEL(OCTEON_CN7XXX)) &&
        (((physical_address & OCTEON_SLI_ADDR) == OCTEON_SLI_ADDR) ||
         ((physical_address & OCTEON_SLI3_ADDR) == OCTEON_SLI3_ADDR)) &&
	(addr < octeon_pci_bar0_size))
    {
        if (size == 4) {
            (*(uint32_t*)(octeon_pci_bar0_ptr + (physical_address & 0xffff))) = cvmx_cpu_to_le32(value);
            return;
        } else {
           /* Writing the low part of the data triggers the actual write. It needs
              to last */
            uint64_t upper = (value >> 32) & 0xffffffffu;
            uint64_t lower = (value & 0xffffffffu);
            (*(uint32_t *)(octeon_pci_bar0_ptr + addr + 4)) = cvmx_cpu_to_le32(upper);
            (*(uint32_t *)(octeon_pci_bar0_ptr + addr)) = cvmx_cpu_to_le32(lower);
            return;
        }
    }

    /* Writing the low part of the data triggers the actual write. It needs to
        be last */
    if (OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_MODEL(OCTEON_CN7XXX))
    {
        *write_mask_hi = cvmx_cpu_to_le32(0);
        if (size == 4)
            *write_mask_lo = cvmx_cpu_to_le32((physical_address & 4) ? 0x0f : 0xf0);
        else
            *write_mask_lo = cvmx_cpu_to_le32(0xff);
    }
    else
    {
        *write_mask_hi = cvmx_cpu_to_le32(0);
        if (size == 4)
            *write_mask_lo = cvmx_cpu_to_le32((physical_address & 4) ? 0xf0 : 0x0f);
        else
            *write_mask_lo = cvmx_cpu_to_le32(0);
    }
    *write_addr_hi = cvmx_cpu_to_le32(physical_address>>32);
    *write_addr_lo = cvmx_cpu_to_le32(physical_address);
    *write_data_hi = cvmx_cpu_to_le32(value>>32);
    *write_data_hi; /* This read is needed to enforce ordering on PowerPC */
    *write_data_lo = cvmx_cpu_to_le32(value);
}

/**
 * Read a 64bit CSR over PCI/PCIe
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 64bit value of the CSR
 */
static uint64_t pci_read_csr(uint64_t physical_address)
{
    return __pci_read_csr_wrapper(physical_address, 8);
}

/**
 * Write a 64bit CSR over PCI/PCIe
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void pci_write_csr(uint64_t physical_address, uint64_t value)
{
    __pci_write_csr_wrapper(physical_address, value, 8);
}

/**
 * Read a 32bit CSR over PCI/PCIe
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 32bit value of the CSR
 */
static uint32_t pci_read_csr32(uint64_t physical_address)
{
    return __pci_read_csr_wrapper(physical_address, 4);
}

/**
 * Write a 32bit CSR over PCI/PCIe
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void pci_write_csr32(uint64_t physical_address, uint32_t value)
{
    __pci_write_csr_wrapper(physical_address, value, 4);
}

/**
 * Read data from a physical memory address.
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
static void pci_read_mem(void *buffer_ptr, uint64_t physical_address, int length)
{
    char *buffer = (char*)buffer_ptr;
    uint32_t block_mask = (1<<22) - 1;
    uint64_t end_address = physical_address + length;

    /* We need to do reads in 4MB aligned chunks. This way we only need to
        mess with one BAR1 index register */
    do
    {
        char *ptr = octeon_pci_bar1_ptr + (physical_address & block_mask);
        if (end_address > (physical_address & ~(uint64_t)block_mask) + block_mask + 1)
            length = block_mask + 1 - (physical_address & block_mask);
        else
            length = end_address - physical_address;

        pci_bar1_setup(physical_address);
        fast_memcpy(buffer, ptr, length);
        buffer += length;
        physical_address += length;
    } while (physical_address < end_address);
}


/**
 * Write data to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
static void pci_write_mem(uint64_t physical_address, const void *buffer_ptr, int length)
{
    char *buffer = (char*)buffer_ptr;
    uint32_t block_mask = (1<<22) - 1;
    uint64_t end_address = physical_address + length;

    /* We need to do writes in 4MB aligned chunks. This way we only need to
        mess with one BAR1 index register */
    do
    {
        void *ptr = octeon_pci_bar1_ptr + (physical_address & block_mask);
        if (end_address > (physical_address & ~(uint64_t)block_mask) + block_mask + 1)
            length = block_mask + 1 - (physical_address & block_mask);
        else
            length = end_address - physical_address;

        pci_bar1_setup(physical_address);
        fast_memcpy(ptr, buffer, length);
        buffer += length;
        physical_address += length;
    } while (physical_address < end_address);
}

/**
 * Atomic:'spinlock_lock':Write 1 to a physical memory address and return the previous value
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 */
void pci_spinlock_lock(uint64_t physical_address)
{
	int length = 4;
	uint32_t block_mask = (1<<22) - 1;
	uint64_t end_address = physical_address + length;
	uint32_t lock;

	if (end_address >
		(physical_address & ~(uint64_t)block_mask) + block_mask + 1)
		length = block_mask + 1 - (physical_address & block_mask);
	else
		length = end_address - physical_address;

	pci_bar1_setup(physical_address);
	do {
		octeon_remote_write_mem16(physical_address, 0xeeee);
		lock = octeon_remote_read_mem32(physical_address);
		octeon_remote_write_mem16(physical_address, 0);
	} while (lock != 0xeeee0000);
	octeon_remote_write_mem32(physical_address, 1);
}

/**
 * Atomic:'spinlock_unlock':Write 0 to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 */
void pci_spinlock_unlock(uint64_t physical_address)
{
	int length = 4;
	uint32_t block_mask = (1<<22) - 1;
	uint64_t end_address = physical_address + length;

	if (end_address >
		(physical_address & ~(uint64_t)block_mask) + block_mask + 1)
		length = block_mask + 1 - (physical_address & block_mask);
	else
		length = end_address - physical_address;

	pci_bar1_setup(physical_address);
	octeon_remote_write_mem32(physical_address, 0);
}



/**
 * Return the Processor ID of the Octeon. This should be identical
 * to the value found in the COP0 PRID register
 *
 * @return Processor ID
 */
static uint32_t pci_get_model(void)
{
    return octeon_pci_model;
}


/**
 * Take the cores in the bit vector out of the debug exception.
 *
 * @param start_mask Cores to start
 */
static void pci_start_cores(uint64_t start_mask)
{
    int core;
    uint64_t reset_status;

    /* Handle Core 0 in reset as a special case. If core 0 is in reset, interpret
        this as a request to take it out of reset */
    //printf("Starting cores 0x%llx\n", (long long)start_mask);
    if (OCTEON_IS_OCTEON3()) {
        /* Power up cores first */
        printf("Powering up additional cores.\n");
        pci_write_csr(CVMX_RST_PP_POWER, 0);
        usleep(2000);
    }
    reset_status = pci_read_csr(CVMX_CIU_PP_RST);
    if (reset_status & 1)
    {
        if (start_mask & 1)
            pci_write_csr(CVMX_CIU_PP_RST, reset_status & -2);
        usleep(2000);
    }
    else
    {
        for (core=0; core<CVMX_MAX_CORES; core++)
            if ((1ull<<core) & start_mask)
            {
                uint64_t base = octeon_remote_debug_handler_get_base(core);
                if (base)
                    octeon_remote_write_mem64(base, 0);
            }
    }
}


/**
 * Cause the cores in the stop mask to take a debug exception
 *
 * @param stop_mask Cores to stop
 */
static void pci_stop_cores(uint64_t stop_mask)
{
    if (octeon_remote_mem_access_ok())
        if (octeon_remote_debug_handler_install(OCTEON_REMOTE_DEBUG_HANDLER, 0))
            return;
    octeon_remote_write_csr(CVMX_CIU_DINT, stop_mask);
}


/**
 * Get a mask where each bit represents a running core. If a core
 * is in the debug exception handler, it's bit will be zero.
 *
 * @return Bit set for every running core
 */
static uint64_t pci_get_running_cores(void)
{
    return ~octeon_remote_read_csr(CVMX_CIU_PP_DBG);
}

/* Return the number of cores available in the chip */
static uint32_t pci_get_num_cores(void)
{
    uint64_t ciu_fuse = octeon_remote_read_csr(CVMX_CIU_FUSE) & 0xffffffffffff;

    return cvmx_dpop(ciu_fuse);
}

/**
 * Get all registers, COP0, TLB, etc for a core
 *
 * @param core      Core to get state info for
 * @param registers All of the core's internal state
 *
 * @return Zero on success, negative on failure
 */
static int pci_get_core_state(int core, octeon_remote_registers_t *registers)
{
    int i, j;
    uint64_t base = octeon_remote_debug_handler_get_base(core);
    if (!base)
        return -1;
    octeon_remote_read_mem(registers, base, sizeof(*registers));

    for (i=0; i<2; i++)
        for (j=0; j<256; j++)
            registers->regs[i][j] = cvmx_be64_to_cpu(registers->regs[i][j]);

    for (i=0; i<cvmx_core_get_tlb_entries(); i++)
        for (j=0; j<4; j++)
            registers->tlb[i][j] = cvmx_be64_to_cpu(registers->tlb[i][j]);

    if (registers->regs[0][0] == 1)
    {
        registers->regs[0][0] = 0;
        return 0;
    }
    else
        return -1;
}


/**
 * Set all registers, COP0, TLB, etc for a given core
 *
 * @param core      Core to set state for
 * @param registers All date for the core
 *
 * @return Zero on success, negative on failure
 */
static int pci_set_core_state(int core, const octeon_remote_registers_t *registers)
{
    int i, j;
    octeon_remote_registers_t debug;
    uint64_t base = octeon_remote_debug_handler_get_base(core);
    if (!base)
        return -1;

    for (i=0; i<2; i++)
        for (j=0; j<256; j++)
            debug.regs[i][j] = cvmx_be64_to_cpu(registers->regs[i][j]);

    for (i=0; i<cvmx_core_get_tlb_entries(); i++)
        for (j=0; j<4; j++)
            debug.tlb[i][j] = cvmx_be64_to_cpu(registers->tlb[i][j]);

    debug.regs[0][0] = cvmx_cpu_to_be64(1);
    octeon_remote_write_mem(base, &debug, sizeof(debug));
    return 0;
}

/**
 * Due to errata, cn56xx pass 1.x can't use the normal reset function.
 *
 * @param stop_core Attempt to stop core 0 from booting, if possible. This is not
 *                  software controlled for PCI. The board must use the PCI_BOOT
 *                  pin to control this.
 *
 * @return Zero on success
 */
static int pci_reset(int stop_core __attribute__ ((unused)))
{
    /* Check to see if we're set up for remote booting */
    if (OCTEON_IS_OCTEON2())
    {
	union cvmx_mio_rst_boot mio_rst_boot;
	mio_rst_boot.u64 = octeon_remote_read_csr(CVMX_MIO_RST_BOOT);
	/* Mask lboot, R/W1C fields, to avoid from clearing. */
	mio_rst_boot.s.lboot = 0;
	mio_rst_boot.s.rboot = stop_core;
	octeon_remote_write_csr(CVMX_MIO_RST_BOOT, mio_rst_boot.u64);
        if (stop_core)
	    printf("Setting OCTEON for remote boot.\n");
    }
    else if (OCTEON_IS_OCTEON3())
    {
	union cvmx_rst_boot rst_boot;
	rst_boot.u64 = octeon_remote_read_csr(CVMX_RST_BOOT);
	/* Mask lboot, lboot_ext, lboot_oci, R/W1C fields, to avoid from clearing. */
	rst_boot.s.lboot = 0;
	rst_boot.s.lboot_ext = 0;
	rst_boot.s.lboot_oci = 0;
	rst_boot.s.rboot = stop_core;
	octeon_remote_write_csr(CVMX_RST_BOOT, rst_boot.u64);
        if (stop_core)
	    printf("Setting OCTEON for remote boot.\n");
    }

    if (!OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2) && !OCTEON_IS_MODEL(OCTEON_CN31XX))
        octeon_remote_write_csr(CVMX_CIU_SOFT_BIST, 1);
    if (OCTEON_IS_MODEL(OCTEON_CN56XX_PASS1_X))
    {
        /* Do workaround for 56XX PCIe reset errata  PCIE-709*/

        /* Code written to boot bus region. Note that we don't care about
        ** saving registers since we are resetting the chip.

         Enable 64 bit loads/stores
        bfc00504:       40096000        mfc0    a5,c0_status
        bfc00508:       240200e0        li      v0,224
        bfc0050c:       00491025        or      v0,v0,a5
        bfc00510:       40826000        mtc0    v0,c0_status

        Set soft bist and reset the chip
        100002d8:       3c03ff80        lui     v1,0xff80
        100002dc:       24020001        li      v0,1
        100002e0:       34630107        ori     v1,v1,0x107
        100002e4:       00031a3c        dsll32  v1,v1,0x8
        100002e8:       34640740        ori     a0,v1,0x740
        100002ec:       34630738        ori     v1,v1,0x738
        100002f0:       fc620000        sd      v0,0(v1)
        100002f4:       fc820000        sd      v0,0(a0)
        */

        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0x0);  /* Auto increments after write */
        /* Most sig. word executes first */
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x40096000ull << 32) | 0x240200e0ull);
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x00491025ull << 32) | 0x40826000ull);

        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x3c03ff80ull << 32) | 0x24020001ull);
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x34630107ull << 32) | 0x00031a3cull);
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0x34640740ull << 32) | 0x34630738ull);
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (0xfc620000ull << 32) | 0xfc820000ull);
        octeon_remote_read_csr(CVMX_MIO_BOOT_LOC_DAT);  /* Make sure writes complete */

        /* We handle this differently depending on if core 0 is running
        ** or not. */
        if ((octeon_remote_read_csr(CVMX_CIU_PP_RST) & 0x1))
        {
            /* Core 0 not running, so put code at reset vector and take out of reset */
            uint64_t reg_val = (1ull << 31) | (0x1fc00000 >> 4);
            octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_CFGX(0), reg_val);
            octeon_remote_read_csr(CVMX_MIO_BOOT_LOC_CFGX(0));  /* Ensure write has completed */
            octeon_remote_write_csr(CVMX_CIU_PP_RST, 0xffe);
        }
        else
        {
            /* Send debug interrupt to cause core 0 to reset chip */
            /* Write code to reset chip at debug exception vector */
            uint64_t reg_val = (1ull << 31) | (0x1fc00480 >> 4);
            octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_CFGX(0), reg_val);
            octeon_remote_read_csr(CVMX_MIO_BOOT_LOC_CFGX(0));  /* Ensure write has completed */
            /* Send debug interrupt to cause core 0 to reset chip */
            octeon_remote_write_csr(CVMX_CIU_DINT, 0x1);
        }
    }
    else if (OCTEON_IS_OCTEON3())
    {
        /* HRM specifies that CIU_SOFT_RST should be read before initiating
        ** a reset over PCI */
        octeon_remote_read_csr(CVMX_RST_SOFT_RST);
        octeon_remote_write_csr(CVMX_RST_SOFT_RST, 1);
    }
    else
    {
        /* HRM specifies that CIU_SOFT_RST should be read before initiating
        ** a reset over PCI */
        octeon_remote_read_csr(CVMX_CIU_SOFT_RST);
        octeon_remote_write_csr(CVMX_CIU_SOFT_RST, 1);
    }

    /* Delay here to ensure that no accesses to Octeon are made
    ** while it is in reset.  1 MS is what CN56XX HRM specifies,
    ** but add some margin. */
    usleep(5000);
    return 0;
}


/**
 * Sample performance / profiling information for a set of cores
 * minimizing the performance impact of taking the sample. The
 * amount of data returned may vary depending on the remote
 * protocol used, but at least the program counter for each core
 * will normally be supported. Values that could not be sampled
 * will be zero.
 *
 * @param coremask Each set bit represent a core that should be sampled.
 * @param sample   Array of samples to populate. Note that the sample array must
 *                 contain elements for cores not set in the coremask. These may
 *                 be filled with sample data even if the coremask bit is clear.
 *                 Some transports may sample all core efficiently and yield
 *                 unwanted core samples.
 *
 * @return Zero on success, negative on failure
 */
static int pci_get_sample(uint64_t coremask, octeon_remote_sample_t sample[])
{
    int core;
    int num_cores = octeon_remote_get_num_cores();

    if (octeon_remote_debug_handler_install(OCTEON_REMOTE_PROFILE_HANDLER, 0))
        return -1;
    octeon_remote_write_csr(CVMX_CIU_DINT, coremask);
    /* Give the cores time to stop */
    usleep(100);
    for (core=0; core<num_cores; core++)
    {
        if ((1ull<<core) & coremask)
        {
            uint64_t base = octeon_remote_debug_handler_get_base(core);
            sample[core].pc = octeon_remote_read_mem64(base + 8*(256 + 24*8 + 0));
            sample[core].perf_count[0] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 1));
            sample[core].perf_count[1] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 3));
            if (octeon_pci_model == OCTEON_CN70XX_PASS1_0
                || octeon_pci_model == OCTEON_CN78XX_PASS1_0
                || octeon_pci_model == OCTEON_CN78XX_PASS2_0
                || octeon_pci_model == OCTEON_CN73XX_PASS1_0
                || octeon_pci_model == OCTEON_CNF75XX_PASS1_0)
            {
                sample[core].perf_count[2] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 5));
                sample[core].perf_count[3] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 7));
            }
	    sample[core].var = octeon_remote_read_mem64(base + PER_CORE_VAR_OFFSET);
        }
    }
    /* The cores will automatically restart */
    return 0;
}


/**
 * Initialize the pointers needed for octeon_remote to work over
 * PCI/PCIe. This doesn't actually create a connection, just setup
 * internal data structures.
 *
 * @param remote_funcs
 *               Function pointers to be populated
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_pci(octeon_remote_funcs_t *remote_funcs)
{
    remote_funcs->open = pci_open;
    remote_funcs->close = pci_close;
    remote_funcs->read_csr = pci_read_csr;
    remote_funcs->write_csr = pci_write_csr;
    remote_funcs->read_csr32 = pci_read_csr32;
    remote_funcs->write_csr32 = pci_write_csr32;
    remote_funcs->read_mem = pci_read_mem;
    remote_funcs->write_mem = pci_write_mem;
    remote_funcs->get_model = pci_get_model;
    remote_funcs->start_cores = pci_start_cores;
    remote_funcs->stop_cores = pci_stop_cores;
    remote_funcs->get_running_cores = pci_get_running_cores;
    remote_funcs->get_num_cores = pci_get_num_cores;
    remote_funcs->get_core_state = pci_get_core_state;
    remote_funcs->set_core_state = pci_set_core_state;
    remote_funcs->reset = pci_reset;
    remote_funcs->get_sample = pci_get_sample;

    if (getenv("OCTEON_PCI_DEBUG"))
        remote_funcs->debug++;

    return 0;
}


#ifdef __mips__

/**
 * Write a 64bit value
 *
 * @param address Address to write to
 * @param val     Value to write
 */
static void target_write64_uint64(uint64_t address, uint64_t val)
{
    address |= (1ull<<63);
    asm volatile ("syncw\nsyncw");
    asm volatile ("sd %[v], 0(%[c])" ::[v] "r" (val), [c] "r" (address));
    asm volatile ("syncw\nsyncw");
}


/**
 * Write a 32bit value to the Octeon NPI register space
 *
 * @param address Address to write to
 * @param val     Value to write
 */
static void target_npi_write32(uint64_t address, uint32_t val)
{
    address = (address ^ 4) | (1ull<<63);
    asm volatile ("syncw\nsyncw");
    asm volatile ("sw %[v], 0(%[c])" ::[v] "r" (val), [c] "r" (address));
    asm volatile ("syncw\nsyncw");
}


/**
 * Read a 32bit value from the Octeon NPI register space
 *
 * @param address Address to read
 * @return The result
 */
static uint32_t target_npi_read32(uint64_t address)
{
    uint32_t result;
    address = (address ^ 4) | (1ull<<63);
    asm volatile ("syncw\nsyncw");
    asm volatile ("lw %[v], 0(%[c])": [v] "=r" (result) : [c] "r" (address));
    return result;
}


/**
 * Setup Octeon running as a target to be able to access the
 * host's BAR registers.
 */
static void pcihost_setup_pci_target(void)
{
    cvmx_pci_cfg01_t cfg01;
    cvmx_pci_ctl_status_2_t ctl_status_2;
    cvmx_pci_cfg19_t cfg19;
    cvmx_pci_cfg16_t cfg16;
    cvmx_pci_cfg22_t cfg22;
    cvmx_pci_cfg56_t cfg56;

    ctl_status_2.u32 = target_npi_read32(CVMX_NPI_PCI_CTL_STATUS_2);
    octeon_remote_debug(0, "PCI Status: %s %s-bit\n",
	   ctl_status_2.s.ap_pcix ? "PCI-X" : "PCI",
	   ctl_status_2.s.ap_64ad ? "64" : "32" );

    /*
    ** TDOMC must be set to one in PCI mode. TDOMC should be set to 4
    ** in PCI-X mode to allow four oustanding splits. Otherwise,
    ** should not change from its reset value. Don't write PCI_CFG19
    ** in PCI mode (0x82000001 reset value), write it to 0x82000004
    ** after PCI-X mode is known. MRBCI,MDWE,MDRE -> must be zero.
    ** MRBCM -> must be one.
    */
    if (ctl_status_2.s.ap_pcix) {
      cfg19.u32 = 0;
      cfg19.s.tdomc = 4;	/* Target Delayed/Split request
                                   outstanding maximum count. [1..31]
                                   and 0=32.  NOTE: If the user
                                   programs these bits beyond the
                                   Designed Maximum outstanding count,
                                   then the designed maximum table
                                   depth will be used instead.  No
                                   additional Deferred/Split
                                   transactions will be accepted if
                                   this outstanding maximum count is
                                   reached. Furthermore, no additional
                                   deferred/split transactions will be
                                   accepted if the I/O delay/ I/O
                                   Split Request outstanding maximum
                                   is reached. */
      cfg19.s.mdrrmc = 2;	/* Master Deferred Read Request Outstanding Max
                                   Count (PCI only).
                                   CR4C[26:24]  Max SAC cycles   MAX DAC cycles
                                    000              8                4
                                    001              1                0
                                    010              2                1
                                    011              3                1
                                    100              4                2
                                    101              5                2
                                    110              6                3
                                    111              7                3
                                   For example, if these bits are programmed to
                                   100, the core can support 2 DAC cycles, 4 SAC
                                   cycles or a combination of 1 DAC and 2 SAC cycles.
                                   NOTE: For the PCI-X maximum outstanding split
                                   transactions, refer to CRE0[22:20]  */

      cfg19.s.mrbcm = 1;	/* Master Request (Memory Read) Byte Count/Byte
                                   Enable select.
                                    0 = Byte Enables valid. In PCI mode, a burst
                                        transaction cannot be performed using
                                        Memory Read command=4?h6.
                                    1 = DWORD Byte Count valid (default). In PCI
                                        Mode, the memory read byte enables are
                                        automatically generated by the core.
                                   Note: N3 Master Request transaction sizes are
                                   always determined through the
                                   am_attr[<35:32>|<7:0>] field.  */
      target_npi_write32(CVMX_NPI_PCI_CFG19, cfg19.u32);
    }


    cfg01.u32 = 0;
    cfg01.s.msae = 1;		/* Memory Space Access Enable */
    cfg01.s.me = 1;		    /* Master Enable */
    cfg01.s.pee = 1;		/* PERR# Enable */
    cfg01.s.see = 1;		/* System Error Enable */
    cfg01.s.fbbe = 1;		/* Fast Back to Back Transaction Enable */

    target_npi_write32(CVMX_NPI_PCI_CFG01, cfg01.u32);
    target_npi_read32(CVMX_NPI_PCI_CFG01);

    /*
    ** Preferrably written to 1 to set MLTD. [RDSATI,TRTAE,
    ** TWTAE,TMAE,DPPMR -> must be zero. TILT -> must not be set to
    ** 1..7.
    */
    cfg16.u32 = 0;
    cfg16.s.mltd = 1;		/* Master Latency Timer Disable */
    target_npi_write32(CVMX_NPI_PCI_CFG16, cfg16.u32);

    /*
    ** Should be written to 0x4ff00. MTTV -> must be zero.
    ** FLUSH -> must be 1. MRV -> should be 0xFF.
    */
    cfg22.u32 = 0;
    cfg22.s.mrv = 0xff;		/* Master Retry Value [1..255] and 0=infinite */
    cfg22.s.flush = 1;		/* AM_DO_FLUSH_I control NOTE: This
				   bit MUST BE ONE for proper N3K
				   operation */
    target_npi_write32(CVMX_NPI_PCI_CFG22, cfg22.u32);

    /*
    ** MOST Indicates the maximum number of outstanding splits (in -1
    ** notation) when OCTEON is in PCI-X mode.  PCI-X performance is
    ** affected by the MOST selection.  Should generally be written
    ** with one of 0x3be807, 0x2be807, 0x1be807, or 0x0be807,
    ** depending on the desired MOST of 3, 2, 1, or 0, respectively.
    */
    cfg56.u32 = 0;
    cfg56.s.pxcid = 7;		/* RO - PCI-X Capability ID */
    cfg56.s.ncp = 0xe8;		/* RO - Next Capability Pointer */
    cfg56.s.dpere = 1;		/* Data Parity Error Recovery Enable */
    cfg56.s.roe = 1;		/* Relaxed Ordering Enable */
    cfg56.s.mmbc = 1;		/* Maximum Memory Byte Count [0=512B,1=1024B,2=2048B,3=4096B] */
    cfg56.s.most = 3;		/* Maximum outstanding Split transactions [0=1 .. 7=32] */

    target_npi_write32(CVMX_NPI_PCI_CFG56, cfg56.u32);

    /*
    ** Affects PCI performance when OCTEON services reads to its
    ** BAR1/BAR2. Refer to Section 10.6.1.  The recommended values are
    ** 0x22, 0x33, and 0x33 for PCI_READ_CMD_6, PCI_READ_CMD_C, and
    ** PCI_READ_CMD_E, respectively. Note that these values differ
    ** from their reset values.
    */
    target_npi_write32(CVMX_NPI_PCI_READ_CMD_6, 0x22);
    target_npi_write32(CVMX_NPI_PCI_READ_CMD_C, 0x33);
    target_npi_write32(CVMX_NPI_PCI_READ_CMD_E, 0x33);

    /* Configure the first SubDID to access memory */
    cvmx_npi_mem_access_subid_t mem_access;
    mem_access.u64 = 0;
    mem_access.s.esr = 1;   /**< Endian-Swap on read. */
    mem_access.s.esw = 1;   /**< Endian-Swap on write. */
    mem_access.s.nsr = 0;   /**< No-Snoop on read. */
    mem_access.s.nsw = 0;   /**< No-Snoop on write. */
    mem_access.s.ror = 0;   /**< Relax Read on read. */
    mem_access.s.row = 0;   /**< Relax Order on write. */
    mem_access.s.ba = 0;    /**< PCI Address bits [63:36]. */
    target_write64_uint64(CVMX_NPI_MEM_ACCESS_SUBID3|(1ull<<63), mem_access.u64);
}

static uint64_t target_read_csr(uint64_t physical_address)
{
    uint64_t result;
    octeon_remote_map_cookie_t cookie;
    physical_address |= (1ull << 63);
    uint64_t *ptr = octeon_remote_map(physical_address, 8, &cookie);
    result = *ptr;
    octeon_remote_unmap(&cookie);
    return result;
}

static void target_write_csr(uint64_t physical_address, uint64_t value)
{
    octeon_remote_map_cookie_t cookie;
    physical_address |= (1ull << 63);
    uint64_t *ptr = octeon_remote_map(physical_address, 8, &cookie);
    *ptr = value;
    octeon_remote_unmap(&cookie);
}

/* If TYPE is set, use SLI CSRs else use NPEI CSRs. */
static void pcihost_setup_pcie_target(int type)
{
    /* Enable bus master and memory */
    if (type)
    {
        cvmx_pemx_cfg_wr_t pemx_cfg_wr;
        pemx_cfg_wr.u64 = 0;
        pemx_cfg_wr.s.addr = CVMX_PCIEEPX_CFG001(octeon_pcie_port);
        pemx_cfg_wr.s.data = 0x6;
        target_write_csr(CVMX_PEMX_CFG_WR(octeon_pcie_port), pemx_cfg_wr.u64);
    } else {
        cvmx_pescx_cfg_wr_t pescx_cfg_wr;
        pescx_cfg_wr.u64 = 0;
        pescx_cfg_wr.s.addr = CVMX_PCIEEPX_CFG001(octeon_pcie_port);
        pescx_cfg_wr.s.data = 0x6;
        target_write_csr(CVMX_PESCX_CFG_WR(octeon_pcie_port), pescx_cfg_wr.u64);
    }

    /* Setup Mem access SubDID 12 to access Host memory */
    if (type)
    {
        cvmx_sli_mem_access_subidx_t mem_access_subid;
        mem_access_subid.u64 = 0;
        mem_access_subid.s.port = octeon_pcie_port; /* Port the request is sent to. */
        mem_access_subid.s.nmerge = 1;
        mem_access_subid.s.esr = 1;
        mem_access_subid.s.esw = 1;
        /* PCIe Address Bits. 12-15 for port 0, 16-19 for port 1. */
        mem_access_subid.u64 |= octeon_pcie_port * 4;
        if (octeon_pcie_port)
            target_write_csr(CVMX_PEXP_SLI_MEM_ACCESS_SUBIDX(16), mem_access_subid.u64);
        else
            target_write_csr(CVMX_PEXP_SLI_MEM_ACCESS_SUBIDX(12), mem_access_subid.u64);
    } else {
        cvmx_npei_mem_access_subidx_t mem_access_subid;
        mem_access_subid.u64 = 0;
        mem_access_subid.s.port = octeon_pcie_port; /* Port the request is sent to. */
        mem_access_subid.s.nmerge = 1;
        mem_access_subid.s.esr = 1;
        mem_access_subid.s.esw = 1;
        //mem_access_subid.s.ba = 0;      /* PCIe Adddress Bits <63:34>. */
        target_write_csr(CVMX_PEXP_NPEI_MEM_ACCESS_SUBIDX(12), mem_access_subid.u64);
    }

}

/**
 * Called when octeon_remote_open() is called using a remote spec
 * for PCI.
 *
 * @param remote_spec
 *               String specification of the remote to connect to.
 *
 * @return Zero on success, negative on failure.
 */
static int pcihost_open(const char *remote_spec __attribute__ ((unused)))
{
    extern uint32_t octeon_remote_linux_get_model(void);

    uint32_t my_model = octeon_remote_linux_get_model();

    /* Figure out if the target card I'm running on supports PCI or PCIe. We
        can't use the normal model or feature stuff since those are redirected
        to the host */
    if ((my_model < OCTEON_CN56XX_PASS1_0) || ((my_model & OCTEON_58XX_FAMILY_MASK) == OCTEON_CN50XX_PASS1_0))
    {
        octeon_remote_debug(1, "We're running on a PCI Octeon target\n");
        /* We're on a part with PCI. Use the position of BAR0 to determine
            if we're using big bars */
        uint32_t cfg04 = target_npi_read32(CVMX_NPI_PCI_CFG04);
        if (cfg04 > (1u<<30))
        {
            /* Host must support big bars */
            octeon_remote_debug(1, "The host BAR0 should be at 0\n");
            octeon_pci_bar0_address = 0x00011b0000000000ull;
        }
        else
        {
            /* Only CN31XX and pass 1&2 of CN38XX should be in this config */
            octeon_remote_debug(1, "The host BAR0 should be at 128MB\n");
            octeon_pci_bar0_address = 0x00011b0000000000ull + (128ull<<20);
        }
        pcihost_setup_pci_target();
    }
    else
    {
        octeon_remote_debug(1, "We're running on a PCIe Octeon target\n");
        /* We're on a part with PCIe. Current Octeon chips only support
            target mode on port 0, so the following address is hard coded
            for port 0. Somebody using a non-transparent bridge might need
            to change this, but that would be a unsupported config anyway */
        octeon_pci_bar0_address = 0x00011b0000000000ull | (1ull<<63);

	switch ((my_model>>8) & 0xff) {
            case 4:
                octeon_pci_model = OCTEON_CN56XX_PASS1_0;
                break;
            case 7:
                octeon_pci_model = OCTEON_CN52XX_PASS1_0;
                break;
            case 0x91:
                octeon_pci_model = OCTEON_CN68XX_PASS1_0;
                break;
            case 0x92:
                octeon_pci_model = OCTEON_CN66XX_PASS1_0;
                break;
            case 0x93:
                octeon_pci_model = OCTEON_CN61XX_PASS1_0;
                break;
            case 0x94:
                octeon_pci_model = OCTEON_CNF71XX_PASS1_0;
                break;
            case 0x95:
                /* The SLI CSR are different between pass1.x and pass2.x,
                   set the model based on the BAR1 size.
                        pass1.x - 32KB
                        pass2.x - 8MB
                 */
                if (octeon_pci_bar1_size == 0x8000)
                    octeon_pci_model = OCTEON_CN78XX_PASS1_0;
                else
                    octeon_pci_model = OCTEON_CN78XX_PASS2_0;
                break;
            case 0x96:
                octeon_pci_model = OCTEON_CN70XX_PASS1_0;
                break;
            case 0x90:
            default:
                octeon_pci_model = OCTEON_CN63XX_PASS1_0;
                break;
        }

        /* USE SLI registers for cn63xx and older modelsm else use NPEI. */
        if (my_model >= OCTEON_CN63XX_PASS1_0)
        {
            /* Octeon II has 2 PCIe ports */
            int pcie_qlm_str;
            if ((pcie_qlm_str = getenv("OCTEON_PCIE_QLM")) != NULL)
                octeon_pcie_port = atoi(pcie_qlm_str);
            /* FIXME: the bar0 address when using 2nd PCIe port */
            pcihost_setup_pcie_target(1);

            if (octeon_pcie_port)
                octeon_pci_bar0_address = 0x00011c0000000000ull | (1ull<<63);

            /* Skip the check, the size of BAR0 is 16KB */
            octeon_pci_bar0_size = 16384;
            goto map_bar;
        }
        else
            pcihost_setup_pcie_target(0);
    }

    /* We don't know if the host supports PCI or PCIe as there might be any
        number of bridges on the bus doing conversions. Map the first part
        of BAR0 so we can access sone registers and figure out what is going
        on */
    octeon_pci_bar0_ptr = octeon_remote_map(octeon_pci_bar0_address, 64, &bar0_cookie);
    if (!octeon_pci_bar0_ptr)
        return -1;

    /* Offset 0x20 in BAR0 is a BAR1 index regsiter in PCIe hosts and
        PCI_WIN_RD_DATA on PCI hosts. If we can write -1 and read it back, it
        must be PCI_WIN_RD_DATA since BAR1 has some bits that are RAZ */
    volatile uint64_t *ptr = (volatile uint64_t*)(octeon_pci_bar0_ptr + 0x20);
    uint64_t saved_value = *ptr;
    octeon_remote_debug(2, "Saved BAR0 offset 0x20 = 0x%llx\n", (ULL)saved_value);
    *ptr = 0xffffffffffffffffull;
    octeon_remote_debug(2, "After write BAR0 offset 0x20 = 0x%llx\n", (ULL)*ptr);
    if (*ptr == 0xffff030000000000ull)
    {
        octeon_remote_debug(1, "Host is a PCIe Octeon (16KB BAR0)\n");
        octeon_pci_bar0_size = 16384;
    }
    else
    {
        octeon_remote_debug(1, "Host is a PCI Octeon (4KB BAR0)\n");
        octeon_pci_bar0_size = 4096;
    }
    *ptr = saved_value;

    octeon_remote_unmap(&bar0_cookie);

map_bar:
    /* Map BAR0 */
    octeon_pci_bar0_ptr = octeon_remote_map(octeon_pci_bar0_address, octeon_pci_bar0_size, &bar0_cookie);
    if (!octeon_pci_bar0_ptr)
        return -1;

    setup_globals();
    return 0;
}


/**
 * Called when octeon_remote_close() is called.
 */
static void pcihost_close(void)
{
    octeon_remote_debug_handler_remove();
    octeon_remote_unmap(&bar0_cookie);
    octeon_pci_bar0_ptr = NULL;
}


/**
 * Read data from a physical memory address.
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
static void pcihost_read_mem(void *buffer_ptr, uint64_t physical_address, int length)
{
    octeon_remote_map_cookie_t direct_cookie;
    physical_address += 0x00011b0000000000ull; /* PCI/PCIe offset */
    void *ptr = octeon_remote_map(physical_address, length, &direct_cookie);
    if (!ptr)
        return;
    fast_memcpy(buffer_ptr, ptr, length);
    octeon_remote_unmap(&direct_cookie);
}


/**
 * Write data to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
static void pcihost_write_mem(uint64_t physical_address, const void *buffer_ptr, int length)
{
    octeon_remote_map_cookie_t direct_cookie;
    physical_address += 0x00011b0000000000ull; /* PCI/PCIe offset */
    void *ptr = octeon_remote_map(physical_address, length, &direct_cookie);
    if (!ptr)
        return;
    fast_memcpy(ptr, buffer_ptr, length);
    octeon_remote_unmap(&direct_cookie);
}
#endif


/**
 * Initialize the pointers needed for octeon_remote to work over
 * PCI/PCIe. This doesn't actually create a connection, just setup
 * internal data structures.
 *
 * @param remote_funcs
 *               Function pointers to be populated
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_pcihost(octeon_remote_funcs_t *remote_funcs __attribute__ ((unused)))
{
#ifdef __mips__
    remote_funcs->open = pcihost_open;
    remote_funcs->close = pcihost_close;
    remote_funcs->read_mem = pcihost_read_mem;
    remote_funcs->write_mem = pcihost_write_mem;
    remote_funcs->read_csr = pci_read_csr;
    remote_funcs->write_csr = pci_write_csr;
    remote_funcs->read_csr32 = pci_read_csr32;
    remote_funcs->write_csr32 = pci_write_csr32;
    remote_funcs->get_model = pci_get_model;
    remote_funcs->start_cores = pci_start_cores;
    remote_funcs->stop_cores = pci_stop_cores;
    remote_funcs->get_running_cores = pci_get_running_cores;
    remote_funcs->get_num_cores = pci_get_num_cores;
    remote_funcs->get_core_state = pci_get_core_state;
    remote_funcs->set_core_state = pci_set_core_state;
    remote_funcs->get_sample = pci_get_sample;
    return 0;
#else
    octeon_remote_debug(-1, "PCIHOST is only supported natively on Octeon\n");
    return -1;
#endif
}

