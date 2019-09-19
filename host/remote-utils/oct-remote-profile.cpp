/****************************************************************
 * Copyright (c) 2003-2004, Cavium Inc.. All rights reserved.
 *
 * This Software is the property of Cavium Inc..  The Software and all
 * accompanying documentation are copyrighted.  The Software made available
 * here constitutes the proprietary information of Cavium Inc..  You
 * agree to take reasonable steps to prevent the disclosure, unauthorized use
 * or unauthorized distribution of the Software.  You shall use this Software
 * solely with Cavium hardware.
 *
 * Except as expressly permitted in a separate Software License Agreement
 * between You and Cavium Inc., you shall not modify, decompile,
 * disassemble, extract, or otherwise reverse engineer this Software.  You
 * shall not make any copy of the Software or its accompanying documentation,
 * except for copying incident to the ordinary and intended use of the
 * Software and the Underlying Program and except for the making of a single
 * archival copy.
 *
 * This Software, including technical data, may be subject to U.S.  export
 * control laws, including the U.S.  Export Administration Act and its
 * associated regulations, and may be subject to export or import regulations
 * in other countries.  You warrant that You will comply strictly in all
 * respects with all such regulations and acknowledge that you have the
 * responsibility to obtain licenses to export, re-export or import the
 * Software.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 * TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 **************************************************************************/

/**
 * @file
 *
 * Octeon profiler over remote interface
 *
 * $Id: oct-pci-profile.cpp 36671 2008-09-04 18:02:24Z rfranz $
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <signal.h>
#include <map>
#include "octeon-remote.h"
#include "octeon-remote-debug-handler.h"
#define CVMX_ADD_IO_SEG(a) (a)  /* There aren't any Mips IO segments when using PCI */
#include "cvmx.h"
#include "cvmx-version.h"
#include "cvmx-core.h"
#include "cvmx-l2c.h"
#include "cvmx-clock.h"
#include "cvmx-config.h"
#include "cvmx-profiler.h"
#include "cvmx-swap.h"
#include "cvmx-interrupt.h"

#define ULL	unsigned long long

typedef struct
{
    uint32_t count[CVMX_MAX_CORES];
} ProfileElement;

typedef struct { 
    int size;
    int sample_read;
    int64_t max_samples;
    int64_t sample_count;
    uint64_t head;
    uint64_t tail;
    uint64_t end;
    uint64_t data;
} cpu_event_block_t;

typedef struct {
    cpu_event_block_t pcpu_blk_info;
    uint64_t pcpu_data;
} ringbuf_t;

typedef std::map<uint64_t,ProfileElement> ProfileData;

static ProfileData      profile_data;
static const char *     proc_perf_label[CVMX_CORE_PERF_MAX];
static const char *     proc_perf_l2label[CVMX_L2C_EVENT_MAX];
static cvmx_core_perf_t perf_core[CVMX_CORE_MAX_PCNT];
static uint32_t         perf_l2[CVMX_L2C_MAX_PCNT];
static int              fast = 0;
static int              samplerate = 1000;
static int              core_mask_cmd_line = 0;
static int              dwell = 5;
static void (*original_sig_int)(int num) = NULL;
static int              finished = 0;
static uint64_t         last_update_bus[CVMX_L2C_MAX_TADS];
static uint64_t         last_update[CVMX_L2C_MAX_TADS][CVMX_L2C_MAX_PCNT];
static uint64_t         coremask;
static uint64_t         config_block_phy_addr = 0;
static int              event_profiler = 0;
static uint64_t         events = 0;
static int              first_time = 1;
static int              number_of_cores;
static uint64_t         pcpu_base_addr[CVMX_MAX_CORES];
static char             perf_ctr_event[15];
static int              collect_sample = 0;
static int              sample_cnt[CVMX_MAX_CORES];
static uint64_t         totsamples;
static uint64_t         sample_var;
static uint64_t         perfcore[CVMX_CORE_MAX_PCNT][CVMX_MAX_CORES];

