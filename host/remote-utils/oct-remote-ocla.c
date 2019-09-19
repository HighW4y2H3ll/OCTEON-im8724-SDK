/***********************license start************************************
 * Copyright (c) 2003-2014 Cavium Inc. (support@cavium.com). All rights
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
/*
 * @file
 *
 * Use the OCLA unit for tracing an Octeon
 *
 * $Id$
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

#include "cvmx.h"
#include "octeon-remote.h"
#undef CVMX_ADD_IO_SEG
#define CVMX_ADD_IO_SEG(a) (a)  /* There aren't any Mips IO segments when using PCI */
#include "cvmx-csr.h"
#include "cvmx-l2c.h"
#include "cvmx-version.h"
#include "cvmx-ocla.h"

#define U64 unsigned long long

/* All names are this long */
#define NAME_LEN		24

#define READ_BUF_SIZE		(256 * 8)

/*
 * Maximum ddr buffer size guaranteed not to overflow.
 * Ocla can be configured to stop capturing after up to 65536 entries (see
 * OCLA_FIFO_TRIG[LIMIT]). Each captured entry is 38 bits. Ocla writes one cache
 * line at a time to ddr. Each cache line is 128 bytes and contains 26 entries. 
 */
#define MAX_DDR_SIZE_NO_OVF	((65536 / 26) * CVMX_CACHE_LINE_SIZE)

/* Must be a multiple of cache lines */
#define DFLT_DDR_BUF_SIZE	MAX_DDR_SIZE_NO_OVF

/* Driver node */
#define NODE_NAME		"/dev/ocla"

#define MATCH_LOWER_DTX_70XX	"l2c_cbc,0,0,0x300500,0xfffffffff"
#define MATCH_UPPER_DTX_70XX	"l2c_cbc,0,1,0x300502,0xfffffffff"

#define MATCH_LOWER_DTX_78XX	"l2c_cbc,%d,0,0x50000,0xfffffffff"
#define MATCH_UPPER_DTX_78XX	"l2c_cbc,%d,1,0x50002,0xfffffffff"

/* Trigger mode definitions */
#define TRIG_NONE		0
#define TRIG_START		1
#define TRIG_STOP		2

#define MAX_MATCHERS		4
#define MAX_FSMS		2
#define MAX_FSM_STATES		16
#define MAX_NUM_DTX		16
#define ALL_CMD			0xfe

#define WAIT_CAPTURE_DONE						       \
	"\n"								       \
	"\n"								       \
	"        Capture in progess. Waiting for capture buffer to fill...\n"  \
	"\n"								       \
	"        Press CTRL-C to stop.\n"				       \
	"\n"								       \
	"\n"

#define USAGE								       \
	"\n"								       \
	"Cavium Octeon On Chip Logic Analyzer (OCLA)\n"			       \
	"\n"								       \
	"Usage:\n"							       \
	"    %s [options]\n"						       \
	"\n"								       \
	"Supported Options:\n"						       \
	"    -c --complex=<complex>\n"					       \
	"        Ocla complex to use (defaults to 0).\n"		       \
	"    -b --buffer=<size>\n"					       \
	"        DDR buffer size. Must be a multiple of 128 bytes and\n"       \
	"        larger than 4095 bytes (defaults to %d).\n"		       \
	"    -d --dtx=<dtx-desc>\n"					       \
	"        Specify a dtx to use. Only used for signal filters.\n"	       \
	"    -f --filter=<match|signal>\n"				       \
	"        Set the filter for the events to capture.\n"		       \
	"    -h --help\n"						       \
	"        Display this usage.\n"					       \
	"    -H --long-help=<argument>\n"				       \
	"        Display usage for a compounded argument.\n"		       \
	"        complex = Display complex usage.\n"			       \
	"        dtx = Display dtx-desc usage.\n"			       \
	"        match = Display match usage.\n"			       \
	"        signal = Display signal usage.\n"			       \
	"        command = Display command descriptions.\n"		       \
	"        examples = Display usage examples.\n"			       \
	"    -m --trig-mode=[none|start|stop]\n"			       \
	"        Set the trigger mode.\n"				       \
	"        none = Trigger is ignored (default).\n"		       \
	"        start = Trigger signals when to start capturing.\n"	       \
	"        stop = Trigger signals when to stop capturing.\n"	       \
	"    -n --node=<node>\n"					       \
	"        Node to use (default to 0).\n"				       \
	"    -r --reset\n"						       \
	"        Reset (disable) all DTXs.\n"				       \
	"    -t --trig=<match|signal>\n"				       \
	"        Set the trigger.\n"					       \
	"    -v --version\n"						       \
	"\n"

#define COMPLEX_USAGE_70XX						       \
	"        Complex0: cores 0, and 1\n"				       \
	"\n"

#define COMPLEX_USAGE_73XX						       \
	"        Complex0: cores 0, 4, 8, and 12\n"			       \
	"        Complex1: cores 1, 5, 9, and 13\n"			       \
	"        Complex2: ROC\n"					       \
	"        Note that only 8 cores can be traced\n"		       \
	"\n"

#define COMPLEX_USAGE_78XX						       \
	"        Complex0: cores 0,  8, 16, 24, 32, and 40\n"		       \
	"        Complex1: cores 1,  9, 17, 25, 33, and 41\n"		       \
	"        Complex2: cores 2, 10, 18, 26, 34, and 42\n"		       \
	"        Complex3: cores 3, 11, 19, 27, 35, and 43\n"		       \
	"        Complex4: ROC\n"					       \
	"        Note that only 24 cores can be traced\n"		       \
	"\n"

#define DTX_DESC_USAGE_70XX						       \
	"\n"								       \
	"dtx-desc=<block-id>,<block-inst>,<half>,<select>,<mask>\n"	       \
	"    <block-id> is one of the following blocks:\n"		       \
	"        agl, dfa, fpa, gmx, iob, ipd, l2c_cbc, l2c_mci, l2c_tad,\n"   \
	"        lmc, mio, pcs, pem, pip, pko, rst, sata, sli, tim, usbdrd\n"  \
	"    <block-inst> is the instance of the block to use\n"	       \
	"    <half> is the debug bus half to use 0=lower half, 1=upper half\n" \
	"    <select> is the dbgsel value for the dtx\n"		       \
	"    <mask> indicates which bits to drive onto the debug bus\n"	       \
	"\n"

#define DTX_DESC_USAGE_78XX						       \
	"\n"								       \
	"dtx-desc=<block-id>,<block-inst>,<half>,<select>,<mask>\n"	       \
	"    <block-id> is one of the following blocks:\n"		       \
	"        ase, bgx, ciu, dfa, dpi, fpa, gser, hna, ila, ilk, iobn,\n"   \
	"        iobp, l2c_cbc, l2c_mci, l2c_tad, lap, lbk, lmc, mio,\n"       \
	"        ocx_lnk, ocx_ole, ocx_top, osm, pem, pki_pbe, pki_pfe,\n"     \
	"        pki_pix, pko, rad, rnm, rst, sli, sso, tim, usbh, zip\n"      \
	"    <block-inst> is the instance of the block to use\n"	       \
	"    <half> is the debug bus half to use 0=lower half, 1=upper half\n" \
	"    <select> is the dbgsel value for the dtx\n"		       \
	"    <mask> indicates which bits to drive onto the debug bus\n"	       \
	"\n"

#define DTX_DESC_USAGE_73XX						       \
	"\n"								       \
	"dtx-desc=<block-id>,<block-inst>,<half>,<select>,<mask>\n"	       \
	"    <block-id> is one of the following blocks:\n"		       \
	"        bch, bgx, ciu, dfa, dpi, fpa, gser, hna, iobn, iobp, key\n"   \
	"        l2c_cbc, l2c_mci, l2c_tad, lbk, lmc, mio, osm, pem\n"	       \
	"        pki_pbe, pki_pfe, pki_pix, pko, rad, rnm, rst, sli, spem\n"   \
	"        sso, tim, usbdrd, xcv, zip\n"				       \
	"    <block-inst> is the instance of the block to use\n"	       \
	"    <half> is the debug bus half to use 0=lower half, 1=upper half\n" \
	"    <select> is the dbgsel value for the dtx\n"		       \
	"    <mask> indicates which bits to drive onto the debug bus\n"	       \
	"\n"

#define DTX_DESC_USAGE_75XX						       \
	"\n"								       \
	"dtx-desc=<block-id>,<block-inst>,<half>,<select>,<mask>\n"	       \
	"    <block-id> is one of the following blocks:\n"		       \
	"        bbx1i, bbx2i, bbx3i, bch, bgx, bts, ciu, denc, dlfe, dpi,\n"  \
	"        fdeq, fpa, gser, iobn, iobp, key, l2c_cbc, l2c_mci,\n"	       \
	"        l2c_tad, lbk, lmc, mdb, mhbw, mio, pem, pki_pbe, pki_pfe,\n"  \
	"        pki_pix, pko, pnb, pnbd, prch, psm, rdec, rfif, rmap,\n"      \
	"        rnm, rst, sli, srio, sso, tdec, tim, ulfe, usbdrd, vdec,\n"   \
	"        wpse, wrce, wrde, wrse, wtxe, xcv, xsx\n"		       \
	"    <block-inst> is the instance of the block to use\n"	       \
	"    <half> is the debug bus half to use 0=lower half, 1=upper half\n" \
	"    <select> is the dbgsel value for the dtx\n"		       \
	"    <mask> indicates which bits to drive onto the debug bus\n"	       \
	"\n"

#define MATCH_USAGE							       \
	"\n"								       \
	"match=<commands>,<sources>,<destinations>,<address>,<mask>\n"         \
	"    <commands> list of the following commands separated by '+':\n"    \
	"        Triggers can only use one command.\n"			       \
	"        Filters can use up to 4 commands. However, for every\n"       \
	"        trigger used, one less command is available.\n"	       \
	"        nop, ldt, ldi, pl2, rpl2, dwb, ldy, ldd, psl1, ldc, lde,\n"   \
	"        ldwb, sty, rstp, stf, stt, stp, stc, stfil1, sttil1,\n"       \
	"        fas32, fas64, wbil2i, ltgl2i, stgl2i, wbl2i, invl2, wbil2,\n" \
	"        wbl2, lckl2, cas32, cas64, saam132, saam164, set8, set16,\n"  \
	"        set32, set64, clr8, clr16, clr32, clr64, incr8, incr16,\n"    \
	"        incr32, incr64, decr8, decr16, decr32, decr64, faa32,\n"      \
	"        faa64, saa32, saa64, iobld8, iobld16, iobld32, iobld64,\n"    \
	"        iobst8, iobst16, iobst32, iobst64, iobaddr, iobdma, lmtst,\n" \
	"        lmtdma, iobsta8, iobsta16, iobsta32, iobsta64, iobaddra,\n"   \
	"        iobdmaa, lmtsta, lmtdmaa, or all\n"

#define SOURCES_USAGE							       \
	"    <sources> is a list of the following separated by '+':\n"	       \
	"        all\n"

#define DESTINATIONS_USAGE						       \
	"    <destinations> is a list of the following separated by '+':\n"    \
	"        all\n"

#define ADDRESS_USAGE							       \
	"    <address> is a 36-bit physical address to match\n"		       \
	"    <mask> Each bit set is a bit that is checked in <address>\n"      \
	"\n"

#define SIGNAL_USAGE							       \
	"\n"								       \
	"signal=<half>,<bits>,<mask>\n"					       \
	"    <half> is the debug bus half to use 0=lower half, 1=upper half\n" \
	"    <bits> indicates the value of the 36 signal bits\n"	       \
	"    <mask> Each bit set is a bit that is checked in <bits>\n"	       \
	"\n"

