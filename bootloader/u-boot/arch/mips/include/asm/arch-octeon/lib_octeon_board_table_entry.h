/***********************license start************************************
 * Copyright (c) 2004-2011 Cavium Inc. (support@cavium.com). All rights
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
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
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
 ***********************license end**************************************/
#ifndef __LIB_OCTEON_BOARD_TABLE_ENTRY_H__
#define __LIB_OCTEON_BOARD_TABLE_ENTRY_H__

/* This file defines the structure for Octeon DRAM configurations for a
    specific board. This code is used in a number of places, so care must be
    taken when changing types and structures in this file. Adding to the
    complexity, this file is also parsed by SWIG to generate Lua mappings
    for DRAM configuration. General rules for structures in this file are
        1) Use simple types as much as possible.
        2) Don't include any other files.
        3) Don't use Uboot, CVMX, or BDK specific types.
    Changes to these structures tend to affect all boards ever used with
    Octeon. Don't make fancy changes unless you can prove other boards aren't
    affected. */


/* The following ifdef contains special goo need to allow SWIG to generate Lua
    wrappers for this file. Generally these should not need any changes */
#ifdef SWIG
%module bdk_board_table_entry
%{
#include <stdint.h>
#include "lib_octeon_board_table_entry.i"
/* Goofy macro to redefine LUA_GLOBALSINDEX as a sequence of commands that
    gets the octeon table. This makes SWIG put its stuff in the octeon module
    instead of the global space. LUA_GLOBALSINDEX isn't valid in Lua 5.2 */
#define LUA_GLOBALSINDEX LUA_REGISTRYINDEX); lua_pop(L, 1); lua_pushvalue(L, -1

static int spd_addrs_index(lua_State* L)
{
    int index = luaL_checkint(L, 2);
    luaL_getmetafield(L, 1, "base");
    luaL_getmetafield(L, 1, "size");
    uint16_t *base = lua_touserdata(L, -2);
    int size = luaL_checkint(L, -1);
    lua_pop(L, 2);
    if ((index < 1) || (index > size))
        return luaL_error(L, "Invalid index to spd_addrs");
    lua_pushnumber(L, base[index-1]);
    return 1;
}

static int spd_addrs_newindex(lua_State* L)
{
    int index = luaL_checkint(L, 2);
    int value = luaL_checkint(L, 3);
    luaL_getmetafield(L, 1, "base");
    luaL_getmetafield(L, 1, "size");
    uint16_t *base = lua_touserdata(L, -2);
    int size = luaL_checkint(L, -1);
    lua_pop(L, 2);
    if ((index < 1) || (index > size))
        return luaL_error(L, "Invalid index to spd_addrs");
    base[index-1] = value;
    return 0;
}

static int spd_addrs_len(lua_State* L)
{
    luaL_getmetafield(L, 1, "size");
    return 1;
}

/* This is defined in lib_octeon_shared.h as a enum, but we can't use that
    file directly. This defines the location of the SPD size */
#define DDR3_SPD_BYTES_PROGRAMMED 0

/**
 * This function returns a SPD as a Lua string when someone
 * indexes the spd_ptrs field of the DDR config. If no SPD
 * is assigned to the field, nil is returned. This function
 * is called when spd_ptrs as accessed through a metatable
 * hook assigned in typemap spd_ptrs.
 *
 * @param L      Lua state
 *
 * @return One Lua value, either nil or a binary string.
 */
static int spd_ptrs_index(lua_State* L)
{
    int index = luaL_checkint(L, 2);
    luaL_getmetafield(L, 1, "base");
    luaL_getmetafield(L, 1, "size");
    uint8_t **base = lua_touserdata(L, -2);
    int size = luaL_checkint(L, -1);
    lua_pop(L, 2);
    if ((index < 1) || (index > size))
        return luaL_error(L, "Invalid index to spd_ptrs");
    if (base[index-1])
    {
        uint8_t *spd = base[index-1];
        uint8_t size_enc = spd[DDR3_SPD_BYTES_PROGRAMMED];
        size_t spd_length;
        switch (size_enc & 0xf)
        {
            case 1:
                spd_length = 128;
                break;
            case 2:
                spd_length = 176;
                break;
            case 3:
                spd_length = 256;
                break;
            default:
                return luaL_error(L, "Invalid SPD size encoding");
        }
        lua_pushlstring(L, (const char *)spd, spd_length);
    }
    else
        lua_pushnil(L);
    return 1;
}

/**
 * This function assigns a SPD as a Lua string when someone
 * indexes the spd_ptrs field of the DDR config. The string
 * length must match the length encoded in byte
 * DDR3_SPD_BYTES_PROGRAMMED of the SPD. This function
 * is called when spd_ptrs as accessed through a metatable
 * hook assigned in typemap spd_ptrs.
 *
 * @param L      Lua state
 *
 * @return Nothing
 */
static int spd_ptrs_newindex(lua_State* L)
{
    int index = luaL_checkint(L, 2);
    size_t value_len = 0;
    const char *value = NULL;
    if (!lua_isnil(L, 3))
        value = luaL_tolstring(L, 3, &value_len);
    luaL_getmetafield(L, 1, "base");
    luaL_getmetafield(L, 1, "size");
    uint8_t **base = lua_touserdata(L, -2);
    int size = luaL_checkint(L, -1);
    lua_pop(L, 2);
    if ((index < 1) || (index > size))
        return luaL_error(L, "Invalid index to spd_ptrs");
    if (value)
    {
        uint8_t size_enc = value[DDR3_SPD_BYTES_PROGRAMMED];
        size_t spd_length;
        switch (size_enc & 0xf)
        {
            case 1:
                spd_length = 128;
                break;
            case 2:
                spd_length = 176;
                break;
            case 3:
                spd_length = 256;
                break;
            default:
                return luaL_error(L, "Invalid SPD size encoding");
        }
        if (value_len != spd_length)
            return luaL_error(L, "Length of string doesn't match SPD length");
        void *spd = malloc(value_len);
        if (!spd)
            return luaL_error(L, "Out of memory allocating SPD");
        memcpy(spd, value, value_len);
        if (base[index-1])
            free(base[index-1]);
        base[index-1] = spd;
    }
    else
    {
        if (base[index-1])
            free(base[index-1]);
        base[index-1] = NULL;
    }
    return 0;
}

/**
 * This function returns the length of the spd_ptrs array.
 *
 * @param L      Lua state
 *
 * @return Number representing the array length
 */
static int spd_ptrs_len(lua_State* L)
{
    luaL_getmetafield(L, 1, "size");
    return 1;
}

%}