struct proc_perf_l2tad_label
{
    cvmx_l2c_tad_event_t type;   /**< type of the event */
    const char *name;            /**< unique name of each event */
    int info;                    /**< based on the type of the event, print the counter value differently */
} proc_perf_l2tad_label[] = { 
        { CVMX_L2C_TAD_EVENT_NONE, "none", 0 },
        { CVMX_L2C_TAD_EVENT_TAG_HIT, "hit", 1 },
        { CVMX_L2C_TAD_EVENT_TAG_MISS, "miss", 1 },
        { CVMX_L2C_TAD_EVENT_TAG_NOALLOC, "no-alloc", 1 },
        { CVMX_L2C_TAD_EVENT_TAG_VICTIM, "victim", 1 },
        { CVMX_L2C_TAD_EVENT_SC_FAIL, "sc-fail", 1 },
        { CVMX_L2C_TAD_EVENT_SC_PASS, "sc-pass", 1 },
        { CVMX_L2C_TAD_EVENT_LFB_VALID, "lfb-valid", 2 },
        { CVMX_L2C_TAD_EVENT_LFB_WAIT_LFB, "lfb-wait-lfb", 2 },
        { CVMX_L2C_TAD_EVENT_LFB_WAIT_VAB, "lfb-wait-vab", 2 },
        { CVMX_L2C_TAD_EVENT_QUAD0_INDEX, "quad0-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD0_READ, "quad0-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD0_BANK, "quad0-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD0_WDAT, "quad0-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD1_INDEX, "quad1-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD1_READ, "quad1-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD1_BANK, "quad1-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD1_WDAT, "quad1-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD2_INDEX, "quad2-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD2_READ, "quad2-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD2_BANK, "quad2-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD2_WDAT, "quad2-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD3_INDEX, "quad3-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD3_READ, "quad3-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD3_BANK, "quad3-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD3_WDAT, "quad3-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD4_INDEX, "quad4-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD4_READ, "quad4-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD4_BANK, "quad4-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD4_WDAT, "quad4-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD5_INDEX, "quad5-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD5_READ, "quad5-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD5_BANK, "quad5-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD5_WDAT, "quad5-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD6_INDEX, "quad6-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD6_READ, "quad6-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD6_BANK, "quad6-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD6_WDAT, "quad6-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD7_INDEX, "quad7-index-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD7_READ, "quad7-read-bus", 3 },
        { CVMX_L2C_TAD_EVENT_QUAD7_BANK, "quad7-banks-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_QUAD7_WDAT, "quad7-wdat-inuse", 4 },
        { CVMX_L2C_TAD_EVENT_MAX, NULL, 0}
};
void init(void)
{
    perf_core[0] = CVMX_CORE_PERF_BRMIS;
    perf_core[1] = CVMX_CORE_PERF_SYNC;

    if (OCTEON_IS_OCTEON3()) {
        perf_core[2] = CVMX_CORE_PERF_FPARITHEXC;
        perf_core[3] = CVMX_CORE_PERF_FPALL;
    }

    memset(proc_perf_label, 0, sizeof(proc_perf_label));

    if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
    {
        perf_l2[0] = CVMX_L2C_EVENT_CYCLES;
        perf_l2[1] = CVMX_L2C_EVENT_DATA_HIT;
        perf_l2[2] = CVMX_L2C_EVENT_DATA_MISS;
        perf_l2[3] = CVMX_L2C_EVENT_INSTRUCTION_MISS;

        memset(proc_perf_l2label, 0, sizeof(proc_perf_l2label));
    }
    else
    {
        perf_l2[0] = CVMX_L2C_TAD_EVENT_TAG_HIT;
        perf_l2[1] = CVMX_L2C_TAD_EVENT_TAG_MISS;
        perf_l2[2] = CVMX_L2C_TAD_EVENT_LFB_WAIT_LFB;
        perf_l2[3] = CVMX_L2C_TAD_EVENT_LFB_WAIT_VAB;
    }

    proc_perf_label[CVMX_CORE_PERF_NONE] = "none";
    proc_perf_label[CVMX_CORE_PERF_CLK] = "clk";
    proc_perf_label[CVMX_CORE_PERF_ISSUE] = "issue";
    proc_perf_label[CVMX_CORE_PERF_RET] = "ret";
    proc_perf_label[CVMX_CORE_PERF_NISSUE] = "nissue";
    proc_perf_label[CVMX_CORE_PERF_SISSUE] = "sissue";
    proc_perf_label[CVMX_CORE_PERF_DISSUE] = "dissue";
    proc_perf_label[CVMX_CORE_PERF_IFI] = "ifi";
    proc_perf_label[CVMX_CORE_PERF_BR] = "br";
    proc_perf_label[CVMX_CORE_PERF_BRMIS] = "brmis";
    proc_perf_label[CVMX_CORE_PERF_J] = "j";
    proc_perf_label[CVMX_CORE_PERF_JMIS] = "jmis";
    proc_perf_label[CVMX_CORE_PERF_REPLAY] = "replay";
    proc_perf_label[CVMX_CORE_PERF_IUNA] = "iuna";
    proc_perf_label[CVMX_CORE_PERF_TRAP] = "trap";
    proc_perf_label[CVMX_CORE_PERF_UULOAD] = "uuload";
    proc_perf_label[CVMX_CORE_PERF_UUSTORE] = "uustore";
    proc_perf_label[CVMX_CORE_PERF_ULOAD] = "uload";
    proc_perf_label[CVMX_CORE_PERF_USTORE] = "ustore";
    proc_perf_label[CVMX_CORE_PERF_EC] = "ec";
    proc_perf_label[CVMX_CORE_PERF_MC] = "mc";
    proc_perf_label[CVMX_CORE_PERF_CC] = "cc";
    proc_perf_label[CVMX_CORE_PERF_CSRC] = "csrc";
    proc_perf_label[CVMX_CORE_PERF_CFETCH] = "cfetch";
    proc_perf_label[CVMX_CORE_PERF_CPREF] = "cpref";
    proc_perf_label[CVMX_CORE_PERF_ICA] = "ica";
    proc_perf_label[CVMX_CORE_PERF_II] = "ii";
    proc_perf_label[CVMX_CORE_PERF_IP] = "ip";
    proc_perf_label[CVMX_CORE_PERF_CIMISS] = "cimiss";
    proc_perf_label[CVMX_CORE_PERF_WBUF] = "wbuf";
    proc_perf_label[CVMX_CORE_PERF_WDAT] = "wdat";
    proc_perf_label[CVMX_CORE_PERF_WBUFLD] = "wbufld";
    proc_perf_label[CVMX_CORE_PERF_WBUFFL] = "wbuffl";
    proc_perf_label[CVMX_CORE_PERF_WBUFTR] = "wbuftr";
    proc_perf_label[CVMX_CORE_PERF_BADD] = "badd";
    proc_perf_label[CVMX_CORE_PERF_BADDL2] = "baddl2";
    proc_perf_label[CVMX_CORE_PERF_BFILL] = "bfill";
    proc_perf_label[CVMX_CORE_PERF_DDIDS] = "ddids";
    proc_perf_label[CVMX_CORE_PERF_IDIDS] = "idids";
    proc_perf_label[CVMX_CORE_PERF_DIDNA] = "didna";
    proc_perf_label[CVMX_CORE_PERF_LDS] = "lds";
    proc_perf_label[CVMX_CORE_PERF_LMLDS] = "lmlds";
    proc_perf_label[CVMX_CORE_PERF_IOLDS] = "iolds";
    proc_perf_label[CVMX_CORE_PERF_DMLDS] = "dmlds";
    proc_perf_label[CVMX_CORE_PERF_STS] = "sts";
    proc_perf_label[CVMX_CORE_PERF_LMSTS] = "lmsts";
    proc_perf_label[CVMX_CORE_PERF_IOSTS] = "iosts";
    proc_perf_label[CVMX_CORE_PERF_IOBDMA] = "iobdma";
    proc_perf_label[CVMX_CORE_PERF_DTLB] = "dtlb";
    proc_perf_label[CVMX_CORE_PERF_DTLBAD] = "dtlbad";
    proc_perf_label[CVMX_CORE_PERF_ITLB] = "itlb";
    proc_perf_label[CVMX_CORE_PERF_SYNC] = "sync";
    proc_perf_label[CVMX_CORE_PERF_SYNCIOB] = "synciob";
    proc_perf_label[CVMX_CORE_PERF_SYNCW] = "syncw";
    if (!OCTEON_IS_MODEL(OCTEON_CN5XXX) && !OCTEON_IS_MODEL(OCTEON_CN3XXX))
    {
        proc_perf_label[CVMX_CORE_PERF_ERETMIS] = "eretmis";
        proc_perf_label[CVMX_CORE_PERF_LIKMIS] = "likmis";
        proc_perf_label[CVMX_CORE_PERF_HAZTR] = "hazard-trap";
    }
    if (OCTEON_IS_OCTEON3())
    {
        proc_perf_label[CVMX_CORE_PERF_DUTLB] = "dutlb";
        proc_perf_label[CVMX_CORE_PERF_IUTLB] = "iutlb";
        proc_perf_label[CVMX_CORE_PERF_CDMISS] = "cdmiss";
        proc_perf_label[CVMX_CORE_PERF_FPUNIMPTRAP] = "fpunimp-trap";
        proc_perf_label[CVMX_CORE_PERF_FPHAZARDTRAP] = "fphazard-trap";
        proc_perf_label[CVMX_CORE_PERF_FPARITHEXC] = "fparith-exc";
        proc_perf_label[CVMX_CORE_PERF_FPMOVC1] = "fpmovc1";
        proc_perf_label[CVMX_CORE_PERF_FPCOPYC1] = "fpcopyc1";
        proc_perf_label[CVMX_CORE_PERF_FPCOMPARE] = "fpcompare";
        proc_perf_label[CVMX_CORE_PERF_FPBRANCH] = "fpbranch";
        proc_perf_label[CVMX_CORE_PERF_FPMOV] = "fpmov";
        proc_perf_label[CVMX_CORE_PERF_FPABSNEG] = "fpabs";
        proc_perf_label[CVMX_CORE_PERF_FPADDSUB] = "fpadd";
        proc_perf_label[CVMX_CORE_PERF_FPCVT] = "fpcvt";
        proc_perf_label[CVMX_CORE_PERF_FPMUL] = "fpmul";
        proc_perf_label[CVMX_CORE_PERF_FPMADD] = "fpmadd";
        proc_perf_label[CVMX_CORE_PERF_FPDIVRECIP] = "fpdiv";
        proc_perf_label[CVMX_CORE_PERF_FPSQRTRSQRT] = "fpsqrt";
        proc_perf_label[CVMX_CORE_PERF_FPLOAD] = "fpload";
        proc_perf_label[CVMX_CORE_PERF_FPSTORE] = "fpstore";
        proc_perf_label[CVMX_CORE_PERF_FPALL] = "fpall";
    }

    if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
    {
        proc_perf_l2label[CVMX_L2C_EVENT_CYCLES] = "cycles";
        proc_perf_l2label[CVMX_L2C_EVENT_INSTRUCTION_MISS] = "imiss";
        proc_perf_l2label[CVMX_L2C_EVENT_INSTRUCTION_HIT] = "ihit";
        proc_perf_l2label[CVMX_L2C_EVENT_DATA_MISS] = "dmiss";
        proc_perf_l2label[CVMX_L2C_EVENT_DATA_HIT] = "dhit";
        proc_perf_l2label[CVMX_L2C_EVENT_MISS] = "miss";
        proc_perf_l2label[CVMX_L2C_EVENT_HIT] = "hit";
        proc_perf_l2label[CVMX_L2C_EVENT_VICTIM_HIT] = "victim-buffer-hit";
        proc_perf_l2label[CVMX_L2C_EVENT_INDEX_CONFLICT] = "lfb-nq-index-conflict";
        proc_perf_l2label[CVMX_L2C_EVENT_TAG_PROBE] = "tag-probe";
        proc_perf_l2label[CVMX_L2C_EVENT_TAG_UPDATE] = "tag-update";
        proc_perf_l2label[CVMX_L2C_EVENT_TAG_COMPLETE] = "tag-probe-completed";
        proc_perf_l2label[CVMX_L2C_EVENT_TAG_DIRTY] = "tag-dirty-victim";
        proc_perf_l2label[CVMX_L2C_EVENT_DATA_STORE_NOP] = "data-store-nop";
        proc_perf_l2label[CVMX_L2C_EVENT_DATA_STORE_READ] = "data-store-read";
        proc_perf_l2label[CVMX_L2C_EVENT_DATA_STORE_WRITE] = "data-store-write";
        proc_perf_l2label[CVMX_L2C_EVENT_FILL_DATA_VALID] = "memory-fill-data-valid";
        proc_perf_l2label[CVMX_L2C_EVENT_WRITE_REQUEST] = "memory-write-request";
        proc_perf_l2label[CVMX_L2C_EVENT_READ_REQUEST] = "memory-read-request";
        proc_perf_l2label[CVMX_L2C_EVENT_WRITE_DATA_VALID] = "memory-write-data-valid";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_NOP] = "xmc-nop";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_LDT] = "xmc-ldt";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_LDI] = "xmc-ldi";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_LDD] = "xmc-ldd";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_STF] = "xmc-stf";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_STT] = "xmc-stt";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_STP] = "xmc-stp";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_STC] = "xmc-stc";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_DWB] = "xmc-dwb";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_PL2] = "xmc-pl2";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_PSL1] = "xmc-psl1";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_IOBLD] = "xmc-iobld";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_IOBST] = "xmc-iobst";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_IOBDMA] = "xmc-iobdma";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_IOBRSP] = "xmc-iobrsp";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_BUS_VALID] = "xmd-bus-valid";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_MEM_DATA] = "xmd-bus-valid-dst-l2c";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_REFL_DATA] = "xmd-bus-valid-dst-iob";
        proc_perf_l2label[CVMX_L2C_EVENT_XMC_IOBRSP_DATA] = "xmd-bus-valid-dst-pp";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_NOP] = "rsc-nop";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_STDN] = "rsc-stdn";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_FILL] = "rsc-fill";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_REFL] = "rsc-refl";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_STIN] = "rsc-stin";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_SCIN] = "rsc-scin";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_SCFL] = "rsc-scfl";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_SCDN] = "rsc-scdn";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_DATA_VALID] = "rsd-data-valid";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_VALID_FILL] = "rsd-data-valid-fill";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_VALID_STRSP] = "rsd-data-valid-strsp";
        proc_perf_l2label[CVMX_L2C_EVENT_RSC_VALID_REFL] = "rsd-data-valid-refl";
        proc_perf_l2label[CVMX_L2C_EVENT_LRF_REQ] = "lrf-req";
        proc_perf_l2label[CVMX_L2C_EVENT_DT_RD_ALLOC] = "dt-rd-alloc";
        proc_perf_l2label[CVMX_L2C_EVENT_DT_WR_INVAL] = "dt-wr-inva";
    }
}