#define COMMAND_USAGE							       \
	"\n"								       \
	"Brief description of the <commands>:\n"			       \
	"    nop     - No operation\n"					       \
	"    ldt     - Instruction Cache / IO Fill-Through: Fill the cache\n"  \
	"              block from L2/DRAM into the L1 instruction cache or\n"  \
	"              to IOB. Do not put the cache block into the L2 cache\n" \
	"              Both cores and IOB initiate these transactions. The\n"  \
	"              cores only fetch instructions via these transactions\n" \
	"    ldi     - Instruction Cache / IO Fill: Fill the cache block\n"    \
	"              from L2/DRAM into the L1 instruction cache or IOB.\n"   \
	"              Put the cache block into the L2 cache if it wasn't\n"   \
	"              already present. Both cores and IOB initiate these\n"   \
	"              transactions. The cores only fetch instructions via\n"  \
	"              these transactions.\n"				       \
	"    pl2     - Prefetch Into L2: Put the cache block into the L2\n"    \
	"              cache if it wasn't already present. This is an\n"       \
	"              ADD-only transaction on the CMI. Only cores initiate\n" \
	"              these transactions (via PREF instructions).\n"	       \
	"    rpl2    - Evict Soon from L2: If the cache block is present in\n" \
	"              the L2 cache (and not locked), clear its USE bit.\n"    \
	"              (Clearing the USE bit makes it more likely that the\n"  \
	"              cache block will be replaced in the future). Do\n"      \
	"              nothing if the cache block is (locked or) not\n"	       \
	"              present in the L2 cache. This is an ADD-busy-only\n"    \
	"              transaction on the CMI. Only cores initiate these\n"    \
	"              transactions (via PREF instructions).\n" 	       \
	"    dwb     - Don't-Write-Back: If the cache block is present in\n"   \
	"              the L2 cache, clear the DIRTY bit. Do nothing if the\n" \
	"              cache block is not present in the L2 cache. If dirty\n" \
	"              data for the location was present in the L2 cache,\n"   \
	"              it may be lost. The current value of the cache block\n" \
	"              is unpredictable after the DWB transaction until the\n" \
	"              block is later stored to, as stale copies of the\n"     \
	"              block may be present in the L1 data caches. Both\n"     \
	"              cores and IOB initiate DWB transactions. (The cores\n"  \
	"              only initiate DWB's via PREF instructions).\n" 	       \
	"    ldd     - Data Cache Fill: Fill the cache block from L2/DRAM\n"   \
	"              into the L1 data of the requesting core. (A later\n"    \
	"              store from another core or IOB will invalidate the\n"   \
	"              block in the L1 data cache. Put the block into the\n"   \
	"              L2 cache if it wasn't already present. Only cores\n"    \
	"              initiate these transactions.\n"			       \
	"    psl1    - Data Cache Fill-Through: Fill the cache block from\n"   \
	"              L2/DRAM into the L1 data cache of the requesting\n"     \
	"              core. (A later store from another core or IOB will\n"   \
	"              invalidate the block in the L1 data cache). Do not\n"   \
	"              put the block into the L2 cache. Only cores initiate\n" \
	"              these transactions via PREF instructions\n" 	       \
	"    stf     - Store-Full: Store to all bytes in the cache block.\n"   \
	"              Zero the bytes not transferred on the STORE bus (or\n"  \
	"              masked off). Invalidate all L1 data cache copies of\n"  \
	"              the block, except for the L1 data cache of the\n"       \
	"              initiator (if the initiator was a core). The core\n"    \
	"              that initiates a STFIL1 self-invalidates the block\n"   \
	"              from its L1 data cache, if necessary. Put the cache\n"  \
	"              block into the L2 cache if it wasn't already present.\n"\
	"              Both cores and IOB initiate STF transactions. Only\n"   \
	"              cores initiate STFIL1 transaction via ZCB\n"	       \
	"              instructions.\n"					       \
	"    stfil1  - Store-Full Invalidate L1.\n"			       \
	"    sttil1  - Store-Through Invalidate L1.\n"			       \
	"    stt     - Store-Through: Store to all bytes in the cache block\n" \
	"              Zero the bytes not transferred on the STORE bus (or\n"  \
	"              masked off). Invalidate all L1 data cache copies of\n"  \
	"              the block, except for the L1 data cache of the\n"       \
	"              initiator (if the initiator was a core). The core\n"    \
	"              that initiates a STFIL1 self-invalidates the block\n"   \
	"              from its L1 data cache, if necessary. Do not put the\n" \
	"              cache block into the L2 cache. Both cores and IOB\n"    \
	"              initiate STT transactions. (The cores only initiate\n"  \
	"              STT's via PREF instructions). Only cores initiate \n"   \
	"              STTIL1 transaction (via ZCB instructions).\n"	       \
	"    stp     - Store-Partial: Store to some of the bytes in the\n"     \
	"              cache block. Do not modify the bytes in the cache\n"    \
	"              block that are not stored. Invalidate all L1 data\n"    \
	"              cache copies of the block, except for the L1 data\n"    \
	"              cache of the initiator (if the initiator was a core).\n"\
	"              Put the cache block into the L2 cache if it wasn't\n"   \
	"              already present. Both cores and IOB initiate these\n"   \
	"              transactions.\n"					       \
	"    stc     - Store-Conditional: If the block is present in the\n"    \
	"              data cache of the requesting core, store to either\n"   \
	"              32-bits or 64-bits and invalidate all other L1 data\n"  \
	"              cache copies of the block in all other cores. If the\n" \
	"              block is not present in the data cache of the\n"	       \
	"              requesting core, fail the transactions, neither\n"      \
	"              storing or invalidating. On success, put the cache\n"   \
	"              block into the L2 cache if it wasn't already present.\n"\
	"              Only cores initiate these transactions (via SC/SCD\n"   \
	"              instructions).\n"				       \
	"    fas32   - L2 Cache Atomic Swap 32-bit\n"			       \
	"    fas64   - L2 Cache Atomic Swap: Return the current value of a\n"  \
	"              naturally-aligned 32-bit or 64-bit word in L2/DRAM\n"   \
	"              and atomically store to it. Invalidate all L1 data\n"   \
	"              cache copies of the surrounding cache block, except\n"  \
	"              for the L1 data cache of the initiator. The core\n"     \
	"              that initiates the transactions self-validates the\n"   \
	"              surrounding block from its L1 data cache, if\n"	       \
	"              necessary. Put the cache block into the L2 cache if\n"  \
	"              it wasn't already present. Only cores initiate these\n" \
	"              transactions (via LAW/LAWD instructions).\n"	       \
	"    wbil2i  - L2 Cache Index Writeback Invalidate: If the selected\n" \
	"              L2 cache block is dirty (i.e. if VALID and DIRTY\n"     \
	"              bits are set), write it back to the DRAM. (The ADD\n"   \
	"              bus identifies the L2 cache block via an index and\n"   \
	"              way, rather than an address for this transaction).\n"   \
	"              Invalidate the selected L2 cache block (i.e. clear\n"   \
	"              the VALID, USED and LOCK bits). Only cores initiate\n"  \
	"              these transactions (via CACHE instructions).\n"	       \
	"    ltgl2i  - L2 Cache Index Load Tag: Read tag and state for the\n"  \
	"              selected L2 cache block into L2C_TAD0_TAG. (The ADD\n"  \
	"              bus identifies the cache block via an index and way,\n" \
	"              rather than an address for this transaction). Only\n"   \
	"              cores initiate these transactions (via CACHE\n"	       \
	"              instructions).\n"				       \
	"    stgl2i  - L2 Cache Index Store Tag: Write tag and state for the\n"\
	"              selected L2 cache block to the current L2C_TAD0_TAG\n"  \
	"              value. (The ADD bus identifies the cache block via\n"   \
	"              an index and way, rather than an address for this\n"    \
	"              transaction). Only cores initiate these transactions\n" \
	"              (via CACHE instructions).\n"			       \
	"    invl2   - L2 Cache Hit Invalidate: If the cache block is\n"       \
	"              present in the L2 cache, invalidate it (i.e. clear\n"   \
	"              the VALID, USED and LOCK bits). Do nothing if the\n"    \
	"              cache block is not present in the L2 cache. If dirty\n" \
	"              data for the locations was present in the L2 cache,\n"  \
	"              it is lost. The current value of the cache block is\n"  \
	"              unpredictable after the INVL2 transaction until the\n"  \
	"              block is later stored to, as stale copies of the\n"     \
	"              block may be present in the L1 data caches. Only\n"     \
	"              cores initiate these transactions (via CACHE\n"	       \
	"              instructions).\n"				       \
	"    wbil2   - L2 Cache Hit Writeback Invalidate: If the cache\n"      \
	"              block is present in the L2 cache, invalide it (i.e\n"   \
	"              clear the VALID, USED and LOCK bits) after first\n"     \
	"              writing the block to DRAM if it was dirty (i.e. if\n"   \
	"              VALID and DIRTY bits were set). Do nothing if the\n"    \
	"              cache block is not present in the L2 cache. Only\n"     \
	"              cores initiate these transactions (via CACHE\n"	       \
	"              instructions).\n"				       \
	"    wbl2    - L2 Cache Nudge/Hit Writeback: If the cache block is\n"  \
	"              present in the L2 cache, clear its DIRTY and USE bits\n"\
	"              after first writing the block to DRAM if it wasi\n"     \
	"              dirty (i.e. if VALID and DIRTY bits were set).\n"       \
	"              (Clearing the USE bit makes it more likely that the\n"  \
	"              cache block will be replaced in the future). Do\n"      \
	"              nothing if the cache block is not present in the L2\n"  \
	"              cache. Do not clear the USE bit if the block is\n"      \
	"              locked. This is an ADD-bus-only transaction on the\n"   \
	"              CMI. Only cores initiate these transactions (via\n"     \
	"              PREF and CACHE instructions).\n"			       \
	"    lckl2   - L2 Cache Fetch and Lock: Set the LOCK (and USE) bit\n"  \
	"              for the block. Put the cache block into the L2 cache\n" \
	"              if it wasn't already present and there is an L2\n"      \
	"              cache way available. LCKL2 is a NOP if it misses in\n"  \
	"              the L2 cache when there are no ways available. Only\n"  \
	"              cores initiate these transactions (via CACHE\n"	       \
	"              instructions).\n" 				       \
	"    set8    - L2 Cache Atomic Fetch and Set 8-bit\n"		       \
	"    set16   - L2 Cache Atomic Fetch and Set 16-bit\n"		       \
	"    set32   - L2 Cache Atomic Fetch and Set 32-bit\n"		       \
	"    set64   - L2 Cache Atomic Fetch and Set: Return the current\n"    \
	"              value of a naturally-aligned 8-bit, 16-bit, 32-bit\n"   \
	"              or 64-bit word in L2/DRAM and atomically set all bits\n"\
	"              in it. Invalidate all L1 data cache copies of the\n"    \
	"              surrounding cache block, except for the L1 data cache\n"\
	"              of the initiator. The core that initiates the\n"	       \
	"              transaction self-invalidates the surrounding block\n"   \
	"              from its L1 data cache, if necessary. Put the cache\n"  \
	"              block into the L2 cache if it wasn't already present.\n"\
	"              Cores initiate SET64 and SET32 transactions (via\n"     \
	"              LAS/LASD instructions). IOB initiates SET32, SET16\n"   \
	"              and SET8 transactions.\n"			       \
	"    clr8    - L2 Cache Atomic Fetch and Clear 8-bit\n"		       \
	"    clr16   - L2 Cache Atomic Fetch and Clear 16-bit\n"	       \
	"    clr32   - L2 Cache Atomic Fetch and Clear 32-bit\n"	       \
	"    clr64   - L2 Cache Atomic Fetch and Clear: Return the current\n"  \
	"              value of a naturally-aligned 8-bit, 16-bit, 32-bit\n"   \
	"              or 64-bit word in L2/DRAM and atomically clear all\n"   \
	"              bits in it. Invalidate all L1 data cache copies of\n"   \
	"              the surrounding cache block, except for the L1 data\n"  \
	"              cache of the initiator. The core that initiates the\n"  \
	"              transaction self-invalidates the surrounding block\n"   \
	"              from its L1 data cache, if necessary. Put the cache\n"  \
	"              block into the L2 cache if it wasn't already present.\n"\
	"              Cores initiate CLR64 and CLR32 transactions (via\n"     \
	"              LAC/LACD instructions). IOB initiates CLR32, CLR16\n"   \
	"              and CLR8 transactions.\n"			       \
	"    incr8   - L2 Cache Atomic Fetch and Increment 8-bit\n"	       \
	"    incr16  - L2 Cache Atomic Fetch and Increment 16-bit\n"	       \
	"    incr32  - L2 Cache Atomic Fetch and Increment 32-bit\n"	       \
	"    incr64  - L2 Cache Atomic Fetch and Increment: Return the\n"      \
	"              current value of a naturally-aligned 32-bit or 64-bit\n"\
	"              word in L2/DRAM and atomically add one to it and\n"     \
	"              update L2/DRAM with the result. Invalidate all L1 \n"   \
	"              data cache copies of the surrounding cache block,\n"    \
	"              except for the L1 data cache of the initiator. The\n"   \
	"              core that initiates the transaction self-invalidates\n" \
	"              the surrounding block from its L1 data cache, if\n"     \
	"              necessary. Put the cache block into the L2 cache if\n"  \
	"              it wasn't already present. Cores initiate INCR64 and\n" \
	"              INCR32 transactions (via LAI/LAID instructions).\n"     \
	"              IOB initiate INCR32, INCR16 and INCR8 transactions.\n"  \
	"    decr8   - L2 Cache Atomic Fetch and Decrement 8-bit\n"	       \
	"    decr16  - L2 Cache Atomic Fetch and Decrement 16-bit\n"	       \
	"    decr32  - L2 Cache Atomic Fetch and Decrement 32-bit\n"	       \
	"    decr64  - L2 Cache Atomic Fetch and Decrement: Return the\n"      \
	"              current value of a naturally-aligned 8-bit, 16-bit,\n"  \
	"              32-bit or 64-bit word in L2/DRAM and atomically \n"     \
	"              subtract one to it and update L2/DRAM with the\n"       \
	"              result. Invalidate all L1 data cache copies of the\n"   \
	"              surrounding cache block, except for the L1 data cache\n"\
	"              of the initiator. The core that initiates the\n"	       \
	"              transaction self-invalidates the surrounding block\n"   \
	"              from its L1 data cache, if necessary. Put the cache\n"  \
	"              block into the L2 cache if it wasn't already present.\n"\
	"              Cores initiate DECR64 and DECR32 transactions (via\n"   \
	"              LAD/LADD instructions). IOB initiate DECR32, DECR16\n"  \
	"              and DECR8 transactions.\n"			       \
	"    faa32   - L2 Cache Atomic Fetch and Add 32-bit\n"		       \
	"    faa64   - L2 Cache Atomic Fetch and Add: Return the current\n"    \
	"              value of a naturally-aligned 32-bit or 64-bit word\n"   \
	"              in L2/DRAM and atomically add a value to it and\n"      \
	"              update L2/DRAM with the result. Invalidate all L1 \n"   \
	"              data cache copies of the surrounding cache block,\n"    \
	"              except for the L1 data cache of the initiator. The\n"   \
	"              core that initiates the transaction self-invalidates\n" \
	"              the surrounding block from its L1 data cache, if\n"     \
	"              necessary. Put the cache block into the L2 cache if\n"  \
	"              it wasn't already present. Only cores initiate\n"       \
	"              these transactions.\n"				       \
	"    saa32   - Store-Atomic Add 32-bit\n"			       \
	"    saa64   - Store-Atomic Add: Read the current value of a\n"	       \
	"              naturally-aligned 32-bit/64-bit word in L2/DRAM and\n"  \
	"              atomically add a valud to it and update L2/DRAM with\n" \
	"              the result. Invalidate all L1 data cache copies of\n"   \
	"              the surrounding cache block, except for the L1 data\n"  \
	"              cache of the initiator. The core that initiates the\n"  \
	"              transaction self-invalidates the surrounding block\n"   \
	"              from its L1 data cache, if necessary. Put the cache\n"  \
	"              block into the L2 cache if it wasn't already present.\n"\
	"              Only cores initiate these transactions.\n"	       \
	"    iobld8  - Load to IOB 8-bit:\n"				       \
	"    iobld16 - Load to IOB 16-bit.\n"				       \
	"    iobld32 - Load to IOB 32-bit.\n"				       \
	"    iobld64 - Load to IOB 64-bit: Forward a naturally-aligned \n"     \
	"              address onto the IOC bus. Either service the IO\n"      \
	"              load directly in IOB or forward it onto the IOI and\n"  \
	"              service it in another Octeon IO device. Return the \n"  \
	"              8-bit, 16-bit, 32-bit or 64-bit result via an IOR\n"    \
	"              single-cycle transfer. Forward this IOR response\n"     \
	"              onto the COMMIT/FILL busses to complete the\n"	       \
	"              transaction. Only cores initiate these transactions\n"  \
	"    iobst8  - Store to IOB 8-bit.\n"				       \
	"    iobst16 - Store to IOB 16-bit.\n"				       \
	"    iobst32 - Store to IOB 32-bit.\n"				       \
	"    iobst64 - Store to IOB: Forward a naturally-aligned address\n"    \
	"              and corresponding 8-bit, 16-bit, 32-bit or 64-bit\n"    \
	"              word onto the IOC bus. Either service the IO store\n"   \
	"              directly in IOB or forward it onto the IOI for\n"       \
	"              another Octeon IO device. Only Cores initiate these\n"  \
	"              transactions.\n"					       \
	"    iobaddr   I/O Send Address Only.\n"			       \
	"    iobdma  - Forward a naturally-aligned address obtained from the\n"\
	"              STORE bus onto the IOC bus. Either service the IO\n"    \
	"              vector load directly in IOB or forward it onto the\n"   \
	"              IOI for another Octeon IO device. The total IOBDMA\n"   \
	"              length may be as large as 255 64-bit words. Respond\n"  \
	"              with a series of 1<=n<=16 word responses. Return\n"     \
	"              each n-vector of 64-bit results via an IOR n-cycle\n"   \
	"              transfer. Forward each IOR response onto the \n"	       \
	"              COMMIT/FILL busses until the transaction is complete.\n"\
	"              Only cores initiate these transactions.\n" 	       \
	"    lmtst     I/O Multi-Quadword Store.\n"			       \
	"    lmtdma    I/O Multi-Quadword Store with Multi-Quadword Load.\n"   \
	"    iobsta8   I/O 1-Byte Store, with Commit.\n"		       \
	"    iobsta16  I/O 2-Byte Store, with Commit.\n"		       \
	"    iobsta32  I/O 4-Byte Store, with Commit.\n"		       \
	"    iobsta64  I/O 8-Byte Store, with Commit.\n"		       \
	"    iobaddra  I/O Send Address Only, with Commit.\n"		       \
	"    iobdmaa   I/O Multi-Quadword Load, with Commit.\n"		       \
	"    lmtsta    I/O Multi-Quadword Store, with Commit.\n"	       \
	"    lmtdmaa   I/O Multi-Quadword Store with Multi-Quadword Load,\n"   \
	"              with Commit.\n"					       \
	"\n"