typedef unsigned long   uint64_t;
typedef long            int64_t;
typedef unsigned int    uint32_t;
typedef int             int32_t;
typedef unsigned short  uint16_t;
typedef short           int16_t;
typedef unsigned char   uint8_t;
typedef char            int8_t;

%typemap(out) array_index [ANY] {
  int i;
  lua_createtable(L, $1_dim0, 0);
  for (i = 0; i < $1_dim0; i++) {
      lua_pushnumber(L, i+1);
      SWIG_NewPointerObj(L, &$1[i], $1_descriptor, 0);
      lua_settable(L, -3);
  }
  SWIG_arg++;
};

%apply array_index [ANY] {dimm_config_t [ANY], dimm_odt_config_t [ANY], ddr_configuration_t [ANY], ddr_config_table_t [ANY]};

%typemap(out) uint16_t spd_addrs[ANY] {
    lua_newtable(L);    /* Our return value, a table with meta methods */
    lua_newtable(L);    /* This will be the meta table for the above table */
    /* Store the base address of the array in the meta table */
    lua_pushlightuserdata(L, $1);
    lua_setfield(L, -2, "base");
    /* Store the length of the array in the meta table */
    lua_pushnumber(L, $1_dim0);
    lua_setfield(L, -2, "size");
    /* Add function hook when getting stuff */
    lua_pushcfunction(L, spd_addrs_index);
    lua_setfield(L, -2, "__index");
    /* Add function hook when setting stuff */
    lua_pushcfunction(L, spd_addrs_newindex);
    lua_setfield(L, -2, "__newindex");
    /* Add function hook for length */
    lua_pushcfunction(L, spd_addrs_len);
    lua_setfield(L, -2, "__len");
    /* Assign the meta table */
    lua_setmetatable(L, -2);
    /* Return a single element */
    SWIG_arg++;
};

