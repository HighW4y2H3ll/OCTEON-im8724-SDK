/*
 * (C) Copyright 2000
 * Subodh Nijsure, SkyStream Networks, snijsure@skystream.com
 *
 * (C) Copyright 2011
 * Cavium Inc., Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-core.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/octeon-model.h>

static struct
{
	uint reg:5;
	uint sel:5;
	/* Bitmap of OCTEON processors supported */
	uint family:22; /* 0 = all, 1 = 3/5xxx, 2 = 6xxx, 4 = 7xxx */
	const char *name;
} COP0_REGS[] = {
	{0, 0, 7, "Index"},
	{1, 0, 7, "Random"},
	{2, 0, 7, "EntryLo0"},
	{3, 0, 7, "EntryLo1"},
	{4, 0, 7, "Context"},
	{4, 2, 6, "UserLocal"},
	{5, 0, 7, "PageMask"},
	{5, 1, 7, "PageGrain"},
	{6, 0, 7, "Wired"},
	{7, 0, 7, "HWREna"},
	{8, 0, 7, "BadVAddr"},
	{8, 1, 4, "BadInstr"},
	{8, 2, 4, "BadInstrP"},
	{9, 0, 7, "Count"},
	{9, 6, 6, "CvmCount"},
	{9, 7, 6, "CvmCtl"},
	{10, 0, 7, "EntryHi"},
	{11, 0, 7, "Compare"},
	{11, 6, 6, "PowThrottle"},
	{11, 7, 6, "CvmMemCtl"},
	{12, 0, 7, "Status"},
	{12, 1, 7, "IntCtl"},
	{12, 2, 7, "SRSCtl"},
	{12, 6, 4, "GuestCtl0"},
	{12, 7, 4, "GTOffset"},
	{13, 0, 7, "Cause"},
	{14, 0, 7, "EPC"},
	{15, 0, 7, "PRId"},
	{15, 1, 7, "Ebase"},
	{16, 0, 7, "Config"},
	{16, 1, 7, "Config1"},
	{16, 2, 7, "Config2"},
	{16, 3, 7, "Config3"},
	{16, 4, 6, "Config4"},
	{16, 5, 4, "Config5"},
	{16, 6, 4, "vmMemCtl2"},
	{16, 7, 4, "CvmVMConfig"},
	{18, 0, 7, "WatchLo0"},
	{18, 1, 7, "WatchLo1"},
	{19, 0, 7, "WatchHi0"},
	{19, 1, 7, "WatchHi1"},
	{20, 0, 7, "XContext"},
	{22, 0, 6, "MulticoreDebug"},
	{22, 2, 4, "CvmCountOffset"},
	{23, 0, 7, "Debug"},
	{23, 6, 6, "Debug2"},
	{24, 0, 7, "DEPC"},
	{25, 0, 7, "PerfControl0"},
	{25, 1, 7, "PerfCount0"},
	{25, 2, 7, "PerfControl1"},
	{25, 3, 7, "PerfCount1"},
	{25, 4, 4, "PerfControl2"},
	{25, 5, 4, "PerfCount2"},
	{25, 6, 4, "PerfControl3"},
	{25, 7, 4, "PerfCount3"},
	{26, 0, 4, "ErrCtl"},
#ifndef CONFIG_OCTEON_SIM
	{27, 0, 7, "IcacheError"},
	{27, 1, 7, "DcacheError"},
	{27, 2, 4, "IcacheDebug"},
	{28, 0, 7, "IcacheTagLo"},
	{28, 1, 7, "IcacheDataLo"},
	{28, 2, 7, "DcacheTagLo"},
	{28, 3, 7, "DcacheDataLo"},
	{29, 1, 7, "IcacheDataHi"},
	{29, 2, 7, "DcacheTagHi"},
	{29, 3, 7, "DcacheDataHi"},
#endif
	{30, 0, 7, "ErrorEPC"},
	{31, 0, 7, "DESAVE"},
	{31, 2, 6, "Kscratch1"},
	{31, 3, 6, "Kscratch2"},
	{31, 4, 6, "Kscratch3"},
	{31, 5, 4, "Kscratch4"},
	{0, 0, 0, NULL}
};