#define EXAMPLES_USAGE							       \
	"\n"								       \
	"Examples:\n"							       \
	"    %s -f ldi\n"						       \
	"        Trace all Icache loads.\n"				       \
	"    %s -f stc+stf,all,all\n"					       \
	"        Trace all stc and stf command.\n"			       \
	"    %s -f stc+stf,all,all,0,0xffffffffffff0000\n"		       \
	"        Trace all stc and stf commands to the first 64KB of memory\n" \
	"        (exception vectors).\n"				       \
	"    %s -d l2c_cbc,0,0,0x50000,0xfffffffff -f 0,0x2,0x2\n"	       \
	"        Trace when bit 2 of l2c_cbc dbgsel 0x50000 is high.\n"	       \
	"    %s -f all,all,all,0,0 -m start -t stp,all,all,0x8,0xfffffffff\n"  \
	"        Start tracing after a stp instruction to address 0x8 is\n"    \
	"        issued.\n"						       \
	"    %s -f all,all,all,0,0 -m stop -t stp,all,all,0x8,0xfffffffff\n"   \
	"        Stop tracing when a stp instruction to address 0x8 is\n"      \
	"        issued.\n"						       \
	"\n"

/*
 * Ocla fifo control packet.
 */
union cvmx_ocla_cap_ctl {
	uint64_t	u64;
	struct cvmx_ocla_cap_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t	rsv0		: 26;
		uint64_t	ctl		: 1;
		uint64_t	rsv1		: 1;
		uint64_t	eot1		: 1;
		uint64_t	eot0		: 1;
		uint64_t	sot1		: 1;
		uint64_t	sot0		: 1;
		uint64_t	cycle		: 32;
#else
		uint64_t	cycle		: 32;
		uint64_t	sot0		: 1;
		uint64_t	sot1		: 1;
		uint64_t	eot0		: 1;
		uint64_t	eot1		: 1;
		uint64_t	rsv1		: 1;
		uint64_t	ctl		: 1;
		uint64_t	rsv0		: 26;
#endif
	} s;
};
typedef union cvmx_ocla_cap_ctl cvmx_ocla_cap_ctl_t;

/*
 * Ocla fifo data packet.
 */
union cvmx_ocla_cap_dat {
	uint64_t	u64;
	struct cvmx_ocla_cap_dat_s {
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t	rsv0		: 26;
		uint64_t	ctl		: 1;
		uint64_t	hi		: 1;
		uint64_t	data		: 36;
#else
		uint64_t	data		: 36;
		uint64_t	hi		: 1;
		uint64_t	ctl		: 1;
		uint64_t	rsv0		: 26;
#endif
	} s;
};
typedef union cvmx_ocla_cap_dat cvmx_ocla_cap_dat_t;


/*
 * Describe a dtx block. Used to decode dtx block IDs.
 *
 *  id:			Block id.
 *  num_instances:	Number of instances for this block type.
 *  name:		Block name.
 */
struct dtx_block_decode {
	cvmx_dtx_id_t		id;
	int			num_instances;
	char			name[NAME_LEN];
};

/* Array used to decode the DTX blocks on the 70xx */
struct dtx_block_decode dtx_blocks_70xx[] = {
	{AGL,		1,	"AGL"},
	{DFA,		1,	"DFA"},
	{FPA,		1,	"FPA"},
	{GMX,		2,	"GMX"},
	{IOB,		1,	"IOB"},
	{IPD,		1,	"IPD"},
	{L2C_CBC,	1,	"L2C_CBC"},
	{L2C_MCI,	1,	"L2C_MCI"},
	{L2C_TAD,	1,	"L2C_TAD"},
	{LMC,		1,	"LMC"},
	{MIO,		1,	"MIO"},
	{PCS,		2,	"PCS"},
	{PEM,		3,	"PEM"},
	{PIP,		1,	"PIP"},
	{PKO,		1,	"PKO"},
	{RST,		1,	"RST"},
	{SATA,		1,	"SATA"},
	{SLI,		1,	"SLI"},
	{TIM,		1,	"TIM"},
	{USBDRD,	2,	"USBDRD"},
	{-1,		0,	"INVALID_BLOCK"}
};

/* Array used to decode the DTX blocks on the 78xx */
struct dtx_block_decode dtx_blocks_78xx[] = {
	{ASE,		1,	"ASE"},
	{BGX,		6,	"BGX"},
	{CIU,		1,	"CIU"},
	{DFA,		1,	"DFA"},
	{DPI,		1,	"DPI"},
	{FPA,		1,	"FPA"},
	{GSER,		14,	"GSER"},
	{HNA,		1,	"HNA"},
	{ILA,		1,	"ILA"},
	{ILK,		1,	"ILK"},
	{IOBN,		1,	"IOBN"},
	{IOBP,		1,	"IOBP"},
	{L2C_CBC,	4,	"L2C_CBC"},
	{L2C_MCI,	4,	"L2C_MCI"},
	{L2C_TAD,	8,	"L2C_TAD"},
	{LAP,		2,	"LAP"},
	{LBK,		1,	"LBK"},
	{LMC,		4,	"LMC"},
	{MIO,		1,	"MIO"},
	{OCX_LNK,	3,	"OCX_LNK"},
	{OCX_OLE,	3,	"OCX_OLE"},
	{OCX_TOP,	1,	"OCX_TOP"},
	{OSM,		1,	"OSM"},
	{PEM,		4,	"PEM"},
	{PKI_PBE,	1,	"PKI_PBE"},
	{PKI_PFE,	1,	"PKI_PFE"},
	{PKI_PIX,	1,	"PKI_PIX"},
	{PKO,		1,	"PKO"},
	{RAD,		1,	"RAD"},
	{RNM,		1,	"RNM"},
	{RST,		1,	"RST"},
	{SLI,		1,	"SLI"},
	{SSO,		1,	"SSO"},
	{TIM,		1,	"TIM"},
	{USBH,		1,	"USBH"},
	{ZIP,		1,	"ZIP"},
	{-1,		0,	"INVALID_BLOCK"}
};

/* Array used to decode the DTX blocks on the 73xx */
struct dtx_block_decode dtx_blocks_73xx[] = {
	{BCH,		1,	"BCH"},
	{BGX,		3,	"BGX"},
	{CIU,		1,	"CIU"},
	{DFA,		1,	"DFA"},
	{DPI,		1,	"DPI"},
	{FPA,		1,	"FPA"},
	{GSER,		7,	"GSER"},
	{HNA,		1,	"HNA"},
	{IOBN,		1,	"IOBN"},
	{IOBP,		1,	"IOBP"},
	{KEY,		1,	"KEY"},
	{L2C_CBC,	2,	"L2C_CBC"},
	{L2C_MCI,	3,	"L2C_MCI"},
	{L2C_TAD,	4,	"L2C_TAD"},
	{LBK,		1,	"LBK"},
	{LMC,		2,	"LMC"},
	{MIO,		1,	"MIO"},
	{OSM,		1,	"OSM"},
	{PEM,		4,	"PEM"},
	{PKI_PBE,	1,	"PKI_PBE"},
	{PKI_PFE,	1,	"PKI_PFE"},
	{PKI_PIX,	1,	"PKI_PIX"},
	{PKO,		1,	"PKO"},
	{RAD,		1,	"RAD"},
	{RNM,		1,	"RNM"},
	{RST,		1,	"RST"},
	{SATA,		1,	"SATA"},
	{SLI,		1,	"SLI"},
	{SPEM,		1,	"SPEM"},
	{SSO,		1,	"SSO"},
	{TIM,		1,	"TIM"},
	{USBDRD,	2,	"USBDRD"},
	{XCV,		1,	"XCV"},
	{ZIP,		1,	"ZIP"},
	{-1,		0,	"INVALID_BLOCK"}
};

