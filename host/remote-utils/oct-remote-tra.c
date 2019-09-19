/***********************license start************************************
 * Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
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
 * Use the TRA unit for tracing an Octeon
 *
 * $Id: oct-remote-tra.c 102370 2014-07-31 23:26:20Z cchavva $
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <assert.h>
#include "cvmx.h"
#include "octeon-remote.h"
#undef CVMX_ADD_IO_SEG
#define CVMX_ADD_IO_SEG(a) (a)  /* There aren't any Mips IO segments when using PCI */
#include "cvmx-csr.h"
#include "cvmx-l2c.h"
#include "cvmx-tra.h"
#include "cvmx-version.h"

/*
 * The data present at the begining of each trace file.
 */
struct tra_file_header {
    uint32_t tfh_prid;	/* the PRID where this trace is captured */
    uint32_t tfh_pad;	/* padding */

    /*
     * one for each TRA unit to record the TRA settings before
     * trace capturing. For OCTEON models where only one TRA unit is
     * available, only the first is used.
     */
#define TFH_TRA_CFG_ENTRY(csr_name)	cvmx_tra_##csr_name##_t csr_name
    struct {
	TFH_TRA_CFG_ENTRY(ctl);
#define TFH_TRA_CFG_BLOCK(block_name)				\
	TFH_TRA_CFG_ENTRY(block_name##_cmd);			\
	TFH_TRA_CFG_ENTRY(block_name##_sid);			\
	TFH_TRA_CFG_ENTRY(block_name##_did);			\
	TFH_TRA_CFG_ENTRY(block_name##_adr_adr);		\
	TFH_TRA_CFG_ENTRY(block_name##_adr_msk)

	TFH_TRA_CFG_BLOCK(filt);
	TFH_TRA_CFG_BLOCK(trig0);
	TFH_TRA_CFG_BLOCK(trig1);
    } tfh_tra[4];
};

#define IS_TRACE_MODEL(model, prid)				\
    __OCTEON_IS_MODEL_COMPILE__(model, prid)
#define IS_TRACE_CN68XX(prid)	IS_TRACE_MODEL(OCTEON_CN68XX, prid)
#define IS_TRACE_CN5XXX(prid)	IS_TRACE_MODEL(OCTEON_CN5XXX, prid)
#define IS_TRACE_CN3XXX(prid)	IS_TRACE_MODEL(OCTEON_CN3XXX, prid)

#define U64 unsigned long long

static int use_wrap_mode = 0;
static int use_mcd0_signal = 0;
static int use_trig_mode = 0;
static FILE *use_output_log = NULL;
static FILE *use_input_log = NULL;
static volatile int finished = 0;
static void (*original_sig_int)(int num);
static U64 trace_mask = 0x1; 

U64 filter_cmd_mask = 0;
U64 filter_sid[4] = {0, 0, 0, 0};
U64 filter_did = 0;
U64 filter_addr = 0;
U64 filter_addr_mask = 0;
U64 trig0_cmd_mask = 0;
U64 trig0_sid[4] = {0, 0, 0, 0};
U64 trig0_did = 0;
U64 trig0_addr = 0;
U64 trig0_addr_mask = 0;
U64 trig1_cmd_mask = 0;
U64 trig1_sid[4] = {0, 0, 0, 0};
U64 trig1_did = 0;
U64 trig1_addr = 0;
U64 trig1_addr_mask = 0;


static uint64_t convert_number(const char *str)
{
    unsigned long long result;

    result = 0;
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


/**
 * Parse the string passed by the user for a filter/trigger.
 *
 * @param _spec     String to parse
 * @param cmd_mask  The requested command mask
 * @param sid       The requested sourc ID mask
 * @param did       The requested destination mask
 * @param addr      The requested address
 * @param addr_mask The requested address mask
 *
 * @return Zero on success, negative on failure
 */
int parse_filter_spec(const char *_spec, U64 *cmd_mask, U64 *sid, U64 *did, U64 *addr, U64 *addr_mask)
{
    char spec[80];
    char *parse = spec;
    char *token;

    *cmd_mask = 0;
    *sid = 0;
    *did = 0;
    *addr = 0;
    *addr_mask = 0; // Zero bits always match

    strncpy(spec, _spec, sizeof(spec));
    spec[sizeof(spec)-1] = 0;

    /* First part is the command bit enable, they should be separated by '+',
        but we don't check */
    token = strsep(&parse, ",");
    if (token)
    {
        char *tkn = strsep(&token, "+");
        while (tkn)
        {
            char cmd[80];
            if (sscanf(tkn, "%s", cmd))
	    {
                if (strcmp(cmd, "all") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_ALL;
                if (strcmp(cmd, "nop") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_NOP;
                if (strcmp(cmd, "ldt") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_LDT;
                if (strcmp(cmd, "ldi") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_LDI;
                if (strcmp(cmd, "pl2") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_PL2;
                if (strcmp(cmd, "rpl2") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_RPL2;
                if (strcmp(cmd, "dwb") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_DWB;
                if (strcmp(cmd, "ldd") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_LDD;
                if (strcmp(cmd, "psl1") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_PSL1;
                if (strcmp(cmd, "iobdma") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBDMA;
                if (strcmp(cmd, "stf") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STF;
                if (strcmp(cmd, "stt") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STT;
                if (strcmp(cmd, "stp") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STP;
                if (strcmp(cmd, "stc") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STC;
                if (strcmp(cmd, "stfil1") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STFIL1;
                if (strcmp(cmd, "sttil1") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STTIL1;
                if (strcmp(cmd, "fas32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_FAS32;
                if (strcmp(cmd, "fas64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_FAS64;
                if (strcmp(cmd, "wbil2i") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_WBIL2I;
                if (strcmp(cmd, "ltgl2i") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_LTGL2I;
                if (strcmp(cmd, "stgl2i") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_STGL2I;
                if (strcmp(cmd, "invl2") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_INVL2;
                if (strcmp(cmd, "wbil2") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_WBIL2;
                if (strcmp(cmd, "wbl2") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_WBL2;
                if (strcmp(cmd, "lckl2") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_LCKL2;
                if (strcmp(cmd, "iobld8") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBLD8;
                if (strcmp(cmd, "iobld16") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBLD16;
                if (strcmp(cmd, "iobld32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBLD32;
                if (strcmp(cmd, "iobld64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBLD64;
                if (strcmp(cmd, "iobst8") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBST8;
                if (strcmp(cmd, "iobst16") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBST16;
                if (strcmp(cmd, "iobst32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBST32;
                if (strcmp(cmd, "iobst64") == 0 || strcmp(cmd, "iobst") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_IOBST64;
                if (strcmp(cmd, "set8") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_SET8;
                if (strcmp(cmd, "set16") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_SET16;
                if (strcmp(cmd, "set32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_SET32;
                if (strcmp(cmd, "set64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_SET64;
                if (strcmp(cmd, "clr8") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_CLR8;
                if (strcmp(cmd, "clr16") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_CLR16;
                if (strcmp(cmd, "clr32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_CLR32;
                if (strcmp(cmd, "clr64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_CLR64;
                if (strcmp(cmd, "incr8") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_INCR8;
                if (strcmp(cmd, "incr16") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_INCR16;
                if (strcmp(cmd, "incr32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_INCR32;
                if (strcmp(cmd, "incr64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_INCR64;
                if (strcmp(cmd, "decr8") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_DECR8;
                if (strcmp(cmd, "decr16") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_DECR16;
                if (strcmp(cmd, "decr32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_DECR32;
                if (strcmp(cmd, "decr64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_DECR64;
                if (strcmp(cmd, "faa32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_FAA32;
                if (strcmp(cmd, "faa64") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_FAA64;
                if (strcmp(cmd, "saa32") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_SAA32;
                if (strcmp(cmd, "saa64") == 0 || strcmp(cmd, "saa") == 0)
                    *cmd_mask |= CVMX_TRA_FILT_SAA64;
            }
            tkn = strsep(&token, "+");
        }
        if (*cmd_mask == 0)
        {
            printf("Invalid command mask\n");
            return -1;
        }
   }

    /* Second part is the SID, they should be separated by '+'. */ 
   token = strsep(&parse, ",");

   char* tkn = strsep(&token, "+");
   while (tkn) 
   {
       int pp;
       if (strstr(tkn, "all"))
       {
           sid[0] |= CVMX_TRA_SID_ALL;
           sid[1] |= CVMX_TRA_SID_ALL;
	   sid[2] |= CVMX_TRA_SID_ALL;
           sid[3] |= CVMX_TRA_SID_ALL;
       }
       if (strstr(tkn, "dwb"))
       {
           sid[0] |= CVMX_TRA_SID_DWB;
           sid[1] |= CVMX_TRA_SID_DWB;
           sid[2] |= CVMX_TRA_SID_DWB;
           sid[3] |= CVMX_TRA_SID_DWB;
       }
       if (strstr(tkn, "iobreq"))
       {
           sid[0] |= CVMX_TRA_SID_IOBREQ;
           sid[1] |= CVMX_TRA_SID_IOBREQ;
           sid[2] |= CVMX_TRA_SID_IOBREQ;
           sid[3] |= CVMX_TRA_SID_IOBREQ;
       }
       if (strstr(tkn, "pko"))
       {
           sid[0] |= CVMX_TRA_SID_PKO;
           sid[1] |= CVMX_TRA_SID_PKO;
           sid[2] |= CVMX_TRA_SID_PKO;
           sid[3] |= CVMX_TRA_SID_PKO;
       }
       if (strstr(tkn, "pki"))
       {
           sid[0] |= CVMX_TRA_SID_PKI;
           sid[1] |= CVMX_TRA_SID_PKI;
           sid[2] |= CVMX_TRA_SID_PKI;
           sid[3] |= CVMX_TRA_SID_PKI;
       }
       if (1==sscanf(tkn, "pp%d", &pp)) {
           /* On cn68xx, each TRA buffer has different PPs.
              TRA0 can trace cores 0,4,..28.
              TRA1 can trace cores 1,5,..29.
              TRA2 can trace cores 2,6,..30.
              TRA3 can trace cores 3,7,..31. */
           if (OCTEON_IS_MODEL(OCTEON_CN68XX))
           {
               if ((pp < 32) && (pp >= 0)) 
                   sid[(pp%4)] |= (1ull << (pp / 4));
           }
           /* Make sure PP is valid number */
           else if ((pp < 16) && (pp >=0))
               sid[0] |= 1ull << pp;
       }
       tkn = strsep(&token, "+");
    } /* End of while tkn*/

    if (sid[0] == 0 && sid[1] == 0 && sid[2] == 0 && sid[3] == 0)
    {
        printf("Invalid source id\n");
        return -1;
    }

    /* Third part is the DID, they should be separated by '+', but we don't check */
    token = strsep(&parse, ",");
    if (token)
    {
        if (strstr(token, "all"))
            *did |= CVMX_TRA_DID_ALL;
        if (strstr(token, "fau"))
            *did |= CVMX_TRA_DID_FAU;
        if (strstr(token, "dpi"))
            *did |= CVMX_TRA_DID_DPI;
        if (strstr(token, "rad"))
            *did |= CVMX_TRA_DID_RAD;
        if (strstr(token, "usb0"))
            *did |= CVMX_TRA_DID_USB0;
        if (strstr(token, "pow"))
            *did |= CVMX_TRA_DID_POW;
        if (strstr(token, "pko"))
            *did |= CVMX_TRA_DID_PKO;
        if (strstr(token, "ipd"))
            *did |= CVMX_TRA_DID_IPD;
        if (strstr(token, "rng"))
            *did |= CVMX_TRA_DID_RNG;
        if (strstr(token, "zip"))
            *did |= CVMX_TRA_DID_ZIP;
        if (strstr(token, "dfa"))
            *did |= CVMX_TRA_DID_DFA;
        if (strstr(token, "fpa"))
            *did |= CVMX_TRA_DID_FPA;
        if (strstr(token, "key"))
            *did |= CVMX_TRA_DID_KEY;
        if (strstr(token, "pci"))
            *did |= CVMX_TRA_DID_PCI;
        if (strstr(token, "sli"))
            *did |= CVMX_TRA_DID_SLI;
        if (strstr(token, "mio"))
            *did |= CVMX_TRA_DID_MIO;
        if (*did == 0)
        {
            printf("Invalid dest id\n");
            return -1;
        }
    }

    /* Fourth part is the address */
    token = strsep(&parse, ",");
    if (token)
    {
        *addr = convert_number(token);
    }

    /* Fifth part is the mask */
    token = strsep(&parse, ",");
    if (token)
    {
        *addr_mask = convert_number(token);
    }

    return 0;
}

/**
 * Parse the string passed by the user for selecting a particular TRA unit.
 *
 * @_spec	String to parse
 *
 * @return Zero on success, negative on failure
 */
int parse_traceargs(const char *_spec)
{
    char spec[10], val[10];
    char *parse = spec;
    char *token;

    strncpy(spec, _spec, sizeof(spec));
    spec[sizeof(spec)-1] = 0;
    trace_mask = 0; /* Reset the value of trace_mask */ 

    token = strsep(&parse, ","); 
    while (token) {
        if (sscanf(token, "%s", val))
            trace_mask |= 1 << atoi(val); 
        token = strsep(&parse, ","); 
    }

    if ((trace_mask == 0) || (trace_mask >= 16)) {
        printf("TRA unit out of range, should be between 0..3\n");
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
int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {"long-help", 0, 0, 'H'},
        {"wrap", 0, 0, 'w'},
        {"trig0", 1, 0, '0'},
        {"trig1", 1, 0, '1'},
        {"trig-mode", 1, 0, 'm'},
        {"filter", 1, 0, 'f'},
        {"mcd0", 0, 0, 'd'},
        {"output", 0, 0, 'o'},
        {"input", 1, 0, 'i'},
        {"tra",1, 0, 't'}, 
        {NULL, 0, 0, 0}
    };

    int option_index = 0;
    int show_usage = 0;
    int show_long_usage = 0;
    int set_tra_unit = 0; 
    int number_of_cores;
    number_of_cores = cvmx_octeon_num_cores();

    while (1)
    {
        char option = getopt_long(argc, argv, "hvHw0:1:m:f:do:i:t:", long_options, &option_index);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'h':
                show_usage = 1;
                break;

            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
                return -1;
                break;
            
            case 'H':
                show_usage = 1;
                show_long_usage = 1;
                break;
            case 'w':
                use_wrap_mode = 1;
                break;
            case '0':
                if (parse_filter_spec(optarg, &trig0_cmd_mask, trig0_sid,
                            &trig0_did, &trig0_addr, &trig0_addr_mask))
                    show_usage = 1;
                break;
            case '1':
                if (parse_filter_spec(optarg, &trig1_cmd_mask, trig1_sid,
                            &trig1_did, &trig1_addr, &trig1_addr_mask))
                    show_usage = 1;
                break;
            case 'm':
                if (strcmp(optarg, "none") == 0)
                {
                    use_trig_mode = 0;
                }
                else if (strcmp(optarg, "start") == 0)
                {
                    use_trig_mode = 2;
                }
                else if (strcmp(optarg, "stop") == 0)
                {
                    use_trig_mode = 3;
                }
                else if (strcmp(optarg, "both") == 0)
                {
                    use_trig_mode = 1;
                }
                else
                {
                    printf("Illegal trig-mode\n");
                    show_usage = 1;
                }
                break;
            case 'f':
                if (parse_filter_spec(optarg, &filter_cmd_mask, filter_sid,
                            &filter_did, &filter_addr, &filter_addr_mask))
                    show_usage = 1;
                break;
            case 'd':
                use_mcd0_signal = 1;
                break;
            case 'o':
                use_output_log = fopen(optarg, "w");
                if (use_output_log == NULL)
                {
                    perror("Failed to open output file");
                    return -1;
                }
                break;
            case 'i':
                use_input_log = fopen(optarg, "r");
                if (use_input_log == NULL)
                {
                    perror("Failed to open input file");
                    return -1;
                }
                break;
            case 't':
                set_tra_unit = 1; 
                if (OCTEON_IS_MODEL(OCTEON_CN68XX)) { 
                     if (parse_traceargs(optarg))
                         show_usage = 1;
                } else {
                     printf("Ignoring -t, not supported on this Octeon Hardware\n");
                }
                break;
            default:
                show_usage = 1;
                break;
        }
    }

    /* Show usage if a filter command wasn't specified and we aren't reading
	from a file */
    if ((filter_cmd_mask == 0) && (use_input_log == NULL))
        show_usage = 1;
    
    if (!set_tra_unit) 
        trace_mask = (1ull << (CVMX_L2C_TADS)) - 1;

    if (show_usage)
    {
        printf("\n"
               "Cavium Octeon TRA Analyzer\n"
               "\n"
               "Usage:\n"
               "    %s [options]\n"
               "\n"
               "Supported Options:\n"
               "    -h --help\n"
               "    -H --long-help\n"
               "    -f --filter=<match>\n"
               "        Set the filter for the events to record. The default\n"
               "        records nothing.\n"
               "    -w --wrap\n"
               "        Overwrite old samples when buffer overflows. When\n"
               "        samples are written faster than we can read them, the\n"
               "        new samples will overwrite old samples.\n"
               "    -m --trig-mode=[none|start|stop|both]\n"
               "        Set the mode of how triggers are used.\n"
               "        none = Triggers are ignored. This is the default.\n"
               "        start = Both triggers signal when TRA should start.\n"
               "        stop = Both triggers signal when TRA should stop.\n"
               "        both = Trigger 1 starts tracing, trigger 2 stops it.\n"
               "    -0 --trig0=<match>\n"
               "    -1 --trig1=<match>\n"
               "    -d --mcd0\n"
               "        Pulse MCD0 to stop cores when the TRA is full. This\n"
               "        gives this program a change to catch up. Once the TRA\n"
               "        buffer is emptied, the cores will be restarted.\n"
               "    -o --output=name\n"
               "        Write TRA output to binary file \"name\". This file can\n"
               "        be read later using the \"-i\" option.\n"
               "    -i --input=name\n"
               "        Read from binary file \"name\" instead of the hardware.\n"
               "        This should be used to read a file created with the \"-o\"\n"
               "        option.\n"
               "    -t --tra=tra_unit(s).\n"
               "        Enter the TRA no. to see the output from a specific TRA buffer\n"
               "\n"
               "<match>=<commands>,<sources>,<destinations>,<address>,<mask>\n"
               "\n", argv[0]);
        if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
            printf("<commands> is a list of the following separated by '+':\n"
               "    dwb, pl2, psl1, ldd, ldi, ldt, stc, stf, stp, stt, iobld8,\n"
               "    iobld16, iobld32, iobld64, iobst, iobdma, or all\n");
        else
            printf("<commands> is a list of the following separated by '+':\n"
               "    nop, ldt, ldi, pl2, rpl2, dwb, ldd, psl1, iobdma,\n"
               "    stf, stt, stp, stc, stfil1, sttil1, fas32, fas64,\n"
               "    wbil2i, ltgl2i, stgl2i, invl2, wbil2, wbl2, lckl2,\n"
               "    iobld8, iobld16, iobld64, iobst8, iobst16, iobst32, iobst64,\n"
               "    set8, set16, set32, set64, clr8, clr16, clr32, clr64,\n"
               "    incr8, incr16, incr32, incr64, decr8, decr16, decr32, decr64,\n"
               "    faa32, faa64, saa32, saa64, or all\n");

        printf("<sources> is a list of the following separated by '+':\n"
               "    dwb, iobreq, pko, pki, pp0-pp%d, or all\n",
               (number_of_cores - 1));
        if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
            printf("<destinations> is a list of the following separated by '+':\n"
               "    pow, rng, zip, dfa, fpa, key, pci, mio, or all\n");
        else
            printf("<destinations> is a list of the following separated by '+':\n"
               "    fau, dpi, rad, usb0, pow, pko, ipd, rng, zip, dfa, fpa, key,\n"
               "    sli, mio, or all\n");
        printf(
               "<address> is a physical address to match\n"
               "<mask> Each bits set is a bit that is checked in <address>\n\n");
        if (show_long_usage)
        {
            printf("Brief description of the <commands>:\n");
            if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
                printf(
               "    dwb     - Don't-Write-Back: Clear the dirty bit in the L2\n"
               "              tags if the cache block is present in the L2\n"
               "              cache. This is an ADD-only transaction on the CMB.\n"
               "    pl2     - Prefetch Into L2: The cache block will be put\n"
               "              into the L2 cache. This is an ADD-only\n"
               "              transaction on the CMB. These transactions are\n"
               "              only issued by cores.\n"
               "    psl1    - Data Cache Fill-Through: The cache block is filled\n"
               "              from L2/DRAM. The block will be filled into the\n"
               "              dcache of the requesting core. A subsequent store\n"
               "              from another core or IOB will cause an invalidate.\n"
               "              The block will not be put into the L2 cache. These\n"
               "              transactions can only be issued by cores.\n"
               "    ldd     - Data Cache Fill: The cache block is filled from\n"
               "              L2/DRAM. The block will be filled into the dcache\n"
               "              of the requesting core. A subsequent store from\n"
               "              another core or IOB will cause an invalidate. The\n"
               "              block is put into the L2 cache. These transactions\n"
               "              can only be issued by cores.\n"
               "    ldi     - Instruction Cache / IO Fill: The cache block is\n"
               "              filled from L2/DRAM. The block is put into the L2\n"
               "              cache. These transactions can be issued either by\n"
               "              cores or by the IOB. A subsequent store from\n"
               "              another core or IOB will NOT cause an invalidate.\n"
               "    ldt     - Instruction Cache / IO Fill-Through: The cache\n"
               "              block is filled from L2/DRAM. The block will not\n"
               "              be put into the L2 cache. These transactions can\n"
               "              be issued either by cores or by the IOB.\n"
               "    stc     - Store-Conditional: Store to either 32 or 64-bits\n"
               "              if the block is currently held in the data cache\n"
               "              of the requesting core. If the block is in the\n"
               "              data cache of the requesting core, the store\n"
               "              happens and a commit indication is returned. All\n"
               "              data cache copies of the block will be invalidated\n"
               "              in the other cores. If the block is not in the\n"
               "              data cache of the requesting core, a failure\n"
               "              indication is returned, no invalidate occurs,\n"
               "              neither does the store. The cache block will be\n"
               "              put into the L2 cache. These transactions are\n"
               "              only issued by cores.\n"
               "    stf     - Store-Full: Store to all bytes in the cache block.\n"
               "              The value of the bytes in the cache block that are\n"
               "              not transferred on the STORE bus or are masked off\n"
               "              will be written to zero. All data cache copies of\n"
               "              the block will be invalidated, except for the data\n"
               "              cache of an initiating core. The cache block will\n"
               "              be put into the L2 cache. These transactions are\n"
               "              issued either by cores or by the IOB.\n"
               "    stp     - Store-Partial: Some of the bytes in the cache\n"
               "              block will be stored. The value of the bytes in\n"
               "              the cache block that are not transferred on the\n"
               "              STORE bus or are masked off are not modified. All\n"
               "              data cache copies of the block will be invalidated,\n"
               "              except for the data cache of an initiating core.\n"
               "              The cache block will be put into the L2 cache.\n"
               "              These transactions are issued either by cores or\n"
               "              by the IOB.\n"
               "    stt     - Store-Through: Store to all bytes in the cache\n"
               "              block. The value of the bytes in the cache block\n"
               "              that are not transferred on the STORE bus or are\n"
               "              masked off will be written to zero. All data cache\n"
               "              copies of the block will be invalidated, except\n"
               "              for the data cache of an initiating core. The\n"
               "              cache block will not be put into the L2 cache.\n"
               "              These transactions are only issued by the IOB.\n"
               "    iobld8  - Load Reflection 8bit: An ADD cycle will be\n"
               "              reflected onto the FILL bus. This is used for\n"
               "              communication from the cores to the IOB. A load\n"
               "              reflection transaction is generated by the cores\n"
               "              to transfer a load to IOB.\n"
               "    iobld16 - Load Reflection 16bit.\n"
               "    iobld32 - Load Reflection 32bit.\n"
               "    iobld64 - Load Reflection 64bit.\n"
               "    iobst   - Store Reflection: Reflect an ADD cycle and\n"
               "              corresponding STORE cycle(s) onto the FILL bus.\n"
               "              This is used for core IOB communication. A store\n"
               "              reflection transaction is generated by the cores\n"
               "              to transfer a store/IOBDMA to IOB. An IOBDMA must\n"
               "              have exactly 64 bits of (aligned) STORE data. A\n"
               "              store may have 64, 32, 16, or 8 bits of aligned\n"
               "              STORE data. These transactions are destined to\n"
               "              IOB.\n"
               "    iobdma  - A store reflection transaction is generated\n"
               "              by the IOB to respond to a prior load/IOBDMA\n"
               "              request from a core. A load response always\n"
               "              contains exactly 64 bits of (aligned) STORE\n"
               "              data. An individual IOBDMA response contains\n"
               "              between one and sixteen 64 bit (aligned) words\n"
               "              of data. These transactions are destined to a core.\n"
               "    saa     - Store atomic add to either 32 or 64 bits. Acts like\n"
               "              an unmerged STP transaction, except that the valid\n"
               "              bytes are added to the memory location, not stored to\n"
               "              it. All data-cache copies of the block are invalidated\n"
               "              in all cores, including the requesting core. The cache\n"
               "              block is put into the L2 cache. These transactions are\n"
               "              issued only by cores.\n"
               "    all     - All of the above commands added together.\n");
            else
                printf(
               "    nop     - None\n"
               "    ldd     - Data Cache Fill: Fill the cache block from L2/DRAM\n"
               "              into the L1 data of the requesting core. (A later\n"
               "              store from another core or IOB will invalidate the\n"
               "              block in the L1 data cache. Put the block into the\n"
               "              L2 cache if it wasn't already present. Only cores\n"
               "              initiate these transactions.\n"
               "    psl1    - Data Cache Fill-Through: Fill the cache block from\n"
               "              L2/DRAM into the L1 data cache of the requesting\n"
	       "              core. (A later store from another core or IOB will\n"
	       "              invalidate the block in the L1 data cache). Do not\n"
	       "              put the block into the L2 cache. Only cores initiate\n"
               "              these transactions via PREF instructions\n" 
               "    ldi     - Instruction Cache / IO Fill: Fill the cache block\n"
               "              from L2/DRAM into the L1 instruction cache or IOB.\n"
               "              Put the cache block into the L2 cache if it wasn't\n"
               "              already present. Both cores and IOB initiate these\n"
               "              transactions. The cores only fetch instructions via\n"
               "              these transactions.\n"
               "    ldt     - Instruction Cache / IO Fill-Through: Fill the cache\n"
               "              block from L2/DRAM into the L1 instruction cache or\n"
               "              to IOB. Do not put the cache block into the L2 cache\n"
               "              Both cores and IOB initiate these transactions. The\n"
               "              cores only fetch instructions via these transactions\n"
               "    stp     - Store-Partial: Store to some of the bytes in the\n"
               "              cache block. Do not modify the bytes in the cache\n"
               "              block that are not stored. Invalidate all L1 data\n"
               "              cache copies of the block, except for the L1 data\n"
               "              cache of the initiator (if the initiator was a core).\n"
               "              Put the cache block into the L2 cache if it wasn't\n"
               "              already present. Both cores and IOB initiate these\n"
               "              transactions.\n"
               "    stfil1  - Store-Full Invalidate L1.\n"
               "    stf     - Store-Full: Store to all bytes in the cache block.\n"
               "              Zero the bytes not transferred on the STORE bus (or\n"
               "              masked off). Invalidate all L1 data cache copies of\n"
               "              the block, except for the L1 data cache of the\n"
               "              initiator (if the initiator was a core). The core\n"
               "              that initiates a STFIL1 self-invalidates the block\n"
               "              from its L1 data cache, if necessary. Put the cache\n"
               "              block into the L2 cache if it wasn't already present.\n"
               "              Both cores and IOB initiate STF transactions. Only\n"
               "              cores initiate STFIL1 transaction via ZCB instructions.\n"
               "    sttil1  - Store-Through Invalidate L1.\n"
               "    stt     - Store-Through: Store to all bytes in the cache block\n"
               "              Zero the bytes not transferred on the STORE bus (or\n"
               "              masked off). Invalidate all L1 data cache copies of\n"
               "              the block, except for the L1 data cache of the\n"
               "              initiator (if the initiator was a core). The core\n"
               "              that initiates a STFIL1 self-invalidates the block\n"
               "              from its L1 data cache, if necessary. Do not put the\n"
               "              cache block into the L2 cache. Both cores and IOB\n"
               "              initiate STT transactions. (The cores only initiate\n"
               "              STT's via PREF instructions). Only cores initiate \n"
               "              STTIL1 transaction (via ZCB instructions).\n"
               "    stc     - Store-Conditional: If the block is present in the\n"
               "              data cache of the requesting core, store to either\n"
               "              32-bits or 64-bits and invalidate all other L1 data\n"
               "              cache copies of the block in all other cores. If the\n"
               "              block is not present in the data cache of the\n"
               "              requesting core, fail the transactions, neither\n"
               "              storing or invalidating. On success, put the cache\n"
               "              block into the L2 cache if it wasn't already present.\n"
               "              Only cores initiate these transactions (via SC/SCD\n"
               "              instructions).\n"
               "    wbil2i  - L2 Cache Index Writeback Invalidate: If the selected\n"
               "              L2 cache block is dirty (i.e. if VALID and DIRTY\n"
               "              bits are set), write it back to the DRAM. (The ADD\n"
               "              bus identifies the L2 cache block via an index and\n"
               "              way, rather than an address for this transaction).\n"
               "              Invalidate the selected L2 cache block (i.e. clear\n"
               "              the VALID, USED and LOCK bits). Only cores initiate\n"
               "              these transactions (via CACHE instructions).\n"
               "    ltgl2i  - L2 Cache Index Load Tag: Read tag and state for the\n"
               "              selected L2 cache block into L2C_TAD0_TAG. (The ADD\n"
               "              bus identifies the cache block via an index and way,\n"
               "              rather than an address for this transaction). Only\n"
               "              cores initiate these transactions (via CACHE instructions).\n"
               "    stgl2i  - L2 Cache Index Store Tag: Write tag and state for the\n"
               "              selected L2 cache block to the current L2C_TAD0_TAG\n"
               "              value. (The ADD bus identifies the cache block via\n"
               "              an index and way, rather than an address for this\n"
               "              transaction). Only cores initiate these transactions\n"
               "              (via CACHE instructions).\n"
               "    invl2   - L2 Cache Hit Invalidate: If the cache block is\n"
               "              present in the L2 cache, invalidate it (i.e. clear\n"
               "              the VALID, USED and LOCK bits). Do nothing if the\n"
               "              cache block is not present in the L2 cache. If dirty\n"
               "              data for the locations was present in the L2 cache,\n"
               "              it is lost. The current value of the cache block is\n"
               "              unpredictable after the INVL2 transaction until the\n"
               "              block is later stored to, as stale copies of the\n"
               "              block may be present in the L1 data caches. Only\n"
               "              cores initiate these transactions (via CACHE\n"
               "              instructions).\n"
               "    wbil2   - L2 Cache Hit Writeback Invalidate: If the cache\n"
               "              block is present in the L2 cache, invalide it (i.e\n"
               "              clear the VALID, USED and LOCK bits) after first\n"
               "              writing the block to DRAM if it was dirty (i.e. if\n"
               "              VALID and DIRTY bits were set). Do nothing if the\n"
               "              cache block is not present in the L2 cache. Only\n"
               "              cores initiate these transactions (via CACHE\n"
               "              instructions).\n"
               "    lckl2   - L2 Cache Fetch and Lock: Set the LOCK (and USE) bit\n"
               "              for the block. Put the cache block into the L2 cache\n"
               "              if it wasn't already present and there is an L2\n"
               "              cache way available. LCKL2 is a NOP if it misses in\n"
               "              the L2 cache when there are no ways available. Only\n"
               "              cores initiate these transactions (via CACHE\n"
               "              instructions).\n" 
               "    dwb     - Don't-Write-Back: If the cache block is present in\n"
               "              the L2 cache, clear the DIRTY bit. Do nothing if the\n"
               "              cache block is not present in the L2 cache. If dirty\n"
               "              data for the location was present in the L2 cache,\n"
               "              it may be lost. The current value of the cache block\n"
               "              is unpredictable after the DWB transaction until the\n"
               "              block is later stored to, as stale copies of the\n"
               "              block may be present in the L1 data caches. Both\n"
               "              cores and IOB initiate DWB transactions. (The cores\n"
               "              only initiate DWB's via PREF instructions).\n" 
               "    pl2     - Prefetch Into L2: Put the cache block into the L2\n"
               "              cache if it wasn't already present. This is an\n"
               "              ADD-only transaction on the CMI. Only cores initiate\n"
               "              these transactions (via PREF instructions).\n"
               "    rpl2    - Evict Soon from L2: If the cache block is present in\n"
               "              the L2 cache (and not locked), clear its USE bit.\n"
               "              (Clearing the USE bit makes it more likely that the\n"
               "              cache block will be replaced in the future). Do\n"
               "              nothing if the cache block is (locked or) not\n"
               "              present in the L2 cache. This is an ADD-busy-only\n"
               "              transaction on the CMI. Only cores initiate these\n"
               "              transactions (via PREF instructions).\n" 
               "    wbl2    - L2 Cache Nudge/Hit Writeback: If the cache block is\n"
               "              present in the L2 cache, clear its DIRTY and USE bits\n"
               "              after first writing the block to DRAM if it wasi\n"
               "              dirty (i.e. if VALID and DIRTY bits were set).\n"
               "              (Clearing the USE bit makes it more likely that the\n"
               "              cache block will be replaced in the future). Do\n"
               "              nothing if the cache block is not present in the L2\n"
               "              cache. Do not clear the USE bit if the block is\n"
               "              locked. This is an ADD-bus-only transaction on the\n"
               "              CMI. Only cores initiate these transactions (via\n"
               "              PREF and CACHE instructions).\n"
               "    saa32   - Store-Atomic Add 32-bit\n"
               "    saa64   - Store-Atomic Add: Read the current value of a\n"
               "              naturally-aligned 32-bit/64-bit word in L2/DRAM and\n"
               "              atomically add a valud to it and update L2/DRAM with\n"
               "              the result. Invalidate all L1 data cache copies of\n"
               "              the surrounding cache block, except for the L1 data\n"
               "              cache of the initiator. The core that initiates the\n"
               "              transaction self-invalidates the surrounding block\n"
               "              from its L1 data cache, if necessary. Put the cache\n"
               "              block into the L2 cache if it wasn't already present.\n"
               "              Only cores initiate these transactions.\n"
               "    faa32   - L2 Cache Atomic Fetch and Add 32-bit\n"
               "    faa64   - L2 Cache Atomic Fetch and Add: Return the current\n"
               "              value of a naturally-aligned 32-bit or 64-bit word\n"
               "              in L2/DRAM and atomically add a value to it and\n"
               "              update L2/DRAM with the result. Invalidate all L1 \n"
               "              data cache copies of the surrounding cache block,\n"
               "              except for the L1 data cache of the initiator. The\n"
               "              core that initiates the transaction self-invalidates\n"
               "              the surrounding block from its L1 data cache, if\n"
               "              necessary. Put the cache block into the L2 cache if\n"
               "              it wasn't already present. Only cores initiate\n"
               "              these transactions.\n"
               "    incr8   - L2 Cache Atomic Fetch and Increment 8-bit\n"
               "    incr16  - L2 Cache Atomic Fetch and Increment 16-bit\n"
               "    incr32  - L2 Cache Atomic Fetch and Increment 32-bit\n"
               "    incr64  - L2 Cache Atomic Fetch and Increment: Return the current\n"
               "              value of a naturally-aligned 32-bit or 64-bit word\n"
               "              in L2/DRAM and atomically add one to it and\n"
               "              update L2/DRAM with the result. Invalidate all L1 \n"
               "              data cache copies of the surrounding cache block,\n"
               "              except for the L1 data cache of the initiator. The\n"
               "              core that initiates the transaction self-invalidates\n"
               "              the surrounding block from its L1 data cache, if\n"
               "              necessary. Put the cache block into the L2 cache if\n"
               "              it wasn't already present. Cores initiate INCR64 and\n"
               "              INCR32 transactions (via LAI/LAID instructions).\n"
               "              IOB initiate INCR32, INCR16 and INCR8 transactions.\n"
               "    decr8   - L2 Cache Atomic Fetch and Decrement 8-bit\n"
               "    decr16  - L2 Cache Atomic Fetch and Decrement 16-bit\n"
               "    decr32  - L2 Cache Atomic Fetch and Decrement 32-bit\n"
               "    decr64  - L2 Cache Atomic Fetch and Decrement: Return the\n"
               "              current value of a naturally-aligned 8-bit, 16-bit,\n"
               "              32-bit or 64-bit word in L2/DRAM and atomically \n"
               "              subtract one to it and update L2/DRAM with the\n"
               "              result. Invalidate all L1 data cache copies of the\n"
               "              surrounding cache block, except for the L1 data cache\n"
               "              of the initiator. The core that initiates the\n"
               "              transaction self-invalidates the surrounding block\n"
               "              from its L1 data cache, if necessary. Put the cache\n"
               "              block into the L2 cache if it wasn't already present.\n"
               "              Cores initiate DECR64 and DECR32 transactions (via\n"
               "              LAD/LADD instructions). IOB initiate DECR32, DECR16\n"
               "              and DECR8 transactions.\n"
               "    fas32   - L2 Cache Atomic Swap 32-bit\n"
               "    fas64   - L2 Cache Atomic Swap: Return the current value of a\n"
               "              naturally-aligned 32-bit or 64-bit word in L2/DRAM\n"
               "              and atomically store to it. Invalidate all L1 data\n"
               "              cache copies of the surrounding cache block, except\n"
               "              for the L1 data cache of the initiator. The core\n"
               "              that initiates the transactions self-validates the\n"
               "              surrounding block from its L1 data cache, if\n"
               "              necessary. Put the cache block into the L2 cache if\n"
               "              it wasn't already present. Only cores initiate these\n"
               "              transactions (via LAW/LAWD instructions).\n"
               "    set8    - L2 Cache Atomic Fetch and Set 8-bit\n"
               "    set16   - L2 Cache Atomic Fetch and Set 16-bit\n"
               "    set32   - L2 Cache Atomic Fetch and Set 32-bit\n"
               "    set64   - L2 Cache Atomic Fetch and Set: Return the current\n"
               "              value of a naturally-aligned 8-bit, 16-bit, 32-bit\n"
               "              or 64-bit word in L2/DRAM and atomically set all bits\n"
               "              in it. Invalidate all L1 data cache copies of the\n"
               "              surrounding cache block, except for the L1 data cache\n"
               "              of the initiator. The core that initiates the\n"
               "              transaction self-invalidates the surrounding block\n"
               "              from its L1 data cache, if necessary. Put the cache\n"
               "              block into the L2 cache if it wasn't already present.\n"
               "              Cores initiate SET64 and SET32 transactions (via\n"
               "              LAS/LASD instructions). IOB initiates SET32, SET16\n"
               "              and SET8 transactions.\n"
               "    clr8    - L2 Cache Atomic Fetch and Clear 8-bit\n"
               "    clr16   - L2 Cache Atomic Fetch and Clear 16-bit\n"
               "    clr32   - L2 Cache Atomic Fetch and Clear 32-bit\n"
               "    clr64   - L2 Cache Atomic Fetch and Clear: Return the current\n"
               "              value of a naturally-aligned 8-bit, 16-bit, 32-bit\n"
               "              or 64-bit word in L2/DRAM and atomically clear all bits\n"
               "              in it. Invalidate all L1 data cache copies of the\n"
               "              surrounding cache block, except for the L1 data cache\n"
               "              of the initiator. The core that initiates the\n"
               "              transaction self-invalidates the surrounding block\n"
               "              from its L1 data cache, if necessary. Put the cache\n"
               "              block into the L2 cache if it wasn't already present.\n"
               "              Cores initiate CLR64 and CLR32 transactions (via\n"
               "              LAC/LACD instructions). IOB initiates CLR32, CLR16\n"
               "              and CLR8 transactions.\n"
               "    iobst8  - Store to IOB 8-bit.\n"
               "    iobst16 - Store to IOB 16-bit.\n"
               "    iobst32 - Store to IOB 32-bit.\n"
               "    iobst64 - Store to IOB: Forward a naturally-aligned address\n"
               "              and corresponding 8-bit, 16-bit, 32-bit or 64-bit\n"
               "              word onto the IOC bus. Either service the IO store\n"
               "              directly in IOB or forward it onto the IOI for\n"
               "              another Octeon IO device. Only Cores initiate these\n"
               "              transactions.\n"
               "    iobld8  - Load to IOB 8-bit:\n"
               "    iobld16 - Load to IOB 16-bit.\n"
               "    iobld32 - Load to IOB 32-bit.\n"
               "    iobld64 - Load to IOB 64-bit: Forward a naturally-aligned \n"
               "              address onto the IOC bus. Either service the IO\n"
               "              load directly in IOB or forward it onto the IOI and\n"
               "              service it in another Octeon IO device. Return the \n"
               "              8-bit, 16-bit, 32-bit or 64-bit result via an IOR\n"
               "              single-cycle transfer. Forward this IOR response\n"
               "              onto the COMMIT/FILL busses to complete the\n"
               "              transaction. Only cores initiate these transactions\n"
               "    iobdma  - Forward a naturally-aligned address obtained from the\n"
               "              STORE bus onto the IOC bus. Either service the IO\n"
               "              vector load directly in IOB or forward it onto the\n"
               "              IOI for another Octeon IO device. The total IOBDMA\n"
               "              length may be as large as 255 64-bit words. Respond\n"
               "              with a series of 1<=n<=16 word responses. Return\n"
               "              each n-vector of 64-bit results via an IOR n-cycle\n"
               "              transfer. Forward each IOR response onto the \n"
               "              COMMIT/FILL busses until the transaction is complete.\n"
               "              Only cores initiate these transactions.\n" 
               "    all     - All of the above commands added together.\n");
            printf(
               "Brief description of the <sources>:\n"
               "    dwb     - Indicates a DWB from the DWB engine in IOB.\n"
               "    iobreq  - Indicates a FPA, TIM, DFA, PCI, ZIP, or POW\n"
               "              read/write, or a PKO write.\n"
               "    pko     - Indicates a PKO read.\n"
               "    pki     - Indicates a PIP/IPD write.\n"
               "    pp0-pp%d- Indicates a particular core.\n"
               "    all     - All of the sources added together.\n",
               (number_of_cores - 1));
            printf("Brief description of the <destinations>:\n");
            if (!(OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX)))
                printf(
               "    fau     - Indicates a destination of FAU accesses\n"
               "    dpi     - Indicates a destination of DPI accesses (DPI NCB CSRs)\n"
               "    rad     - Indicates a destination of RAD accesses\n"
               "    usb0    - Indicates a destination of USB0 accesses (UAHC0 EHCI\n"
               "              and OHCI NCB CSRs)\n"
               "    pow     - Indicates a destination of POW (get work, add\n"
               "              work, status/memory/index loads, NULLRd loads,\n"
               "              CSRs).\n"
               "    pko     - Indicates a destination of PKO (doorbells).\n"
               "    ipd     - Indicates a destination of IPD CSR accesses\n"
               "    rng     - Indicates a destination of RNG (loads/IOBDMAs).\n"
               "    zip     - Indicates a destination of ZIP (doorbells).\n"
               "    dfa     - Indicates a destination of DFA (CSR's, doorbells,\n"
               "              and other operations).\n"
               "    fpa     - Indicates a destination of FPA (free pool\n"
               "    key     - Indicates a destination of KEY (reads/writes).\n"
               "    sli     - Indicates a destination of SLI (access to RSL-type\n"
               "              CSRs from many Octeon blocks, PCI/sRIO bus operations,\n"
               "              and SLI CSRs).\n"
               "    mio     - Indicates a destination of CIU or GPIO (for CSRs).\n"
               "    all     - Matches all destinations.\n");
            else
                printf(
               "    pow     - Indicates a destination of POW (get work, add\n"
               "              work, status/memory/index loads, NULLRd loads,\n"
               "              CSRs).\n"
               "    rng     - Indicates a destination of RNG (loads/IOBDMAs).\n"
               "    zip     - Indicates a destination of ZIP (doorbells).\n"
               "    dfa     - Indicates a destination of DFA (CSR's, doorbells,\n"
               "              and other operations).\n"
               "    fpa     - Indicates a destination of FPA (free pool\n"
               "              allocates / frees).\n"
               "    key     - Indicates a destination of KEY (reads/writes).\n"
               "    pci     - Indicates a destination of PCI (access to RSL-type\n"
               "              CSRs from many Octeon blocks, PCI bus operations,\n"
               "              and PCI CSRs).\n"
               "    mio     - Indicates a destination of CIU or GPIO (for CSRs).\n"
               "    all     - Matches all destinations.\n");
	    printf(
	       "<tra_unit(s)> selects the TRA unit(s) for o68.\n"
	       "    It consists of one or more comma-separated numbers (within [0,3]).\n"
	       "    It is ignored for models other than CN68XX.\n");
            printf(
               "Examples:\n"
               "    %s -f \"ldi,all\"\n"
               "        Trace all Icache loads.\n"
               "    %s -f \"stc+stf+stp+stt,pp5+pp4,all\"\n"
               "        Trace all stores from Core 4 and 5.\n"
               "    %s -t 1,3 -f \"ldi,all\"\n"
               "        Trace all Icache loads from cores traced by TRA units 1 and 3.\n"
               "    %s -f \"stc+stf+stp+stt,all,all,0,0xffffffffffff0000\"\n"
               "        Trace all stores to the first 64KB of memory (exception vectors).\n\n", 
               argv[0], argv[0], argv[0], argv[0]);
        }

        printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
        return -1;
    }
    else
        return 0;
}


/**
 * Setup the Octeon debug handler to catch when TRA overflows
 *
 * @return Returns the coremask including all cores that exist
 */
static int setup_debug_handler(void)
{
    int core;
    int number_of_cores;
    int coremask;

    /* Read teh fuse register to get the number of cores */
    number_of_cores = cvmx_octeon_num_cores();
    printf("Found %d cores\n", number_of_cores);
    coremask = (1ull<<number_of_cores)-1;

    /* Install the PCI debug handler and stop the cores so we can program them */
    octeon_remote_stop_cores(coremask);
    usleep(100000);

    /* Make every core stop on a MCD0 signal */
    for (core=0; core<number_of_cores; core++)
    {
        uint64_t multicoredebug = 0;
        multicoredebug |= 3; // Reset MCD signals
        multicoredebug |= 1<<8; // Stop on MCD0 pulse
        multicoredebug |= 1<<12; // Executing SDBBP pulses MCD0
        octeon_remote_write_register(core, 0x100 + 22*8 + 0, multicoredebug);
    }

    /* Start the cores back up */
    octeon_remote_start_cores(coremask);
    return coremask;
}


/**
 * This signal handler is called when Control-C is pressed. It
 * cleanly exits the processing loop below.
 *
 * @param num    Signal received
 */
static void sig_int_handler(int num)
{
    finished = 1;
    if (original_sig_int)
        original_sig_int(num);
}


static void tra_show_headers_text(void)
{
    if (!OCTEON_IS_MODEL(OCTEON_CN3XXX) && !OCTEON_IS_MODEL(OCTEON_CN5XXX))
        printf("%-34s ", "Raw data");
    else
        printf("%-16s ", "Raw data");

    printf("D%10s %-8s %-8s%-2s %-8s %s\n", "Timestamp", "Command", "Source",
        "Msk", "Dest", "Address");

    fflush(NULL);
}


/**
 * Decode a TRA entry into human readable output
 *
 * @param tra_unit is where the data is from (only matters in o68)
 * @param tra_ctl is the TRA_CTL's content under which this trace
 *     buffer is captured.
 * @param data is the trace buffer data to decode
 */
static void tra_decode_text(int tra_unit, cvmx_tra_ctl_t tra_ctl,
    cvmx_tra_data_t data)
{
    cvmx_tra_decode_text_v2(tra_unit, tra_ctl, data);
}

static void tra_log_headers_bin(void)
{
    struct tra_file_header h;
    int tad;

    bzero((void *)&h, sizeof(h));
    h.tfh_prid = octeon_remote_get_model();
#define TFH_TRA_READ_BLOCK(block_name, BLOCK_NAME, tra_unit)		    \
    do									    \
    {									    \
        h.tfh_tra[tra_unit].block_name##_cmd.u64 = octeon_remote_read_csr(  \
	    CVMX_TRAX_##BLOCK_NAME##_CMD(tra_unit));			    \
        h.tfh_tra[tra_unit].block_name##_sid.u64 = octeon_remote_read_csr(  \
	    CVMX_TRAX_##BLOCK_NAME##_SID(tra_unit));			    \
        h.tfh_tra[tra_unit].block_name##_did.u64 = octeon_remote_read_csr(  \
	    CVMX_TRAX_##BLOCK_NAME##_DID(tra_unit));			    \
        h.tfh_tra[tra_unit].block_name##_adr_adr.u64 = 			    \
	    octeon_remote_read_csr(					    \
	        CVMX_TRAX_##BLOCK_NAME##_ADR_ADR(tra_unit));		    \
        h.tfh_tra[tra_unit].block_name##_adr_msk.u64 = 			    \
	    octeon_remote_read_csr(					    \
	        CVMX_TRAX_##BLOCK_NAME##_ADR_MSK(tra_unit));		    \
    } while (0)

#define TFH_TRA_READ_TRA_CFG(tra_unit)					    \
    do									    \
    {									    \
        h.tfh_tra[tra_unit].ctl.u64 = octeon_remote_read_csr(		    \
	    CVMX_TRAX_CTL(tra_unit));					    \
        TFH_TRA_READ_BLOCK(filt, FILT, tra_unit);			    \
        TFH_TRA_READ_BLOCK(trig0, TRIG0, tra_unit);			    \
        TFH_TRA_READ_BLOCK(trig1, TRIG1, tra_unit);			    \
    } while (0)

    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask)
            TFH_TRA_READ_TRA_CFG(tad);

    if (fwrite(&h, sizeof(h), 1, use_output_log) == 0)
        perror("error writing tra header");
    
    return;
}

static void tra_log_data_bin(int tra_unit, cvmx_tra_ctl_t tra_ctl, cvmx_tra_data_t data)
{
    if (OCTEON_IS_MODEL(OCTEON_CN68XX))
        fwrite(&tra_unit, sizeof(int), 1, use_output_log);
    fwrite(&data.u128.data, sizeof(data.u128.data), 1, use_output_log);
    if (!OCTEON_IS_MODEL(OCTEON_CN3XXX) && !OCTEON_IS_MODEL(OCTEON_CN5XXX))
        fwrite(&data.u128.datahi, sizeof(data.u128.datahi), 1, use_output_log);
}

/**
 * Setup the TRA hardware for use
 */
static void tra_hardware_setup(void)
{
    cvmx_tra_ctl_t tra_ctl;
    int tad;

    tra_ctl.u64 = 0;
    tra_ctl.s.ignore_o = use_wrap_mode; /* We interpret wrap as being allowed to overwrite data */
    tra_ctl.s.mcd0_thr = use_mcd0_signal;
    tra_ctl.s.mcd0_trg = 0;
    tra_ctl.s.full_thr = 2; /* 3/4 full */
    tra_ctl.s.time_grn = 1;
    tra_ctl.s.trig_ctl = use_trig_mode;
    tra_ctl.s.wrap = 1; /* We never want the TRA to stop */
    tra_ctl.s.ena = 0;

    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
    {
        if ((1 << tad) & trace_mask) {
            cvmx_tra_setup_v2(tad, tra_ctl, filter_cmd_mask, filter_sid[tad], filter_did, filter_addr, filter_addr_mask);

            cvmx_tra_trig_setup_v2(tad, 0, trig0_cmd_mask, trig0_sid[tad], trig0_did, trig0_addr, trig0_addr_mask);
            cvmx_tra_trig_setup_v2(tad, 1, trig1_cmd_mask, trig1_sid[tad], trig1_did, trig1_addr, trig1_addr_mask);
        }
    }
    /* Note that the TRA CTL must be written before ENA=1. That is why we write
       it here and enable later */
    printf("TRA control = 0x%016llx\n", (U64)tra_ctl.u64);

    printf("TRA Filter setup:              ");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf("       TRA #%d       ", tad);
        }
    printf("\n    CVMX_TRA_FILT_CMD     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_FILT_CMD(tad)));
        }
    printf("\n    CVMX_TRA_FILT_SID     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) { 
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_FILT_SID(tad)));
        }
    printf("\n    CVMX_TRA_FILT_DID     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_FILT_DID(tad)));
        }
    printf("\n    CVMX_TRA_FILT_ADR_ADR =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", filter_addr);
        }
    printf("\n    CVMX_TRA_FILT_ADR_MSK =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", filter_addr_mask);
        }

    printf("\nTRA trig0 setup:              ");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) { 
            printf("       TRA #%d       ", tad);
        }
    printf("\n    CVMX_TRA_TRIG0_CMD     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_TRIG0_CMD(tad)));
        }
    printf("\n    CVMX_TRA_TRIG0_SID     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_TRIG0_SID(tad)));
        }
    printf("\n    CVMX_TRA_TRIG0_DID     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_TRIG0_DID(tad)));
        }
    printf("\n    CVMX_TRA_TRIG0_ADR_ADR =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", trig0_addr);
        }
    printf("\n    CVMX_TRA_TRIG0_ADR_MSK =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", trig0_addr_mask);
        }

    printf("\nTRA trig1 setup:              ");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf("       TRA #%d       ", tad);
        }
    printf("\n    CVMX_TRA_TRIG1_CMD     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_TRIG1_CMD(tad)));
        }
    printf("\n    CVMX_TRA_TRIG1_SID     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_TRIG1_SID(tad)));
        }
    printf("\n    CVMX_TRA_TRIG1_DID     =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", (U64)cvmx_read_csr(CVMX_TRAX_TRIG1_DID(tad)));
        }
    printf("\n    CVMX_TRA_TRIG1_ADR_ADR =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", trig1_addr);
        }
    printf("\n    CVMX_TRA_TRIG1_ADR_MSK =");
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        if ((1 << tad) & trace_mask) {
            printf(" 0x%016llx, ", trig1_addr_mask);
        }	
   printf("\n");

    /* Clear any past interrupts from TRA */
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        octeon_remote_write_csr(CVMX_TRAX_INT_STATUS(tad), 0xf);
}


/**
 * Setup the TRA and loop reading from it
 */
static void tra_hardware_read(void (*log_start)(void),
    void (*log_function)(int tra_unit, cvmx_tra_ctl_t tra_ctl, cvmx_tra_data_t data))
{
    octeon_remote_lock();
    cvmx_tra_ctl_t tra_ctl;
    int tad;

    /* Install the PCI debug handler in case the user enables pulsing MCD0 */
    uint32_t coremask = setup_debug_handler();

    tra_hardware_setup();
    log_start();

    /* Control-C signal handler tells us when to die */
    original_sig_int = signal(SIGINT, sig_int_handler);

    /* Everthing is setup, start TRA */
    for (tad = 0; tad < CVMX_L2C_TADS; tad++)
    {
	if ((1 << tad) & trace_mask) {
            tra_ctl.u64 = octeon_remote_read_csr(CVMX_TRAX_CTL(tad));
            tra_ctl.s.ena = 1;
            octeon_remote_write_csr(CVMX_TRAX_CTL(tad), tra_ctl.u64);
	}
    }

    while (!finished)
    {
        cvmx_tra_data_t data[4];
        int valid = 0;

        for (tad = 0; tad < CVMX_L2C_TADS; tad++)
            if ((1 << tad) & trace_mask) {	
                data[tad] = cvmx_tra_read_v2(tad);
	    }	

        for (tad = 0; tad < CVMX_L2C_TADS; tad++)
        {
            if ((1 << tad) & trace_mask) {
                tra_ctl.u64 = octeon_remote_read_csr(CVMX_TRAX_CTL(tad));
                if ((OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX)) && data[tad].cmn.valid)
	       	    valid = 1;
                else if (data[tad].cmn2.valid)
		    valid = 1;
                else
                    valid = 0;
  
                if (valid) { 
                    log_function(tad, tra_ctl, data[tad]);
                } else {
                       /* No data. See if MCD0 stopped cores and we need to restart them */
                       if (use_mcd0_signal && (octeon_remote_get_running_cores() != coremask)) {
                           //printf("Restarting cores after falling behind\n");
                           /* Clear any past interrupts from TRA */
                           for (tad = 0; tad < CVMX_L2C_TADS; tad++)
                               octeon_remote_write_csr(CVMX_TRAX_INT_STATUS(tad), 0xf);
                           octeon_remote_start_cores(coremask);
                       } else {
                               /* We seem to be keeping up, yield to someone else */
                               usleep(0);
                       }
               }
            }
	} /* End of for loop */
    } /* End of while loop */

    /* We're done, restore the interrupt handler and stop TRA */
    signal(SIGINT, original_sig_int);
    cvmx_tra_enable(0);

    /* Make sure the cores are left running */
    if (use_mcd0_signal)
        octeon_remote_start_cores(coremask);
    octeon_remote_unlock();
    octeon_remote_close();
}

static void tra_file_dump_header(struct tra_file_header *ph)
{
    int i;

    if (IS_TRACE_CN68XX(ph->tfh_prid))
        printf("CN68XX trace");
    else if(IS_TRACE_CN5XXX(ph->tfh_prid) | IS_TRACE_CN3XXX(ph->tfh_prid))
        printf("Octeon/Octeon Plus trace");
    else 
        printf("Octeon II trace");
    printf("(header size = %ld bytes)\n", sizeof(struct tra_file_header));

    if (IS_TRACE_CN68XX(ph->tfh_prid))
        for (i = 0; i < 4; i++)
		printf("    TRA_CTL(%d) = 0x%llx\n", i,
		       (long long)ph->tfh_tra[i].ctl.u64);
    else
	    printf("    TRA_CTL = 0x%llx\n", (long long)ph->tfh_tra[0].ctl.u64);

    return;
}

static void tra_file_read(FILE *input, void (*log_start)(void),
    void (*log_function)(int tra_unit, cvmx_tra_ctl_t tra_ctl, cvmx_tra_data_t data))
{
    struct tra_file_header h;

    if (fread(&h, sizeof(h), 1, input) != 1)
    {
	perror("tra_file_read: reading header");
	return;
    }

#if 1
    tra_file_dump_header(&h);
#endif

    log_start();

    while (!feof(input))
    {
        cvmx_tra_data_t data;
        int tad;

	tad = 0;
	if (IS_TRACE_CN68XX(h.tfh_prid))
            if (fread(&tad, sizeof(int), 1, input) != 1)
	    {
	        perror("tra_file_read: reading tra unit number");
                break;
	    }

        if (fread(&data.u128.data, sizeof(data.u128.data), 1, input) != 1)
	{
	    perror("tra_file_read: reading data");
            break;
        }

	assert (tad < CVMX_L2C_TADS);

        if ((!IS_TRACE_CN3XXX(h.tfh_prid)) && (!IS_TRACE_CN5XXX(h.tfh_prid)))
	{
            if (fread(&data.u128.datahi, sizeof(data.u128.datahi), 1, input) != 1)
	    {
	        perror("tra_file_read: reading datahi");
                break;
	    }
	}

        log_function(tad, h.tfh_tra[tad].ctl, data);
    }

    return;
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
    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if (!octeon_has_feature(OCTEON_FEATURE_TRA))
    {
        printf("This chip does not support the TRA buffer.\n");
        return -1;
    }

    if (!octeon_remote_mem_access_ok())
    {
        octeon_remote_close();
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }

    if (parse_options(argc, argv))
        return -1;

    if (use_input_log)
    {
        tra_file_read(use_input_log, tra_show_headers_text, tra_decode_text);
        fclose(use_input_log);
    }
    else if (use_output_log)
    {
        tra_hardware_read(tra_log_headers_bin, tra_log_data_bin);
        fclose(use_output_log);
    }
    else
        tra_hardware_read(tra_show_headers_text, tra_decode_text);

    return 0;
}