typedef struct {
	u64 regs[2][256];
	u64 tlb[256][4];

} octeon_remote_registers_t;

octeon_remote_registers_t regs;

int do_octreginfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	octeon_remote_registers_t regs;
	int	r, half, num_cop0;
	int	num_tlb_entries = cvmx_core_get_tlb_entries();
	int	family;
	int	i;

	memset(&regs, 0, sizeof(regs));

	printf("Coprocessor 0 registers:\n");

	if (OCTEON_IS_OCTEON3())
		family = 4;
	else if (OCTEON_IS_OCTEON2())
		family = 2;
	else
		family = 1;

	num_cop0 = 0;
	i = 0;
	debug("Family is %d\n", family);
	while (COP0_REGS[i].name)
		if (COP0_REGS[i++].family & family)
			num_cop0++;

	debug("%d cop0 registers\n", num_cop0);
	debug("Index\n");
	/* Offset = (reg * 8) + sel */
	regs.regs[1][0]   = read_c0_index();		/* Index */
	regs.regs[1][8]   = read_c0_random();		/* Random */
	regs.regs[1][16]  = read_64bit_c0_entrylo0();	/* EntryLo0 */
	regs.regs[1][24]  = read_64bit_c0_entrylo1();	/* EntryLo1 */
	regs.regs[1][32]  = read_64bit_c0_context();	/* Context */
	regs.regs[1][40]  = read_c0_pagemask();		/* PageMask */
	regs.regs[1][41]  = read_c0_pagegrain();	/* PageGrain */
	regs.regs[1][48]  = read_c0_wired();		/* Wired */
	regs.regs[1][56]  = read_c0_hwrena();		/* HWREna */
	regs.regs[1][64]  = read_64bit_c0_badvaddr();	/* BadVAddr */
	regs.regs[1][72]  = read_c0_count();		/* Count */
	regs.regs[1][78] = read_64bit_c0_cvmcount();	/* CvmCount */
	regs.regs[1][79] = read_64bit_c0_cvmctl();		/* CvmCtl */
	regs.regs[1][80]  = read_64bit_c0_entryhi();	/* EntryHi */
	regs.regs[1][88]  = read_c0_compare();		/* Compare */
	regs.regs[1][96]  = read_c0_status();		/* Status */
	regs.regs[1][97]  = read_c0_intctl();		/* IntCtl */
	regs.regs[1][98]  = read_c0_srsctl();		/* SRSCtl */
	regs.regs[1][104] = read_c0_cause();		/* Cause */
	regs.regs[1][112] = read_64bit_c0_epc();		/* EPC */
	regs.regs[1][120] = read_c0_prid();		/* PRid */
	regs.regs[1][121] = read_c0_ebase();		/* EBase */
	regs.regs[1][128] = read_c0_config();		/* Config 0 */
	regs.regs[1][129] = read_c0_config1();		/* Config 1 */
	regs.regs[1][130] = read_c0_config2();		/* Config 2 */
	regs.regs[1][131] = read_c0_config3();		/* Config 3 */
	regs.regs[1][144] = read_64bit_c0_watchlo0();	/* WatchLo0 */
	regs.regs[1][145] = read_64bit_c0_watchlo1();	/* WatchLo1 */
	regs.regs[1][152] = read_64bit_c0_watchhi0();	/* WatchLo0 */
	regs.regs[1][153] = read_64bit_c0_watchhi1();	/* WatchLo1 */
	regs.regs[1][160] = read_64bit_c0_xcontext();	/* XContext */
	regs.regs[1][184] = read_c0_debug();		/* Debug */
	regs.regs[1][192] = read_64bit_c0_depc();	/* Debug Exception PC */
	regs.regs[1][200] = read_c0_perfctrl0();	/* PerfControl0 */
	regs.regs[1][201] = read_64bit_c0_perfcntr0();	/* PerfCount0 */
	regs.regs[1][202] = read_c0_perfctrl1();	/* PerfControl1 */
	regs.regs[1][203] = read_64bit_c0_perfcntr1();	/* PerfCount1 */