/* Array used to decode the DTX blocks on the 75xx */
struct dtx_block_decode dtx_blocks_75xx[] = {
	{BBX1I,		1,	"BBX1I"},
	{BBX2I,		1,	"BBX2I"},
	{BBX3I,		1,	"BBX3I"},
	{BCH,		1,	"BCH"},
	{BGX,		1,	"BGX"},
	{BTS,		1,	"BTS"},
	{CIU,		1,	"CIU"},
	{DENC,		1,	"DENC"},
	{DLFE,		1,	"DLFE"},
	{DPI,		1,	"DPI"},
	{FDEQ,		2,	"FDEQ"},
	{FPA,		1,	"FPA"},
	{GSER,		9,	"GSER"},
	{IOBN,		1,	"IOBN"},
	{IOBP,		1,	"IOBP"},
	{KEY,		1,	"KEY"},
	{L2C_CBC,	2,	"L2C_CBC"},
	{L2C_MCI,	2,	"L2C_MCI"},
	{L2C_TAD,	4,	"L2C_TAD"},
	{LBK,		1,	"LBK"},
	{LMC,		2,	"LMC"},
	{MDB,		18,	"MDB"},
	{MHBW,		1,	"MHBW"},
	{MIO,		1,	"MIO"},
	{PEM,		2,	"PEM"},
	{PKI_PBE,	1,	"PKI_PBE"},
	{PKI_PFE,	1,	"PKI_PFE"},
	{PKI_PIX,	1,	"PKI_PIX"},
	{PKO,		1,	"PKO"},
	{PNB,		2,	"PNB"},
	{PNBD,		2,	"PNBD"},
	{PRCH,		1,	"PRCH"},
	{PSM,		1,	"PSM"},
	{RDEC,		1,	"RDEC"},
	{RFIF,		1,	"RFIF"},
	{RMAP,		1,	"RMAP"},
	{RNM,		1,	"RNM"},
	{RST,		1,	"RST"},
	{SLI,		1,	"SLI"},
	{SRIO,		2,	"SRIO"},
	{SSO,		1,	"SSO"},
	{TDEC,		1,	"TDEC"},
	{TIM,		1,	"TIM"},
	{ULFE,		1,	"ULFE"},
	{USBDRD,	1,	"USBDRD"},
	{VDEC,		1,	"VDEC"},
	{WPSE,		1,	"WPSE"},
	{WRCE,		1,	"WRCE"},
	{WRDE,		1,	"WRDE"},
	{WRSE,		1,	"WRSE"},
	{WTXE,		1,	"WTXE"},
	{XCV,		1,	"XCV"},
	{XSX,		1,	"XSX"},
	{-1,		0,	"INVALID_BLOCK"}
};

/*
 * Supported commands.
 */
typedef enum {
	NOP 		= 0x00,
	LDT 		= 0x01,
	LDI 		= 0x02,
	PL2 		= 0x03,
	RPL2 		= 0x04,
	DWB 		= 0x05,
	LDY 		= 0x06,
	LDD 		= 0x08,
	PSL1 		= 0x09,
	LDC 		= 0x0a,
	LDE 		= 0x0b,
	LDWB 		= 0x0d,
	STY 		= 0x0e,
	RSTP 		= 0x0f,
	STF		= 0x10,
	STT		= 0x11,
	STP		= 0x12,
	STC		= 0x13,
	STFIL1		= 0x14,
	STTIL1		= 0x15,
	FAS32		= 0x16,
	FAS64		= 0x17,
	WBIL2I		= 0x18,
	LTGL2I		= 0x19,
	STGL2I		= 0x1a,
	WBL2I		= 0x1b,
	INVL2		= 0x1c,
	WBIL2		= 0x1d,
	WBL2		= 0x1e,
	LCKL2		= 0x1f,
	CAS32		= 0x22,
	CAS64		= 0x23,
	SAAM132		= 0x26,
	SAAM164		= 0x27,
	SET8		= 0x28,
	SET16		= 0x29,
	SET32		= 0x2a,
	SET64		= 0x2b,
	CLR8		= 0x2c,
	CLR16		= 0x2d,
	CLR32		= 0x2e,
	CLR64		= 0x2f,
	INCR8		= 0x30,
	INCR16		= 0x31,
	INCR32		= 0x32,
	INCR64		= 0x33,
	DECR8		= 0x34,
	DECR16		= 0x35,
	DECR32		= 0x36,
	DECR64		= 0x37,
	FAA32		= 0x3a,
	FAA64		= 0x3b,
	SAA32		= 0x3e,
	SAA64		= 0x3f,
	IOBLD8		= 0x40,
	IOBLD16		= 0x41,
	IOBLD32		= 0x42,
	IOBLD64		= 0x43,
	IOBST8		= 0x44,
	IOBST16		= 0x45,
	IOBST32		= 0x46,
	IOBST64		= 0x47,
	IOBADDR		= 0x48,
	IOBDMA		= 0x49,
	LMTST		= 0x4a,
	LMTDMA		= 0x4b,
	IOBSTA8		= 0x64,
	IOBSTA16	= 0x65,
	IOBSTA32	= 0x66,
	IOBSTA64	= 0x67,
	IOBADDRA	= 0x68,
	IOBDMAA		= 0x69,
	LMTSTA		= 0x6a,
	LMTDMAA		= 0x6b,
	INVALID_CMD	= -1
} cvmx_cmd_id_t;

/*
 * Supported sources.
 */
typedef enum {
	ALL_SRC,
	INVALID_SRC = -1
} cvmx_src_id_t;

/*
 * Supported destinations.
 */
typedef enum {
	ALL_DST,
	INVALID_DST = -1
} cvmx_dst_id_t;

/*
 * Describes an ID. Used to decode bit fields.
 *
 *  u:		Union containing the hex value of the bit field ID.
 *  name:	Bit field name.
 */
struct bit_field_decode {
	union {
		int		id;
		cvmx_cmd_id_t	cmd_id;
		cvmx_src_id_t	src_id;
		cvmx_dst_id_t	dst_id;
	} u;
	char		name[NAME_LEN];
};

/* Array used to decode the command */
static struct bit_field_decode	commands_dec_7xxx[] = {
	{{NOP}, 	"NOP"},
	{{LDT},		"LDT"},
	{{LDI}, 	"LDI"},
	{{PL2}, 	"PL2"},
	{{RPL2},	"RPL2"},
	{{DWB}, 	"DWB"},
	{{LDY}, 	"LDY"},
	{{LDD}, 	"LDD"},
	{{PSL1},	"PSL1"},
	{{LDC}, 	"LDC"},
	{{LDE}, 	"LDE"},
	{{LDWB},	"LDWB"},
	{{STY},		"STY"},
	{{RSTP},	"RSTP"},
	{{STF},		"STF"},
	{{STT},		"STT"},
	{{STP},		"STP"},
	{{STC},		"STC"},
	{{STFIL1},	"STFIL1"},
	{{STTIL1},	"STTIL1"},
	{{FAS32},	"FAS32"},
	{{FAS64},	"FAS64"},
	{{WBIL2I},	"WBIL2I"},
	{{LTGL2I},	"LTGL2I"},
	{{STGL2I},	"STGL2I"},
	{{WBL2I},	"WBL2I"},
	{{INVL2},	"INVL2"},
	{{WBIL2},	"WBIL2"},
	{{WBL2},	"WBL2"},
	{{LCKL2},	"LCKL2"},
	{{CAS32},	"CAS32"},
	{{CAS64},	"CAS64"},
	{{SAAM132},	"SAAM132"},
	{{SAAM164},	"SAAM164"},
	{{SET8},	"SET8"},
	{{SET16},	"SET16"},
	{{SET32},	"SET32"},
	{{SET64},	"SET64"},
	{{CLR8},	"CLR8"},
	{{CLR16},	"CLR16"},
	{{CLR32},	"CLR32"},
	{{CLR64},	"CLR64"},
	{{INCR8},	"INCR8"},
	{{INCR16},	"INCR16"},
	{{INCR32},	"INCR32"},
	{{INCR64},	"INCR64"},
	{{DECR8},	"DECR8"},
	{{DECR16},	"DECR16"},
	{{DECR32},	"DECR32"},
	{{DECR64},	"DECR64"},
	{{FAA32},	"FAA32"},
	{{FAA64},	"FAA64"},
	{{SAA32},	"SAA32"},
	{{SAA64},	"SAA64"},
	{{IOBLD8},	"IOBLD8"},
	{{IOBLD16},	"IOBLD16"},
	{{IOBLD32},	"IOBLD32"},
	{{IOBLD64},	"IOBLD64"},
	{{IOBST8},	"IOBST8"},
	{{IOBST16},	"IOBST16"},
	{{IOBST32},	"IOBST32"},
	{{IOBST64},	"IOBST64"},
	{{IOBADDR},	"IOBADDR"},
	{{IOBDMA},	"IOBDMA"},
	{{LMTST},	"LMTST"},
	{{LMTDMA},	"LMTDMA"},
	{{IOBSTA8},	"IOBSTA8"},
	{{IOBSTA16},	"IOBSTA16"},
	{{IOBSTA32},	"IOBSTA32"},
	{{IOBSTA64},	"IOBSTA64"},
	{{IOBADDRA},	"IOBADDRA"},
	{{IOBDMAA},	"IOBDMAA"},
	{{LMTSTA},	"LMTSTA"},
	{{LMTDMAA},	"LMTDMAA"},
	{{ALL_CMD},	"ALL"},
	{{-1},		"INVALID_CMD"}
};

/* Array used to decode the source */
static struct bit_field_decode	sources_dec_7xxx[] = {
	{{ALL_SRC},	"ALL"},
	{{-1},		"INVALID_SRC"}
};

/* Array used to decode the destination */
static struct bit_field_decode	destinations_dec_7xxx[] = {
	{{ALL_DST},	"ALL"},
	{{-1},		"INVALID_DST"}
};

/* Data packet format coming from dtx l2c 0x300500 */
union cvmx_dtx_l2c_300500 {
	uint64_t	u64;
	struct cvmx_dtx_l2c_300500_s {
#if __BYTE_ORDER == __BIG_ENDIAN
		uint64_t	rsv0		: 30;
		uint64_t	xmd_msk		: 8;
		uint64_t	stid		: 5;
		uint64_t	node		: 2;
		uint64_t	did		: 8;
		uint64_t	sid		: 3;
		uint64_t	cmd		: 7;
		uint64_t	val_0a		: 1;
#else
		uint64_t	val_0a		: 1;
		uint64_t	cmd		: 7;
		uint64_t	sid		: 3;
		uint64_t	did		: 8;
		uint64_t	node		: 2;
		uint64_t	stid		: 5;
		uint64_t	xmd_msk		: 8;
		uint64_t	rsv0		: 30;
#endif
	} s;
};
typedef union cvmx_dtx_l2c_300500 cvmx_dtx_l2c_300500_t;

/* Data packet format coming from dtx l2c 0x300502 */
union cvmx_dtx_l2c_300502 {
	uint64_t	u64;
	struct cvmx_dtx_l2c_300502_s {
#if __BYTE_ORDER == __BIG_ENDIAN
		uint64_t	rsv0		: 28;
		uint64_t	addr		: 36;
#else
		uint64_t	addr		: 36;
		uint64_t	rsv0		: 28;
#endif
	} s;
};
typedef union cvmx_dtx_l2c_300502 cvmx_dtx_l2c_300502_t;

/* Data to display to the user */
struct cvmx_display_data_s {
	uint64_t	raw_lo_data;
	uint64_t	raw_hi_data;
	uint64_t	addr;
	char		cmd[NAME_LEN];
	uint8_t		xmd_msk;
	uint8_t		stid;
	uint8_t		node;
	uint8_t		did;
	uint8_t		sid;
};
typedef struct cvmx_display_data_s cvmx_display_data_t;

/* The 70xx only supports the following dtx blocks */
static const cvmx_dtx_id_t valid_70xx_dtx[] = {
	AGL, DFA, FPA, GMX, IOB, IPD, L2C_CBC, L2C_MCI, L2C_TAD,
	LMC, MIO, PCS, PEM, PIP, PKO, POW, RST, SLI, TIM, USBDRD,
	INVALID_BLOCK_ID
};

/* The 78xx ocla complexes 0 to 3 support the following dtx blocks */
static const cvmx_dtx_id_t valid_78xx_03_dtx[] = {
	L2C_CBC, L2C_MCI, L2C_TAD, INVALID_BLOCK_ID
};

/* The 78xx ocla complex 4 support the following dtx blocks */
static const cvmx_dtx_id_t valid_78xx_4_dtx[] = {
	ASE, BGX, CIU, DFA, DPI, FPA, GSER, HNA, ILA, ILK, IOBN, IOBP, LAP, LBK,
	LMC, MIO, OCX_LNK, OCX_OLE, OCX_TOP, OSM, PEM, PKI_PBE, PKI_PFE,
	PKI_PIX, PKO, RAD, RNM, RST, SLI, SSO, TIM, USBH, ZIP,
	INVALID_BLOCK_ID
};

/* The 73xx ocla complex 2 supports the following dtx blocks */
static const cvmx_dtx_id_t valid_73xx_2_dtx[] = {
	BCH, BGX, CIU, DFA, DPI, FPA, GSER, HNA, IOBN, IOBP, KEY, LBK, LMC, MIO,
	OSM, PEM, PKI_PBE, PKI_PFE, PKI_PIX, PKO, RAD, RNM, RST, SATA, SLI,
	SPEM, SSO, TIM, USBDRD, XCV, ZIP, INVALID_BLOCK_ID
};

/* The 75xx ocla complex 2 supports the following dtx blocks */
static const cvmx_dtx_id_t valid_75xx_2_dtx[] = {
	BBX1I, BBX2I, BBX3I, BCH, BGX, BTS, CIU, DENC, DLFE, DPI, FDEQ, FPA,
	GSER, IOBN, IOBP, KEY, LBK, LMC, MDB, MHBW, MIO, PEM, PKI_PBE, PKI_PFE,
	PKI_PIX, PKO, PNB, PNBD, PRCH, PSM, RDEC, RFIF, RMAP, RNM, RST, SLI,
	SRIO, SSO, TDEC, TIM, ULFE, USBDRD, VDEC, WPSE, WRCE, WRDE, WRSE, WTXE,
	XCV, XSX, INVALID_BLOCK_ID
};

