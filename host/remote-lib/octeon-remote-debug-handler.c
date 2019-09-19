/***********************license start************************************
 * Copyright (c) 2009 Cavium Inc. (support@cavium.com). All rights
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
 * This provides a simplied interface to install EJTAG debug exception
 * handlers into Octeon's memory.
 *
 * $Id: octeon-remote-map.h 41588 2009-03-19 19:41:00Z vmalov $
 */
#include <stdlib.h>
#include <stdint.h>
#include "octeon-remote.h"
#include "octeon-remote-debug-handler.h"
#define CVMX_ADD_IO_SEG(a) (a)
#include "cvmx.h"
#include "cvmx-swap.h"

#define ULL unsigned long long

static int installed_handler = -1;
static uint64_t debug_handler_base = 0;

#define PARTN_SZ	16384	/* size of each partition */
#define TOMIPS(foo)	cvmx_cpu_to_be32(foo)

static uint64_t insn64(uint64_t val)
{
    /* Note: here we check for real byte order mode, not for memory mode */
    return octeon_remote_is_le() ?
        (val >> 32) | (val << 32) :
        val;
}

typedef uint32_t mips_insn_t;

static mips_insn_t insn_nop(void) {return (mips_insn_t)0;}

static mips_insn_t insn_syncw(void)
{
	union {
		struct {
			uint32_t	sync:6;
			uint32_t	syncw:5;
			uint32_t	zero:15;
			uint32_t	special:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {15, 4, 0, 0}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_deret(void)
{
	union {
		struct {
			uint32_t	deret:6;
			uint32_t	zero:19;
			uint32_t	co:1;
			uint32_t	cop0:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {31, 0, 1, 16}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_sd(uint32_t rt, uint32_t base, uint32_t offset)
{
	union {
		struct {	/* sd rt, offset(base) */
			uint32_t	offset:16;
			uint32_t	rt:5;
			uint32_t	base:5;
			uint32_t	op:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {offset, rt, base, 63}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_ld(uint32_t rt, uint32_t base, uint32_t offset)
{
	union {
		struct {	/* ld rt, offset(base) */
			uint32_t	offset:16;
			uint32_t	rt:5;
			uint32_t	base:5;
			uint32_t	op:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {offset, rt, base, 55}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_dmfc0(uint32_t rt, uint32_t rd, uint32_t sel)
{
	union {
		struct {	/* dmfc0 rt, rd */
					/* dmfc0 rt, rd, sel */
			uint32_t	sel:3;
			uint32_t	zero:8;
			uint32_t	rd:5;
			uint32_t	rt:5;
			uint32_t	dmf:5;
			uint32_t	op:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {sel, 0, rd, rt, 1, 16}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_lui(uint32_t rt, uint32_t immediate)
{
	union {
		struct {	/* lui rt, immediate */
			uint32_t	immed:16;
			uint32_t	d:5;
			uint32_t	zero:5;
			uint32_t	op:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {immediate, rt, 0, 15}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_ori(uint32_t rs, uint32_t rt, uint32_t immediate)
{
	union {
		struct {	/* ori rt, rs, immediate */
			uint32_t	immediate:16;
			uint32_t	rt:5;
			uint32_t	rs:5;
			uint32_t	op:6;
		} s;
		mips_insn_t	u;
	} insn = {.s = {immediate, rt, rs, 13}};

	return TOMIPS(insn.u);
}

static mips_insn_t insn_dsll(uint32_t rt, uint32_t rd, uint32_t sa)
{
	union {
		struct {	/* dsll rd, rt, sa */
			uint32_t	op:6;
			uint32_t	sa:5;
			uint32_t	rd:5;
			uint32_t	rt:5;
			uint32_t	zero:11;
		} s;
		mips_insn_t	u;
	} insn = {.s = {56, sa, rd, rt, 0}};

	return TOMIPS(insn.u);
}

static void octeon_remote_write_code(uint64_t physical_address, const uint64_t *code, int length)
{
    int offset;
    for (offset = 0; offset < length; offset += 8, code ++) {
        octeon_remote_write_mem64((physical_address + offset),
                insn64(cvmx_be64_to_cpu(*code)));
    }
}

static int fixup_profile_handler(uint64_t prof_start, uint64_t prof_size,
    uint64_t var_addr)
{
	int t0 = 8, k0 = 26, k1 = 27;

	if (!var_addr)
		return -1;

	/*
	 * Overwrite the last part of the profile handler to include the var.
	 */
#define SYNCW_OFFSET	0x6C	/* syncw in the tail is where we start to overwrite */
	mips_insn_t *insn_loc = (mips_insn_t *)(prof_start + SYNCW_OFFSET);

#define INSTALL_INSN(insn) 						\
	do {								\
		*insn_loc = insn;					\
		insn_loc += 1;						\
		if ((uint64_t)insn_loc >= (prof_start + prof_size)) {	\
			fprintf(stderr,	"ERROR:"			\
			    "%s() profile handler space overflow.\n",	\
			    __func__);					\
			return -2;					\
		}							\
	} while(0)


#define INSTALL_INSN_DLI(_r, _val)					\
	do {								\
		uint16_t v63_48, v47_32, v31_16, v15_0;			\
									\
		v15_0 = (uint16_t) ((_val) & 0xFFFFULL);		\
		v31_16 = (uint16_t)(((_val) >> 16) & 0xFFFFULL);	\
		v47_32 = (uint16_t)(((_val) >> 32) & 0xFFFFULL);	\
		v63_48 = (uint16_t)((_val) >> 48);			\
									\
		INSTALL_INSN(insn_lui((_r), v63_48));			\
		INSTALL_INSN(insn_ori((_r), (_r), v47_32));		\
		INSTALL_INSN(insn_dsll((_r), (_r), 16));		\
		INSTALL_INSN(insn_ori((_r), (_r), v31_16));		\
		INSTALL_INSN(insn_dsll((_r), (_r), 16));		\
		INSTALL_INSN(insn_ori((_r), (_r), v15_0));		\
	} while (0)

	INSTALL_INSN(insn_sd(k1, k0, k1 * 8));		/* save k1 */
	INSTALL_INSN(insn_sd(t0, k0, t0 * 8));		/* save t0 */
	INSTALL_INSN_DLI(k1, var_addr);			/* k1 points to var */
	INSTALL_INSN(insn_ld(t0, k1, 0));		/* var is in t0 */
	INSTALL_INSN(insn_sd(t0, k0, PER_CORE_VAR_OFFSET));	/* save t0 */

	/*
	 * The profiler handler tail
	 */
	INSTALL_INSN(insn_ld(k1, k0, k1 * 8));	/* restore k1 */
	INSTALL_INSN(insn_ld(t0, k0, t0 * 8));	/* restore t0 */
	INSTALL_INSN(insn_syncw());		/* syncw */
	INSTALL_INSN(insn_dmfc0(k0, 31, 0));	/* restore k0 */
	INSTALL_INSN(insn_deret());		/* deret */
	INSTALL_INSN(insn_nop());		/* nop */

	return 0;
}

int octeon_remote_debug_handler_install(octeon_remote_debug_handler_t handler, uint64_t data)
{
    extern char octeon_remote_debug_handler_begin[];
    extern char octeon_remote_debug_handler_end[];
    extern char octeon_remote_debug_handler2_begin[];
    extern char octeon_remote_debug_handler2_end[];
    extern char octeon_remote_debug_handler3_begin[];
    extern char octeon_remote_debug_handler3_end[];
    extern char octeon_remote_profile_handler_begin[];
    extern char octeon_remote_profile_handler_end[];

    uint64_t profile_begin = (uint64_t) octeon_remote_profile_handler_begin;
    uint64_t profile_end = (uint64_t) octeon_remote_profile_handler_end;
    uint64_t profile_size = profile_end - profile_begin;

    OCTEON_REMOTE_DEBUG_CALLED("handler=%d", handler);
    if (installed_handler == (int)handler)
    {
        OCTEON_REMOTE_DEBUG_RETURNED("%d - Already installed", 0);
        return 0;
    }

    if (!debug_handler_base)
    {
        uint64_t min_size = 16384 * (cvmx_octeon_num_cores() + 1);
        uint64_t block_size;
        if (octeon_remote_named_block_find("octeon_remote_scratch", &debug_handler_base, &block_size))
        {
            if (block_size < min_size)
            {
                octeon_remote_debug(-1, "Named block \"octeon_remote_scratch\" is too small (%llu, must be %llu).\n",
                    (ULL)block_size, (ULL)min_size);
                return -1;
            }
            /* Block already exists and we are good to go */
        }
        else
        {
            debug_handler_base = octeon_remote_named_block_alloc(min_size, 0, 0, 16384, "octeon_remote_scratch");
            if (!debug_handler_base)
            {
                const char *address = getenv("OCTEON_REMOTE_SCRATCH_ADDRESS");
                if (address)
                {
                    sscanf(address, "%lli", (ULL*)&debug_handler_base);
                    octeon_remote_debug(2, "Using scratch memory address from OCTEON_REMOTE_SCRATCH_ADDRESS of 0x%llx\n", (ULL)debug_handler_base);
                }
                else
                {
                    octeon_remote_debug(-1, "Unable to determine scratch memory address. Set OCTEON_REMOTE_SCRATCH_ADDRESS to the address dedicated for remote utility use.\n");
                    return -1;
                }
            }
        }

	fixup_profile_handler(profile_begin, profile_size, data);

        octeon_remote_debug(2, "Copying profile handler to 0x%llx\n", (ULL)debug_handler_base);
        octeon_remote_write_code(debug_handler_base, (const void *)profile_begin, profile_size);

        octeon_remote_debug(2, "Copying debug handler to 0x%llx\n", (ULL)debug_handler_base + profile_size);
        if (OCTEON_IS_OCTEON2())
            octeon_remote_write_code(debug_handler_base + profile_size,
                (const void *)octeon_remote_debug_handler2_begin,
                (uintptr_t)octeon_remote_debug_handler2_end - (uintptr_t)octeon_remote_debug_handler2_begin);
        else if (OCTEON_IS_OCTEON3())
            octeon_remote_write_code(debug_handler_base + profile_size,
                (const void *)octeon_remote_debug_handler3_begin,
                (uintptr_t)octeon_remote_debug_handler3_end - (uintptr_t)octeon_remote_debug_handler3_begin);
        else
            octeon_remote_write_code(debug_handler_base + profile_size,
                (const void *)octeon_remote_debug_handler_begin,
                (uintptr_t)octeon_remote_debug_handler_end - (uintptr_t)octeon_remote_debug_handler_begin);

        /* Fixup the address calculations in the profile handler */
        uint64_t core_base = debug_handler_base + 16384;
        octeon_remote_write_mem64(debug_handler_base + 16, insn64(0x675a0000003ad3faull | ((core_base<<32)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + 24, insn64(0x675a0000003ad3faull | ((core_base<<17)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + 32, insn64(0x675a0000003ad3faull | ((core_base<< 2)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + 40, insn64(0x675a0000003ad3faull | ((core_base>>13)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + 48, insn64(0x675a0008003ad13aull | ((core_base>>28)&0xf00000000)));

        /* Fixup the address calculations in the debug handler */
        octeon_remote_write_mem64(debug_handler_base + profile_size + 16, insn64(0x675a0000003ad3faull | ((core_base<<32)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + profile_size + 24, insn64(0x675a0000003ad3faull | ((core_base<<17)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + profile_size + 32, insn64(0x675a0000003ad3faull | ((core_base<< 2)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + profile_size + 40, insn64(0x675a0000003ad3faull | ((core_base>>13)&0x7fff00000000)));
        octeon_remote_write_mem64(debug_handler_base + profile_size + 48, insn64(0x675a0008003ad13aull | ((core_base>>28)&0xf00000000)));

        octeon_remote_debug(2, "Installing bootbus region 1\n");
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_CFGX(1), (1ull << 31) | (0x1fc00480 >> 4));
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0x80);  /* Auto increments after write */
        /* Most sig. word executes first */
        /*      dmtc0   ra, $COP0_DESAVE    40bff800 */
        /*      bal     1f                  04110001 */
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, insn64(0x40bff80004110001ull));
        /*       nop                        00000000 */
        /* 1:   ld      ra, 12(ra)          dfff000c */
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, insn64(0x00000000dfff000cull));
        /*      jr      ra                  03e00008 */
        /*      dmfc0  ra, $COP0_DESAVE     403ff800 */
        octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, insn64(0x03e00008403ff800ull));

/* #define DUMP_PROF_HANDLER */
#ifdef DUMP_PROF_HANDLER
	{
	    int fd = 2; /* stderr */
	    if (write(fd, profile_begin, profile_size) != 
	        profile_size) {
		    printf("Error: dump_prof_handler\n");
		    exit(1);
	    }
	}
#endif
    }

    uint64_t address = debug_handler_base;
    if (handler != OCTEON_REMOTE_PROFILE_HANDLER)
        address += profile_size;
    octeon_remote_debug(2, "Writing secondary handler address 0x%llx\n", (1ull<<63) | address);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0x98);
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_DAT, (1ull<<63) | address);
    octeon_remote_read_csr(CVMX_MIO_BOOT_LOC_DAT);  /* Ensure write has completed */

    /* For the handler other than the profile one we must set the core stop
        mode. A value of "2" in the R0 storage location tells the low level
        stub to continue after saving the registers. A value of "0" tells
        it to stop, writing a "1" to this loc, and wait for someone to write
        a zero */
    if (handler != OCTEON_REMOTE_PROFILE_HANDLER)
    {
        int stop_type = (handler == OCTEON_REMOTE_SAVE_HANDLER) ? 2 : 0;
        int num_cores = cvmx_octeon_num_cores();
        int core;
        for (core=0; core<num_cores; core++)
            octeon_remote_write_mem64(debug_handler_base + (core+1) * 16384, stop_type);
    }

    installed_handler = handler;
    OCTEON_REMOTE_DEBUG_RETURNED("%d", 0);
    return 0;
}

int octeon_remote_debug_handler_remove(void)
{
    OCTEON_REMOTE_DEBUG_CALLED("");
    if (installed_handler == -1)
    {
        OCTEON_REMOTE_DEBUG_RETURNED("%d - Nothing to do", 0);
        return 0;
    }
    octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_CFGX(1), 0);
    installed_handler = -1;
    OCTEON_REMOTE_DEBUG_RETURNED("%d", 0);
    return 0;
}

uint64_t octeon_remote_debug_handler_get_base(int core)
{
    uint64_t result;
    OCTEON_REMOTE_DEBUG_CALLED("core=%d", core);
    if ((installed_handler==-1) || !debug_handler_base)
    {
        octeon_remote_debug(-1, "Debug handler not installed\n");
        OCTEON_REMOTE_DEBUG_RETURNED("%d", -1);
        return 0;
    }

    result = debug_handler_base + (core+1) * 16384;
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)result);
    return result;
}

int octeon_remote_debug_handler_is_installed(void)
{
    int result;
    OCTEON_REMOTE_DEBUG_CALLED("");
    result = (installed_handler != -1);
    OCTEON_REMOTE_DEBUG_RETURNED("%d", result);
    return result;
}