#ifndef CONFIG_OCTEON_SIM
	regs.regs[1][216] = read_64bit_c0_cacheerr();	/* IcacheError */
	regs.regs[1][217] = read_64bit_c0_dcacheerr();	/* DcacheError */
	regs.regs[1][224] = read_64bit_c0_taglo();	/* IcacheTagLo */
	regs.regs[1][225] = read_64bit_c0_icache_datalo();/*IcacheDataLo */
	regs.regs[1][226] = read_64bit_c0_dtaglo();	/* DcacheTagLo */
	regs.regs[1][227] = read_64bit_c0_dcache_datalo();/*DcacheDataLo */
	regs.regs[1][233] = read_64bit_c0_icache_datahi();/*IcacheDataHi */
	regs.regs[1][234] = read_64bit_c0_dtaghi();	/* DcacheTagHi */
	regs.regs[1][235] = read_64bit_c0_dcache_datahi();/*DcacheDataHi */
#endif
	if (family & 6) {
		debug("errorepc\n");
		regs.regs[1][240] = read_64bit_c0_errorepc();	/* ErrorEPC */
		regs.regs[1][248] = read_64bit_c0_desave();	/* DESAVE */
		regs.regs[1][79]  = read_64bit_c0_cvmctl();	/* CvmCtl */
		regs.regs[1][95]  = read_64bit_c0_cvmmemctl();	/* CvmMemCtl */
		regs.regs[1][78]  = read_64bit_c0_cvmcount();	/* CvmCount */
		regs.regs[1][176] = read_64bit_c0_mcdebug();	/* MultiCoreDebug */
		regs.regs[1][34]  = read_64bit_c0_userlocal();	/* UserLocal */
		regs.regs[1][132] = read_c0_config4();		/* Config4 */
		regs.regs[1][94]  = read_64bit_c0_pwrthr();	/* PwrThr */
		regs.regs[1][190] = read_64bit_c0_debug2();	/* Debug2 */
		regs.regs[1][250] = read_64bit_c0_kscratch1();	/* Kscratch1 */
		regs.regs[1][251] = read_64bit_c0_kscratch2();	/* Kscratch1 */
		regs.regs[1][252] = read_64bit_c0_kscratch3();	/* Kscratch1 */
	}
	if (family & 4) {
		debug("badinstr\n");
		regs.regs[1][65] = read_c0_badinstr();	/* BadInstr */
		regs.regs[1][66] = read_c0_badinstrp();	/* BadInstrP */
		regs.regs[1][84] = read_c0_guestctl1();	/* GuestCtl1 */
		regs.regs[1][85] = read_c0_guestctl2();	/* GuestCtl2 */
		regs.regs[1][102] = read_c0_guestctl0();/* GuestCtl0 */
		regs.regs[1][103] = read_c0_gtoffset();	/* GTOffset */
		regs.regs[1][133] = read_c0_config5();	/* Config5 */
		regs.regs[1][134] = read_64bit_c0_vmmemctl2();	/* vmMemCtl2 */
		regs.regs[1][135] = read_64bit_c0_cvm_vm_config();/* cvmVmConfig */
		regs.regs[1][178] = read_64bit_c0_cvmcount_offset();/* CvmCountOffset */
		regs.regs[1][204] = read_c0_perfctrl2();	/* PerfControl0 */
		regs.regs[1][205] = read_c0_perfcntr2();	/* PerfCount0 */
		regs.regs[1][206] = read_c0_perfctrl3();	/* PerfControl1 */
		regs.regs[1][207] = read_c0_perfcntr3();	/* PerfCount1 */
		regs.regs[1][208] = read_c0_ecc();		/* ErrCtl */
		regs.regs[1][218] = read_c0_cachedebug();	/* CacheDebug */
		regs.regs[1][253] = read_64bit_c0_kscratch4();	/* KScratch4 */
	}

	half = (num_cop0 + 1) / 2;
	for (r = 0; r < half; r++) {
		if (COP0_REGS[r].family & family)
			printf("  ($%2u,%u) %-9s 0x%016llx",
			       COP0_REGS[r].reg, COP0_REGS[r].sel,
			       COP0_REGS[r].name,
			       regs.regs[1][COP0_REGS[r].reg * 8
					+ COP0_REGS[r].sel]);
		else
			for (i = 0; i < 38; i++)
				putc(' ');
		if ((r + half < num_cop0) &&
		    (COP0_REGS[r + half].family & family))
			printf("  ($%2u,%u) %-14s 0x%016llx\n",
			       COP0_REGS[r+half].reg, COP0_REGS[r+half].sel,
			       COP0_REGS[r+half].name,
			       regs.regs[1][COP0_REGS[r+half].reg * 8
					+ COP0_REGS[r+half].sel]);
		else if (r + half < num_cop0)
			puts("\n");

	}
	if (num_cop0 & 1)
		puts("\n");

	num_tlb_entries = cvmx_core_get_tlb_entries();
	for (r = 0; r < num_tlb_entries; r++) {
		write_c0_index(r);
		tlb_read();
		regs.tlb[r][0] = read_64bit_c0_entryhi();
		regs.tlb[r][1] = read_c0_pagemask();
		regs.tlb[r][2] = read_64bit_c0_entrylo0();
		regs.tlb[r][3] = read_64bit_c0_entrylo1();
	}
	printf("TLB:\n");
	for (r = 0; r < num_tlb_entries; r++) {
		uint64_t va = regs.tlb[r][0] >> 12 << 12;
		uint8_t  asid       = regs.tlb[r][0] & 0xff;
		uint64_t pagemask   = (regs.tlb[r][1] | 0x1fff) >> 1;
		int      pagesize   = 1 << (cvmx_pop(pagemask) - 10);
		uint64_t page0      = regs.tlb[r][2] >> 6 << 12;
		int      cache0     = (regs.tlb[r][2] >> 3) & 7;
		int      dirty0     = (regs.tlb[r][2] >> 2) & 1;
		int      valid0     = (regs.tlb[r][2] >> 1) & 1;
		int      glbl0      = regs.tlb[r][2] & 1;
		int      ri0        = (regs.tlb[r][2] >> 63) & 1;
		int      xi0        = (regs.tlb[r][2] >> 62) & 1;
		uint64_t page1      = regs.tlb[r][3] >> 6 << 12;
		int      cache1     = (regs.tlb[r][3] >> 3) & 7;
		int      dirty1     = (regs.tlb[r][3] >> 2) & 1;
		int      valid1     = (regs.tlb[r][3] >> 1) & 1;
		int      glbl1      = regs.tlb[r][3] & 1;
		int      ri1        = (regs.tlb[r][3] >> 63) & 1;
		int      xi1        = (regs.tlb[r][3] >> 62) & 1;

		if ((va>>62) == 3)
			va |= 0x3ffe000000000000ull;

		if (regs.tlb[r][2] || regs.tlb[r][3])
			printf("%2d: Virtual=0x%016llx\n"
			       "\tPage0=0x%09llx,C=%d,D=%d,V=%d,G=%d,RI=%d,XI=%d\n"
			       "\tPage1=0x%09llx,C=%d,D=%d,V=%d,G=%d,RI=%d,XI=%d\n"
			       "\tASID=%3d Size=%dKB\n",
			       r, va,
			       page0, cache0, dirty0, valid0, glbl0, ri0, xi0,
			       page1, cache1, dirty1, valid1, glbl1, ri1, xi1,
			       asid, pagesize);
	}

	return 0;
}

/**************************************************/

#if defined(CONFIG_CMD_OCTEON_REGINFO)
U_BOOT_CMD(
	octreginfo,	2,	1,	do_octreginfo,
	"print register information",
	""
);
#endif