/*
 * Specify a filter/trigger using match format.
 *
 *  cmds:		Commands to match.
 *  num_cmds:		Number of commands to match.
 *  sid:		Source ID to match.
 *  did:		Destionation ID to match.
 *  addr:		Address to match.
 *  mask:		Mask applied to addr indicating wich bits to compare.
 */
struct match {
	int		cmds[MAX_MATCHERS];
	int		num_cmds;
	int		sid;
	int		did;
	uint64_t	addr;
	uint64_t	mask;
};

/*
 * Specify a filter/trigger using signal format.
 *
 *  en:			Indicates if the filter/trigger is enabled.
 *  bits:		Value of bits to match.
 *  mask:		Mask applied to bits indicating which bits to compare.
 */
struct signal {
	int		en;
	uint64_t	bits;
	uint64_t	mask;
};

/*
 * Supported filter/trigger formats.
 */
enum {
	MATCH_FORMAT,
	SIGNAL_FORMAT,
	INVALID_FORMAT
};

/*
 * Filter and trigger specification.
 *
 *  format:		Format used by the filter/trigger.
 *  u:			Filter/trigger configuration information.
 */
struct filter_info {
	int			format;
	union {
		struct match	match;
		struct signal	signal[2];
	} u;
};

/*
 * Configurarion of all DTXs used by an ocla complex.
 *
 *  cnt:	Number of configured DTXs.
 *  dtx:	Configured DTXs.
 */
struct dtx_info {
	uint			cnt;
	cvmx_dtx_def_t		dtx[MAX_NUM_DTX];
};

/*
 * Contains all the information needed to process a command.
 *
 *  node:			Node to capture data on.
 *  ix:				Ocla complex to use.
 *  dtx_reset:			Reset all DTXs.
 *  trig_mode:			Specifies the function of the trigger.
 *  dtx_spec:			DTX specifications.
 *  filter:			Filter specification.
 *  trig:			Trigger specification.
 *  buf_size:			Requested ddr capture buffer size.
 */
struct command_info {
	uint			node;
	uint			ix;
	int			dtx_reset;
	int			trig_mode;
	struct dtx_info		dtx_info;
	struct filter_info	filter;
	struct filter_info	trig;
	uint			buf_size;
};

/*
 * complex_info:		Information needed to configure a ocla complex.
 *
 *  num_fil_mat:		Number of matchers used by the filter.
 *  trig_mat:			Matcher used by the trigger.
 *  fsm_action:			Action for each fsm state.
 *  pbuf:			Pointer to physical address of ddr capture
 *				buffer.
 *  buf_size:			Size of ddr capture buffer.
 */
struct complex_info {
	int 			num_fil_mat;
	int			trig_mat;
	cvmx_fsm_action_t	fsm_action[MAX_FSMS][MAX_FSM_STATES];
	uint64_t		pbuf;
	uint			buf_size;
};


static struct command_info command;
static struct complex_info complex;
static volatile int finished = 0;
void (*original_sig_int)(int num);

/* Pointers to soc specific data */
static struct dtx_block_decode *dtx_blocks_dec;
static char *dtx_desc_usage;
static struct bit_field_decode *commands_dec;
static struct bit_field_decode *sources_dec;
static struct bit_field_decode *destinations_dec;


/*
 * This signal handler is called when Control-C is pressed. It
 * cleanly exits the read loop.
 *
 * num:				Signal received
 */
static void sig_int_handler(int num)
{
	finished = 1;
	if (original_sig_int)
		original_sig_int(num);
}

/*
 * Allocate a free dtx.
 *
 *  Returns:			Pointer to free dtx, or NULL on failure.
 */
 static cvmx_dtx_def_t * alloc_dtx(void)
 {
	 cvmx_dtx_def_t	*dtx_def = NULL;

	 if (command.dtx_info.cnt < MAX_NUM_DTX) {
		 dtx_def = &command.dtx_info.dtx[command.dtx_info.cnt];
		 command.dtx_info.cnt++;
	 }

	 return dtx_def;
 }

/*
 * Find the name of a given id.
 *
 *  id:				ID to find name for.
 *  field_desc:			Array to seach for ID.
 *
 *  Returns:			ID name.
 */
static char * id2name(int			id,
		      struct bit_field_decode	*field_dec)
{
	while(field_dec->u.id >= 0) {
		if (id == field_dec->u.id)
			return field_dec->name;
		field_dec++;
	}

	return NULL;
}

/*
 * Get the hex value of a DTX block name and verify the instance is valid for
 * that block.
 *
 *  name:			Name of DTX block to decode.
 *  inst:			DTX block instance.
 *
 *  Returns:			DTX block id, or -1 if name is not a valid block
 *				or instance is out of range.
 */
static int dtx_name2id(char	*name,
		       int	inst)
{
	struct dtx_block_decode	*block = dtx_blocks_dec;

	/* Look up the block name and verify the instance */
	while(block->id >= 0) {
		if (!(strcasecmp(block->name, name))) {
			if (inst >= 0 && inst < block->num_instances)
				return block->id;
			else {
				printf("ERROR: Invalid block-inst [%d]\n",
				       inst);
				return -1;
			}
		}
		block++;
	}

	printf("ERROR: Invalid block-id [%s]\n", name);
	return -1;
}

/*
 * Get the hex value of a command.
 *
 *  name:			Name of command to decode.
 *
 *  Returns:			Command index, or -1 if name is not a valid
 *				command.
 */
static int cmd_name2id(char	*name)
{
	struct bit_field_decode *cmd_dec = commands_dec;

	/* Find the command value */
	while(cmd_dec->u.cmd_id >=0) {
		if (!(strcasecmp(cmd_dec->name, name)))
			return cmd_dec->u.cmd_id;
		cmd_dec++;
	}

	return -1;
}

/*
 * Get the hex value of a source.
 *
 *  name:			Name of source to look up.
 *
 *  Returns:			Source index, or -1 if name is not a valid
 *				source.
 */
static int src_name2id(char	*name)
{
	struct bit_field_decode *src_dec = sources_dec;

	/* Find the source name */
	while(src_dec->u.src_id >= 0) {
		if (!(strcasecmp(src_dec->name, name)))
			return src_dec->u.src_id;
		src_dec++;
	}

	return -1;
}

/*
 * Get the hex value of a destination.
 *
 *  name:			Name of destination to look up.
 *
 *  Returns:			Destination index, or -1 if name is not a valid
 *				destination.
 */
static int dst_name2id(char	*name)
{
	struct bit_field_decode *dst_dec = destinations_dec;

	/* Find the source name */
	while(dst_dec->u.dst_id >= 0) {
		if (!(strcasecmp(dst_dec->name, name)))
			return dst_dec->u.dst_id;
		dst_dec++;
	}

	return -1;
}

/*
 * Make sure "all" is the only command in the list.
 *
 *  cmds:			Pointer to list of commands.
 *  num_cmds:			Number of commands on the list.
 */
static void all_cmd_is_unique(int	*cmds,
			      int	*num_cmds)
{
	int	i;
	int	all_id = cmd_name2id("ALL");

	for (i = 0; i < *num_cmds; i++) {
		if (cmds[i] == all_id) {
			cmds[0] = all_id;
			*num_cmds = 1;
			break;
		}
	}
}

/*
 * Verify the DTXs are properly configured.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int verify_dtx_configuration(void)
{
	const cvmx_dtx_id_t	*valid_dtx;
	const cvmx_dtx_id_t	*valid_id;
	uint			i;

	/* Make sure at least one dtx is configured */
	if (command.dtx_info.cnt == 0) {
		printf("ERROR: No dtx-desc configured\n");
		return -1;
	}

	/* The match filter should have 2 DTXs configured */
	if ((command.filter.format == MATCH_FORMAT) &&
	    (command.dtx_info.cnt != 2)) {
		printf("ERROR: dtx-desc configured for match filter\n");
		return -1;
	}

	/* Verify the ocla complex supports all the DTXs */
	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		valid_dtx = valid_70xx_dtx;
	} else if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		/* Ocla 0:3 support different DTXs than ocla 4 */
		if (command.ix < 4)
			valid_dtx = valid_78xx_03_dtx;
		else
			valid_dtx = valid_78xx_4_dtx;
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		/* Ocla 0:1 support different DTXs than ocla 2 */
		if (command.ix < 2)
			valid_dtx = valid_78xx_03_dtx;
		else
			valid_dtx = valid_73xx_2_dtx;
	} else if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		/* Ocla 0:1 support different DTXs than ocla 2 */
		if (command.ix < 2)
			valid_dtx = valid_78xx_03_dtx;
		else
			valid_dtx = valid_75xx_2_dtx;
	} else {
		printf("ERROR: Unsupported octeon model\n");
		return -EINVAL;
	}

	for (i = 0; i < command.dtx_info.cnt; i++) {
		if (command.dtx_info.dtx[i].id != INVALID_BLOCK_ID) {
			valid_id = valid_dtx;
			while ((command.dtx_info.dtx[i].id != *valid_id) &&
			       (*valid_id != INVALID_BLOCK_ID))
				valid_id++;

			if (*valid_id == INVALID_BLOCK_ID) {
				printf("ERROR: Invalid dtx block-id\n");
				return -EINVAL;
			}
		}
	}

	/* Verify the dtx instance matches the complex index as needed */
	if ((OCTEON_IS_MODEL(OCTEON_CN78XX) && command.ix < 4) ||
	    (OCTEON_IS_MODEL(OCTEON_CN73XX) && command.ix < 2) ||
	    (OCTEON_IS_MODEL(OCTEON_CNF75XX) && command.ix < 2)) {
		for (i = 0; i < command.dtx_info.cnt; i++) {
			if (command.dtx_info.dtx[i].inst != command.ix) {
				printf("ERROR: dtx instance must match ocla "
				       "complex\n");
				return -EINVAL;
			}
		}
	}

	return 0;
}

/*
 * Verify the input parameters are compatible with each other.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int verify_params(void)
{
	/* A filter command must be specified */
	if (command.filter.format == INVALID_FORMAT) {
		printf("ERROR: A filter must be specified\n");
		return -EINVAL;
	}

	/* Verify the DTXs */
	if (verify_dtx_configuration())
		return -EINVAL;

	/* The ddr buffer must be a multiple of 128 bytes */
	if ((command.buf_size < CVMX_CACHE_LINE_SIZE * 32) ||
	    command.buf_size % CVMX_CACHE_LINE_SIZE) {
		printf("ERROR: Invalid ddr buffer size\n");
		return -EINVAL;
	}

	/* The trigger must match the mode */
	if ((command.trig_mode == TRIG_START) ||
	    (command.trig_mode == TRIG_STOP)) {
		if (command.trig.format == INVALID_FORMAT) {
			printf("ERROR: No trigger defined\n");
			return -EINVAL;
		}
	}

	/* The filter and trigger must use the same format */
	if (command.trig_mode != TRIG_NONE) {
		if (command.filter.format != command.trig.format) {
			printf("ERROR: Filter and trigger format don't "
			       "match\n");
			return -EINVAL;
		}
	}

	/* Match triggers can only have one command */
	if (command.trig.format == MATCH_FORMAT) {
		if (command.trig.u.match.num_cmds > 1) {
			printf("ERROR: Triggers only support one command\n");
			return -EINVAL;
		}
	}

	/*
	 * The number of match filter commands should be equal to the number of
	 * matchers minus the number of configured triggers.
	 */
	if (command.filter.format == MATCH_FORMAT) {
		int	trig_num;

		trig_num = command.trig.format != INVALID_FORMAT ? 1 : 0;
		if (command.filter.u.match.num_cmds >
		    (MAX_MATCHERS - trig_num)) {
			printf("ERROR: Too many filter commands\n");
			return -EINVAL;
		}
	}

	return 0;
}

