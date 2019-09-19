/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.  

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries. 

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/


#include "cvmx.h"
#include "cvmx-pcie.h"
#include "cvmx-interrupt.h"
#include "cvmx-key.h"

#define MAX_NUM_PCIE 2
#define MAX_NUM_BUSSES 16
#define MAX_NUM_DEVICES 32
#define MAX_NUM_FUNCS 8

#define CONFIG_VENDOR_DEVICE_ID 0x0
#define CONFIG_CLASS_CODE_HIGH  0xb
#define CONFIG_PCI_HEADER_TYPE  0xe
#define CONFIG_PRIMARY_BUS      0x18
#define CONFIG_SECONDARY_BUS    0x19
#define CONFIG_SUBORDINATE_BUS  0x1a

#define CONFIG_CLASS_BRIDGE     0x6

static char force_skip[MAX_NUM_PCIE][MAX_NUM_BUSSES][MAX_NUM_DEVICES][MAX_NUM_FUNCS];
static int reboots = 0;
static int except;
static int do_bus(int pcie_port, int bus, int indent);

static void exception_handler(uint64_t registers[32])
{
    except = 1;
}

static void do_pcie_reset(int pcie_port)
{
    int bus;
    int device;
    int func;
    int key_loc;

    //printf("Clearing key memory\n");
    for (key_loc=0; key_loc<CVMX_KEY_MEM_SIZE; key_loc+=8)
        cvmx_key_write(key_loc, 0);
    //printf("Writing state\n");
    for (pcie_port=0; pcie_port<MAX_NUM_PCIE; pcie_port++)
    {
        for (bus=0; bus<MAX_NUM_BUSSES; bus++)
        {
            for (device=0; device<MAX_NUM_DEVICES; device++)
            {
                for (func=0; func<MAX_NUM_FUNCS; func++)
                {
                    if (force_skip[pcie_port][bus][device][func])
                    {
                        int key_loc = (device>>3) | (bus<<2) | (pcie_port<<6);
                        //printf("Saving %d:%02d:%02d.%d loc=0x%x\n", pcie_port, bus, device, func, key_loc);
                        uint64_t v = cvmx_key_read(key_loc*8);
                        v |= 1ull << (func + 8*(device&7));
                        cvmx_key_write(key_loc*8, v);
                    }
                }
            }
        }
    }
    //printf("Writing magic\n");
    cvmx_key_write(8192-16, reboots+1);
    cvmx_key_write(8192-8, 0xdeadbeef);
    printf("Reseting\n");
    fflush(NULL);
    CVMX_SYNCS;
    cvmx_wait(1000);
    cvmx_write_csr(CVMX_CIU_SOFT_BIST, 0);
    cvmx_read_csr(CVMX_CIU_SOFT_BIST);
    while (1)
    {
        cvmx_read_csr(CVMX_CIU_SOFT_RST);
        cvmx_write_csr(CVMX_CIU_SOFT_RST, 1);
    }
#if 0
    // FIXME: This doesn't work
    cvmx_pcie_rc_shutdown(pcie_port);
    if (cvmx_pcie_rc_initialize(pcie_port))
    {
        printf("Failed to init PCIe port %d\n", pcie_port);
        abort();
    }
#endif
}

static void read_state(void)
{
    int pcie_port;
    int bus;
    int device;
    int func;

    /* Start off assuming we won't need to skip any devices */
    memset(force_skip, 0, sizeof(force_skip));
    printf("Checking for init_data\n");
    if (cvmx_key_read(8192-8) != 0xdeadbeef)
    {
        printf("No init_data\n");
        return;
    }
    printf("Found init_data\n");
    reboots = cvmx_key_read(8192-16);
    printf("On reboot number %d\n", reboots);
    for (pcie_port=0; pcie_port<MAX_NUM_PCIE; pcie_port++)
    {
        for (bus=0; bus<MAX_NUM_BUSSES; bus++)
        {
            for (device=0; device<MAX_NUM_DEVICES; device++)
            {
                for (func=0; func<MAX_NUM_FUNCS; func++)
                {
                    int key_loc = (device>>3) | (bus<<2) | (pcie_port<<6);
                    uint64_t v = cvmx_key_read(key_loc*8);
                    if (v & (1ull << (func + 8*(device&7))))
                    {
                        //printf("Restoring %d:%02d:%02d.%d loc=0x%x\n", pcie_port, bus, device, func, key_loc);
                        force_skip[pcie_port][bus][device][func] = 1;
                    }
                }
            }
        }
    }
    cvmx_key_write(8192-8, 0);
}