/**
 * Disable event counting
 *
 */
static
void stop_perf_counter(int core)
{
    uint32_t perf_ctrl_reg;
    int counter;

    for (counter=0; counter<CVMX_CORE_PCNT*2; counter=counter+2)
    {
	if (counter > CVMX_CORE_PCNT)
		break;
        perf_ctrl_reg = octeon_remote_read_register(core, 0x100 + 25*8 + counter);
        perf_ctrl_reg &= ~(0x1f); 
        octeon_remote_write_register(core, 0x100 + 25*8 + counter, perf_ctrl_reg);
    }
}

/**
 * Set or clear the Interrupt mask bits IM[7-0] of Status Register 
 * 
 * @param core       The requested core
 * @param irq_number The requested irq_number 
 * @param op         The field to select set/clear operation
 *
 */
static
void set_clear_bit(int core, int irq_number, int op)
{
    uint32_t status_reg = octeon_remote_read_register(core, 0x100 + 12*8 + 0);
    if (op) 
       status_reg &= ~(1 << (8 + irq_number));
    else
       status_reg |= (1 << (8 + irq_number));
    octeon_remote_write_register(core, 0x100 + 12*8 + 0, status_reg);
}

static
void show_profile_stats(void)
{
    int cpu;
    printf("\nEvent          :  %s\n", perf_ctr_event);
    printf("Sampling Rate  :  %lld\n", (unsigned long long)events);
    printf("Total samples  :  %lld\n", (unsigned long long)totsamples);

    printf("\nCore#:  ");
    for (cpu=0; cpu < number_of_cores; cpu++)
    {
        if (coremask & (1ull << cpu))
            printf(" %5u", cpu);
    }
    printf("\nSamples:"); 
    for (cpu=0; cpu < number_of_cores; cpu++)
    {       
        if (coremask & (1ull << cpu))   
            printf(" %5u",sample_cnt[cpu]);
    }
    printf("\n");
}

/**
 * Mask the Perf counter Interrupt line when profiler is stopped 
 */
static
void end_profiler(void)
{
    uint32_t core;
    octeon_remote_stop_cores(coremask);
    usleep(100000);
    for (core=0; core < cvmx_octeon_num_cores(); core++)
    {
        set_clear_bit(core, CVMX_IRQ_MIPS6, 1); /* Mask the respective interrupt bit on all cores */
        stop_perf_counter(core);
    }
    octeon_remote_start_cores(coremask);

    if (event_profiler)
    {
        /* collect_sample would be '1' if we have collected atleast one sample */
        if (!collect_sample)
        {
            printf("\nINFO: No samples collected for '%s' event with %lld sample rate\n", perf_ctr_event, (unsigned long long)events);
            printf("INFO: Try with smaller sample-rate for same event\n"); 
        }
        else
            show_profile_stats();
    }
}

/**
 * Parse the string passed by User for Selecting Core counter 
 * 
 * @param option  The Performance counter Register selected - 1 
 * @param event_name   The Requested Event to be counted 
 *
 * @return Zero on success, negative on failure. 
 */ 
static
int parse_perf_ctr_events(char option, char *event_name)
{
    int index;
    for (index=0; index<CVMX_CORE_PERF_MAX; index++)
    {
	/* Older models have only 2 performance counters */
	if (((option - '0') > 2) && !OCTEON_IS_OCTEON3())
		return -1;

        if ((OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX)) && (index > 0x3a))
        {
            printf("Invalid Core Performance Counter(%c) Event(%s)\n", option, event_name);
            return -1;
        }
        if (proc_perf_label[index] && (strcmp(event_name, proc_perf_label[index]) == 0))
        {
	    /* New counters added between 0x3b - 0x3f, 0x43 - 0x65 for OIII. */
            if (OCTEON_IS_OCTEON2() && ((index > 0x3a && index < 0x40) || index > 0x42))
            {
                printf("Invalid Core Performance Counter(%c) Event(%s) index = 0x%x\n", option, event_name, index);
                return -1;
            }
            perf_core[option - '1'] = (cvmx_core_perf_t)index;
            break;
        }
    }

    if (index == CVMX_CORE_PERF_MAX)
    {
        printf("Invalid perf counter: %s\n", event_name);
        return -1;
    }
    return 0;
}