/*
 * Get the trigger mode from the command line.
 *
 *  _spec:			String containing the trigger mode.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_trig_mode(const char	*_spec)
{
	command.trig_mode = strtol(_spec, NULL, 0);
	if (strcmp(optarg, "none") == 0)
		command.trig_mode = TRIG_NONE;
	else if (strcmp(optarg, "start") == 0)
		command.trig_mode = TRIG_START;
	else if (strcmp(optarg, "stop") == 0)
		command.trig_mode = TRIG_STOP;
	else {
		printf("ERROR: Illegal trig-mode=%s\n", optarg);
		return -1;
	}

	return 0;
}

/*
 * Get the ocla complex from the command line.
 *
 *  _spec:			String containing the ocla index.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_complex(const char	*_spec)
{
	command.ix = strtol(_spec, NULL, 0);
	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		if (command.ix >= 1) {
			printf("ERROR: Invalid ocla complex index. Valid range "
			       "[0]\n");
			return -1;
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		if (command.ix >= 5) {
			printf("ERROR: Invalid ocla complex index. Valid range "
			       "[0-4]\n");
			return -1;
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		   OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		if (command.ix >= 3) {
			printf("ERROR: Invalid ocla complex index. Valid range "
			       "[0-2]\n");
			return -1;
		}
	} else {
		printf("ERROR: Unsupported octeon model\n");
		return -1;
	}

	return 0;
}

/*
 * Get the node from the command line.
 *
 *  _spec:			String containing the node.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_node(const char	*_spec)
{
	command.node = strtol(_spec, NULL, 0);
	if (command.node >= CVMX_MAX_NODES) {
		printf("ERROR: Invalid node. Node range [0-%d]\n",
		       CVMX_MAX_NODES - 1);
		return -1;
	}

	return 0;
}

/*
 * Display information about one of the compounded arguments.
 *
 *  _spec:			String containing name of compounded argument.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_long_help(const char	*_spec,
			   char 	*argv0)
{
	if (!(strcasecmp(_spec, "complex"))) {
		if (OCTEON_IS_MODEL(OCTEON_CN70XX))
			printf(COMPLEX_USAGE_70XX);
		else if (OCTEON_IS_MODEL(OCTEON_CN73XX) ||
			 OCTEON_IS_MODEL(OCTEON_CNF75XX))
			printf(COMPLEX_USAGE_73XX);
		else if (OCTEON_IS_MODEL(OCTEON_CN78XX))
			printf(COMPLEX_USAGE_78XX);
	} else if (!(strcasecmp(_spec, "dtx")))
		printf(dtx_desc_usage);
	else if (!(strcasecmp(_spec, "match"))) {
		printf(MATCH_USAGE);
		printf(SOURCES_USAGE);
		printf(DESTINATIONS_USAGE);
		printf(ADDRESS_USAGE);
	} else if (!(strcasecmp(_spec, "signal")))
		printf(SIGNAL_USAGE);
	else if (!(strcasecmp(_spec, "command")))
		printf(COMMAND_USAGE);
	else if (!(strcasecmp(_spec, "examples")))
		printf(EXAMPLES_USAGE, argv0, argv0, argv0, argv0, argv0,
		       argv0);
	else {
		printf("Invalide argument %s\n", _spec);
		return -1;
	}

	return 0;
}

/*
 * Parse the string passed by the user for a dtx definition. The string is in
 * dtx-desc format (<block-id>,<block-inst>,<half>,<select>,<mask>).
 *
 *  _line:			DTX definition string to parse.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_dtx_def(const char	*_line)
{
	char		line[80];
	char		*parse = line;
	char 		*token;
	int		id;
	char		*id_name = NULL;
	int		inst = -1;
	int		half = -1;
	cvmx_dtx_def_t	*dtx_def;

	strncpy(line, _line, sizeof(line));
	line[sizeof(line) - 1] = 0;

	/* First part is the block id name */
	token = strsep(&parse, ",");
	if (token)
		id_name = token;

	/* Second part is the block instance */
	token = strsep(&parse, ",");
	if (token) {
		inst = strtol(token, NULL, 0);
		if ((id = dtx_name2id(id_name, inst)) < 0)
			return -1;
	}
	else {
		printf("ERROR: block-inst value is missing\n");
		return -1;
	}

	/* Third part is the 36-bit debug bus half */
	token = strsep(&parse, ",");
	if (token) {
		half = strtol(token, NULL, 0);
		if (half < 0 || half > 1 ) {
			printf("ERROR: Invalid half value [%s]\n", token);
			return -1;
		}
	} else {
		printf("ERROR: half value is missing\n");
		return -1;
	}

	/* We can now update the dtx_def data */
	if ((dtx_def = alloc_dtx()) == NULL) {
		printf("ERROR: DTX per half limit reached\n");
		return -1;
	}
	dtx_def->id = id;
	dtx_def->half = half;
	dtx_def->inst = inst;

	/* Fourth part is the select value */
	token = strsep(&parse, ",");
	if (token)
		dtx_def->sel = strtol(token, NULL, 0);
	else {
		printf("ERROR: select value missing\n");
		return -1;
	}

	/* Fith part is the enable mask */
	token = strsep(&parse, ",");
	if (token)
		dtx_def->ena = strtol(token, NULL, 0);
	else {
		printf("ERROR: mask value missing\n");
		return -1;
	}

	return 0;
}

/*
 * Parse the string passed by the user for a filter/trigger. The string is in
 * match format (<commands>,<sources>,<destinations>,<address>,<mask>).
 *
 *  _spec:			String to parse
 *  filter_info:		Pointer to filter/trigger to update.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_match_filter(const char		*_spec,
			      struct filter_info	*filter_info)
{
	char	spec[80];
	char	*parse = spec;
	char	*token;

	/* No duplicate filter/triggers */
	if (filter_info->format != INVALID_FORMAT) {
		printf("ERROR: duplicate filter or trigger\n");
		return -1;
	}

	strncpy(spec, _spec, sizeof(spec));
	spec[sizeof(spec) - 1] = 0;

	filter_info->format = MATCH_FORMAT;
	filter_info->u.match.num_cmds = 0;

	/* First part is the command, they are be separated by '+' */
	token = strsep(&parse, ",");
	if (token) {
		int	num_cmds = 0;

		char *tkn = strsep(&token, "+");
		while (tkn) {
			int	cmd;

			/* Make sure the command is valid and get its index */
			if ((cmd = cmd_name2id(tkn)) < 0) {
				printf("ERROR: Invalid command [%s]\n", tkn);
				return -1;
			}

			filter_info->u.match.cmds[num_cmds] = cmd;
			num_cmds += 1;
			filter_info->u.match.num_cmds = num_cmds;
			if (num_cmds >= MAX_MATCHERS)
				break;
			tkn = strsep(&token, "+");
		}
	}

	/* Enforce "all" is a single command */
	all_cmd_is_unique(filter_info->u.match.cmds,
			  &filter_info->u.match.num_cmds);

	/* Second part is the SID (optional) */ 
	filter_info->u.match.sid = 0;
	token = strsep(&parse, ",");
	if (token) {
		if ((filter_info->u.match.sid = src_name2id(token)) < 0) {
			printf("ERROR: Invalid sid [%s]\n", token);
			return -1;
		}
	}

	/* Third part is the DID (optional) */ 
	filter_info->u.match.did = 0;
	token = strsep(&parse, ",");
	if (token) {
		if ((filter_info->u.match.did = dst_name2id(token)) < 0) {
			printf("ERROR: Invalid did [%s]\n", token);
			return -1;
		}
	}

	/* Fourth part is the address (optional) */
	filter_info->u.match.addr = 0;
	token = strsep(&parse, ",");
	if (token)
		filter_info->u.match.addr = strtol(token, NULL, 0);

	/* Fifth part is the mask (optional) */
	filter_info->u.match.mask = 0;
	token = strsep(&parse, ",");
	if (token)
		filter_info->u.match.mask = strtol(token, NULL, 0);

	return 0;
}

/*
 * Parse the string passed by the user for a filter/trigger. The string is in
 * signal format (<half>,<bits>,<mask>).
 *
 *  _spec:			String to parse
 *  filter_info:		Pointer to filter/trigger to update.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_signal_filter(const char		*_spec,
			       struct filter_info	*filter_info)
{
	char	spec[80];
	char	*parse = spec;
	char	*token;
	uint	half = 0;

	strncpy(spec, _spec, sizeof(spec));
	spec[sizeof(spec) - 1] = 0;

	filter_info->format = SIGNAL_FORMAT;

	/* First part is the 36-bit debug bus half */
	token = strsep(&parse, ",");
	if (token) {
		half = strtol(token, NULL, 0);
		if (half > 1) {
			printf("ERROR: Invalid half value=%s\n", token);
			return -1;
		}
	}
	else {
		printf("ERROR: Missing half value\n");
			return -1;
	}

	/* No duplicate filter/triggers */
	if (filter_info->u.signal[half].en) {
		printf("ERROR: duplicate filter or trigger\n");
		return -1;
	}
	filter_info->u.signal[half].en = 1;

	/* Second part is the value of the 36-bits */
	token = strsep(&parse, ",");
	if (token)
		filter_info->u.signal[half].bits = strtol(token, NULL, 0);
	else {
		printf("ERROR: Missing bits parameter\n");
		return -1;
	}

	/* Third part is the mask for the 36-bits */
	filter_info->u.signal[half].mask = 0xfffffffffull;
	token = strsep(&parse, ",");
	if (token)
		filter_info->u.signal[half].mask = strtol(token, NULL, 0);

	return 0;
}

/*
 * Parse the string passed by the user for a filter/trigger.
 *
 *  _spec:			String to parse
 *  filter_info:		Pointer to filter/trigger to update.
 *
 *  Returns:			Zero on success, negative on failure
 */
static int parse_filter(const char		*_spec,
			struct filter_info	*filter_info)
{
	char	spec[80];
	char	*parse = spec;
	char 	*token;
	int	rc = -1;

	/*
	 * Determine the format of the filter spec. Two formats are currently
	 * supported, match and signal. The first token in a match spec is the
	 * command and the first token in a signal spec is the half.
	 */
	strncpy(spec, _spec, sizeof(spec));
	spec[sizeof(spec) - 1] = 0;
	token = strsep(&parse, ",");
	if (token) {
		if (isdigit(_spec[0]))
			rc = parse_signal_filter(_spec, filter_info);
		else {
			if ((rc = parse_match_filter(_spec, filter_info)) < 0)
				return rc;
		}
	}

	return rc;
}

/*
 * Parse the command line options
 *
 *  argc:			Arguments
 *  argv:			Argument strings
 *
 *  Returns:			Zero if capture is to continue, negative
 *				otherwise.
 */
int parse_options(int argc, char * const *argv)
{
	const struct option long_options[] = {
		{"buffer", 1, 0, 'b'},
		{"complex", 1, 0, 'c'},
		{"dtx", 1, 0, 'd'},
		{"filter", 1, 0, 'f'},
		{"help", 0, 0, 'h'},
		{"long-help", 0, 0, 'H'},
		{"trig-mode", 1, 0, 'm'},
		{"node", 1, 0, 'n'},
		{"reset", 0, 0, 'r'},
		{"trig", 1, 0, 't'},
		{"version", 0, 0, 'v'},
		{NULL, 0, 0, 0}
	};

	int option_index = 0;

	while (1) {
		char option = getopt_long(argc, argv, "hvrb:c:d:f:H:m:n:t:",
					  long_options, &option_index);
		if (option <= 0)
			break;

		switch (option) {
		case 'b':
			command.buf_size = strtol(optarg, NULL, 0);
			break;

		case 'c':
			if (parse_complex(optarg))
				return -1;
			break;

		case 'd':
			if (parse_dtx_def(optarg))
				return -1;
			break;

		case 'f':
			if (parse_filter(optarg, &command.filter))
				return -1;
			break;

		case 'h':
			printf(USAGE, argv[0], DFLT_DDR_BUF_SIZE);
			return -1;

		case 'H':
			parse_long_help(optarg, argv[0]);
			return -1;

		case 'm':
			if (parse_trig_mode(optarg))
				return -1;
			break;

		case 'n':
			if (parse_node(optarg))
				return -1;
			break;

		case 'r':
			command.dtx_reset = 1;
			break;

		case 'v':
			printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
			return -1;
            
		case 't':
			if (parse_filter(optarg, &command.trig))
				return -1;
			break;

		default:
			return -1;
		}
	}

	/*
	 * Configure the DTXs needed for format filters. This needs to be done
	 * after all arguments have been parsed as the ocla complex index is
	 * needed.
	 */
	if (command.filter.format == MATCH_FORMAT) {
		/* Match filters need the following 2 DTXs */
		if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
			parse_dtx_def(MATCH_LOWER_DTX_70XX);
			parse_dtx_def(MATCH_UPPER_DTX_70XX);
		} else if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
			   OCTEON_IS_MODEL(OCTEON_CN73XX) ||
			   OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
			char	dtx[40];

			snprintf(dtx, 40, MATCH_LOWER_DTX_78XX, command.ix);
			parse_dtx_def(dtx);
			snprintf(dtx, 40, MATCH_UPPER_DTX_78XX, command.ix);
			parse_dtx_def(dtx);
		}
	}

	/* Verify the input parameters are compatible with each other */
	return verify_params();
}

/*
 * Display the output header.
 */
static void display_header(void)
{
	if (command.filter.format == MATCH_FORMAT)
		printf("\n\n%-35s\t%-12s%-10s%-6s%-6s%-6s%-6s%s\n",
		       "Raw data", "Timestamp", "Command", "Node", "Stid",
		       "Did", "Sid", "Address");
	else
		printf("\n\n%-35s\t%-12s\n", "Raw data", "Timestamp");

	fflush(NULL);
}

/*
 * Save the data from the low entry.
 *
 *  raw_data:			Low data packet read from ocla fifo.
 *  display_data:		Updated with the decoded raw_data.
 */
static void decode_l2c_300500_data(uint64_t		raw_data,
				   cvmx_display_data_t	*display_data)
{
	cvmx_dtx_l2c_300500_t	data;
	char			*cmd;

	data.u64 = raw_data;

	display_data->xmd_msk 	= data.s.xmd_msk;
	display_data->stid 	= data.s.stid;
	display_data->node	= data.s.node;
	display_data->did	= data.s.did;
	display_data->sid	= data.s.sid;

	if ((cmd = id2name(data.s.cmd, commands_dec))) {
		strncpy(display_data->cmd, cmd, NAME_LEN);
		display_data->cmd[NAME_LEN - 1] = '\0';
	}
	else
		snprintf(display_data->cmd, NAME_LEN, "%#x", data.s.cmd);
}