static int do_device(int pcie_port, int bus, int device, int num_busses, int indent)
{
    int func;
    int found_multi = 0;

    for (func=0; func<MAX_NUM_FUNCS; func++)
    {
	uint32_t h, v;
	uint8_t b;
        /* Skip reads that are already know to cause CN56XX pass 1 to bus
            error. As an optimization also skip higher functions on the same
            device */
        if (force_skip[pcie_port][bus][device][func])
            break;
        /* Read the vendor and device id twice. We do it twice because a
            CN56XX pass 1 errata can cause the read for a missing device to
            either return bad data or bus error. If the first read returns
            bad data, the second is guaranteed to cause a bus error */
        except = 0;
        cvmx_wait_usec(10000);
        v = cvmx_pcie_config_read32(pcie_port, bus, device, func, CONFIG_VENDOR_DEVICE_ID);
        if (!except)
            v = cvmx_pcie_config_read32(pcie_port, bus, device, func, CONFIG_VENDOR_DEVICE_ID);
        /* If an exception occured, then we need to mark this location bad and
            redo the complete bus scan */
        if (except)
        {
            printf("%*s%d:%02d:%02d.%d Fail\n", indent,"", pcie_port, bus, device, func);
            force_skip[pcie_port][bus][device][func] = 1;
            do_pcie_reset(pcie_port);
            return -1;
        }

	if (func && !found_multi)
            continue;

        /* Nothing here, skip the rest of the functions */
        if (v == 0xffffffff)
            break;

        h = cvmx_pcie_config_read8(pcie_port, bus, device, func, CONFIG_PCI_HEADER_TYPE);
	if (!func)
           found_multi = h & 0x80;

        /* Check if this is a bridge */
        b = cvmx_pcie_config_read8(pcie_port, bus, device, func, CONFIG_CLASS_CODE_HIGH);
        if (b == CONFIG_CLASS_BRIDGE)
        {
            int r;
            printf("%*s%d:%02d:%02d.%d 0x%08x Bridge\n", indent,"", pcie_port, bus, device, func, (unsigned int)v);
            /* Setup the bridge to allow us to scan the stuff behind it */
            cvmx_pcie_config_write8(pcie_port, bus, device, func, CONFIG_PRIMARY_BUS, bus);
            cvmx_pcie_config_write8(pcie_port, bus, device, func, CONFIG_SECONDARY_BUS, bus+num_busses+1);
            cvmx_pcie_config_write8(pcie_port, bus, device, func, CONFIG_SUBORDINATE_BUS, 255);
            cvmx_pcie_config_read8(pcie_port, bus, device, func, CONFIG_SUBORDINATE_BUS);
            /* Scan behind the bridge */
            r = do_bus(pcie_port, bus+num_busses+1, indent+2);
            /* We get a -1 if a bus error requires a full rescan */
            if (r == -1)
                return -1;
            /* Update the max bus behind the bridge based on the scan results */
            cvmx_pcie_config_write8(pcie_port, bus, device, func, CONFIG_SUBORDINATE_BUS, bus+num_busses+r);
            cvmx_pcie_config_read8(pcie_port, bus, device, func, CONFIG_SUBORDINATE_BUS);
            num_busses += r;
        }
        else
            printf("%*s%d:%02d:%02d.%d 0x%08x\n", indent,"", pcie_port, bus, device, func, (unsigned int)v);
    }
    return num_busses;
}

static int do_bus(int pcie_port, int bus, int indent)
{
    int device;
    int num_busses = 0;
    for (device=0; device<MAX_NUM_DEVICES; device++)
    {
        num_busses = do_device(pcie_port, bus, device, num_busses, indent);
        /* We get a -1 if a bus error requires a full rescan */
        if (num_busses == -1)
            return -1;
    }
    return num_busses+1;
}

int main()
{
    uint64_t cvmmemctl;
    int pcie_port;

    cvmx_user_app_init();
    if (cvmx_sysinfo_get()->init_core != cvmx_get_core_num())
        return 0;

    /* Install an exception handler to catch bus errors caused by an errata
        for CN56XX pass 1 */
    cvmx_interrupt_set_exception(exception_handler);

    /* Shorten the DID timeout so bus errors for PCIe config reads
       from non existent devices happen faster */
    asm volatile ("dmfc0 %[rt],$11,7" : [rt] "=r" (cvmmemctl));
    cvmmemctl &= ~(3ull<<21);
    cvmmemctl |= 2ull<<21;
    asm volatile ("dmtc0 %[rt],$11,7" :: [rt] "r" (cvmmemctl));

    read_state();

    for (pcie_port=0; pcie_port<MAX_NUM_PCIE; pcie_port++)
    {
        int num_busses;
        if (cvmx_pcie_rc_initialize(pcie_port))
            continue;
        printf("Probing PCIe port %d\n", pcie_port);
        cvmx_wait_usec(1000000);
        do
        {
            num_busses = do_bus(pcie_port, 1, 2);
        } while (num_busses == -1);
        printf("PCIe port %d had %d busses\n", pcie_port, num_busses);
    }
    printf("Enumeration took %d reboots\n", reboots);
    return 0;
}