/**
 * Parse the command line options
 * 
 * @param argc   Arguments
 * @param argv   Argument strings
 *
 * @return Zero on success, negative on failure. Failure will also display usage.
 */
int perf_only = 0;
int count = 1<<30;
int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"perf1", 1, 0, '1'},
        {"perf2", 1, 0, '2'},
        {"perf3", 1, 0, '3'},
        {"perf4", 1, 0, '4'},
        {"l2perf1", 1, 0, 'a'},
        {"l2perf2", 1, 0, 'b'},
        {"l2perf3", 1, 0, 'c'},
        {"l2perf4", 1, 0, 'd'},
        {"list-events", 0, 0, 'l'}, 
        {"fast", 0, 0, 'e'},
        {"samples", 1, 0, 's'},
        {"dwell", 1, 0, 'f'},
        {"perfonly", 0, 0, 'g'},
        {"events", 1, 0, 't'},
        {"count", 1, 0, 'C'},
        {"core_mask", 1, 0, 'x'},
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
	{"sample-var", 1, 0, 'V'},
        {NULL, 0, 0, 0}
    };

    int option_index = 0;
    int show_usage = 0;
    int index;
    int errflag = 0;

    while (1)
    {
        char option = getopt_long(argc, argv, "1:2:3:4:a:b:c:d:lef:gC:t:h:s:x:V:v", long_options, &option_index);
        if (option <= 0)
            break;
        switch (option)
        {
            case '1':
            case '2':
            case '3':
            case '4':
                if (parse_perf_ctr_events(option, optarg))
                    show_usage = 1;
                break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
                if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
                {
                    for (index=0; index<CVMX_L2C_EVENT_MAX; index++)
                    {
                        if (proc_perf_l2label[index] && (strcmp(optarg, proc_perf_l2label[index]) == 0))
                        {
                            perf_l2[option - 'a'] = (cvmx_l2c_event_t)index;
                            break;
                        }
                    }
                    if (index == CVMX_L2C_EVENT_MAX)
                        show_usage = 1;
                }
                else
                {
                    for (index=0; proc_perf_l2tad_label[index].name; index++)
                    {
                        if (strcmp(optarg, proc_perf_l2tad_label[index].name) == 0)
                        {
                            perf_l2[option - 'a'] = (cvmx_l2c_tad_event_t)index;
                            break;
                        }
                    }
                    if (proc_perf_l2tad_label[index].type == CVMX_L2C_TAD_EVENT_MAX)
                    {
                        printf("Invalid l2 event: %s\n", optarg);
                        show_usage = 1;
                    }
                }
                break;
            case 'l':
                printf("%s: Available events:\n    ", argv[0]);
                for (index=1; index<CVMX_CORE_PERF_MAX; index++)
                {
                    if ((index & 7) == 0)
                        printf("\n    ");
                    if (proc_perf_label[index])
                        printf("%s ", proc_perf_label[index]);
                }
                printf("\n");
                return -1;
            case 'e':
                fast = 1;
                errflag++;
                break;
            case 'f':
                dwell = atoi(optarg);
                break;
            case 'g':
                perf_only = 1;
                break;
            case 's':
                samplerate = atoi(optarg);
                errflag++;
                break;
            case 'C':
                count = atoi(optarg);
                break;
            case 't':
                event_profiler = 1;
                sscanf(optarg, "%[^:]:%lld", perf_ctr_event, (unsigned long long *)&events);
                if(parse_perf_ctr_events('1', perf_ctr_event))
                    show_usage = 1;
                if (events < 500)  {
                    printf("events count is less than allowed threshold value\n");
                    show_usage = 1;
                }
                break;
            case 'x':
                core_mask_cmd_line = (int)strtoul(optarg,0,0);
                break;
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
                return -1;
                break;
	    case 'V':
	    	if (sscanf(optarg, "%llx", (unsigned long long *)&sample_var) != 1) {
		    printf("sample-var arg error\n");
                    show_usage = 1;
		}
	    	break;
            case 'h':
            default:
                show_usage = 1;
                break;
        }
    }

    if(errflag == 2) {
        printf("%s: --fast, --samples options are mutually exclusive\n", argv[0]);
        show_usage = 1;
    }

    if(event_profiler == 1) 
        first_time = 0;

    if (show_usage)
    {
        printf("\n"
               "Cavium Octeon Profiler\n"
               "\n"
               "Usage:\n"
               "    %s [options]\n"
               "\n"
               "Supported Options:\n"
               "    --perf1=<Core counter>\n"
               "    --perf2=<Core counter>\n"
               "    --perf3=<Core counter> # Available from CN7XXX onwards\n"
               "    --perf4=<Core counter> # Available from CN7XXX onwards\n"
               "    --l2perf1=<L2 counter>\n"
               "    --l2perf2=<L2 counter>\n"
               "    --l2perf3=<L2 counter>\n"
               "    --l2perf4=<L2 counter>\n"
               "    --list-events               # List available events\n"
               "    --events=<event name>:<count>  # Enable profiling based on event and threshold count(min 500)\n"
               "    --fast			# Sample as quickly as possible\n"
               "    --samples=<samples per second> # No. of samples taken per second, default is 1000/sec\n"
               "				# High sample rates may not yield expected results\n"
               "    --dwell=<seconds>           # Number of seconds between printing\n"
               "    --perfonly                  # Only display performance counters\n"
               "    --count=<number>            # Number of dwell times to sample then exit\n"
               "    --core_mask=<mask>          # Mask which specifies the cores to sample\n"
	       "    --sample-var=<var addr>     # Sample var@<var addr> (address in hex)\n"
               "    --help\n", argv[0]);
        printf("\nNote: --fast, --samples options are mutually exclusive\n");
        printf("\nPossible Core counters:\n");
        for (index=0; index<CVMX_CORE_PERF_MAX; index++)
        {
            if ((index & 7) == 0)
                printf("\n    ");
            if (proc_perf_label[index])
                printf("%s ", proc_perf_label[index]);
        }
        printf("\n\nPossible L2 counters:");
        if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
        {
            for (index=0; index<CVMX_L2C_EVENT_MAX; index++)
            {
                if (!proc_perf_l2label[index])
                    continue;
                if ((index & 3) == 0)
                    printf("\n    ");
                if (proc_perf_l2label[index])
                    printf("%s ", proc_perf_l2label[index]);
            }
        }
        else
        {
            for (index=0; proc_perf_l2tad_label[index].name; index++)
            {
		if (proc_perf_l2tad_label[index].type >= 0xc0 && !OCTEON_IS_OCTEON3())
			break;

                if ((index & 3) == 0)
                    printf("\n    ");
                printf("%s ", proc_perf_l2tad_label[index].name);
            }
        }
        printf("\n\n");
        printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
        return -1;
    }
    else
        return 0;
}

/**
 * This signal handler is called when Control-C is pressed. It
 * cleanly exits the processing loop below.
 *
 * @param num    Signal received
 */
static void sig_int_handler(int num)
{
    end_profiler();
    finished = 1;
    if (original_sig_int)
        original_sig_int(num);
}

/**
 * Get the per cpu base addresses from the 'event_block_config'
 * block and store in pcpu_base_addr array.
 *
 */
static void read_config_blk(void)
{
    int cpu;
    for (cpu=0; cpu < number_of_cores; cpu++)
        pcpu_base_addr[cpu] = octeon_remote_read_mem64(config_block_phy_addr \
                        + offsetof(cvmx_config_block_t, pcpu_base_addr) + (cpu*8));
}