/*
 * Save the data from the hi entry.
 *
 *  raw_data:			High data packet read from ocla fifo.
 *  display_data:		Updated with the decoded raw_data.
 */
static void decode_l2c_300502_data(uint64_t		raw_data,
				   cvmx_display_data_t	*display_data)
{
	cvmx_dtx_l2c_300502_t	data;

	data.u64 = raw_data;

	display_data->addr = data.s.addr;
}

/*
 * Decode the raw data and display it.
 *
 *  raw_data:			Packet read from ocla fifo.
 */
static void decode_raw_data(uint64_t raw_data)
{
	static uint32_t			cycle = 0;
	static uint64_t			time_delta = 0;
	cvmx_ocla_cap_ctl_t		cap_ctl;
	cvmx_ocla_cap_dat_t		cap_dat;
	static cvmx_display_data_t	display_data;
	static int			i = 0;

	/* The raw data could be a control packet or a data packet */
	cap_ctl.u64 = raw_data;
	cap_dat.u64 = raw_data;

	/* Calculate the time since the last transaction */
	if (cap_ctl.s.ctl) {
		if (cap_ctl.s.sot0) {
			time_delta = cap_ctl.s.cycle - 1 - cycle;
			cycle = cap_ctl.s.cycle - 1;
		}
		else {
			printf("0x%016llx\t\t\t\t    Control Packet\n",
			       (long long)raw_data);
		}
	}
	else if (!cap_dat.s.hi) {
		/*
		 * It's a low data packet. Save it. Need to wait for the high
		 * data packet to display all the information for this bus 
		 * event.
		 */
		display_data.raw_lo_data = cap_dat.u64;
		if (command.filter.format == MATCH_FORMAT)
			decode_l2c_300500_data(cap_dat.s.data, &display_data);
	} 
	else if (cap_dat.s.hi) {
		/* High data packet */
		display_data.raw_hi_data = cap_dat.u64;
		if (command.filter.format == MATCH_FORMAT)
			decode_l2c_300502_data(cap_dat.s.data, &display_data);

		/* Every 50 lines print the headers */
		if (!(--i % 50))
			display_header();

		/* Print the raw data */
		printf("0x%016llx %016llx", (long long)display_data.raw_hi_data,
		       (long long)display_data.raw_lo_data);

		/* Print the time delta */
		printf("\t%-12ld", time_delta);

		/* Print the command, node, stid, did, sid, and address */
		if (command.filter.format == MATCH_FORMAT)
			printf("%-10s%-6d%-6d%-6d%-6d0x%016llx\n",
			       display_data.cmd, display_data.node,
			       display_data.stid, display_data.did,
			       display_data.sid, (long long)display_data.addr);
		else
			printf("\n");
	}
}

/*
 * Setup the matchers (signal format).
 *
 *  returns:			0 on success, error otherwise.
 */
static int signal_matcher_setup(void)
{
	/* The filter uses matcher0 */
	complex.num_fil_mat = 1;
	cvmx_matcher_init(command.node, command.ix, 0,
			  command.filter.u.signal[0].mask,
			  command.filter.u.signal[0].bits,
			  command.filter.u.signal[1].mask,
			  command.filter.u.signal[1].bits);

	/* The trigger uses matcher1 */
	complex.trig_mat = 1;
	if (command.trig.format != INVALID_FORMAT) {
		cvmx_matcher_init(command.node, command.ix, 1,
				  command.trig.u.signal[0].mask,
				  command.trig.u.signal[0].bits,
				  command.trig.u.signal[1].mask,
				  command.trig.u.signal[1].bits);
	}

	return 0;
}

/*
 * Setup the matchers (match format).
 *
 *  returns:			0 on success, error otherwise.
 */
static int match_matcher_setup(void)
{
	struct match	*match;
	int		avail_fil_mat = MAX_MATCHERS;
	int		next_mat = 0;
	int		i;

	/*
	 * The trigger needs one matcher. The rest of the matchers are available
	 * for the filter to use. If no trigger is configured, the filter gets
	 * all available matchers. The filter always uses the lowest matchers.
	 */
	if (command.trig.format != INVALID_FORMAT)
		avail_fil_mat--;

	/*
	 * Configure as many filter matchers as possible. If the command is
	 * "all", we need to use two filters, one for the command (matcher0) and
	 * one for the address (matcher1). The reason we can't use a single 
	 * matcher for the "all" command is because the matcher ANDs the lower
	 * and upper half results. However, we need the complement of the half
	 * matching the command as we set it to the no command code (nop). So
	 * it would look like this: !nop && addr -> match.
	 */
	match = &command.filter.u.match;
	if (match->cmds[0] == ALL_CMD) {
		cvmx_matcher_init(command.node, command.ix, 0, 0xfe, 0, 0, 0);
		cvmx_matcher_init(command.node, command.ix, 1, 0, 0,
				  match->mask, match->addr);
		complex.num_fil_mat = 2;
	}
	else {
		for (i = 0; i < match->num_cmds && i < avail_fil_mat; i++) {
			cvmx_matcher_init(command.node, command.ix, i, 0xfe,
					  match->cmds[i] << 1, match->mask,
					  match->addr);
		}
		complex.num_fil_mat = i;
	}

	/* Configure the trigger matcher if present */
	next_mat = complex.num_fil_mat;
	if (command.trig.format != INVALID_FORMAT) {
		match = &command.trig.u.match;
		complex.trig_mat = next_mat;
		if (match->cmds[0] == ALL_CMD) {
			cvmx_matcher_init(command.node, command.ix,
					  complex.trig_mat, 0, 0, match->mask,
					  match->addr);
		}
		else {
			cvmx_matcher_init(command.node, command.ix,
					  complex.trig_mat, 0xfe,
					  match->cmds[0] << 1, match->mask,
					  match->addr);
		}
	}

	return 0;
}

/*
 * Setup the matchers.
 *
 *  returns:			0 on success, error otherwise.
 */
static int matcher_setup(void)
{
	int	rc = 0;

	if (command.filter.format == MATCH_FORMAT)
		rc = match_matcher_setup();
	else
		rc = signal_matcher_setup();

	return rc;
}

/*
 * Configure fsm0 for the filters.
 *
 *  fsm1_st_2cap:		State fsm1 must be in for fsm0 to capture.
 *
 *  returns:			0 on success, error otherwise.
 */
static int fsm_set_filters(int	fsm1_st_2cap)
{
	cvmx_fsm_input_t	mcd;
	cvmx_fsm_input_t	matcher;
	cvmx_fsm_input_t	fsm0_st;
	cvmx_fsm_input_t	fsm1_st;
	struct match		*match;
	int			cur_st = 0;

	/*
	 * Two cases are considered:
	 *  1. We want to capture all commands.
	 *  2. We want to capture specific commands.
	 *
	 * Filters use fsm0.
	 */
	match = &command.filter.u.match;
	if (match->cmds[0] == ALL_CMD) {
		/*
		 * FSM0 stays in state0 when there's something on the bus
		 * and the address match.
		 */
		mcd.msk = 0;
		matcher.msk = 3;
		matcher.val = 2;
		fsm0_st.msk = 0;
		fsm1_st.msk = 0xf;
		fsm1_st.val = fsm1_st_2cap;
		cvmx_fsm_init(command.node, command.ix, 0, &mcd, &matcher,
			      &fsm0_st, &fsm1_st, cur_st);

		/* This state captures */
		complex.fsm_action[0][cur_st++] = ACTION_CAP;

		/*
		 * FSM0 goes to state1 otherwise. Going to state1 has lower
		 * priority.
		 */
		mcd.msk = 0;
		matcher.msk = 0;
		fsm0_st.msk = 0;
		fsm1_st.msk = 0;
		cvmx_fsm_init(command.node, command.ix, 0, &mcd, &matcher,
			      &fsm0_st, &fsm1_st, cur_st++);
	}
	else {
		int	i;

		/*
		 * FSM0 must go to a state that captures for each matcher in
		 * use.
		 */
		for (i = 0; i < complex.num_fil_mat; i++) {
			mcd.msk = 0;
			matcher.msk = 1 << i;
			matcher.val = 1 << i;
			fsm0_st.msk = 0;
			fsm1_st.msk = 0xf;
			fsm1_st.val = fsm1_st_2cap;
			cvmx_fsm_init(command.node, command.ix, 0, &mcd,
				      &matcher, &fsm0_st, &fsm1_st, cur_st);

			/* Each of these states capture */
			complex.fsm_action[0][cur_st++] = ACTION_CAP;
		}

		/*
		 * FSM0 goes to the next state otherwise. This state has the
		 * lowest priority.
		 */
		mcd.msk = 0;
		matcher.msk = 0;
		fsm0_st.msk = 0;
		fsm1_st.msk = 0;
		cvmx_fsm_init(command.node, command.ix, 0, &mcd, &matcher,
			      &fsm0_st, &fsm1_st, cur_st);
	}

	return 0;
}

/*
 * Configure fsm1 for the start trigger.
 *
 *  returns:			State fsm1 needs to be in for fsm0 to capture.
 */
static int fsm_set_start_trigger(void)
{
	cvmx_fsm_input_t	mcd;
	cvmx_fsm_input_t	matcher;
	cvmx_fsm_input_t	fsm0_st;
	cvmx_fsm_input_t	fsm1_st;

	/* FSM1 stays in state0 until the start trigger */
	mcd.msk = 0;
	matcher.msk = 1 << complex.trig_mat;
	matcher.val = 0;
	fsm0_st.msk = 0;
	fsm1_st.msk = 0xf;
	fsm1_st.val = 0;
	cvmx_fsm_init(command.node, command.ix, 1, &mcd, &matcher, &fsm0_st,
		      &fsm1_st, 0);

	/* FSM1 goes to state1 when in state0 and trigger occurs */
	mcd.msk = 0;
	matcher.msk = 1 << complex.trig_mat;
	matcher.val = 1 << complex.trig_mat;
	fsm0_st.msk = 0;
	fsm1_st.msk = 0xf;
	fsm1_st.val = 0;
	cvmx_fsm_init(command.node, command.ix, 1, &mcd, &matcher, &fsm0_st,
		      &fsm1_st, 1);

	/* Capture the trigger event */
	complex.fsm_action[1][1] = ACTION_CAP;

	/* FSM1 goes to state2 when in state1 */
	mcd.msk = 0;
	matcher.msk = 0;
	fsm0_st.msk = 0;
	fsm1_st.msk = 0x1;
	fsm1_st.val = 1;
	cvmx_fsm_init(command.node, command.ix, 1, &mcd, &matcher, &fsm0_st,
		      &fsm1_st, 2);

	/* Fsm0 must capture when fsm1 is in state 2 */
	return 2;
}

/*
 * Configure fsm1 for the stop trigger.
 *
 *  returns:			State fsm1 needs to be in for fsm0 to capture.
 */
static int fsm_set_stop_trigger(void)
{
	cvmx_fsm_input_t	mcd;
	cvmx_fsm_input_t	matcher;
	cvmx_fsm_input_t	fsm0_st;
	cvmx_fsm_input_t	fsm1_st;

	/* FSM1 stays in state0 until the stop trigger */
	mcd.msk = 0;
	matcher.msk = 1 << complex.trig_mat;
	matcher.val = 0;
	fsm0_st.msk = 0;
	fsm1_st.msk = 0xf;
	fsm1_st.val = 0;
	cvmx_fsm_init(command.node, command.ix, 1, &mcd, &matcher, &fsm0_st,
		      &fsm1_st, 0);

	/* FSM1 goes to state1 when in state0 and trigger occurs */
	mcd.msk = 0;
	matcher.msk = 1 << complex.trig_mat;
	matcher.val = 1 << complex.trig_mat;
	fsm0_st.msk = 0;
	fsm1_st.msk = 0xf;
	fsm1_st.val = 0;
	cvmx_fsm_init(command.node, command.ix, 1, &mcd, &matcher, &fsm0_st,
		      &fsm1_st, 1);

	/* Capture the trigger and generate an interrupt */
	complex.fsm_action[1][1] = ACTION_CAP | ACTION_INT;

	/* FSM1 goes to state2 when in state1 */
	mcd.msk = 0;
	matcher.msk = 0;
	fsm0_st.msk = 0;
	fsm1_st.msk = 0x1;
	fsm1_st.val = 1;
	cvmx_fsm_init(command.node, command.ix, 1, &mcd, &matcher, &fsm0_st,
		      &fsm1_st, 2);

	/* Fsm0 must capture when fsm1 is in state 0 */
	return 0;
}

/*
 * Setup the state machines.
 *
 *  returns:			0 on success, error otherwise.
 */
static int fsm_setup(void)
{
	int	fsm1_st;

	/*
	 * Three trigger modes are supported. The trigger mode determines how
	 * fsm1 is configured.
	 */
	switch (command.trig_mode) {
	case TRIG_START:
		fsm1_st = fsm_set_start_trigger();
		break;
	case TRIG_STOP:
		fsm1_st = fsm_set_stop_trigger();
		break;
	case TRIG_NONE:
	default:
		fsm1_st = 0;
		break;
	}

	/* Configure the filters */
	fsm_set_filters(fsm1_st);

	return 0;
}