%typemap(out) uint8_t *spd_ptrs[ANY] {
    lua_newtable(L);    /* Our return value, a table with meta methods */
    lua_newtable(L);    /* This will be the meta table for the above table */
    /* Store the base address of the array in the meta table */
    lua_pushlightuserdata(L, $1);
    lua_setfield(L, -2, "base");
    /* Store the length of the array in the meta table */
    lua_pushnumber(L, $1_dim0);
    lua_setfield(L, -2, "size");
    /* Add function hook when getting stuff */
    lua_pushcfunction(L, spd_ptrs_index);
    lua_setfield(L, -2, "__index");
    /* Add function hook when setting stuff */
    lua_pushcfunction(L, spd_ptrs_newindex);
    lua_setfield(L, -2, "__newindex");
    /* Add function hook for length */
    lua_pushcfunction(L, spd_ptrs_len);
    lua_setfield(L, -2, "__len");
    /* Assign the meta table */
    lua_setmetatable(L, -2);
    /* Return a single element */
    SWIG_arg++;
};

#endif



/* Structure that provides DIMM information, either in the form of an SPD TWSI address,
** or a pointer to an array that contains SPD data.  One of the two fields must be valid. */
typedef struct {
    uint16_t spd_addrs[2];  /* TWSI address of SPD, 0 if not used */
    uint8_t *spd_ptrs[2];   /* pointer to SPD data array, NULL if not used */
} dimm_config_t;

typedef struct {
        uint8_t odt_ena;            /* FIX: dqx_ctl for Octeon 3 DDR4 */
        uint64_t odt_mask;          /* FIX: wodt_mask for Octeon 3 */
#if defined(SWIG) || defined(SWIGLUA)
        uint64_t modereg_params1;
        uint64_t modereg_params2;
#else
        cvmx_lmcx_modereg_params1_t modereg_params1;
        cvmx_lmcx_modereg_params2_t modereg_params2;
#endif
        uint8_t qs_dic;             /* FIX: rodt_ctl for Octeon 3 */
        uint64_t rodt_ctl;          /* FIX: rodt_mask for Octeon 3 */
        uint8_t dic;
} dimm_odt_config_t;

typedef struct {
    uint32_t ddr_board_delay;
    uint8_t lmc_delay_clk;
    uint8_t lmc_delay_cmd;
    uint8_t lmc_delay_dq;
} ddr_delay_config_t;