/**
 * Main
 * 
 * @param argc
 * @param argv
 * 
 * @return
 */
int main(int argc, char * const *argv)
{
    cvmx_core_perf_control_t perf_core_control0;
    cvmx_core_perf_control_t perf_core_control1;
    cvmx_core_perf_control_t perf_core_control2;
    cvmx_core_perf_control_t perf_core_control3;
    int core;
    int qos;
    uint64_t prev_sample_count = 0;
    uint64_t l2perfval = 0;
    uint64_t qos_thr = 0, iq_cnt = 0, iq_com_cnt = 0, qos_we = 0;
    uint64_t threshold_value = 0;
    const char *remote_spec = getenv("OCTEON_REMOTE_PROTOCOL");

    /* Control-C signal handler tells us when to die */
    original_sig_int = signal(SIGINT, sig_int_handler);

    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if (!octeon_remote_mem_access_ok())
    {
        octeon_remote_close();
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }

    init();
    if (parse_options(argc, argv))
        return -1;

    memset(perfcore, 0, sizeof(perfcore));

    octeon_remote_lock();
    number_of_cores = cvmx_octeon_num_cores();
    coremask = (1ull<<number_of_cores)-1;

    if(core_mask_cmd_line)
      coremask = coremask & core_mask_cmd_line;

    perf_core_control0.u32 = 0;
    perf_core_control0.s.ie = 1;
    perf_core_control0.s.u = 1;
    perf_core_control0.s.s = 1;
    perf_core_control0.s.k = 1;
    perf_core_control0.s.ex = 1;
    perf_core_control0.s.event = perf_core[0];

    perf_core_control1.u32 = perf_core_control0.u32;
    perf_core_control1.s.event = perf_core[1];
    perf_core_control2.u32 = perf_core_control0.u32;
    perf_core_control2.s.event = perf_core[2];
    perf_core_control3.u32 = perf_core_control0.u32;
    perf_core_control3.s.event = perf_core[3];

    if (sample_var) {
        if (event_profiler == 1) {
	    printf("ERROR: sampling variable is for stats only and not events\n");
	    return -1;
	}
        if (octeon_remote_debug_handler_install(OCTEON_REMOTE_PROFILE_HANDLER,
	    sample_var) != 0) {
	    printf("ERROR: Failed to install the profiler handler\n");
	    return -1;
	}
    }

    if (event_profiler == 1)
    {
        void *buffer;
        if ((buffer = (void *) calloc(1, EVENT_BUFFER_SIZE)) == NULL) {
            perror("calloc failed");
            exit(1);
        } 

        uint64_t block_phy_addr = 0;
        uint64_t pbsize, config_pbsize;
        threshold_value = (1ull << 63) - events;

        //printf("the size of block is %d\n", EVENT_BUFFER_SIZE);

        /* octeon_remote_named_block_find    1 on success, 0 on failure
         * octeon_remote_named_block_alloc   return Address on success, 0xffffffffffffffff on failure 
         */
        if (!octeon_remote_named_block_find(EVENT_BUFFER_BLOCK, &block_phy_addr, &pbsize))
        {
            block_phy_addr = octeon_remote_named_block_alloc(EVENT_BUFFER_SIZE, 0, 0, 128, EVENT_BUFFER_BLOCK);
            assert (block_phy_addr != ~0ull);
        }
        octeon_remote_write_mem(block_phy_addr, buffer, EVENT_BUFFER_SIZE);
	
        if (!octeon_remote_named_block_find(EVENT_BUFFER_CONFIG_BLOCK, &config_block_phy_addr, &config_pbsize))
        {
            config_block_phy_addr = octeon_remote_named_block_alloc(EBC_BLOCK_SIZE, 0, 0, 128, EVENT_BUFFER_CONFIG_BLOCK);
            assert (config_block_phy_addr != ~0ull);
        }

	octeon_remote_write_mem64(config_block_phy_addr + offsetof(cvmx_config_block_t, events), events);
	octeon_remote_write_mem64(config_block_phy_addr + offsetof(cvmx_config_block_t, sample_count), 0);
    }

    octeon_remote_stop_cores(coremask);
    usleep(100000);
    for (core=0; core<number_of_cores; core++)
    {

        if ((strcasecmp(remote_spec, "linux") == 0 
             || strcasecmp(remote_spec, "pcihost") == 0)
            && (OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_MODEL(OCTEON_CNF7XXX)))
        {
            /* L2C_COP_MAPX(offset), here offset is calculated as
               0..2 = cop0 offset
               0..4 = cop0 sel
               0..5 = core
               
               13     8 7   3 2    0
               [ core ] [sel][offset] */
            int offset = (core << 8) & 0x3f00;
            if (event_profiler == 1)
            {
                int val = offset | (((25<<3) & 0xf8) | 1);
                /* Write threshold value to PerfCount0 Register of all available cores */
                octeon_remote_write_csr(CVMX_L2C_COP0_MAPX(val), threshold_value);
            }
            offset |= ((25<<3) & 0xf8);
            octeon_remote_write_csr(CVMX_L2C_COP0_MAPX(offset), perf_core_control0.u32);
            offset |= 2;
            octeon_remote_write_csr(CVMX_L2C_COP0_MAPX(offset), perf_core_control1.u32);
        }
        else if ((strcasecmp(remote_spec, "linux") == 0 
             || strcasecmp(remote_spec, "pcihost") == 0)
            && (OCTEON_IS_OCTEON3()))
        {
            cvmx_l2c_cop0_adr_t cop0_adr;

            cop0_adr.u64 = 0;
            cop0_adr.s.ppid = core;
            cop0_adr.s.root = 1;
            cop0_adr.s.rd = 25;
            cop0_adr.s.sel = 0;
            octeon_remote_write_csr(CVMX_L2C_COP0_ADR, cop0_adr.u64);
            octeon_remote_read_csr(CVMX_L2C_COP0_DAT);
            octeon_remote_write_csr(CVMX_L2C_COP0_DAT, perf_core_control0.u32);
            cop0_adr.s.sel = 2;
            octeon_remote_write_csr(CVMX_L2C_COP0_ADR, cop0_adr.u64);
            octeon_remote_read_csr(CVMX_L2C_COP0_DAT);
            octeon_remote_write_csr(CVMX_L2C_COP0_DAT, perf_core_control1.u32);
            cop0_adr.s.sel = 4;
            octeon_remote_write_csr(CVMX_L2C_COP0_ADR, cop0_adr.u64);
            octeon_remote_read_csr(CVMX_L2C_COP0_DAT);
            octeon_remote_write_csr(CVMX_L2C_COP0_DAT, perf_core_control2.u32);
            cop0_adr.s.sel = 6;
            octeon_remote_write_csr(CVMX_L2C_COP0_ADR, cop0_adr.u64);
            octeon_remote_read_csr(CVMX_L2C_COP0_DAT);
            octeon_remote_write_csr(CVMX_L2C_COP0_DAT, perf_core_control3.u32);
        }
        else
        {    
            if (event_profiler == 1)
                /* Write threshold value to PerfCount0 Register of all available cores */
                octeon_remote_write_register(core, 0x100 + 25*8 + 1, threshold_value);

            octeon_remote_write_register(core, 0x100 + 25*8 + 0, perf_core_control0.u32);
            octeon_remote_write_register(core, 0x100 + 25*8 + 2, perf_core_control1.u32);
            if (OCTEON_IS_OCTEON3()) {
                octeon_remote_write_register(core, 0x100 + 25*8 + 4, perf_core_control2.u32);
                octeon_remote_write_register(core, 0x100 + 25*8 + 6, perf_core_control3.u32);
            }
        }
        set_clear_bit(core, CVMX_IRQ_MIPS6, 0);
    }
    octeon_remote_start_cores(coremask);

    if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
    {
        cvmx_l2c_pfctl_t l2perf;
        l2perf.u64 = 0;
        l2perf.s.cnt3ena = 1;
        l2perf.s.cnt3clr = 1;
        l2perf.s.cnt3sel = perf_l2[3];
        l2perf.s.cnt2ena = 1;
        l2perf.s.cnt2clr = 1;
        l2perf.s.cnt2sel = perf_l2[2];
        l2perf.s.cnt1ena = 1;
        l2perf.s.cnt1clr = 1;
        l2perf.s.cnt1sel = perf_l2[1];
        l2perf.s.cnt0ena = 1;
        l2perf.s.cnt0clr = 1;
        l2perf.s.cnt0sel = perf_l2[0];
        l2perfval = l2perf.u64;
        octeon_remote_write_csr(CVMX_L2C_PFCTL, l2perfval);
    }
    else
    {
        cvmx_l2c_tadx_prf_t l2c_tadx_prf;
        int tad;

        l2c_tadx_prf.u64 = 0;
        l2c_tadx_prf.s.cnt3sel = perf_l2[3];
        l2c_tadx_prf.s.cnt2sel = perf_l2[2];
        l2c_tadx_prf.s.cnt1sel = perf_l2[1];
        l2c_tadx_prf.s.cnt0sel = perf_l2[0];

        for (tad=0; tad<CVMX_L2C_TADS; tad++)
            octeon_remote_write_csr(CVMX_L2C_TADX_PRF(tad), l2c_tadx_prf.u64);
    }

    time_t start_time = time(NULL);

    ringbuf_t temp_cpu_buffer;
    octeon_remote_sample_t sample[64];
    uint64_t prev_ops_cnt[4] = {0, 0, 0, 0};
    uint64_t prev_dclk_cnt[4] = {0, 0, 0, 0};
    int cpu;
    uint64_t head, tail, end, pcpu_data_start;
    uint64_t pcpu_new_tail[number_of_cores];
    cvmx_sample_entry_t nsample;
    int valid_tail[number_of_cores];
    uint64_t total;
    int cnt;

    memset(valid_tail, 0, sizeof(valid_tail));

    while (!finished)
    {
        if (event_profiler == 1)
        {
            totsamples = octeon_remote_read_mem64(config_block_phy_addr + offsetof(cvmx_config_block_t, sample_count));
            if (!totsamples)
                continue;

            if (totsamples == prev_sample_count) /* No new samples yet */
                continue;
            else
                prev_sample_count = totsamples;

            read_config_blk();
            for (cpu=0; cpu < number_of_cores; cpu++)
            {
                if ((coremask & (1ull << cpu)) == 0)
                    continue;  

		octeon_remote_read_mem(&temp_cpu_buffer, pcpu_base_addr[cpu], sizeof(ringbuf_t) + PADBYTES);

                head = cvmx_cpu_to_be64(temp_cpu_buffer.pcpu_blk_info.head);
                end = cvmx_cpu_to_be64(temp_cpu_buffer.pcpu_blk_info.end);
                pcpu_data_start = cvmx_cpu_to_be64(temp_cpu_buffer.pcpu_data);
                sample_cnt[cpu] = cvmx_cpu_to_be64(temp_cpu_buffer.pcpu_blk_info.sample_count); 

                if (!valid_tail[cpu]) 
                    pcpu_new_tail[cpu] = tail = cvmx_cpu_to_be64(temp_cpu_buffer.pcpu_blk_info.tail);
                else
                    tail = pcpu_new_tail[cpu];
		
                while (tail != head)
                {
                    if (tail == end)
                        tail = pcpu_data_start;

                    octeon_remote_read_mem(&nsample, tail, sizeof(cvmx_sample_entry_t));

                    core = cvmx_cpu_to_be32(nsample.core);
                    sample[core].pc = cvmx_cpu_to_be32(nsample.pc);
                    profile_data[sample[core].pc].count[core]++;

                    tail += sizeof(cvmx_sample_entry_t);
                    pcpu_new_tail[cpu] = tail;
                   
                    collect_sample = 1;
                }
                if (tail)
                    valid_tail[cpu] = 1;
            }
        }
        else
        { 
            /* Rate limit our samples */
	    if (!fast) 
                usleep(1000000/samplerate);

            if (octeon_remote_get_sample(coremask, sample))
            {
                printf("Failed to get sample. Aborting\n");
                break;
            }

            for (core=0; core<number_of_cores; core++)
            {
                if (coremask & (1ull << core))
                    profile_data[sample[core].pc].count[core]++;
            }
        }
//printf("time = 0x%llx, start_time = 0x%llx\n", time(NULL), start_time);
        if ((time(NULL) - start_time >= dwell) || first_time)
        {
            ProfileData::const_iterator iter;
            if (1)
            {
                if (!perf_only && !first_time)
                {
                    printf("%19s","core#:");
                    for (core=0; core<number_of_cores; core++)
                    {
                        if (coremask & (1ull << core))
                            printf(" %5u", core);
                    }
                    printf("\n");
                    for (iter=profile_data.begin(); iter!=profile_data.end(); iter++)
                    {
                        printf("0x%016llx:", (unsigned long long)iter->first);
                        for (core=0; core<number_of_cores; core++)
                        {
                            if (coremask & (1ull << core))
                                printf(" %5u", iter->second.count[core]);
                        }
                        printf("\n");
                    }
                }

                if (event_profiler)
                {
                    profile_data.clear();
                    start_time = time(NULL);
                    if (!count--)
                        break;
                    printf("\n");
                    continue;     
                }           
                if ((strcasecmp(remote_spec, "linux") == 0 
                     || strcasecmp(remote_spec, "pcihost") == 0)
                    && (OCTEON_IS_MODEL(OCTEON_CN3XXX)
                        || OCTEON_IS_MODEL(OCTEON_CN5XXX)))
                {
                    printf("Performance counter events don't work in this Octeon model\n");
                    goto skip_printing_perf_counters;
                }

                for (cnt = 0; cnt < CVMX_CORE_PCNT; cnt++)
                {
                    total = 0;
                    for (core=0; core<number_of_cores; core++)
                    {
                        uint64_t counter;

                        if ((coremask & (1ull << core)) == 0)
                            continue;

                        counter = sample[core].perf_count[cnt];
                        if (core)
                            total += counter - perfcore[cnt][core];
                        if (!first_time)
                        {
                            if ((core & 7) == 0)
                                printf("\n%s ", proc_perf_label[perf_core[cnt]]);
                            printf("%9llu[%2d],", (unsigned long long)counter - perfcore[cnt][core], core);
                        }
                        perfcore[cnt][core] = counter;
                    }

                    if (!first_time)
                    {
                        printf("\ntotal (excluding 0) %s ", proc_perf_label[perf_core[cnt]]);
                        printf("%9llu,", (unsigned long long)total);
                    }
                }
                
skip_printing_perf_counters:
                if (!first_time)
                {
                    if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
                    {
                        printf("\n");
                        printf("L2 %s %9llu, %s %9llu, %s %9llu, %s %9llu\n",
                           proc_perf_l2label[perf_l2[0]], (unsigned long long)octeon_remote_read_csr(CVMX_L2C_PFC0),
                           proc_perf_l2label[perf_l2[1]], (unsigned long long)octeon_remote_read_csr(CVMX_L2C_PFC1),
                           proc_perf_l2label[perf_l2[2]], (unsigned long long)octeon_remote_read_csr(CVMX_L2C_PFC2),
                           proc_perf_l2label[perf_l2[3]], (unsigned long long)octeon_remote_read_csr(CVMX_L2C_PFC3));
                    }
                    else
                    {
                        int tad;
                        int counter;
                        uint64_t clock_rate = cvmx_clock_get_rate(CVMX_CLOCK_CORE);
                        uint64_t current_cycle = cvmx_clock_get_count(CVMX_CLOCK_CORE);

                        /* Get the total counter value by reading each TAD. */
                        for (tad=0; tad<CVMX_L2C_TADS; tad++)
                        {
                            uint64_t delta_cycle = current_cycle - last_update_bus[tad];
                            uint64_t delta_cycle_percent = delta_cycle / 100;
                            uint64_t ops_cnt[4], dclk_cnt[4];

                            if (delta_cycle_percent == 0)
                                delta_cycle_percent = 1;
                            last_update_bus[tad] = current_cycle;

                            printf("\nL2 statistics for TAD %d\n", tad);
                            /* Counts the number of cycles the XMC(Addr bus) was busy. So the
                               percentage used is simply 100 * count / RCLK */
                            printf("bus_xmc(addr) count  : %9llu\n", (unsigned long long)octeon_remote_read_csr(CVMX_L2C_XMCX_PFC(tad)) / delta_cycle_percent);
                            octeon_remote_write_csr(CVMX_L2C_XMCX_PFC(tad), 0);
                            /* Counts the number of cycles the XMD(Store bus) was busy. So the
                               percentage used is simply 100 * count / RCLK */
                            printf("bus_xmd(store) count : %9llu\n", (unsigned long long)octeon_remote_read_csr(CVMX_L2C_XMDX_PFC(tad)) / delta_cycle_percent);
                            octeon_remote_write_csr(CVMX_L2C_XMDX_PFC(tad), 0);
                            /* Counts the number of cycles the RSC(Commit bus) was busy. So the
                               percentage used is simply 100 * count / RCLK */
                            printf("bus_rsc(commit) count: %9llu\n", (unsigned long long)octeon_remote_read_csr(CVMX_L2C_RSCX_PFC(tad)) / delta_cycle_percent);
                            octeon_remote_write_csr(CVMX_L2C_RSCX_PFC(tad), 0);
                            /* Counts the number of cycles the RSD(Fill bus) was busy. So the
                                percentage used is simply 100 * count / RCLK */
                            printf("bus_rsd(fill) count  : %9llu\n", (unsigned long long)octeon_remote_read_csr(CVMX_L2C_RSDX_PFC(tad)) / delta_cycle_percent);
                            octeon_remote_write_csr(CVMX_L2C_RSDX_PFC(tad), 0);
                            /* There is only one IO bus */
                            if (tad == 0)
                            {
                                /* Counts the number of cycles the IOC(IO request bus) was busy.
                                   So the percentage used is simply 100 * count / RCLK */ 
                                   printf("bus_ioc(IO req) count: %9llu\n", (unsigned long long)octeon_remote_read_csr(CVMX_L2C_IOCX_PFC(tad)) / delta_cycle_percent);
                                   octeon_remote_write_csr(CVMX_L2C_IOCX_PFC(tad), 0);
                                   /* Counts the number of cycles the IOR(IO response bus) was busy.
                                      So the percentage used is simply 100 * count / RCLK */
                                   printf("bus_ior(IO req) count: %9llu\n", (unsigned long long)octeon_remote_read_csr(CVMX_L2C_IORX_PFC(tad)) / delta_cycle_percent);
                                   octeon_remote_write_csr(CVMX_L2C_IORX_PFC(tad), 0);
                            }

                            /* Read the current performance counter settings */
                            for (counter = 0; counter < CVMX_L2C_MAX_PCNT; counter++)
                            {
                                uint64_t value = 0;
                                /* Calculate time since last update */
                                delta_cycle = current_cycle - last_update[tad][counter];
                                if (delta_cycle == 0)
                                    delta_cycle = 1;
                                delta_cycle_percent = delta_cycle / 100;
                                if (delta_cycle_percent == 0)
                                    delta_cycle_percent = 1;
                                last_update[tad][counter] = current_cycle;

                                if (OCTEON_IS_OCTEON3())
                                    value = octeon_remote_read_csr(CVMX_L2C_TADX_PFCX(counter, tad));
                                else
                                {
                                    /* Read the appropriate counter */
                                    switch (counter)
                                    {
                                        case 0:
                                            value = octeon_remote_read_csr(CVMX_L2C_TADX_PFC0(tad));
                                            break;
                                        case 1:
                                            value = octeon_remote_read_csr(CVMX_L2C_TADX_PFC1(tad));
                                            break;
                                        case 2:
                                            value = octeon_remote_read_csr(CVMX_L2C_TADX_PFC2(tad));
                                            break;
                                        case 3:
                                            value = octeon_remote_read_csr(CVMX_L2C_TADX_PFC3(tad));
                                            break;
                                    }
                                }

                                printf("%s ", proc_perf_l2tad_label[perf_l2[counter]].name);
                                /* Figure out what to do based on the counter type */
                                /*  Value is a simple count, not a utilization. Scale to 
                                    be cout/sec */
                                if (proc_perf_l2tad_label[perf_l2[counter]].info == 1)
                                    printf("count: %9llu\n", (unsigned long long)(value * clock_rate / delta_cycle));
                                /* There are 16 LFBs, so utilization percentage 
                                   is 100 * count / (RCLK*16) */
                                else if (proc_perf_l2tad_label[perf_l2[counter]].info == 2)
                                    printf("bus utilization: %4llu%%\n", (unsigned long long)(value / (delta_cycle_percent * 16)));
                                /* Count is number of cycle bus in use, so utilization
                                   percentage is 100 * count / RCLK */
                                else if (proc_perf_l2tad_label[perf_l2[counter]].info == 3)
                                    printf("bus utilization: %4llu%%\n", (unsigned long long)(value / delta_cycle_percent));
                                /* Count is number of banks bus in use per cycle (there are
                                   4 banks), so utilization percentage is 100 * count/(RCLK*4) */
                                else if (proc_perf_l2tad_label[perf_l2[counter]].info == 4)
                                    printf("bus utilization: %4llu%%\n", (unsigned long long)(value / (delta_cycle_percent * 4)));
                                else
                                    printf("None\n");

                            }

                            /* Check if the DRAM controller is initialized */
                            if (OCTEON_IS_MODEL(OCTEON_CN68XX))
                            {
                                cvmx_lmcx_dll_ctl2_t ctl2;
                                ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(tad));
                                if (ctl2.cn68xx.intf_en == 0)
                                    continue;
                            }
                            if (OCTEON_IS_MODEL(OCTEON_CN78XX))
                            {
				
                                cvmx_lmcx_dll_ctl2_t ctl2;
				if (tad < 4)
                                {
                                    ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(tad));
                                    if (ctl2.cn78xx.intf_en == 0)
                                        continue;

                                    ops_cnt[tad] = octeon_remote_read_csr(CVMX_LMCX_OPS_CNT(tad));
                                    dclk_cnt[tad] = octeon_remote_read_csr(CVMX_LMCX_DCLK_CNT(tad));

                                    printf("DRAM ops count: %lld, dclk count: %lld, utilization: %2.2f%%\n", 
                                       (unsigned long long)ops_cnt[tad] - prev_ops_cnt[tad],
                                       (unsigned long long)dclk_cnt[tad] - prev_dclk_cnt[tad],
                                       (float)(ops_cnt[tad] - prev_ops_cnt[tad])/(dclk_cnt[tad] - prev_dclk_cnt[tad])*100);
                                    prev_ops_cnt[tad] = ops_cnt[tad];
                                    prev_dclk_cnt[tad] = dclk_cnt[tad];
                                }
                            }
                            else
                            {
                                ops_cnt[tad] = octeon_remote_read_csr(CVMX_LMCX_OPS_CNT(tad));
                                dclk_cnt[tad] = octeon_remote_read_csr(CVMX_LMCX_DCLK_CNT(tad));

                                printf("DRAM ops count: %lld, dclk count: %lld, utilization: %2.2f%%\n", 
                                       (unsigned long long)ops_cnt[tad] - prev_ops_cnt[tad],
                                       (unsigned long long)dclk_cnt[tad] - prev_dclk_cnt[tad],
                                       (float)(ops_cnt[tad] - prev_ops_cnt[tad])/(dclk_cnt[tad] - prev_dclk_cnt[tad])*100);
                                prev_ops_cnt[tad] = ops_cnt[tad];
                                prev_dclk_cnt[tad] = dclk_cnt[tad];
                            }
                            
                            if (OCTEON_IS_OCTEON2())
                            {
                                octeon_remote_write_csr(CVMX_L2C_TADX_PFC0(tad), 0);
                                octeon_remote_write_csr(CVMX_L2C_TADX_PFC1(tad), 0);
                                octeon_remote_write_csr(CVMX_L2C_TADX_PFC2(tad), 0);
                                octeon_remote_write_csr(CVMX_L2C_TADX_PFC3(tad), 0);
                            }
                            else
                            {
                                for (counter = 0; counter < CVMX_L2C_MAX_PCNT; counter++)
                                    octeon_remote_write_csr(CVMX_L2C_TADX_PFCX(counter, tad), 0);
                            }
                        }

                        if (!octeon_has_feature(OCTEON_FEATURE_FPA3))
                        {
			    printf("\nFPA Pool stats\n");
                            printf("FPA Pool %d (Packet Buffers) : %2llu free\n",
                              CVMX_FPA_PACKET_POOL, (ULL)cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(CVMX_FPA_PACKET_POOL)));
                            printf("FPA Pool %d (WQE buffers)    : %2llu free\n",
                             CVMX_FPA_WQE_POOL, (ULL)cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(CVMX_FPA_WQE_POOL)));
                            printf("FPA Pool %d (PKO cmd buffers): %2llu free, PKO output queues %d\n",
                             CVMX_FPA_OUTPUT_BUFFER_POOL, (ULL)cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(CVMX_FPA_OUTPUT_BUFFER_POOL)),
                             256 >> cvmx_read_csr(CVMX_PKO_REG_QUEUE_MODE));
                        
                            printf("\nSSO Qos Input Queue Statistics:\n");
                            for (qos=0; qos<=7; qos++)
                            {
                                if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
                                    qos_thr = (ULL)cvmx_read_csr(CVMX_SSO_QOS_THRX(qos));
                                    iq_cnt = (ULL)cvmx_read_csr(CVMX_SSO_IQ_CNTX(qos));
                                } else {
                                    qos_thr = (ULL)cvmx_read_csr(CVMX_POW_QOS_THRX(qos)); 
                                    iq_cnt = (ULL)cvmx_read_csr(CVMX_POW_IQ_CNTX(qos));
                                }
                                
                                printf("Queue %d: WQE count %2lld, ", qos, ((ULL)iq_cnt & 0xffffffff));

                                if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
                                    printf("Buffers allocated %2lld\n", ((ULL)qos_thr >> 24) & 0x7ff);
                                    //printf(" Max Threshold %2lld\n", ((ULL)qos_thr >> 12) & 0x3ff); 
                                } else {
                                    printf("Buffers allocated %2lld\n", ((ULL)qos_thr >> 36) & 0xfff); 
                                    //printf("Max Threshold %2lld\n", ((ULL)qos_thr >> 14) & 0xfff); 
                                } 
			    }
                            if (OCTEON_IS_MODEL(OCTEON_CN68XX))
                            {
                                iq_com_cnt = (ULL)cvmx_read_csr(CVMX_SSO_IQ_COM_CNT);
                                qos_we = (ULL)cvmx_read_csr(CVMX_SSO_QOS_WE);
                            }
                            else
                                iq_com_cnt = cvmx_read_csr(CVMX_POW_IQ_COM_CNT);
                            printf("Total WQEs in all QOS levels         : %2lld (in-unit & in-memory)\n", (ULL)iq_com_cnt & 0xffffffff);
                            printf("Total Free Buffers in all QOS levels : %2lld\n", OCTEON_IS_MODEL(OCTEON_CN68XX) ? ((ULL)qos_we & 0xfff) : (((ULL)qos_thr >> 24) & 0x7ff));
                        }
                    }
                }
                if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX))
                    octeon_remote_write_csr(CVMX_L2C_PFCTL, l2perfval);

                {
                    /* Compute DRAM utilization */
                    uint64_t ops_cnt, dclk_cnt;

                    if (OCTEON_IS_MODEL(OCTEON_CN5XXX) || OCTEON_IS_MODEL(OCTEON_CN3XXX)) {
                        ops_cnt = (octeon_remote_read_csr(CVMX_LMC_OPS_CNT_HI) << 32) | octeon_remote_read_csr(CVMX_LMC_OPS_CNT_LO);
                        dclk_cnt = (octeon_remote_read_csr(CVMX_LMC_DCLK_CNT_HI) << 32) | octeon_remote_read_csr(CVMX_LMC_DCLK_CNT_LO);

                        if (!first_time)
                        {
                            printf("DRAM ops count: %lld, dclk count: %lld, utilization: %2.2f%%\n\n", 
                                (unsigned long long)ops_cnt - prev_ops_cnt[0],
                                (unsigned long long)dclk_cnt - prev_dclk_cnt[0],
                                (float)(ops_cnt - prev_ops_cnt[0])/(dclk_cnt - prev_dclk_cnt[0])*100 );
                        }
                        prev_ops_cnt[0] = ops_cnt;
                        prev_dclk_cnt[0] = dclk_cnt;
             
                        printf("Packet Buffers : %2llu free, FPA pool %d\n",
                           (ULL)cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(CVMX_FPA_PACKET_POOL)), CVMX_FPA_PACKET_POOL);
                        printf("WQE buffers    : %2llu free, FPA pool %d\n",
                           (ULL)cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(CVMX_FPA_WQE_POOL)), CVMX_FPA_WQE_POOL);
                        printf("PKO cmd buffers: %2llu free, FPA pool %d, PKO output queues %d\n",
                           (ULL)cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(CVMX_FPA_OUTPUT_BUFFER_POOL)),
                           CVMX_FPA_OUTPUT_BUFFER_POOL, 256 >> cvmx_read_csr(CVMX_PKO_REG_QUEUE_MODE));
                        for (qos=0; qos<=7; qos++)
                        {
                            qos_thr = (ULL)cvmx_read_csr(CVMX_POW_QOS_THRX(qos));
                            iq_cnt = (ULL)cvmx_read_csr(CVMX_POW_IQ_CNTX(qos));
                            iq_com_cnt = (ULL)cvmx_read_csr(CVMX_POW_IQ_COM_CNT);

                            printf("queue %d: WQE count %2lld, Total WQE count %2lld",
                                qos, (ULL)iq_cnt & 0xffffffff, (ULL)iq_com_cnt & 0xffffffff);
                            printf("Free Buffers %2lld, Buffers allocated %2lld, Max Threshold %2lld\n",
                                ((ULL)qos_thr >> 24) & 0xfff, ((ULL)qos_thr >> 36) & 0xfff, ((ULL)qos_thr >> 12) & 0x7ff);
                        }                        
              
                    }
                }
                fflush(stdout);
            }

            profile_data.clear();
            start_time = time(NULL);
            if (!count--)
                break;
            if (first_time)
            {
                //printf("discarding first partial sample interval\n");
                first_time = 0;
            }
        }
    }

    octeon_remote_unlock();
    octeon_remote_close();
    return 0;
}