/*
 * Configure the action to perform by the finite machine states.
 *
 *  returns:			0 on success, error otherwise.
 */
static int fsm_state_set_action(void)
{
	cvmx_fsm_action_t	*fsm_action;
	int			i;
	int			j;

	for (i = 0; i < MAX_FSMS; i++) {
		fsm_action = complex.fsm_action[i];
		for (j = 0; j < MAX_FSM_STATES; j++) {
			if (fsm_action[j] != NO_ACTION)
				cvmx_fsm_state_set(command.node, command.ix,
						   i, j, fsm_action[j]);
		}
	}
	return 0;
}

/*
 * Reset the ocla hardware to default values.
 *
 *  Returns:			Zero on success, error otherwise.
 */
static int hardware_unconfig(void)
{
	struct dtx_info	*dtx_info;
	uint		i;

	/* Must disable all DTXs used by this ocla complex */
	dtx_info = &command.dtx_info;
	for (i = 0; i < dtx_info->cnt; i++)
		cvmx_dtx_disable(command.node, &dtx_info->dtx[i]);

	/* Disable the ocla complex */
	cvmx_ocla_disable(command.node, command.ix);

	return 0;
}

/*
 * Get the maximum number of entries ocla can capture.
 *
 * return	Maximum number of entries ocla can capture, 0 for no limit.
 */
static int get_max_num_entries(void)
{
	cvmx_oclax_const_t	con;
	int			max_entries = 0;
	int			node = command.node;
	int			ix = command.ix;

	if ((command.trig_mode == TRIG_NONE) ||
	    (command.trig_mode == TRIG_START)) {
		/*
		 * If not trigger is defined, simply capture a reasonable 
		 * amount of entries (same as for a start trigger).
		 * If a start trigger is defined, the maximum number of entries
		 * to capture must be such that the buffer doesn't overflow
		 * (start trigger entry must be preserved).
		 */
		if (complex.pbuf && complex.buf_size) {
			max_entries = complex.buf_size > MAX_DDR_SIZE_NO_OVF ?
				MAX_DDR_SIZE_NO_OVF : complex.buf_size;
			max_entries = (max_entries / CVMX_CACHE_LINE_SIZE) * 26;
		}
		else {
			con.u64 = cvmx_read_csr_node(node,
						     CVMX_OCLAX_CONST(ix));
			max_entries = con.s.dat_size - 6;
		}
	}
	else if (command.trig_mode == TRIG_STOP) {
		/* No limit for stop triggers */
		max_entries = 0;
	}

	return max_entries;
}

/*
 * Intialize the ocla hardware.
 *
 * return	Zero on success, error otherwise.
 */
static int hardware_config(void)
{
	uint		i;
	int		rc;
	int		num_entries;
	cvmx_cap_sel_t	lo_cap;
	cvmx_cap_sel_t	hi_cap;

	/* Reset all DTXs on all nodes if requested */
	if (command.dtx_reset) {
		if ((rc = cvmx_dtx_reset())) {
			printf("ERROR: failed to reset DTXs\n");
			return rc;
		}
	}

	/* Configure the DTXs */
	for (i = 0; i < command.dtx_info.cnt; i++) {
		rc = cvmx_dtx_enable(command.node, &command.dtx_info.dtx[i]);
		if (rc < 0)
			goto hardware_config_err;
	}

	/* Reset the ocla complex */
	if ((rc = cvmx_ocla_reset(command.node, command.ix)) < 0)
		goto hardware_config_err;

	/* Get the number of entries to use */
	num_entries = get_max_num_entries();

	if (OCTEON_IS_MODEL(IS_OCLA_REV1)) {
		lo_cap = FSM0_OR_FSM1_REV1;
		hi_cap = FSM0_OR_FSM1_REV1;
	} else {
		lo_cap = FSM0_OR_FSM1_REV2;
		hi_cap = FSM0_OR_FSM1_REV2;
	}

	/* Configure the ocla complex */
	if ((rc = cvmx_ocla_init(command.node, command.ix, complex.pbuf,
				 complex.buf_size, num_entries,
				 lo_cap, hi_cap)) < 0)
		goto hardware_config_err;

	/* Initialize the matchers */
	matcher_setup();

	/* Setup the state machines */
	fsm_setup();

	/* Enable the fsm */
	cvmx_fsm_enable(command.node, command.ix, 0);
	cvmx_fsm_enable(command.node, command.ix, 1);

	/* Set the action to perform by the fsm states */
	fsm_state_set_action();

	return 0;

 hardware_config_err:
	hardware_unconfig();
	return rc;
}

/*
 * Send ioctl to kernel driver to request capture.
 *
 *  fd:				Open filed descriptor to use.
 */
static int linux_send_cap_req(int	fd)
{
	struct cap_req	req;
	int		rc = 0;

	/* Build the capture request */
	req.node	= command.node;
	req.ix		= command.ix;

	/* Send the request */
	if ((rc = ioctl(fd, OCLA_CAP_REQ, &req)) < 0)
		printf("ERROR: Ioctl CAP_REQ command failed\n");

	return rc;
}

/*
 * Send request to get a kernel buffer to use as capture buffer.
 */
static int linux_send_ddr_buf_req(int	fd)
{
	struct ddr_buf_req	req;
	int			rc;

	/* Build the capture buffer request */
	req.node	= command.node;
	req.ix		= command.ix;
	req.size	= command.buf_size;
	req.pbuf	= 0;

	/* Send the request */
	if ((rc = ioctl(fd, OCLA_DDR_BUF_REQ, &req)) < 0) {
		printf("ERROR: Ioctl CAP_REQ command failed\n");
		return rc;
	}

	complex.pbuf = req.pbuf;
	complex.buf_size = command.buf_size;

	return 0;
}

/*
 * Read the ocla entries via the kernel driver.
 */
static int linux_data_read(void)
{
	int	fd;
	char	*buf;
	char	*bufp;
	int	cnt;
	int	i;
	int	rc = 0;

	octeon_remote_lock();

	if ((fd = open(NODE_NAME, O_RDWR)) < 0) {
		printf("WARNING: Failed to open device %s\n", NODE_NAME);
		rc = -ENOENT;
		goto linux_data_read_err3;
	}

	if ((buf = malloc(READ_BUF_SIZE)) == NULL) {
		printf("ERROR: Failed to allocate memory\n");
		rc = -ENOMEM;
		goto linux_data_read_err2;
	}

	/* Get a kernel buffer (ddr capture buffer) */
	if ((rc = linux_send_ddr_buf_req(fd)))
		goto linux_data_read_err1;

	/* Configure the hardware */
	if ((rc = hardware_config()))
		goto linux_data_read_err;

	/* Build and send a capture request to driver */
	if ((rc = linux_send_cap_req(fd)))
		goto linux_data_read_err;

	/* Control-C signal handler tells us when to die */
	original_sig_int = signal(SIGINT, sig_int_handler);

	printf(WAIT_CAPTURE_DONE);

	/* Wait until capture is done (block here until capture is finished) */
	cnt = read(fd, buf, READ_BUF_SIZE);

	/* If capture terminated via ctrl-c, must stop capture */
	if (finished || cnt < 0)
		cvmx_ocla_disable(command.node, command.ix);

	/* Read the captured data */
	display_header();
	while (cnt > 0) {
		bufp = buf;
		for (i = 0; i < cnt / 8; i++) {
			decode_raw_data(*(uint64_t *)bufp);
			bufp += 8;
		}
		cnt = read(fd, buf, READ_BUF_SIZE);
	}

	/* We're done, restore the interrupt handler and stop OCLA */
	signal(SIGINT, original_sig_int);

 linux_data_read_err:
	hardware_unconfig();
 linux_data_read_err1:
	free(buf);
 linux_data_read_err2:
	close(fd);
 linux_data_read_err3:
	octeon_remote_unlock();
	return rc;
}

/*
 * Check if capture has finished.
 *
 *  Returns:			1 if capture finished, 0 otherwise.
 */
static int is_capture_finished(void)
{
	int	rc = 0;

	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		cvmx_ciu_cib_oclax_rawx_t	raw;

		raw.u64 = cvmx_read_csr(CVMX_CIU_CIB_OCLAX_RAWX(0, 0));
		rc = (raw.s.state_fsm1_int || raw.s.state_trigfull);

		/* Clear interrupt */
		if (rc)
			cvmx_write_csr(CVMX_CIU_CIB_OCLAX_RAWX(0, 0), raw.u64);
	} else if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		   OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		   OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		cvmx_oclax_state_int_t	state_int;

		state_int.u64 = cvmx_read_csr(CVMX_OCLAX_STATE_INT(command.ix));
		rc = (state_int.s.fsm1_int || state_int.s.trigfull);

		/* Clear interrupt */
		if (rc) {
			state_int.u64 = 0;
			state_int.s.fsm0_int = 1;
			state_int.s.fsm1_int = 1;
			state_int.s.trigfull = 1;
			state_int.s.captured = 1;
			cvmx_write_csr(CVMX_OCLAX_STATE_INT(command.ix),
				       state_int.u64);
		}
	}

	return rc;
}

/*
 * Read the ocla entries via the pci bus.
 */
static int pci_data_read(void)
{
	uint64_t	data;
	int		rc;

	octeon_remote_lock();

	/* Configure the hardware */
	if ((rc = hardware_config()))
		goto pci_data_read_err;
		
	/* Control-C signal handler tells us when to die */
	original_sig_int = signal(SIGINT, sig_int_handler);

	printf(WAIT_CAPTURE_DONE);

	/* Wait until capture is finished or user stops waiting */
	while (!finished && !is_capture_finished()) {
		sleep(1);
		continue;
	}

	/* Read ocla data fifo */
	display_header();
	while (cvmx_ocla_get_packet(command.node, command.ix, &data) == 0)
		decode_raw_data(data);

	/* We're done, restore the interrupt handler and stop OCLA */
	signal(SIGINT, original_sig_int);

 pci_data_read_err:
	hardware_unconfig();
	octeon_remote_unlock();
	return rc;
}

/*
 * Main
 */
int main(int argc, char * const *argv)
{
	const char	*protocol;
	int		i;
	int		j;
	int		rc = 0;

	/* Check at least an argument is present */
	if (argc < 2) {
		printf(USAGE, argv[0], DFLT_DDR_BUF_SIZE);
		return -EINVAL;
	}

	if ((rc = octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0)))
		return rc;

	if (!octeon_has_feature(OCTEON_FEATURE_OCLA)) {
		printf("This chip does not have OCLA support.\n");
		rc = -ENODEV;
		goto main_err;
	}

	if (!octeon_remote_mem_access_ok()) {
		printf("ERROR: DRAM not setup, board needs to be booted\n");
		rc = -EPERM;
		goto main_err;
	}

	/* Initialize the command structure */
	memset(&command, 0, sizeof(command));
	for (i = 0; i < MAX_NUM_DTX; i++)
		command.dtx_info.dtx[i].id = INVALID_BLOCK_ID;
	command.trig_mode = TRIG_NONE;
	command.filter.format = INVALID_FORMAT;
	command.trig.format = INVALID_FORMAT;
	command.buf_size = DFLT_DDR_BUF_SIZE;

	/* Initialize the complex structure */
	memset(&complex, 0, sizeof(complex));
	for (i = 0; i < MAX_FSMS; i++) {
		for (j = 0; j < MAX_FSM_STATES; j++)
			complex.fsm_action[i][j] = NO_ACTION;
	}

	/* Initialize global pointers */
	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		dtx_desc_usage = DTX_DESC_USAGE_70XX;
		dtx_blocks_dec = dtx_blocks_70xx;
		commands_dec = commands_dec_7xxx;
		sources_dec = sources_dec_7xxx;
		destinations_dec = destinations_dec_7xxx;
	} else if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		dtx_desc_usage = DTX_DESC_USAGE_78XX;
		dtx_blocks_dec = dtx_blocks_78xx;
		commands_dec = commands_dec_7xxx;
		sources_dec = sources_dec_7xxx;
		destinations_dec = destinations_dec_7xxx;
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		dtx_desc_usage = DTX_DESC_USAGE_73XX;
		dtx_blocks_dec = dtx_blocks_73xx;
		commands_dec = commands_dec_7xxx;
		sources_dec = sources_dec_7xxx;
		destinations_dec = destinations_dec_7xxx;
	} else if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		dtx_desc_usage = DTX_DESC_USAGE_75XX;
		dtx_blocks_dec = dtx_blocks_75xx;
		commands_dec = commands_dec_7xxx;
		sources_dec = sources_dec_7xxx;
		destinations_dec = destinations_dec_7xxx;
	} else {
 		printf("ERROR: Unsupported octeon model\n");
 		return -EINVAL;
	}

	if ((rc = parse_options(argc, argv)))
		goto main_err;

	protocol = getenv("OCTEON_REMOTE_PROTOCOL");
	/* Linux protocol */
	if (!strcasecmp(protocol, "LINUX")) {
		if ((rc = linux_data_read()) == -ENOENT) {
			/* Kernel driver isn't loaded, try PCI protocol */
			protocol = "PCI";
		}
	}
	/* All other protocols */
	if (strcasecmp(protocol, "LINUX"))
		rc = pci_data_read();

 main_err:
	octeon_remote_close();
	return rc;
}