/*
  The parameters below make up the custom_lmc_config data structure.
  This structure is used to customize the way that the LMC DRAM
  Controller is configured for a particular board design.

  The HRM describes LMC Read Leveling which supports automatic
  selection of per byte-lane delays.  When measuring the read delays
  the LMC configuration software sweeps through a range of settings
  for LMC0_COMP_CTL2[RODT_CTL], the Octeon II on-die-termination
  resistance and LMC0_MODEREG_PARAMS1[RTT_NOM_XX], the DRAM
  on-die-termination resistance.  The minimum and maximum parameters
  for rtt_nom_idx and rodt_ctl listed below determine the ranges of
  ODT settings used for the measurements.  Note that for rtt_nom an
  index is used into a sorted table rather than the direct csr setting
  in order to optimize the sweep.

   .min_rtt_nom_idx: 1=120ohms, 2=60ohms, 3=40ohms, 4=30ohms, 5=20ohms
   .max_rtt_nom_idx: 1=120ohms, 2=60ohms, 3=40ohms, 4=30ohms, 5=20ohms
   .min_rodt_ctl: 1=20ohms, 2=30ohms, 3=40ohms, 4=60ohms, 5=120ohms
   .max_rodt_ctl: 1=20ohms, 2=30ohms, 3=40ohms, 4=60ohms, 5=120ohms

   The settings below control the Octeon II drive strength for the CK,
   ADD/CMD, and DQ/DQS signals.  1=24ohms, 2=26.67ohms, 3=30ohms,
   4=34.3ohms, 5=40ohms, 6=48ohms, 6=60ohms.

   .dqx_ctl: Drive strength control for DDR_DQX/DDR_DQS_X_P/N drivers.
   .ck_ctl: Drive strength control for DDR_CK_X_P/DDR_DIMMX_CSX_L/DDR_DIMMX_ODT_X drivers.
   .cmd_ctl: Drive strength control for CMD/A/RESET_L/CKEX drivers.

   The LMC controller software selects the most optimal CAS Latency
   that complies with the appropriate SPD values and the frequency
   that the DRAMS are being operated.  When operating the DRAMs at
   frequencies substantially lower than their rated frequencies it
   might be necessary to limit the minimum CAS Latency the LMC
   controller software is allowed to select in order to make the DRAM
   work reliably.

   .min_cas_latency: Minimum allowed CAS Latency


   The value used for LMC0_RLEVEL_CTL[OFFSET_EN] determine how the
   read-leveling information that the Octeon II gathers is interpreted
   to determine the per-byte read delays.

   .offset_en: Value used for LMC0_RLEVEL_CTL[OFFSET_EN].
   .offset_udimm: Value used for LMC0_RLEVEL_CTL[OFFSET] for UDIMMS.
   .offset_rdimm: Value used for LMC0_RLEVEL_CTL[OFFSET] for RDIMMS.


   The LMC configuration software sweeps through a range of ODT
   settings while measuring the per-byte read delays.  During those
   measurements the software makes an assessment of the quality of the
   measurements in order to determine which measurements provide the
   most accurate delays.  The automatic settings provide the option to
   allow that same assessment to determine the most optimal RODT_CTL
   and/or RTT_NOM settings.

   The automatic approach might provide the best means to determine
   the settings used for initial poweron of a new design.  However,
   the final settings should be determined by board analysis, testing,
   and experience.

   .ddr_rtt_nom_auto: 1 means automatically set RTT_NOM value.
   .ddr_rodt_ctl_auto: 1 means automatically set RODT_CTL value.

   .rlevel_compute: Enables software interpretation of per-byte read
                    delays using the measurements collected by the
                    Octeon II rather than completely relying on the
                    Octeon II to determine the delays.  1=software
                    computation is recomended since a more complete
                    analysis is implemented in software.

   .rlevel_comp_offset: Set to 2 unless instructed differently by Cavium.

   .rlevel_average_loops: Determines the number of times the read-leveling
   			  sequence is run for each rank.  The results is
                          then averaged across the number of loops. The
                          default setting is 1.

   .ddr2t_udimm:
   .ddr2t_rdimm: Turn on the DDR 2T mode. 2-cycle window for CMD and
                 address. This mode helps relieve setup time pressure
                 on the address and command bus. Please refer to
                 Micron's tech note tn_47_01 titled DDR2-533 Memory
                 Design Guide for Two Dimm Unbuffered Systems for
                 physical details.

   .disable_sequential_delay_check: As result of the flyby topology
   	prescribed in the JEDEC specifications the byte delays should
   	maintain a consistent increasing or decreasing trend across
   	the bytes on standard dimms.  This setting can be used disable
   	that check for unusual circumstances where the check is not
   	useful.

   .maximum_adjacent_rlevel_delay_increment: An additional sequential
  	delay check for the delays that result from the flyby
  	topology. This value specifies the maximum difference between
  	the delays of adjacent bytes.  A value of 0 disables this
  	check.

    .fprch2 Front Porch Enable: When set, the turn-off
        time for the default DDR_DQ/DQS drivers is FPRCH2 CKs earlier.
        00 = 0 CKs
        01 = 1 CKs
        10 = 2 CKs

   .parity: The parity input signal PAR_IN on each dimm must be
            strapped high or low on the board.  This bit is programmed
            into LMC0_DIMM_CTL[PARITY] and it must be set to match the
            board strapping.  This signal is typically strapped low.

    .mode32b: Enable 32-bit datapath mode.  Set to 1 if only 32 DQ pins
    	      are used. (cn61xx, cn71xx)

    .dll_write_offset: FIXME: Add description
    .dll_read_offset:  FIXME: Add description
  */

typedef struct {
    const char part[20];
    int speed;
    uint64_t rlevel_rank[4][4];
} rlevel_table_t;

typedef struct {
    uint8_t min_rtt_nom_idx;
    uint8_t max_rtt_nom_idx;
    uint8_t min_rodt_ctl;
    uint8_t max_rodt_ctl;
    uint8_t dqx_ctl;
    uint8_t ck_ctl;
    uint8_t cmd_ctl;
    uint8_t ctl_ctl;
    uint8_t min_cas_latency;
    uint8_t offset_en;
    uint8_t offset_udimm;
    uint8_t offset_rdimm;
    uint8_t rlevel_compute;
    uint8_t ddr_rtt_nom_auto;
    uint8_t ddr_rodt_ctl_auto;
    uint8_t rlevel_comp_offset_udimm;
    uint8_t rlevel_comp_offset_rdimm;
    int8_t  ptune_offset;
    int8_t  ntune_offset;
    uint8_t rlevel_average_loops;
    uint8_t ddr2t_udimm;
    uint8_t ddr2t_rdimm;
    uint8_t disable_sequential_delay_check;
    uint8_t maximum_adjacent_rlevel_delay_increment;
    uint8_t parity;
    uint8_t fprch2;
    uint8_t mode32b;
    const int8_t *dll_write_offset;
    const int8_t *dll_read_offset;
    int8_t measured_vref;
    rlevel_table_t *rlevel_table;    // FIXME!!! pointer to table
} ddr3_custom_config_t;

#define DDR_CFG_T_MAX_DIMMS     5
typedef struct ddr_configuration {
    dimm_config_t dimm_config_table[DDR_CFG_T_MAX_DIMMS];
    dimm_odt_config_t odt_1rank_config[4];
    dimm_odt_config_t odt_2rank_config[4];
    dimm_odt_config_t odt_4rank_config[4];
    ddr_delay_config_t unbuffered;
    ddr_delay_config_t registered;
    ddr3_custom_config_t custom_lmc_config;
} ddr_configuration_t;


/* Structures/defines for organizing the DDR configuration parameters
** for all boards.  These are used by both u-boot and oct-remote-boot to
** look up the DDR parameters.
*/

/* Maximum number of configurations per board. Some boards support different chips,
** and each chip needs a different configuration.  Others need different configurations
** based on revision.  Revisions MUST be listed in ascending order (configs for higher
** revisions must have higher indexes in the array.)
*/
#define MAX_DDR_CONFIGS_PER_BOARD    2
typedef struct
{
    uint32_t chip_model;  /* OCTEON model that this config is for */
    /* Maximum board revision level that this configuration supports. */
    uint8_t max_maj_rev; /* 0 means all revs */
    uint8_t max_min_rev;
    ddr_configuration_t ddr_config[4];  /* One config for each interface */
} ddr_config_table_t;

/* Entry for the board table.  This table has an entry for each board,
** and allows the configuration to be looked up from the table.  The table
** is populated with static initializers, and is shared between u-boot and
** oct-remote-boot.
** Some fields are of interest to only u-boot or oct-remote-boot.
*/
typedef struct
{
    int board_type;
    ddr_config_table_t chip_ddr_config[MAX_DDR_CONFIGS_PER_BOARD];
    int default_ddr_clock_hz;
    int default_ddr_ref_hz;
    int default_cpu_ref_hz;
    int eeprom_addr;
    char bootloader_name[100];   /* Only used if default needs to be overridden */
} board_table_entry_t;


/* The following functions are called from Lua so must be defined in this
    file. These functions are not generally callable for normal C code and
    should not be used */
#if defined(SWIG) || defined(SWIGLUA)
ddr_config_table_t *bdk_dram_lookup_board(const char *board_name);
int bdk_dram_config_raw(int node, const ddr_config_table_t *ddr_config_table, int ddr_clock_hertz);
#endif

#endif  /*  __LIB_OCTEON_BOARD_TABLE_ENTRY_H__  */
