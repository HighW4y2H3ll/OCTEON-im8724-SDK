/***********************license start************************************
 * Copyright (c) 2012 - 2016 Cavium Inc. (support@cavium.com).  All rights
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
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#ifdef DEBUG
#include <command.h>
#endif
#include <mmc.h>
#include <part.h>
#include <malloc.h>
#include <errno.h>
#include <asm/arch/octeon_mmc.h>
#include <asm/io.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/octeon-feature.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/cvmx-mio-defs.h>
#include <asm/arch/octeon_board_mmc.h>
#include <asm/arch/octeon-model.h>
#include <linux/list.h>
#include <div64.h>
#include <watchdog.h>
#include <asm/gpio.h>

/* Enable support for SD as well as MMC */
#define CONFIG_OCTEON_MMC_SD

#define POWER_ON_TIME		40	/* See SD 4.1 spec figure 6-5 */
/*
 * Due to how MMC is implemented in the OCTEON processor it is not
 * possible to use the generic MMC support.  However, this code
 * implements all of the MMC support found in the generic MMC driver
 * and should be compatible with it for the most part.
 *
 * Currently both MMC and SD/SDHC are supported.
 */

/**
 * Timeout used when waiting for commands to complete.  We need to keep this
 * above the hardware watchdog timeout which is usually limited to 1000ms
 */
#define WATCHDOG_COUNT		(1100)	/* in msecs */

/**
 * Long timeout for commands which might take a while to complete.
 */
#define MMC_TIMEOUT_LONG	1000

/**
 * Erase timeout
 */
#define MMC_TIMEOUT_ERASE	0

/**
 * Short timeout used for most commands in msecs
 */
#define MMC_TIMEOUT_SHORT	10

#ifndef CONFIG_OCTEON_MAX_MMC_SLOT
# define CONFIG_OCTEON_MAX_MMC_SLOT		4
#endif

#ifndef CONFIG_OCTEON_MMC_MIN_BUS_SPEED_HZ
# define CONFIG_OCTEON_MMC_MIN_BUS_SPEED_HZ		400000
#endif

#ifndef CONFIG_OCTEON_MMC_SD
# undef IS_SD
# define IS_SD(x) (0)
#endif

#ifndef CONFIG_SYS_MMC_MAX_BLK_COUNT
# define CONFIG_SYS_MMC_MAX_BLK_COUNT	8191
#elif CONFIG_SYS_MMC_MAX_BLK_COUNT > 8191
# error CONFIG_SYS_MMC_MAX_BLK_COUNT is too large! Must not exceed 8191!
#endif

#define MMC_CMD_FLAG_CTYPE_XOR(x)	(((x) & 3) << 16)
#define MMC_CMD_FLAG_RTYPE_XOR(x)	(((x) & 7) << 20)
#define MMC_CMD_FLAG_OFFSET(x)		(((x) & 0x3f) << 24)
#define MMC_CMD_FLAG_IGNORE_CRC_ERR	(1 << 30)
#define MMC_CMD_FLAG_STRIP_CRC		(1 << 31)

DECLARE_GLOBAL_DATA_PTR;

static LIST_HEAD(mmc_devices);

static int cur_dev_num = -1;
static int init_time = 1;

static int last_bus_id[CVMX_MAX_NODES];

int mmc_set_blocklen(struct mmc *mmc, int len);

static int mmc_send_cmd_timeout(struct mmc *mmc, struct mmc_cmd *cmd,
				struct mmc_data *data, uint32_t flags,
				uint timeout);

static int mmc_send_cmd_flags(struct mmc *mmc, struct mmc_cmd *cmd,
			      struct mmc_data *data, uint32_t flags);

static int mmc_send_acmd(struct mmc *mmc, struct mmc_cmd *cmd,
			 struct mmc_data *data, uint32_t flags);

static void mmc_set_ios(struct mmc *mmc);

static void mmc_switch_dev(struct mmc *mmc);

/**
 * This is the external mmc_send_cmd function.  It was required that
 * the internal version support flags so this version is required.
 */
static int octeon_mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
			       struct mmc_data *data);

#ifdef CONFIG_OCTEON_MMC_SD
static int sd_set_ios(struct mmc *mmc);
#endif

static const struct mmc_ops octeon_mmc_ops = {
	.send_cmd = octeon_mmc_send_cmd,
	.set_ios = mmc_set_ios,
	.init = NULL,
	.getcd = mmc_getcd,
	.getwp = mmc_getwp,
};

#ifdef DEBUG
const char *mmc_reg_str(uint64_t reg)
{
	switch (reg) {
	case CVMX_MIO_NDF_DMA_CFG:	return "MIO_NDF_DMA_CFG";
	case CVMX_MIO_NDF_DMA_INT:	return "MIO_NDF_DMA_INT";
	case CVMX_MIO_NDF_DMA_INT_EN:	return "MIO_NDF_DMA_INT_EN";
	case CVMX_MIO_EMM_DMA_CFG:	return "MIO_EMM_DMA_CFG";
	case CVMX_MIO_EMM_DMA_ADR:	return "MIO_EMM_DMA_ADR";
	case CVMX_MIO_EMM_DMA_INT:	return "MIO_EMM_DMA_INT";
	case CVMX_MIO_EMM_CFG:		return "MIO_EMM_CFG";
	case CVMX_MIO_EMM_MODEX(0):	return "MIO_EMM_MODE0";
	case CVMX_MIO_EMM_MODEX(1):	return "MIO_EMM_MODE1";
	case CVMX_MIO_EMM_MODEX(2):	return "MIO_EMM_MODE2";
	case CVMX_MIO_EMM_MODEX(3):	return "MIO_EMM_MODE3";
	case CVMX_MIO_EMM_SWITCH:	return "MIO_EMM_SWITCH";
	case CVMX_MIO_EMM_DMA:		return "MIO_EMM_DMA";
	case CVMX_MIO_EMM_CMD:		return "MIO_EMM_CMD";
	case CVMX_MIO_EMM_RSP_STS:	return "MIO_EMM_RSP_STS";
	case CVMX_MIO_EMM_RSP_LO:	return "MIO_EMM_RSP_LO";
	case CVMX_MIO_EMM_RSP_HI:	return "MIO_EMM_RSP_HI";
	case CVMX_MIO_EMM_INT:		return "MIO_EMM_INT";
	case CVMX_MIO_EMM_INT_EN:	return "MIO_EMM_INT_EN";
	case CVMX_MIO_EMM_WDOG:		return "MIO_EMM_WDOG";
	case CVMX_MIO_EMM_SAMPLE:	return "MIO_EMM_SAMPLE";
	case CVMX_MIO_EMM_STS_MASK:	return "MIO_EMM_STS_MASK";
	case CVMX_MIO_EMM_RCA:		return "MIO_EMM_RCA";
	case CVMX_MIO_EMM_BUF_IDX:	return "MIO_EMM_BUF_IDX";
	case CVMX_MIO_EMM_BUF_DAT:	return "MIO_EMM_BUF_DAT";
	default:			return "UNKNOWN";
	}
}
#endif

/**
 * Obtains a field from the multi-word CSD register
 *
 * @param[in]	mmc	MMC data structure
 * @param	start	starting bit offset [0-127]
 * @param	end	ending bit offset [0-127]
 *
 * @return	field extracted from the CSD register
 */
static uint32_t get_csd_bits(const struct mmc *mmc, unsigned start,
			     unsigned end)
{
	const uint32_t *csd = mmc->csd;
	int start_off = 3 - start / 32;
	int end_off = 3 - end / 32;
	int start_bit = start & 0x1f;
	int end_bit = end & 0x1f;
	uint32_t val;
	uint32_t mask;
	if (start_off == end_off) {
		val = csd[start_off] >> start_bit;
		mask = (1 << (end - start + 1)) - 1;
		val &= mask;
	} else {
		mask = (1ULL << (end_bit + 1)) - 1;
		val = (csd[end_off] & mask) << (32 - start_bit);
		mask = (1 << (32 - start_bit)) - 1;
		val |= (csd[start_off] >> start_bit) & mask;
	}
	return val;
}

static inline void mmc_write_csr(const struct mmc *mmc, uint64_t reg,
				 uint64_t value)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
#ifdef DEBUG_CSR
	printf("        %s: %s(%d) <= 0x%llx\n", __func__, mmc_reg_str(reg),
	       host->node, value);
#endif
	cvmx_write_csr_node(host->node, reg, value);
	cvmx_read_csr_node(host->node, reg);
}

static inline uint64_t mmc_read_csr(const struct mmc *mmc, uint64_t reg)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	uint64_t value = cvmx_read_csr_node(host->node, reg);
#ifdef DEBUG_CSR
	printf("        %s: %s(%d) => 0x%llx\n", __func__, mmc_reg_str(reg),
	       host->node, value);
#endif
	return value;
}
#ifdef DEBUG
static void mmc_print_status(uint32_t status)
{
#ifdef DEBUG_STATUS
	static const char *state[] = {
		"Idle",		/* 0 */
		"Ready",	/* 1 */
		"Ident",	/* 2 */
		"Standby",	/* 3 */
		"Tran",		/* 4 */
		"Data",		/* 5 */
		"Receive",	/* 6 */
		"Program",	/* 7 */
		"Dis",		/* 8 */
		"Btst",		/* 9 */
		"Sleep",	/* 10 */
		"reserved",	/* 11 */
		"reserved",	/* 12 */
		"reserved",	/* 13 */
		"reserved",	/* 14 */
		"reserved"	/* 15 */ };
	if (status & R1_APP_CMD)
		puts("MMC ACMD\n");
	if (status & R1_SWITCH_ERROR)
		puts("MMC switch error\n");
	if (status & R1_READY_FOR_DATA)
		puts("MMC ready for data\n");
	printf("MMC %s state\n", state[R1_CURRENT_STATE(status)]);
	if (status & R1_ERASE_RESET)
		puts("MMC erase reset\n");
	if (status & R1_WP_ERASE_SKIP)
		puts("MMC partial erase due to write protected blocks\n");
	if (status & R1_CID_CSD_OVERWRITE)
		puts("MMC CID/CSD overwrite error\n");
	if (status & R1_ERROR)
		puts("MMC undefined device error\n");
	if (status & R1_CC_ERROR)
		puts("MMC device error\n");
	if (status & R1_CARD_ECC_FAILED)
		puts("MMC internal ECC failed to correct data\n");
	if (status & R1_ILLEGAL_COMMAND)
		puts("MMC illegal command\n");
	if (status & R1_COM_CRC_ERROR)
		puts("MMC CRC of previous command failed\n");
	if (status & R1_LOCK_UNLOCK_FAILED)
		puts("MMC sequence or password error in lock/unlock device command\n");
	if (status & R1_CARD_IS_LOCKED)
		puts("MMC device locked by host\n");
	if (status & R1_WP_VIOLATION)
		puts("MMC attempt to program write protected block\n");
	if (status & R1_ERASE_PARAM)
		puts("MMC invalid selection of erase groups for erase\n");
	if (status & R1_ERASE_SEQ_ERROR)
		puts("MMC error in sequence of erase commands\n");
	if (status & R1_BLOCK_LEN_ERROR)
		puts("MMC block length error\n");
	if (status & R1_ADDRESS_ERROR)
		puts("MMC address misalign error\n");
	if (status & R1_OUT_OF_RANGE)
		puts("MMC address out of range\n");
#endif
}
#endif

#ifdef CONFIG_PARTITIONS
block_dev_desc_t *mmc_get_dev(int dev)
{
	struct mmc *mmc = find_mmc_device(dev);

	if (mmc)
		debug("%s: Found mmc device %d\n", __func__, dev);
	else
		debug("%s: mmc device %d not found\n", __func__, dev);

	if (!mmc || mmc_init(mmc))
		return NULL;
	return &mmc->block_dev;
}
#endif

static void mmc_print_registers(struct mmc *mmc)
{
#ifdef DEBUG_REGISTERS
	struct mmc_host *host = NULL;
	int bus;
	cvmx_mio_ndf_dma_cfg_t ndf_dma_cfg;
	cvmx_mio_ndf_dma_int_t ndf_dma_int;
	cvmx_mio_emm_dma_cfg_t emm_dma_cfg;
	cvmx_mio_emm_dma_adr_t emm_dma_adr;
	cvmx_mio_emm_dma_int_t emm_dma_int;
	cvmx_mio_emm_cfg_t emm_cfg;
	cvmx_mio_emm_modex_t emm_mode;
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_dma_t emm_dma;
	cvmx_mio_emm_cmd_t emm_cmd;
	cvmx_mio_emm_rsp_sts_t emm_rsp_sts;
	cvmx_mio_emm_rsp_lo_t emm_rsp_lo;
	cvmx_mio_emm_rsp_hi_t emm_rsp_hi;
	cvmx_mio_emm_int_t emm_int;
	cvmx_mio_emm_wdog_t emm_wdog;
	cvmx_mio_emm_sample_t emm_sample;
	cvmx_mio_emm_sts_mask_t emm_sts_mask;
	cvmx_mio_emm_rca_t emm_rca;

	static const char *bus_width_str[] = {
		"1-bit data bus (power on)",
		"4-bit data bus",
		"8-bit data bus",
		"reserved (3)",
		"reserved (4)",
		"4-bit data bus (dual data rate)",
		"8-bit data bus (dual data rate)",
		"reserved (7)",
		"reserved (8)",
		"invalid (9)",
		"invalid (10)",
		"invalid (11)",
		"invalid (12)",
		"invalid (13)",
		"invalid (14)",
		"invalid (15)",
	};

	static const char *ctype_xor_str[] = {
		"No data",
		"Read data into Dbuf",
		"Write data from Dbuf",
		"Reserved"
	};

	static const char *rtype_xor_str[] = {
		"No response",
		"R1, 48 bits",
		"R2, 136 bits",
		"R3, 48 bits",
		"R4, 48 bits",
		"R5, 48 bits",
		"Reserved 6",
		"Reserved 7"
	};

	if (!mmc) {
		printf("%s: Error: MMC data structure required\n", __func__);
		return;
	}

	host = (struct mmc_host *)mmc->priv;
	printf("%s: bus id: %u\n", __func__, host->bus_id);

	if (host->use_ndf) {
		ndf_dma_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_CFG);
		printf("MIO_NDF_DMA_CFG:                0x%016llx\n", ndf_dma_cfg.u64);
		printf("    63:    en:                  %s\n", ndf_dma_cfg.s.en ? "enabled" : "disabled");
		printf("    62:    rw:                  %s\n", ndf_dma_cfg.s.rw ? "write" : "read");
		printf("    61:    clr:                 %s\n", ndf_dma_cfg.s.clr ? "clear" : "not clear");
		printf("    59:    swap32:              %s\n", ndf_dma_cfg.s.swap32 ? "yes" : "no");
		printf("    58:    swap16:              %s\n", ndf_dma_cfg.s.swap16 ? "yes" : "no");
		printf("    57:    swap8:               %s\n", ndf_dma_cfg.s.swap8 ? "yes" : "no");
		printf("    56:    endian:              %s\n", ndf_dma_cfg.s.endian ? "little" : "big");
		printf("    36-55: size:                %u\n", ndf_dma_cfg.s.size);
		printf("    0-35:  adr:                 0x%llx\n", (uint64_t)ndf_dma_cfg.s.adr);

		ndf_dma_int.u64 = mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_INT);
		printf("\nMIO_NDF_DMA_INT:              0x%016llx\n", ndf_dma_int.u64);
		printf("    0:     Done:    %s\n", ndf_dma_int.s.done ? "yes" : "no");

		emm_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CFG);
		printf("\nMIO_EMM_CFG:                  0x%016llx\n", emm_cfg.u64);
		printf("    16:    boot_fail:           %s\n", emm_cfg.s.boot_fail ? "yes" : "no");
		printf("    3:     bus_ena3:            %s\n", emm_cfg.s.bus_ena & 0x08 ? "yes" : "no");
		printf("    2:     bus_ena2:            %s\n", emm_cfg.s.bus_ena & 0x04 ? "yes" : "no");
		printf("    1:     bus_ena1:            %s\n", emm_cfg.s.bus_ena & 0x02 ? "yes" : "no");
		printf("    0:     bus_ena0:            %s\n", emm_cfg.s.bus_ena & 0x01 ? "yes" : "no");
	} else {
		emm_dma_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA_CFG);
		printf("MIO_EMM_DMA_CFG:                0x%016llx\n", emm_dma_cfg.u64);
		printf("    63:    en:                  %s\n", emm_dma_cfg.s.en ? "enabled" : "disabled");
		printf("    62:    rw:                  %s\n", emm_dma_cfg.s.rw ? "write" : "read");
		printf("    61:    clr:                 %s\n", emm_dma_cfg.s.clr ? "clear" : "not clear");
		printf("    59:    swap32:              %s\n", emm_dma_cfg.s.swap32 ? "yes" : "no");
		printf("    58:    swap16:              %s\n", emm_dma_cfg.s.swap16 ? "yes" : "no");
		printf("    57:    swap8:               %s\n", emm_dma_cfg.s.swap8 ? "yes" : "no");
		printf("    56:    endian:              %s\n", emm_dma_cfg.s.endian ? "little" : "big");
		printf("    36-55: size:                %u\n", emm_dma_cfg.s.size);

		emm_dma_adr.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA_ADR);
		printf("MIO_EMM_DMA_ADR:        0x%016llx\n", emm_dma_adr.u64);
		printf("    0-41:  adr:                 0x%llx\n", emm_dma_adr.s.adr);

		emm_dma_int.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA_INT);
		printf("\nMIO_EMM_DMA_INT:              0x%016llx\n", emm_dma_int.u64);
		printf("    1:     FIFO:                %s\n", emm_dma_int.s.fifo ? "yes" : "no");
		printf("    0:     Done:                %s\n", emm_dma_int.s.done ? "yes" : "no");

		emm_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CFG);
		printf("\nMIO_EMM_CFG:                  0x%016llx\n", emm_cfg.u64);
		printf("    16:    boot_fail:           %s\n", emm_cfg.s.boot_fail ? "yes" : "no");
		printf("    3:     bus_ena3:            %s\n", emm_cfg.s.bus_ena & 0x08 ? "yes" : "no");
		printf("    2:     bus_ena2:            %s\n", emm_cfg.s.bus_ena & 0x04 ? "yes" : "no");
		printf("    1:     bus_ena1:            %s\n", emm_cfg.s.bus_ena & 0x02 ? "yes" : "no");
		printf("    0:     bus_ena0:            %s\n", emm_cfg.s.bus_ena & 0x01 ? "yes" : "no");
	}
	for (bus = 0; bus < 4; bus++) {
		emm_mode.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_MODEX(bus));
		printf("\nMIO_EMM_MODE%u:               0x%016llx\n", bus, emm_mode.u64);
		printf("    48:    hs_timing:           %s\n", emm_mode.s.hs_timing ? "yes" : "no");
		printf("    40-42: bus_width:           %s\n", bus_width_str[emm_mode.s.bus_width]);
		printf("    32-35: power_class          %u\n", emm_mode.s.power_class);
		printf("    16-31: clk_hi:              %u\n", emm_mode.s.clk_hi);
		printf("    0-15:  clk_lo:              %u\n", emm_mode.s.clk_lo);
	}

	emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
	printf("\nMIO_EMM_SWITCH:               0x%016llx\n", emm_switch.u64);
	printf("    60-61: tbus_id:             %u\n", emm_switch.s.bus_id);
	printf("    59:    switch_exe:          %s\n", emm_switch.s.switch_exe ? "yes" : "no");
	printf("    58:    switch_err0:         %s\n", emm_switch.s.switch_err0 ? "yes" : "no");
	printf("    57:    switch_err1:         %s\n", emm_switch.s.switch_err1 ? "yes" : "no");
	printf("    56:    switch_err2:         %s\n", emm_switch.s.switch_err2 ? "yes" : "no");
	printf("    48:    hs_timing:           %s\n", emm_switch.s.hs_timing ? "yes" : "no");
	printf("    42-40: bus_width:           %s\n", bus_width_str[emm_switch.s.bus_width]);
	printf("    32-35: power_class:         %u\n", emm_switch.s.power_class);
	printf("    16-31: clk_hi:              %u\n", emm_switch.s.clk_hi);
	printf("    0-15:  clk_lo:              %u\n", emm_switch.s.clk_lo);

	emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
	printf("\nMIO_EMM_DMA:                  0x%016llx\n", emm_dma.u64);
	printf("    60-61: bus_id:              %u\n", emm_dma.s.bus_id);
	printf("    59:    dma_val:             %s\n", emm_dma.s.dma_val ? "yes" : "no");
	printf("    58:    sector:              %s mode\n", emm_dma.s.sector ? "sector" : "byte");
	printf("    57:    dat_null:            %s\n", emm_dma.s.dat_null ? "yes" : "no");
	printf("    51-56: thres:               %u\n", emm_dma.s.thres);
	printf("    50:    rel_wr:              %s\n", emm_dma.s.rel_wr ? "yes" : "no");
	printf("    49:    rw:                  %s\n", emm_dma.s.rw ? "write" : "read");
	printf("    48:    multi:               %s\n", emm_dma.s.multi ? "yes" : "no");
	printf("    32-47: block_cnt:           %u\n", emm_dma.s.block_cnt);
	printf("    0-31:  card_addr:           0x%x\n", emm_dma.s.card_addr);

	emm_cmd.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CMD);
	printf("\nMIO_EMM_CMD:                  0x%016llx\n", emm_cmd.u64);
	printf("    60-61: bus_id:              %u\n", emm_cmd.s.bus_id);
	printf("    59:    cmd_val:             %s\n", emm_cmd.s.cmd_val ? "yes" : "no");
	printf("    55:    dbuf:                %u\n", emm_cmd.s.dbuf);
	printf("    49-54: offset:              %u\n", emm_cmd.s.offset);
	printf("    41-42: ctype_xor:           %s\n", ctype_xor_str[emm_cmd.s.ctype_xor]);
	printf("    38-40: rtype_xor:           %s\n", rtype_xor_str[emm_cmd.s.rtype_xor]);
	printf("    32-37: cmd_idx:             %u\n", emm_cmd.s.cmd_idx);
	printf("    0-31:  arg:                 0x%x\n", emm_cmd.s.arg);

	emm_rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
	printf("\nMIO_EMM_RSP_STS:              0x%016llx\n", emm_rsp_sts.u64);
	printf("    60-61: bus_id:              %u\n", emm_rsp_sts.s.bus_id);
	printf("    59:    cmd_val:             %s\n", emm_rsp_sts.s.cmd_val ? "yes" : "no");
	printf("    58:    switch_val:          %s\n", emm_rsp_sts.s.switch_val ? "yes" : "no");
	printf("    57:    dma_val:             %s\n", emm_rsp_sts.s.dma_val ? "yes" : "no");
	printf("    56:    dma_pend:            %s\n", emm_rsp_sts.s.dma_pend ? "yes" : "no");
	printf("    28:    dbuf_err:            %s\n", emm_rsp_sts.s.dbuf_err ? "yes" : "no");
	printf("    23:    dbuf:                %u\n", emm_rsp_sts.s.dbuf);
	printf("    22:    blk_timeout:         %s\n", emm_rsp_sts.s.blk_timeout ? "yes" : "no");
	printf("    21:    blk_crc_err:         %s\n", emm_rsp_sts.s.blk_crc_err ? "yes" : "no");
	printf("    20:    rsp_busybit:         %s\n", emm_rsp_sts.s.rsp_busybit ? "yes" : "no");
	printf("    19:    stp_timeout:         %s\n", emm_rsp_sts.s.stp_timeout ? "yes" : "no");
	printf("    18:    stp_crc_err:         %s\n", emm_rsp_sts.s.stp_crc_err ? "yes" : "no");
	printf("    17:    stp_bad_sts:         %s\n", emm_rsp_sts.s.stp_bad_sts ? "yes" : "no");
	printf("    16:    stp_val:             %s\n", emm_rsp_sts.s.stp_val ? "yes" : "no");
	printf("    15:    rsp_timeout:         %s\n", emm_rsp_sts.s.rsp_timeout ? "yes" : "no");
	printf("    14:    rsp_crc_err:         %s\n", emm_rsp_sts.s.rsp_crc_err ? "yes" : "no");
	printf("    13:    rsp_bad_sts:         %s\n", emm_rsp_sts.s.rsp_bad_sts ? "yes" : "no");
	printf("    12:    rsp_val:             %s\n", emm_rsp_sts.s.rsp_val ? "yes" : "no");
	printf("    9-11:  rsp_type:            %s\n", rtype_xor_str[emm_rsp_sts.s.rsp_type]);
	printf("    7-8:   cmd_type:            %s\n", ctype_xor_str[emm_rsp_sts.s.cmd_type]);
	printf("    1-6:   cmd_idx:             %u\n", emm_rsp_sts.s.cmd_idx);
	printf("    0:     cmd_done:            %s\n", emm_rsp_sts.s.cmd_done ? "yes" : "no");

	emm_rsp_lo.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO);
	printf("\nMIO_EMM_RSP_STS_LO:           0x%016llx\n", emm_rsp_lo.u64);

	emm_rsp_hi.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_HI);
	printf("\nMIO_EMM_RSP_STS_HI:           0x%016llx\n", emm_rsp_hi.u64);

	emm_int.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_INT);
	printf("\nMIO_EMM_INT:                  0x%016llx\n", emm_int.u64);
	printf("    6:    switch_err:           %s\n", emm_int.s.switch_err ? "yes" : "no");
	printf("    5:    switch_done:          %s\n", emm_int.s.switch_done ? "yes" : "no");
	printf("    4:    dma_err:              %s\n", emm_int.s.dma_err ? "yes" : "no");
	printf("    3:    cmd_err:              %s\n", emm_int.s.cmd_err ? "yes" : "no");
	printf("    2:    dma_done:             %s\n", emm_int.s.dma_done ? "yes" : "no");
	printf("    1:    cmd_done:             %s\n", emm_int.s.cmd_done ? "yes" : "no");
	printf("    0:    buf_done:             %s\n", emm_int.s.buf_done ? "yes" : "no");

	emm_wdog.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_WDOG);
	printf("\nMIO_EMM_WDOG:                 0x%016llx (%u)\n", emm_wdog.u64, emm_wdog.s.clk_cnt);

	emm_sample.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SAMPLE);
	printf("\nMIO_EMM_SAMPLE:               0x%016llx\n", emm_sample.u64);
	printf("    16-25: cmd_cnt:             %u\n", emm_sample.s.cmd_cnt);
	printf("    0-9:   dat_cnt:             %u\n", emm_sample.s.dat_cnt);

	emm_sts_mask.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_STS_MASK);
	printf("\nMIO_EMM_STS_MASK:             0x%016llx\n", emm_sts_mask.u64);

	emm_rca.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RCA);
	printf("\nMIO_EMM_RCA:                  0x%016llx\n", emm_rca.u64);
	printf("    0-15:  card_rca:            %u\n", emm_rca.s.card_rca);
	puts("\n");
#endif /* DEBUG_REGISTERS */
}

struct mmc *find_mmc_device(int dev_num)
{
	struct mmc *m;
	struct list_head *entry;

	debug("%s(%d)\n", __func__, dev_num);
	/* If nothing is available, try and initialize */
	if (list_empty(&mmc_devices)) {
		debug("MMC device not found, initializing\n");
		if (mmc_initialize(gd->bd)) {
			debug("%s(%d): no devices found\n", __func__, dev_num);
			return NULL;
		}
		debug("%s: Done initializing\n", __func__);
	}

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);
		debug("  testing MMC dev %d == %d\n",
		      m->block_dev.dev, dev_num);
		if (m->block_dev.dev == dev_num)
			return m;
	}

	printf("MMC Device %d not found\n", dev_num);

	return NULL;
}

/**
 * Enables the MMC bus, disabling NOR flash and other boot bus device access
 *
 * @param mmc - bus to enable
 */
static void mmc_enable(struct mmc *mmc)
{
	cvmx_mio_emm_cfg_t emm_cfg;
	struct mmc_host *host = mmc->priv;

	debug("%s(%d)\n", __func__, host->bus_id);

	emm_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CFG);
	emm_cfg.s.bus_ena |= 1 <<host->bus_id;
	mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);

	debug("%s: Wrote 0x%llx to MIO_EMM_CFG\n", __func__, emm_cfg.u64);
	debug("%s: MIO_EMM_MODE: 0x%llx\n", __func__,
	      mmc_read_csr(mmc, CVMX_MIO_EMM_MODEX(host->bus_id)));
	debug("%s: MIO_EMM_SWITCH: 0x%llx\n", __func__,
	      mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH));
}

/**
 * Disables the MMC bus, enabling NOR flash and other boot bus device access
 *
 * @param mmc - bus to disable
 */
static void mmc_disable(struct mmc *mmc)
{
	cvmx_mio_emm_cfg_t emm_cfg;
	struct mmc_host *host = mmc->priv;
	debug("%s(%d:%d):\n", __func__, host->node, host->bus_id);
again:
	emm_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CFG);
	emm_cfg.s.bus_ena &= ~(1 << host->bus_id);
	debug("before:\n");
	mmc_print_registers(mmc);
	mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);
	debug("after:\n");
	mmc_print_registers(mmc);
	if (OCTEON_IS_MODEL(OCTEON_CN73XX) || OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		/* McBuggin 26703 - EMMC CSR reset doesn't consistantly work
		 *
		 * Software workaround:
		 * 1. Clear the MIO_EMM_CFG[BUS_ENA] bits/
		 * 2. Read MIO_EMM_MODE0[CLK_HI] for the value 2500 which
		 *    indicates the CSRs are in reset.
		 * 3. If not in reset, set the MIO_EMM_CFG[BUS_ENA] bits and
		 *    repeat steps 1-3, otherwise exit.
		 */
		if (emm_cfg.s.bus_ena == 0) {
			union cvmx_mio_emm_modex emm_mode;

			emm_mode.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_MODEX(0));
			if (emm_mode.s.clk_hi != 2500) {
				debug("%s: reset failed, clk_hi: %d, try again\n",
				      __func__, (int)emm_mode.s.clk_hi);
				emm_cfg.s.bus_ena |= (1 << host->bus_id);
				mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);
				mdelay(1);
				goto again;
			}
		}
	}
	last_bus_id[host->node] = -1;
}

/**
 * Sets the hardware MMC watchdog in microseconds
 *
 * @param[in]	mmc	pointer to MMC data structure
 * @param	timeout	How long to wait in usecs
 */
static void mmc_set_watchdog(const struct mmc *mmc, ulong timeout)
{
	cvmx_mio_emm_wdog_t emm_wdog;
	uint64_t val;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;

	val = ((u64)timeout * mmc->clock) / 1000000;
	if (val >= (1 << 26)) {
		debug("%s: warning: timeout %lu exceeds max value %llu, truncating\n",
		      __func__, timeout,
		     (uint64_t)(((1 << 26) - 1) * 1000000ULL) / (uint64_t)mmc->clock);
		val = (1 << 26) - 1;
	}
	emm_wdog.u64 = 0;
	emm_wdog.s.clk_cnt = val;

	debug("%s(%p(%s), %lu) clock: %u, period: %u, clk_cnt: %llu)\n",
	      __func__, mmc, mmc->cfg->name, timeout,
	      mmc->clock, host->clk_period, emm_wdog.u64);
	mmc_write_csr(mmc, CVMX_MIO_EMM_WDOG, emm_wdog.u64);
}

/**
 * Fills in the DMA configuration registers, clears interrupts, sets the
 * watchdog and starts the transfer to or from the MMC/SD card.
 *
 * @param mmc	pointer to MMC data structure
 * @param write	whether this is a write operation or not
 * @param clear	whether this is a DMA abort operation or not
 * @param block	starting block number to read/write
 * @param adr	physical address to DMA from/to
 * @param size	Number of blocks to transfer
 * @param timeout	timeout to set watchdog less than.
 */
static void mmc_start_dma(const struct mmc *mmc, bool write, bool clear,
			  uint32_t block, uint64_t adr, uint32_t size,
			  int timeout)
{
	const struct mmc_host *host = (struct mmc_host *)mmc->priv;
	cvmx_mio_ndf_dma_cfg_t ndf_dma_cfg;
	cvmx_mio_ndf_dma_int_t ndf_dma_int;
	cvmx_mio_emm_dma_cfg_t emm_dma_cfg;
	cvmx_mio_emm_dma_adr_t emm_dma_adr;
	cvmx_mio_emm_dma_int_t emm_dma_int;
	cvmx_mio_emm_dma_t emm_dma;
	cvmx_mio_emm_int_t emm_int;

	debug("%s(%p(%d), %s, %s, 0x%x, 0x%llx, 0x%x, %d)\n", __func__, mmc,
	      host->bus_id, write ? "true" : "false", clear ? "true" : "false",
	      block, adr, size, timeout);
	if (host->use_ndf) {
		ndf_dma_int.u64 = 0;
		ndf_dma_int.s.done = 1;
		mmc_write_csr(mmc, CVMX_MIO_NDF_DMA_INT, ndf_dma_int.u64);

		ndf_dma_cfg.u64 = 0;
		ndf_dma_cfg.s.en = 1;
		ndf_dma_cfg.s.rw = !!write;
		ndf_dma_cfg.s.clr = !!clear;
		ndf_dma_cfg.s.size =
				((uint64_t)(size * mmc->read_bl_len) / 8) - 1;
		ndf_dma_cfg.s.adr = adr;
		debug("%s: Writing 0x%llx to mio_ndf_dma_cfg\n",
		      __func__, ndf_dma_cfg.u64);
		mmc_write_csr(mmc, CVMX_MIO_NDF_DMA_CFG, ndf_dma_cfg.u64);
	} else {
		emm_dma_int.u64 = 0;
		emm_dma_int.s.done = 1;
		emm_dma_int.s.fifo = 1;
		mmc_write_csr(mmc, CVMX_MIO_EMM_DMA_INT, emm_dma_int.u64);

		emm_dma_cfg.u64 = 0;
		emm_dma_cfg.s.en = 1;
		emm_dma_cfg.s.rw = !!write;
		emm_dma_cfg.s.clr = !!clear;
		emm_dma_cfg.s.size =
				((uint64_t)(size * mmc->read_bl_len) / 8) - 1;

		emm_dma_adr.u64 = 0;
		emm_dma_adr.s.adr = adr;
		debug("%s: Writing 0x%llx to mio_emm_dma_cfg and 0x%llx to mio_emm_dma_adr\n",
		      __func__, emm_dma_cfg.u64, emm_dma_adr.u64);
		mmc_write_csr(mmc, CVMX_MIO_EMM_DMA_ADR, emm_dma_adr.u64);
		mmc_write_csr(mmc, CVMX_MIO_EMM_DMA_CFG, emm_dma_cfg.u64);
	}
	emm_dma.u64 = 0;
	emm_dma.s.bus_id = host->bus_id;
	emm_dma.s.dma_val = 1;
	emm_dma.s.rw = !!write;
	emm_dma.s.sector = mmc->high_capacity ? 1 : 0;
	/* NOTE: For SD we can only support multi-block transfers if
	 * bit 33 (CMD_SUPPORT) is set in the SCR register.
	 */
	if ((size > 1)
	    && ((IS_SD(mmc) && (host->flags & OCTEON_MMC_FLAG_SD_CMD23))
		|| !IS_SD(mmc)))
		emm_dma.s.multi = 1;
	else
		emm_dma.s.multi = 0;

	emm_dma.s.block_cnt = size;
	if (!mmc->high_capacity)
		block *= mmc->read_bl_len;
	emm_dma.s.card_addr = block;

	debug("%s: card address: 0x%x, size: %d, multi: %d\n",
	      __func__, block, size, emm_dma.s.multi);
	/* Clear interrupt */
	emm_int.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_INT);
	mmc_write_csr(mmc, CVMX_MIO_EMM_INT, emm_int.u64);

	CVMX_SYNCW;
	mmc_set_watchdog(mmc, timeout * 1000 - 1000);

	debug("%s: Writing 0x%llx to mio_emm_dma\n", __func__, emm_dma.u64);
	mmc_write_csr(mmc, CVMX_MIO_EMM_DMA, emm_dma.u64);
	CVMX_SYNCW;
}

/**
 * This function must be called when it is possible that the MMC bus has changed
 *
 * @param mmc - pointer to MMC data structure
 */
static inline void mmc_switch_dev(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_sample_t emm_sample;
	cvmx_mio_emm_rca_t emm_rca;
	cvmx_mio_emm_sts_mask_t emm_sts_mask;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;

	if (host->bus_id == last_bus_id[host->node])
		return;

	emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
#ifdef DEBUG
	emm_rca.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RCA);
	debug("%s: Switching from:\n"
	      "    bus id: %d, clock period: %d, width: %d, rca: 0x%x, high speed: %d to\n"
	      "    bus id: %d, clock period: %d, width: %d, rca: 0x%x, high speed: %d\n",
	      __func__, last_bus_id[host->node], emm_switch.s.clk_lo * 2,
	      emm_switch.s.bus_width, emm_switch.s.hs_timing,
	      emm_rca.s.card_rca, host->bus_id,
	      host->clk_period, host->bus_width, mmc->rca,
	      !!(mmc->card_caps & MMC_MODE_HS));
#endif
	/* mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, 1 << host->bus_id);*/
	emm_switch.s.bus_width = host->bus_width;
	emm_switch.s.hs_timing = mmc->clock > 20000000;
	debug("%s: hs timing: %d, caps: 0x%x\n", __func__,
	      emm_switch.s.hs_timing, mmc->card_caps);
	emm_switch.s.clk_hi = (host->clk_period + 1) / 2;
	emm_switch.s.clk_lo = (host->clk_period + 1) / 2;
	emm_switch.s.bus_id = host->bus_id;
	emm_switch.s.power_class = host->power_class;
	debug("%s: Setting MIO_EMM_SWITCH to 0x%llx\n", __func__, emm_switch.u64);
	emm_switch.s.bus_id = 0;
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	udelay(100);
	emm_switch.s.bus_id = host->bus_id;
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	debug("Switching RCA to 0x%x\n", mmc->rca);
	emm_rca.u64 = 0;
	emm_rca.s.card_rca = mmc->rca;
	mmc_write_csr(mmc, CVMX_MIO_EMM_RCA, emm_rca.u64);
	last_bus_id[host->node] = host->bus_id;
	mdelay(100);
	/* Update the watchdog to 100 ms */
	mmc_set_watchdog(mmc, 100000);

	emm_sample.u64 = 0;
	emm_sample.s.cmd_cnt = host->cmd_clk_skew;
	emm_sample.s.dat_cnt = host->dat_clk_skew;
	mmc_write_csr(mmc, CVMX_MIO_EMM_SAMPLE, emm_sample.u64);
	/* Set status mask */
	emm_sts_mask.u64 = 0;
	emm_sts_mask.s.sts_msk = 1 << 7 | 1 << 22 | 1 << 23 | 1 << 19;
	mmc_write_csr(mmc, CVMX_MIO_EMM_STS_MASK, emm_sts_mask.u64);

	debug("%s: MIO_EMM_MODE(%d): 0x%llx, MIO_EMM_SWITCH: 0x%llx\n",
	      __func__, host->bus_id,
	      mmc_read_csr(mmc, CVMX_MIO_EMM_MODEX(host->bus_id)),
	      mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH));
	mmc_print_registers(mmc);
}

/**
 * Reads one or more sectors into memory
 *
 * @param mmc	mmc data structure
 * @param src	source sector number
 * @param dst	pointer to destination address to read into
 * @param size	number of sectors to read
 *
 * @return number of sectors read
 */
int mmc_read(struct mmc *mmc, u64 src, uchar *dst, int size)
{
	uint64_t dma_addr;
	cvmx_mio_emm_dma_t emm_dma;
	cvmx_mio_ndf_dma_int_t ndf_dma_int;
	cvmx_mio_emm_dma_int_t emm_dma_int;
	cvmx_mio_emm_rsp_sts_t rsp_sts;
	cvmx_mio_emm_int_t emm_int;
	cvmx_mio_emm_sts_mask_t emm_sts_mask;
	int timeout;
	int dma_retry_count = 0;
	struct mmc_cmd cmd;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	ulong start_time;
	bool timed_out = false;
	int i __attribute__((unused));


	debug("%s(src: 0x%llx, dst: 0x%p, size: %d)\n", __func__, src, dst, size);
#ifdef DEBUG
	memset(dst, 0xEE, size * mmc->read_bl_len);
#endif
	mmc_switch_dev(mmc);

	debug("Setting block length to %d\n", mmc->read_bl_len);
	if (!IS_SD(mmc) || (IS_SD(mmc) && mmc->high_capacity))
		mmc_set_blocklen(mmc, mmc->read_bl_len);

	/* Enable appropriate errors */
	emm_sts_mask.u64 = 0;
	emm_sts_mask.s.sts_msk = R1_BLOCK_READ_MASK;
	mmc_write_csr(mmc, CVMX_MIO_EMM_STS_MASK, emm_sts_mask.u64);
	debug("%s: MIO_EMM_STS_MASK: 0x%llx\n", __func__, emm_sts_mask.u64);

	dma_addr = cvmx_ptr_to_phys(dst);
	debug("%s: dma address: 0x%llx\n", __func__, dma_addr);

	timeout = 1000 + (size * 50);
	mmc_set_watchdog(mmc, timeout * 1000 - 100);

	mmc_start_dma(mmc, false, false, src, dma_addr, size, timeout);

retry_dma:
	debug("%s: timeout: %d\n", __func__, timeout);
	start_time = get_timer(0);
	if (host->use_ndf) {
		do {
			ndf_dma_int.u64 = mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_INT);
#ifdef DEBUG
			debug("%s: ndf_dma_int: 0x%llx, ndf_dma_cfg: 0x%llx, mio_emm_dma: 0x%llx, rsp sts: 0x%llx, time: %lu\n",
			      __func__, ndf_dma_int.u64,
			      mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_CFG),
			      mmc_read_csr(mmc, CVMX_MIO_EMM_DMA),
			      mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS),
			      get_timer(start_time));
#endif
			if (ndf_dma_int.s.done) {
				mmc_write_csr(mmc, CVMX_MIO_NDF_DMA_INT,
					      ndf_dma_int.u64);
				debug("%s: DMA completed normally\n", __func__);
				break;
			}

			WATCHDOG_RESET();
			udelay(1);
			timed_out = get_timer(start_time) > timeout;
			rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);

			if (rsp_sts.s.dma_pend) {
				printf("%s: DMA error, rsp status: 0x%llx\n",
				       __func__, rsp_sts.u64);
				break;
			}
		} while (!timed_out);
		timed_out |= !ndf_dma_int.s.done;
	} else {
		do {
			emm_dma_int.u64 = mmc_read_csr(mmc,
						       CVMX_MIO_EMM_DMA_INT);
			debug("%s: mio_emm_dma_int: 0x%llx\n", __func__,
			      emm_dma_int.u64);

			if (emm_dma_int.s.done) {
				mmc_write_csr(mmc, CVMX_MIO_EMM_DMA_INT,
					      emm_dma_int.u64);
				break;
			}

			WATCHDOG_RESET();
			udelay(1);
		} while (get_timer(start_time) < timeout);
		timed_out = !emm_dma_int.s.done;
	}
	rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
	debug("%s: rsp_sts: 0x%llx\n", __func__, rsp_sts.u64);
	if (timed_out || rsp_sts.s.dma_val || rsp_sts.s.dma_pend) {
		emm_int.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_INT);
		mmc_print_registers(mmc);
		if (timed_out) {
			printf("%s(%s, 0x%llx, %p, %d)\n",
			       __func__, mmc->cfg->name, src, dst, size);
			printf("MMC read DMA timeout, status: 0x%llx, interrupt status: 0x%llx\n",
			       rsp_sts.u64, emm_int.u64);
			printf("    MIO_EMM_DMA: 0x%llx, last command: %d\n",
			       emm_dma.u64, rsp_sts.s.cmd_idx);
			printf("    MIO_EMM_RSP_LO: 0x%llx, HI: 0x%llx\n",
			       mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO),
			       mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_HI));
		} else {
			if (rsp_sts.s.blk_timeout)
				printf("Block timeout error detected\n");
			if (rsp_sts.s.blk_crc_err)
				printf("Block CRC error detected\n");
		}
		if (dma_retry_count++ < 3) {
			/* DMA still pending, terminate it */
#ifdef DEBUG
			if (rsp_sts.s.dma_pend)
				debug("%s: rsp_sts_low: 0x%llx\n", __func__,
				      mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO));
#endif
			emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
			emm_dma.s.dma_val = 1;
			emm_dma.s.dat_null = 1;
			mmc_write_csr(mmc, CVMX_MIO_EMM_DMA, emm_dma.u64);
			start_time = get_timer(0);
			do {
				rsp_sts.u64 = mmc_read_csr(mmc,
							   CVMX_MIO_EMM_RSP_STS);
				if (rsp_sts.s.dma_val == 0)
					break;
				udelay(1);
			} while (get_timer(start_time) < timeout);
			timeout += 1000 + size;
#if 0
			debug("Sending stop command\n");
			cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
			cmd.cmdarg = 0;
			cmd.resp_type = MMC_RSP_R1b;
			if (mmc_send_cmd_flags(mmc, &cmd, NULL, 0)) {
				printf("Error sending stop transmission cmd\n");
			}
#endif
#ifdef DEBUG
			print_buffer(0, dst, 1, 512, 0);
#endif
			debug("Retrying MMC read DMA\n");
			goto retry_dma;
		} else {
#if 0
			cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
			cmd.cmdarg = 0;
			cmd.resp_type = MMC_RSP_R1b;
			if (mmc_send_cmd_flags(mmc, &cmd, NULL, 0))
				printf("Error sending stop transmission cmd\n");
#endif
			printf("mmc read block %llu, size %d DMA failed, terminating...\n",
			       src, size);
			emm_dma.s.dma_val = 1;
			emm_dma.s.dat_null = 1;
			mmc_write_csr(mmc, CVMX_MIO_EMM_DMA, emm_dma.u64);
			timeout = 1000 + size;
			start_time = get_timer(0);
			if (host->use_ndf)
				do {
					ndf_dma_int.u64 =
						mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_INT);
					if (ndf_dma_int.s.done)
						break;
					udelay(1);
					WATCHDOG_RESET();
					timed_out = (get_timer(start_time) > timeout);
				} while (!timed_out);
			else
				do {
					emm_dma_int.u64 =
						mmc_read_csr(mmc, CVMX_MIO_EMM_DMA_INT);
					if (emm_dma_int.s.done)
						break;
					udelay(1);
					WATCHDOG_RESET();
					timed_out = (get_timer(start_time) > timeout);
				} while (!timed_out);
			if (timed_out)
				puts("Error: MMC read DMA failed to terminate!\n");

			return 0;
		}
	}

	if (dma_retry_count) {
		debug("Success after %d DMA retries\n", dma_retry_count);
	}

	if (timed_out) {
		printf("MMC read block %llu timed out\n", src);
		debug("Read status 0x%llx\n", rsp_sts.u64);
		emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
		debug("EMM_DMA: 0x%llx\n", emm_dma.u64);

		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		if (octeon_mmc_send_cmd(mmc, &cmd, NULL))
			printf("Error sending stop transmission cmd\n");
		return 0;
	}
	debug("Read status 0x%llx\n", rsp_sts.u64);

	emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
	debug("EMM_DMA: 0x%llx\n", emm_dma.u64);
#ifdef DEBUG
	print_buffer(0, dst, 1, 512, 0);
#endif
	return size - emm_dma.s.block_cnt;
}

/**
 * Writes sectors to MMC device
 *
 * @param[in,out] mmc - MMC device to write to
 * @param start - starting sector to write to
 * @param size - number of sectors to write
 * @param src - pointer to source address of buffer containing sectors
 *
 * @return number of sectors or 0 if none.
 *
 * NOTE: This checks the GPIO write protect if it is present
 */
static ulong
mmc_write(struct mmc *mmc, ulong start, int size, const void *src)
{
	uint64_t dma_addr;
	cvmx_mio_emm_dma_t emm_dma;
	cvmx_mio_ndf_dma_int_t ndf_dma_int;
	cvmx_mio_emm_dma_int_t emm_dma_int;
	cvmx_mio_emm_rsp_sts_t rsp_sts;
	cvmx_mio_emm_int_t emm_int;
	struct mmc_cmd cmd;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int timeout;
	int dma_retry_count = 0;
	int rc;
	ulong start_time;
	bool timed_out = false;

	debug("%s(start: %lu, size: %d, src: 0x%p)\n", __func__, start,
	      size, src);

	mmc_switch_dev(mmc);

	/* Poll for ready status */
	timeout = 10000;	/* 10 seconds */
	start_time = get_timer(0);
	timed_out = true;
	do {
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmdidx = MMC_CMD_SEND_STATUS;
		cmd.cmdarg = mmc->rca << 16;
		cmd.resp_type = MMC_RSP_R1;
		rc = octeon_mmc_send_cmd(mmc, &cmd, NULL);
		if (rc) {
			printf("%s: Error getting device status\n", __func__);
			return 0;
		} else if (cmd.response[0] & R1_READY_FOR_DATA) {
			timed_out = false;
			break;
		}
		udelay(1);
		timed_out = (get_timer(start_time) > timeout);
	} while (!timed_out);
	debug("%s: Device status: 0x%x\n", __func__, cmd.response[0]);
	if (timed_out) {
		printf("%s: Device timed out waiting for empty buffer, response: 0x%x\n",
		       __func__, cmd.response[0]);
		return 0;
	}

	dma_addr = cvmx_ptr_to_phys((void *)src);
	timeout = 5000 + 5000 * size;

	mmc_start_dma(mmc, true, false, start, dma_addr, size, timeout);

retry_dma:
	start_time = get_timer(0);
	do {
#ifdef DEBUG
		if (ctrlc()) {
			printf("Interrupted by user\n");
			break;
		}
#endif

		rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
		if (((rsp_sts.s.dma_val == 0) || (rsp_sts.s.dma_pend == 1))
		    && rsp_sts.s.cmd_done)
			break;

		WATCHDOG_RESET();
		udelay(1);
		timed_out = (get_timer(start_time) >= timeout);
	} while (!timed_out);

	if (timed_out) {
		printf("%s: write command completion timeout for cmd %d\n",
		       __func__, rsp_sts.s.cmd_idx);
	}
	/*rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);*/
	debug("emm_rsp_sts: 0x%llx, cmd: %d, response: 0x%llx\n",
	      rsp_sts.u64, rsp_sts.s.cmd_idx,
	      mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO));
	if (rsp_sts.s.cmd_val || timed_out || rsp_sts.s.dma_val
	    || rsp_sts.s.dma_pend) {
		emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
		emm_int.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_INT);
		printf("%s: Error detected: MIO_EMM_RSP_STS: 0x%llx, MIO_EMM_DMA: 0x%llx,\n"
		       "    %s: 0x%llx, timeout: %d\n",
		       __func__, rsp_sts.u64, emm_dma.u64,
		       host->use_ndf ? "MIO_NDF_DMA_CFG" : "MIO_EMM_DMA_CFG",
		       host->use_ndf ? mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_CFG) :
		       mmc_read_csr(mmc, CVMX_MIO_EMM_DMA_CFG), timeout);
		printf("Last command index: %d\n", rsp_sts.s.cmd_idx);
		printf("emm_int: 0x%llx\n", emm_int.u64);
		mdelay(10);
		rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
		printf("Re-read rsp_sts: 0x%llx, cmd_idx: %d\n", rsp_sts.u64,
		       rsp_sts.s.cmd_idx);
		printf("  RSP_LO: 0x%llx\n", mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO));
		if (timed_out) {
			printf("%s(mmc, 0x%lx, %d, 0x%p)\n",
			       __func__, start, size, src);
			printf("MMC write DMA timeout, status: 0x%llx, interrupt status: 0x%llx\n",
			       rsp_sts.u64, emm_int.u64);
			printf("    MIO_EMM_DMA: 0x%llx, last command: %d\n",
			       emm_dma.u64, rsp_sts.s.cmd_idx);
		} else {
			if (rsp_sts.s.blk_timeout)
				printf("Block timeout error detected\n");
			if (rsp_sts.s.blk_crc_err)
				printf("Block CRC error detected\n");
			if (rsp_sts.s.dma_val) {
				printf("DMA still valid\n");
			}
		}

		if (dma_retry_count++ < 3 && rsp_sts.s.dma_pend) {
			/* DMA still pending, terminate it */
			emm_dma.s.dma_val = 1;
			timeout = 2000 * size;
			cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
			cmd.cmdarg = 0;
			cmd.resp_type = MMC_RSP_R1b;
			octeon_mmc_send_cmd(mmc, &cmd, NULL);
			mmc_write_csr(mmc, CVMX_MIO_EMM_DMA, emm_dma.u64);
			debug("Retrying MMC write DMA\n");
			goto retry_dma;
		} else {
			emm_dma.s.dma_val = 1;
			emm_dma.s.dat_null = 1;
			mmc_write_csr(mmc, CVMX_MIO_EMM_DMA, emm_dma.u64);
			start_time = get_timer(0);
			if (host->use_ndf)
				do {
					ndf_dma_int.u64 =
						mmc_read_csr(mmc, CVMX_MIO_NDF_DMA_INT);
					if (ndf_dma_int.s.done)
						break;
					udelay(1);
				} while (get_timer(start_time) < timeout);
			else
				do {
					emm_dma_int.u64 =
						mmc_read_csr(mmc, CVMX_MIO_EMM_DMA_INT);
					if (emm_dma_int.s.done)
						break;
					udelay(1);
				} while (get_timer(start_time) < timeout);
			if (timeout <= 0)
				puts("Error: MMC write DMA failed to terminate!\n");
			return 0;
		}
	}

	if (dma_retry_count) {
		debug("Success after %d DMA retries\n", dma_retry_count);
	}

	if (timed_out) {
		printf("MMC write block %lu timed out\n", start);
		debug("Write status 0x%llx\n", rsp_sts.u64);
		emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
		debug("EMM_DMA: 0x%llx\n", emm_dma.u64);

		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		if (octeon_mmc_send_cmd(mmc, &cmd, NULL))
			printf("Error sending stop transmission cmd\n");
		return 0;
	}
	debug("Write status 0x%llx\n", rsp_sts.u64);

#if 1
	/* Poll status if we can't send data right away */
	if (!((rsp_sts.s.cmd_idx == MMC_CMD_SEND_STATUS)
	     && rsp_sts.s.cmd_done
	     && ((mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO) >> 8) & R1_READY_FOR_DATA))) {
		/* Poll for ready status */
		timeout = 10000;	/* 10 seconds */
		start_time = (get_timer(0));
		do {
			memset(&cmd, 0, sizeof(cmd));
			cmd.cmdidx = MMC_CMD_SEND_STATUS;
			cmd.cmdarg = mmc->rca << 16;
			cmd.resp_type = MMC_RSP_R1;
			rc = octeon_mmc_send_cmd(mmc, &cmd, NULL);
			if (rc) {
				printf("%s: Error getting post device status\n",
				       __func__);
				return 0;
			}
			if (cmd.response[0] & R1_READY_FOR_DATA)
				break;
			mdelay(1);
			timed_out = get_timer(start_time) > timeout;
		} while (!timed_out);
		if (timed_out) {
			printf("%s: Device timed out waiting for empty buffer\n",
			       __func__);
			return 0;
		}
	}
#endif
	emm_dma.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_DMA);
	debug("EMM_DMA: 0x%llx\n", emm_dma.u64);

	return size - emm_dma.s.block_cnt;
}

static ulong mmc_erase_t(struct mmc *mmc, ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	ulong end;
	int err, start_cmd, end_cmd;

	if (mmc->high_capacity)
		end = start + blkcnt - 1;
	else {
		end = (start + blkcnt - 1) * mmc->write_bl_len;
		start *= mmc->write_bl_len;
	}

	if (!mmc_getcd(mmc)) {
		printf("%s: Error: Card not detected\n", __func__);
		return 0;
	}

	if (IS_SD(mmc)) {
		start_cmd = SD_CMD_ERASE_WR_BLK_START;
		end_cmd = SD_CMD_ERASE_WR_BLK_END;
	} else {
		start_cmd = MMC_CMD_ERASE_GROUP_START;
		end_cmd = MMC_CMD_ERASE_GROUP_END;
	}

	cmd.cmdidx = start_cmd;
	cmd.cmdarg = start;
	cmd.resp_type = MMC_RSP_R1;

	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = end_cmd;
	cmd.cmdarg = end;
	cmd.resp_type = MMC_RSP_R1b;

	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_R1b;

	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err) {
		printf("%s err: %d\n", __func__, err);
		goto err_out;
	}

	return 0;

err_out:
	printf("mmc erase failed, err: %d\n", err);
	return err;
}

/**
 * Reads blkcnt sectors from the specified device into the specified buffer
 * @param dev_num	Device number to read from
 * @param start		Starting sector number
 * @param blkcnt	Number of sectors to read
 * @param[out] dst	Pointer to buffer to contain the data
 *
 * @return number of blocks read or 0 if error
 */
static ulong mmc_bread(int dev_num, lbaint_t start, lbaint_t blkcnt, void *dst)
{
	lbaint_t cur, blocks_todo = blkcnt;
	struct mmc *mmc = find_mmc_device(dev_num);
	unsigned char bounce_buffer[4096];

	debug("%s(%d, %llu, %llu, %p)\n", __func__, dev_num, (uint64_t)start,
	      (uint64_t)blkcnt, dst);
	if (!mmc)
		return 0;

	if (blkcnt == 0)
		return 0;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%llx exceeds max(0x%llx)\n",
		       (uint64_t)(start + blkcnt),
		       (uint64_t)mmc->block_dev.lba);
		return 0;
	}

	if (!mmc_getcd(mmc)) {
		printf("%s: Error: Card not detected\n", __func__);
		return 0;
	}

	mmc_enable(mmc);
	if (((ulong)dst) & 7) {
		debug("%s: Using bounce buffer due to alignment\n", __func__);
		do {
			if (mmc_read(mmc, start, bounce_buffer, 1) != 1)
				return 0;
			memcpy(dst, bounce_buffer, mmc->read_bl_len);
			WATCHDOG_RESET();
			dst += mmc->read_bl_len;
			start++;
			blocks_todo--;
		} while (blocks_todo > 0);
	} else {
		do {
			cur = min(blocks_todo, mmc->cfg->b_max);
			if (mmc_read(mmc, start, dst, cur) != cur) {
				blkcnt = 0;
				break;
			}
			WATCHDOG_RESET();
			blocks_todo -= cur;
			start += cur;
			dst += cur * mmc->read_bl_len;
		} while (blocks_todo > 0);
	}
	mmc_disable(mmc);

	return blkcnt;
}

/**
 * Writes blkcnt sectors to the specified device from the specified buffer
 * @param dev_num	Device number to write to
 * @param start		Starting sector number
 * @param blkcnt	Number of sectors to write
 * @param[in] src	Pointer to buffer that contains the data
 *
 * @return number of blocks written or 0 if error
 */
static ulong mmc_bwrite(int dev_num, lbaint_t start, lbaint_t blkcnt,
			const void *src)
{
	lbaint_t cur, blocks_todo = blkcnt;
	struct mmc *mmc = find_mmc_device(dev_num);
	unsigned char bounce_buffer[4096];

	debug("%s(%d, %llu, %llu, %p)\n", __func__, dev_num, (uint64_t)start,
	      (uint64_t)blkcnt, src);
	if (!mmc) {
		printf("MMC Write: device %d not found\n", dev_num);
		return 0;
	}

	if (blkcnt == 0)
		return 0;
	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%llx exceeds max(0x%llx)\n",
		       (uint64_t)(start + blkcnt),
		       (uint64_t)mmc->block_dev.lba);
		return 0;
	}
	if (!mmc_getcd(mmc)) {
		printf("%s: Error: Card not detected\n", __func__);
		return 0;
	}

	if (mmc_getwp(mmc)) {
		printf("%s: Failed due to write protect switch\n", __func__);
		return 0;
	}
	mmc_enable(mmc);
	if (((ulong)src) & 7) {
		debug("%s: Using bounce buffer due to alignment\n", __func__);
		do {
			memcpy(bounce_buffer, src, mmc->write_bl_len);
			if (mmc_write(mmc, start, 1, bounce_buffer) != 1)
				return 0;
			WATCHDOG_RESET();
			src += mmc->write_bl_len;
			start++;
			blocks_todo--;
		} while (blocks_todo > 0);
	} else {
		do {
			ulong ret;
			cur = min(blocks_todo, mmc->cfg->b_max);
			ret = mmc_write(mmc, start, cur, src);
			if (ret != cur) {
				printf("%s: ERROR: Not enough blocks written (%lu != %u)\n",
				       __func__, ret, (u32)cur);
				blkcnt = 0;
				break;
			}
			WATCHDOG_RESET();
			blocks_todo -= cur;
			start += cur;
			src += cur * mmc->write_bl_len;
		} while (blocks_todo > 0);
	}
	mmc_disable(mmc);
	return blkcnt;
}

/**
 * Performs a block erase operation on a MMC or SD device
 *
 * @param	dev_num		Device number to erase
 * @param	start		Starting block number
 * @param	blkcnt		Number of blocks to erase
 *
 * @return	Number of sectors actually erased or 0 if error
 */
static ulong
mmc_berase(int dev_num, lbaint_t start, lbaint_t blkcnt)
{
	int err = 0;
	struct mmc *mmc = find_mmc_device(dev_num);

	if (!mmc)
		return 0;

	debug("%s(%d, 0x%llx, 0x%llx)\n", __func__, dev_num,
	      (unsigned long long)start, (unsigned long long)blkcnt);
	if ((start % mmc->erase_grp_size) || (blkcnt % mmc->erase_grp_size))
		printf("\n\nCaution!  Your device's erase group is 0x%x\n"
		       "The erase range would be changed to 0x%llx~0x%llx\n\n",
		       mmc->erase_grp_size,
		       (uint64_t)(start & ~(mmc->erase_grp_size - 1)),
		       (uint64_t)((start + blkcnt + mmc->erase_grp_size)
				  & ~(mmc->erase_grp_size - 1)) - 1);

	mmc_enable(mmc);
	err = mmc_erase_t(mmc, start, blkcnt);
	mmc_disable(mmc);

	return err ? 0 : blkcnt;
}

void print_mmc_devices(char separator)
{
	struct mmc *m;
	struct list_head *entry;
	int rc;

	debug("%s(\'%c\')\n", __func__, separator);
        /* If nothing is available, try and initialize */
        if (list_empty(&mmc_devices)) {
		debug("%s(%c): No devices found, initializing\n",
		      __func__, separator);
                rc = mmc_initialize(gd->bd);

		debug("%s: Done initializing, rc: %d\n", __func__, rc);
		if (rc)
			return;
	}

        list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);
		printf("%s: %d", m->cfg->name, m->block_dev.dev);
		debug("(next: %p)\n", entry->next);
		if (entry->next != &mmc_devices)
			printf("%c ", separator);
	}
	printf("\n");
	debug("%s: Done.\n", __func__);
}

void print_mmc_device_info(struct mmc *mmc)
{
	struct mmc_host *host;
	const char *type;
	const char *version;
	uint32_t card_type;
	int prev = 0;
	int i;
	static const char *cbx_str[4] = {
		"Card (removable)",
		"BGA (Discrete embedded)",
		"POP",
		"Reserved"
	};
	static const char *power_classes[16] = {
		"100", "120", "150", "180", "200", "220", "250", "300",
		"350", "400", "450", "500", "600", "700", "800", ">800",
	};

	host = (struct mmc_host *)mmc->priv;
	card_type = host->ext_csd[EXT_CSD_CARD_TYPE];
	if (IS_SD(mmc)) {
		if (mmc->high_capacity)
			type = "SDHC or SDXC";
		else
			type = "SD";
	} else {
		type = "MMC";
	}

	switch (mmc->version) {
#ifdef CONFIG_OCTEON_MMC_SD
	case SD_VERSION_2:
	case SD_VERSION_3:
		if (mmc->scr[0] & (1 << 16)) {
			if (mmc->scr[0] & (1 << 10))
				version = "SD 4.XX";
			else
				version = "SD 3.0X";
		} else {
			version = "SD 2.00";
		}
		break;
	case SD_VERSION_1_10:		version = "SD 1.10";		break;
	case SD_VERSION_1_0:		version = "SD 1.0";		break;
#endif
	case MMC_VERSION_4:
		switch (host->ext_csd[EXT_CSD_REV]) {
			case 0:		version = "MMC v4.0";		break;
			case 1:		version = "MMC v4.1";		break;
			case 2:		version = "MMC v4.2";		break;
			case 3:		version = "MMC v4.3";		break;
			case 4:		version = "MMC v4.4 (obsolete)";break;
			case 5:		version = "MMC v4.41";		break;
			case 6:		version = "MMC v4.5/4.51";	break;
			case 7:		version = "MMC v5.0/v5.01";	break;
			case 8:		version = "MMC v5.1";		break;
			default:	version = "MMC > v5.1";		break;
		}
		break;
	case MMC_VERSION_3:		version = "MMC 3";		break;
	case MMC_VERSION_2_2:		version = "MMC 2.2";		break;
	case MMC_VERSION_1_4:		version = "MMC 1.4";		break;
	case MMC_VERSION_1_2:		version = "MMC 1.2";		break;
	case MMC_VERSION_UNKNOWN:	version = "MMC Unknown";	break;
	default:			version = "Unknown";		break;
	}

	printf("Name:                  %s\n", mmc->cfg->name);
	printf("Type:                  %s\n", type);
	printf("Version:               %s\n", version);
	printf("Manufacturer ID:       0x%02x\n", (mmc->cid[0] >> 24) & 0xff);
	if (IS_SD(mmc)) {
		printf("OEM ID:                %c%c\n",
		       (mmc->cid[0] >> 16) & 0xff, (mmc->cid[0] >> 8) & 0xff);
	} else {
		printf("Device Type:           %s\n",
		       cbx_str[(mmc->cid[0] >> 16) & 3]);
		printf("OEM ID:                0x%02x\n",
		       (mmc->cid[0] >> 8) & 0xff);
	}
	printf("Vendor:                %s\n", mmc->block_dev.vendor);
	printf("Product:               %s\n", mmc->block_dev.product);
	printf("Revision:              %s\n", mmc->block_dev.revision);
	if (IS_SD(mmc)) {
		printf("Manufacturing Date:    %d/%d\n",
		       (mmc->cid[3] >> 8) & 0xf,
		       ((mmc->cid[3] >> 12) & 0xff) + 2000);
	} else {
		int start_year;
		if ((host->ext_csd[EXT_CSD_REV] > 4)
		    && ((mmc->cid[3] >> 8) & 0xf) <= 12)
			start_year = 2013;
		else
			start_year = 1997;
		printf("Manufacturing Date:    %d/%d\n",
		       (mmc->cid[3] >> 12) & 0xf,
		       ((mmc->cid[3] >> 8) & 0xf) + start_year);
	}
	printf("Capacity:              %llu bytes (%llu blocks)\n",
	       mmc->capacity, mmc->capacity / mmc->read_bl_len);
	printf("Read block length:     %u\n", mmc->read_bl_len);
	printf("Write block length:    %u\n", mmc->write_bl_len);
	printf("High capacity:         %s\n", mmc->high_capacity ? "yes" : "no");
	printf("Bus width:             %u bits\n", mmc->bus_width);
	printf("Bus frequency:         %u\n", mmc->clock);
	if (!mmc->card_caps & MMC_MODE_HS)
		printf("Transfer frequency:    %u\n", mmc->tran_speed);
	printf("Bus DDR:               %s\n", host->ddr ? "yes" : "no");
	if (!IS_SD(mmc))
		printf("Erase group size:      %u\n", mmc->erase_grp_size);
	printf("Relative Card Address: 0x%x\n", mmc->rca);
	printf("Device is %sremovable\n", host->non_removable ? "non-" : "");
	if (IS_SD(mmc)) {
		const char *sd_security;
		uint8_t sd_spec, sd_spec3, sd_spec4;
		const char *spec_ver;
		const char *bus_widths;

		sd_spec = (mmc->scr[0] >> 24) & 0xf;
		sd_spec3 = (mmc->scr[0] >> 15) & 1;
		sd_spec4 = (mmc->scr[0] >> 10) & 1;
		printf("SCR register:          0x%08x %08x\n",
		       mmc->scr[0], mmc->scr[1]);
		printf(" structure version:    %s\n",
		       (mmc->scr[0] & 0xf0000000) ? "Unknown" : "1.0");
		if ((sd_spec == 0) && (sd_spec3 == 0) && (sd_spec4 == 0))
			spec_ver = "1.0 and 1.01";
		else if ((sd_spec == 1) && (sd_spec3 == 0) && (sd_spec4 == 0))
			spec_ver = "1.10";
		else if ((sd_spec == 2) && (sd_spec3 == 0) && (sd_spec4 == 0))
			spec_ver = "2.00";
		else if ((sd_spec == 2) && (sd_spec3 == 1) && (sd_spec4 == 0))
			spec_ver = "3.0X";
		else if ((sd_spec == 2) && (sd_spec3 == 1) && (sd_spec4 == 1))
			spec_ver = "4.XX";
		else
			spec_ver = "Reserved";
		printf(" Specification ver:    %s\n", spec_ver);
		printf(" Data stat after erase: %d\n", (mmc->scr[0] >> 23) & 1);
		switch((mmc->scr[0] >> 20) & 7) {
		case 0:
			sd_security = "None";
			break;
		case 1:
			sd_security = "Not Used";
			break;
		case 2:
			sd_security = "SDSC Card (Security Version 1.01)";
			break;
		case 3:
			sd_security = "SDHC Card (Security Version 2.00)";
			break;
		case 4:
			sd_security = "SDXC Card (Security Version 3.xx)";
			break;
		default:
			sd_security = "Reserved/Unknown";
			break;
		}
		printf(" SD Security:          %s\n", sd_security);
		switch ((mmc->scr[0] >> 16) & 0xf) {
		case 1:
			bus_widths = "1 bit (DAT0)";
			break;
		case 4:
			bus_widths = "4 bits (DAT0-3)";
			break;
		case 5:
			bus_widths = "1 bit (DAT0) and 4 bits (DAT0-3)";
			break;
		default:
			bus_widths = "Unknown";
		}
		printf(" SD Bus Widths:        %s\n", bus_widths);
		if ((mmc->scr[0] >> 11) & 7)
			printf("SD Extended Security supported\n");

		if (mmc->scr[0] & 8)
			printf(" Extension Register Mult-Block (CMD58/59) supported\n");
		if (mmc->scr[0] & 4)
			printf(" Extension Register Single Block (CMD48/49) supported\n");
		if (mmc->scr[0] & 2)
			printf(" SD Set Block Count (CMD23) supported\n");
		if (mmc->scr[0] & 1)
			printf(" SDXC Speed Class Control (CMD20) supported\n");
	} else {
		if (card_type != 0 && mmc->version == MMC_VERSION_4) {
			puts("Supported bus speeds: ");
			if (card_type & EXT_CSD_CARD_TYPE_26) {
				puts(" 26MHz");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_52) {
				if (prev)
					putc(',');
				puts(" 52MHz");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_DDR_1_8V) {
				if (prev)
					putc(',');
				puts(" DDR 1.8V, 3V");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_DDR_1_2V) {
				if (prev)
					putc(',');
				puts(" DDR 1.2V");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_HS200_1_8V) {
				if (prev)
					putc(',');
				puts(" HS200 1.8V");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_HS200_1_2V) {
				if (prev)
					putc(',');
				puts(" HS200 1.2V");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_HS400_1_8V) {
				if (prev)
					putc(',');
				puts(" HS400 1.8V");
				prev = 1;
			}
			if (card_type & EXT_CSD_CARD_TYPE_HS400_1_2V) {
				if (prev)
					putc(',');
				puts(" HS400 1.2V");
				prev = 1;
			}
			puts("\n");
		}
		printf("Current power Class:   %smA\n",
		       power_classes[host->ext_csd[EXT_CSD_POWER_CLASS] & 0xF]);
		printf("Power 4-bit@52MHz:     %smA\n",
		       power_classes[host->ext_csd[EXT_CSD_PWR_CL_52_360] & 0xF]);
		printf("Power 8-bit@52MHz:     %smA\n",
		       power_classes[(host->ext_csd[EXT_CSD_PWR_CL_52_360] >> 4) & 0xF]);
		printf("Power 4-bit@26MHz:     %smA\n",
		       power_classes[host->ext_csd[EXT_CSD_PWR_CL_26_360] & 0xF]);
		printf("Power 8-bit@26MHz:     %smA\n",
		       power_classes[(host->ext_csd[EXT_CSD_PWR_CL_26_360] >> 4) & 0xF]);
		printf("Power 4-bit@52MHz DDR: %smA\n",
		       power_classes[host->ext_csd[EXT_CSD_PWR_CL_DDR_52_360] & 0xF]);
		printf("Power 8-bit@52MHz DDR: %smA\n",
		       power_classes[(host->ext_csd[EXT_CSD_PWR_CL_DDR_52_360] >> 4) & 0xF]);
	}
	printf("OCR register:          0x%x\n", mmc->ocr);
	printf("CSD register:          0x%08x 0x%08x 0x%08x 0x%08x\n",
	       mmc->csd[0], mmc->csd[1], mmc->csd[2], mmc->csd[3]);
	if (!IS_SD(mmc)) {
		static const char *rst_n_function[] = {
			"RST_n signal is temporarily disabled",
			"RST_n signal is permanently enabled",
			"RST_n signal is permanently disabled",
			"Reserved"
		};
		const char *str;
		int life;

		printf("RST_n_FUNCTION:        %s\n",
		       rst_n_function[host->ext_csd[EXT_CSD_RST_N_FUNCTION] & 3]);
		switch (host->ext_csd[267]) {
		case 0:		str = "Not defined";	break;
		case 1:		str = "Normal";		break;
		case 2:		str = "Warning";	break;
		case 3:		str = "Urgent";		break;
		default:	str = "Reserved";	break;
		}
		printf("Pre EOL:               %s\n", str);

		puts("Life time estimation A: ");
		if (host->ext_csd[268] > 0 && host->ext_csd[268] < 0xb) {
			life = host->ext_csd[268] * 10;
			printf("Device Life time used: %d - %d\n",
			       life - 10, life);
		} else if (host->ext_csd[268] == 0xb) {
			puts("Device has exceeded its maximum estimated life time\n");
		} else {
			puts("Device life time unknown\n");
		}

		puts("Life time estimation B: ");
		if (host->ext_csd[269] > 0 && host->ext_csd[269] < 0xb) {
			life = host->ext_csd[269] * 10;
			printf("Device Life time used: %d - %d\n",
			       life - 10, life);
		} else if (host->ext_csd[269] == 0xb) {
			puts("Device has exceeded its maximum estimated life time\n");
		} else {
			puts("Device life time unknown\n");
		}

		puts("Extended CSD register:");
		for (i = 0; i < 512; i++) {
			if (i % 16 == 0)
				printf("\n%3u: ", i);
			if (i % 16 == 8)
				puts("- ");
			printf("%02x ", (uint32_t)host->ext_csd[i]);
		}
		puts("\n");
	}
}

/**
 * Sets the transfer block size which is usually 512 bytes
 *
 * @param mmc	pointer to mmc data structure
 * @param len	block length to use
 *
 * @return 0 for success, error otherwise
 */
int mmc_set_blocklen(struct mmc *mmc, int len)
{
	struct mmc_cmd cmd;
	int err;

	mmc_switch_dev(mmc);

	if (mmc->card_caps & MMC_MODE_DDR_52MHz) {
		debug("MMC set block length skipped in DDR mode\n");
		return 0;
	}

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = len;

	debug("%s: Setting block length to %d\n", __func__, len);
	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		printf("%s: Error setting block length to %d\n", __func__, len);

	return err;
}

#ifdef CONFIG_SYS_MMC_SET_DEV
int mmc_set_dev(int dev_num)
{
	debug("%s(%d)\n", __func__, dev_num);
	cur_dev_num = dev_num;
	return 0;
}
#endif

int __board_mmc_getcd(struct mmc *mmc)
{
	return -1;
}
int board_mmc_getcd(struct mmc *mmc)
	__attribute__((weak, alias("__board_mmc_getcd")));


int mmc_legacy_init(int dev_num)
{
	struct mmc *mmc;
	int rc;

	mmc = find_mmc_device(dev_num);
	if (mmc == NULL) {
		printf("Error: could not find MMC device %d\n", dev_num);
		return -1;
	}
	rc = mmc_init(mmc);
	if (rc) {
		printf("Error: could not initialize MMC device %d\n", dev_num);
	}
	return rc;
}

/**
 * Wait for a command to respond
 *
 * @param	mmc	MMC device
 * @param	bus_id	bus ID to wait on
 * @param	cmd_idx	command index to wait on
 * @param	flags	command flags
 * @param	timeout	timeout in ms, if 0 wait forever
 *
 * @return	0 if command returned within the timeout, TIMEOUT if command
 *		timed out or -1 if error.
 */
static int
oct_mmc_wait_cmd(struct mmc *mmc, int bus_id, int cmd_idx, int flags,
		 uint timeout)
{
	cvmx_mio_emm_rsp_sts_t emm_rsp_sts;
	unsigned long base_time;
	ulong time = 0;

	debug("%s(bus: %d, cmd: 0x%x, flags: 0x%x, timeout: %u\n", __func__,
	      bus_id, cmd_idx, flags, timeout);
	base_time = get_timer(0);

	do {
		emm_rsp_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
		if (emm_rsp_sts.s.cmd_done || emm_rsp_sts.s.rsp_timeout)
			break;
		WATCHDOG_RESET();
		udelay(1);
		time += 1;
	} while (!timeout || (get_timer(base_time) < timeout + 10));

	debug("%s: bus_id: %d, cmd_idx: %d response is 0x%llx after %lu ms (%lu loops)\n",
	      __func__, bus_id, cmd_idx, emm_rsp_sts.u64, get_timer(base_time),
	      time);
	if (!emm_rsp_sts.s.cmd_done) {
		debug("%s: Wait for command index %d timed out after %ums\n",
		      __func__, cmd_idx, timeout);
#ifdef DEBUG
		mmc_print_registers(mmc);
#endif
		return TIMEOUT;
	}
	if (bus_id >= 0 && emm_rsp_sts.s.bus_id != bus_id) {
		debug("%s: Mismatch bus_id, expected %d for cmd idx %d, got %d\n",
		      __func__, bus_id, cmd_idx, emm_rsp_sts.s.bus_id);
#ifdef DEBUG
		mmc_print_registers(NULL);
#endif
		return -1;	/* Wrong bus ID */
	}
	if (emm_rsp_sts.s.rsp_timeout
	    || (emm_rsp_sts.s.rsp_crc_err &&
		!(flags & MMC_CMD_FLAG_IGNORE_CRC_ERR))
	    || emm_rsp_sts.s.rsp_bad_sts) {
		uint64_t status = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO) >> 8;
		debug("%s: Bad response for bus id %d, cmd id %d:\n"
		      "    rsp_timeout: %d\n"
		      "    rsp_bad_sts: %d\n"
		      "    rsp_crc_err: %d\n",
		     __func__, bus_id, cmd_idx,
		     emm_rsp_sts.s.rsp_timeout,
		     emm_rsp_sts.s.rsp_bad_sts,
	             emm_rsp_sts.s.rsp_crc_err);
#ifdef DEBUG
		mmc_print_registers(NULL);
#endif
		if (emm_rsp_sts.s.rsp_type == 1 && emm_rsp_sts.s.rsp_bad_sts) {
			debug("    Response status: 0x%llx\n", status & 0xffffffff);
#ifdef DEBUG
			mmc_print_status(status);
			mmc_print_registers(NULL);
#endif
		}
		return -1;
	}
	if (emm_rsp_sts.s.cmd_idx != cmd_idx) {
		debug("%s: response for bus id %d, cmd idx %d mismatch command index %d\n",
		      __func__, bus_id, cmd_idx, emm_rsp_sts.s.cmd_idx);
#ifdef DEBUG
		mmc_print_registers(NULL);
#endif
		return -1;
	}
	return 0;
}

/**
 * Send a command with the specified timeout in milliseconds.
 *
 * @param mmc		pointer to MMC data structure
 * @param cmd		pointer to command data structure
 * @param data		pointer to data descriptor, NULL for none
 * @param flags		flags passed for the command
 * @param timeout	time to wait for command to complete in milliseconds
 *
 * @return		0 for success, error otherwise
 */
static int
mmc_send_cmd_timeout(struct mmc *mmc, struct mmc_cmd *cmd,
		     struct mmc_data *data,
		     uint32_t flags, uint timeout)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	cvmx_mio_emm_cmd_t emm_cmd;
	cvmx_mio_emm_buf_idx_t emm_buf_idx;
	cvmx_mio_emm_buf_dat_t emm_buf_dat;
	u64 resp_lo;
	u64 resp_hi;
	int i;
	int bus_id = host->bus_id;

	debug("%s(bus: %d, cmd: 0x%x, data: %p, flags: 0x%x, timeout: %u)\n",
	      __func__, bus_id, cmd->cmdidx, data, flags, timeout);
	if (bus_id != last_bus_id[host->node])
		mmc_switch_dev(mmc);

	/* Set the hardware timeout */
	mmc_set_watchdog(mmc, timeout ? timeout * 1000 : (1 << 26) - 1);

	/* Clear any interrupts */
	mmc_write_csr(mmc, CVMX_MIO_EMM_INT, mmc_read_csr(mmc, CVMX_MIO_EMM_INT));
	emm_cmd.u64 = 0;
	emm_cmd.s.cmd_val = 1;
	emm_cmd.s.bus_id = bus_id;
	emm_cmd.s.cmd_idx = cmd->cmdidx;
	emm_cmd.s.arg = cmd->cmdarg;
	emm_cmd.s.ctype_xor = (flags >> 16) & 3;
	emm_cmd.s.rtype_xor = (flags >> 20) & 7;
	emm_cmd.s.offset = (flags >> 24) & 0x3f;

	debug("mmc cmd: %d, arg: 0x%x flags: 0x%x reg: 0x%llx, bus id: %d\n",
	      cmd->cmdidx, cmd->cmdarg, flags, emm_cmd.u64, bus_id);

	if (data && data->flags & MMC_DATA_WRITE) {
		const char *src = data->src;
		if (!src) {
			printf("%s: Error, source buffer is NULL\n", __func__);
			return -1;
		}
		if (data->blocksize > 512) {
			printf("%s: Error: data size %u exceeds 512\n",
			       __func__, data->blocksize);
		}
		emm_buf_idx.u64 = 0;
		emm_buf_idx.s.inc = 1;
		mmc_write_csr(mmc, CVMX_MIO_EMM_BUF_IDX, emm_buf_idx.u64);
		for (i = 0; i < (data->blocksize + 7) / 8; i++) {
			memcpy(&emm_buf_dat.u64, src, sizeof(emm_buf_dat));
			mmc_write_csr(mmc, CVMX_MIO_EMM_BUF_DAT, emm_buf_dat.u64);
			debug("mmc cmd: buffer 0x%x: 0x%llx\n",
			      i*8, emm_buf_dat.u64);
			src += sizeof(emm_buf_dat);
		}
		debug("mmc cmd: wrote %d 8-byte blocks to buffer\n", i);
	}
	mmc_write_csr(mmc, CVMX_MIO_EMM_CMD, emm_cmd.u64);

	if (oct_mmc_wait_cmd(mmc, bus_id, cmd->cmdidx, flags, timeout)) {
		if (!init_time) {
			debug("mmc cmd: Error waiting for bus %d, command index %d to complete\n",
			       bus_id, cmd->cmdidx);
		}
		return TIMEOUT;
	}
	debug("%s: Response flags: 0x%x\n", __func__, cmd->resp_type);
	if (!cmd->resp_type & MMC_RSP_PRESENT) {
		debug("%s: no response expected for command index %d, returning\n",
		      __func__, cmd->cmdidx);
		return 0;
	}

	resp_lo = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO);
	if (cmd->resp_type & MMC_RSP_136) {
		resp_hi = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_HI);
		debug("mmc cmd: response hi: 0x%016llx\n", resp_hi);
		cmd->response[0] = resp_hi >> 32;
		cmd->response[1] = resp_hi & 0xffffffff;
		cmd->response[2] = resp_lo >> 32;
		cmd->response[3] = resp_lo & 0xffffffff;
	} else if ((cmd->resp_type & MMC_RSP_CRC)
		   || (flags & MMC_CMD_FLAG_STRIP_CRC)) {	/* No CRC */
		cmd->response[0] = (resp_lo >> 8) & 0xffffffff;
	} else {
		cmd->response[0] = resp_lo & 0xffffffff;
	}
	debug("mmc cmd: response lo: 0x%016llx\n", resp_lo);
	if (data && data->flags & MMC_DATA_READ) {
		char *dest = data->dest;

		if (!dest) {
			printf("%s: Error, destination buffer NULL!\n",
			       __func__);
			return -1;
		}
		if (data->blocksize > 512) {
			printf("%s: Error: data size %u exceeds 512\n",
			       __func__, data->blocksize);
		}
		emm_buf_idx.u64 = 0;
		emm_buf_idx.s.inc = 1;
		mmc_write_csr(mmc, CVMX_MIO_EMM_BUF_IDX, emm_buf_idx.u64);
		for (i = 0; i < (data->blocksize + 7) / 8; i++) {
			emm_buf_dat.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_BUF_DAT);
			memcpy(dest, &emm_buf_dat.u64, sizeof(emm_buf_dat));
			       dest += sizeof(emm_buf_dat);
		}
	}
	return 0;
}

/**
 * Send a command with the specified flags.
 *
 * @param mmc		pointer to MMC data structure
 * @param cmd		pointer to command data structure
 * @param data		pointer to data descriptor, NULL for none
 * @param flags		flags passed for the command
 *
 * @return		0 for success, error otherwise
 */
static int mmc_send_cmd_flags(struct mmc *mmc, struct mmc_cmd *cmd,
			      struct mmc_data *data, uint32_t flags)
{
	uint timeout;
	/**
	 * This constant has a 1 bit for each command which should have a short
	 * timeout and a 0 for each bit with a long timeout.  Currently the
	 * following commands have a long timeout:
	 *   CMD6, CMD17, CMD18, CMD24, CMD25, CMD32, CMD33, CMD35, CMD36 and
	 *   CMD38.
	 */
	static const uint64_t timeout_short = 0xFFFFFFA4FCF9FFDFULL;

	if (cmd->cmdidx >= 64) {
		printf("%s: Error: command index %d is out of range\n",
		       __func__, cmd->cmdidx);
		return -1;
	}
	if (timeout_short & (1ULL << cmd->cmdidx))
		timeout = MMC_TIMEOUT_SHORT;
	else if (cmd->cmdidx == 6 && IS_SD(mmc))
		timeout = 2560;
	else if (cmd->cmdidx == 38)
		timeout = MMC_TIMEOUT_ERASE;
	else
		timeout = MMC_TIMEOUT_LONG;
	debug("%s: CMD%d: timeout: %u\n", __func__, cmd->cmdidx, timeout);
	return mmc_send_cmd_timeout(mmc, cmd, data, flags, timeout);
}

/**
 * Sends a SD acmd with flags
 *
 * @param mmc		pointer to MMC data structure
 * @param cmd		pointer to command data structure
 * @param data		pointer to data descriptor, NULL for none
 * @param flags		flags passed for the command
 *
 * @return		0 for success, error otherwise
 */
static int mmc_send_acmd(struct mmc *mmc, struct mmc_cmd *cmd,
			 struct mmc_data *data, uint32_t flags)
{
	struct mmc_cmd acmd;
	int err;

	acmd.cmdidx = MMC_CMD_APP_CMD;
	acmd.cmdarg = mmc->rca << 16;
	acmd.resp_type = MMC_RSP_R1;

	if (!IS_SD(mmc)) {
		debug("%s: Error, not SD card\n", __func__);
		return -1;
	}
	err = octeon_mmc_send_cmd(mmc, &acmd, NULL);
	if (err) {
		printf("%s: Error sending ACMD to SD card\n", __func__);
		return err;
	}
	if (cmd)
		return mmc_send_cmd_flags(mmc, cmd, data, flags);
	else
		return 0;
}

/** Change the bus width */
static void mmc_set_bus_width(struct mmc *mmc, uint width)
{
	struct mmc_host *host = mmc->priv;
	mmc->bus_width = min(width, host->max_width);
}

static int mmc_pre_idle(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0xf0f0f0f0;	/* Software Reset */
	cmd.resp_type = MMC_RSP_NONE;

	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		debug("%s: error %d\n", __func__, err);
	else
		mdelay(20);
	return err;
}

static int mmc_go_idle(struct mmc* mmc)
{
	struct mmc_cmd cmd;
	int err;
	int i;

	for (i = 0; i < 5; i++) {
		debug("%s: Going idle try %d\n", __func__, i);
		/* Do this 5 times to clear the bus */
		cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_NONE;

		err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
		if (err)
			return err;
	}
	mdelay(20);	/* Wait 20ms */
	return 0;
}

#ifdef CONFIG_OCTEON_MMC_SD
static int sd_send_relative_addr(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;

	memset(&cmd, 0, sizeof(cmd));

	cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_R6;
	err = mmc_send_cmd_timeout(mmc, &cmd, NULL, MMC_CMD_FLAG_RTYPE_XOR(2), 2);
	if (err) {
		printf("%s: error sending command\n", __func__);
		return err;
	}
	mmc->rca = cmd.response[0] >> 16;
	mmc_write_csr(mmc, CVMX_MIO_EMM_RCA, mmc->rca);
	debug("%s: SD RCA is %d (0x%x)\n", __func__, mmc->rca, mmc->rca);
	debug("%s: MIO_EMM_RCA: 0x%llx\n", __func__,
	      mmc_read_csr(mmc, CVMX_MIO_EMM_RCA));
	return 0;
}
#endif

/**
 * Send the set relatice address command
 *
 * @param mmc	pointer to mmc data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_set_relative_addr(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	memset(&cmd, 0, sizeof(cmd));

	cmd.cmdidx = MMC_CMD_SET_RELATIVE_ADDR;
	cmd.cmdarg = mmc->rca << 16;
	cmd.resp_type = MMC_RSP_R1;
	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		printf("%s: Failed to set RCA to %d\n", __func__, mmc->rca);

	return err;
}

static int mmc_select_card(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	debug("%s: entry\n", __func__);
	memset(&cmd, 0, sizeof(cmd));

	cmd.cmdidx = MMC_CMD_SELECT_CARD;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = mmc->rca << 16;

	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		printf("%s: Error selecting card with rca %d\n",
		       __func__, mmc->rca);
	else
		mmc_write_csr(mmc, CVMX_MIO_EMM_RCA, mmc->rca);
	return err;
}

/**
 * Get the CID data structure from the MMC/SD device
 *
 * @param mmc	pointer to mmc data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_all_send_cid(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	memset(&cmd, 0, sizeof(cmd));

	debug("%s: Getting CID\n", __func__);
	cmd.cmdidx = MMC_CMD_ALL_SEND_CID;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err) {
		debug("%s: Error getting all CID\n", __func__);
		return err;
	}

	memcpy(mmc->cid, &cmd.response[0], 16);
#ifdef DEBUG
	print_buffer(0, mmc->cid, 1, 16, 0);
	debug("        Manufacturer: 0x%x\n", mmc->cid[0] >> 24);
	if (!IS_SD(mmc)) {
		debug("        Device/BGA:   ");
		switch ((mmc->cid[0] >> 16) & 3) {
		case 0:
			debug("Device (removable)\n");
			break;
		case 1:
			debug("BGA (Discrete embedded)\n");
			break;
		case 2:
			debug("POP\n");
			break;
		default:
			debug("Reserved\n");
			break;
		}
	}
	if (IS_SD(mmc)) {
		debug("        OID:          %02x%02x\n", (mmc->cid[0] >> 16) & 0xff,
		      (mmc->cid[0] >> 8) & 0xff);
		debug("        Product Name: %c%c%c%c%c\n", mmc->cid[0] & 0xff,
		      (mmc->cid[1] >> 24) & 0xff, (mmc->cid[1] >> 16) & 0xff,
		      (mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
		debug("        Product Revision: %d.%d\n",
		      (mmc->cid[2] >> 20) & 0xf, (mmc->cid[2] >> 16) & 0xf);
		debug("        Product Serial Number: 0x%x\n",
		      ((mmc->cid[2] & 0xffffff) << 8) | ((mmc->cid[3] >> 24) & 0xff));
		debug("        Manufacturing Date: %d/%d\n", (mmc->cid[3] >> 8) & 0xf,
		      ((mmc->cid[3] >> 12) & 0xff) + 2000);
	} else {
		debug("        OID:          0x%x\n", (mmc->cid[0] >> 8) & 0xFF);
		debug("        Product Name: %c%c%c%c%c%c\n", mmc->cid[0] & 0xff,
		      (mmc->cid[1] >> 24) & 0xff, (mmc->cid[1] >> 16) & 0xff,
		      (mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff,
		      (mmc->cid[2] >> 24) & 0xff);
		debug("        Product Revision: %d.%d\n",
		      (mmc->cid[2] >> 20) & 0xf, (mmc->cid[2] >> 16) & 0xf);
		debug("        Product Serial Number: 0x%x\n",
		      ((mmc->cid[2] & 0xffff) << 16) | ((mmc->cid[3] >> 16) & 0xffff));
		debug("        Manufacturing Date: %d/%d\n", (mmc->cid[3] >> 12) & 0xf,
		      ((mmc->cid[3] >> 8) & 0xf) + 2013);
	}
#endif
	return 0;
}

/**
 * Get the CSD data structure
 *
 * @param mmc	pointer to MMC data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_get_csd(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	memset(&cmd, 0, sizeof(cmd));

	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	err = octeon_mmc_send_cmd(mmc, &cmd, NULL);
	if (err) {
		printf("%s: Error getting CSD\n", __func__);
		return err;
	}
	mmc->csd[0] = cmd.response[0];
	mmc->csd[1] = cmd.response[1];
	mmc->csd[2] = cmd.response[2];
	mmc->csd[3] = cmd.response[3];
	debug("%s: CSD: 0x%08x 0x%08x 0x%08x 0x%08x\n", __func__,
	      mmc->csd[0], mmc->csd[1], mmc->csd[2], mmc->csd[3]);
	return 0;
}

#ifdef CONFIG_OCTEON_MMC_SD
static int sd_set_bus_width_speed(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;
#ifdef DEBUG
	struct mmc_host *host = mmc->priv;
	debug("%s(%d) width: %d %d\n", __func__, host->bus_id,
	      mmc->bus_width, host->bus_width);
#endif

	memset(&cmd, 0, sizeof(cmd));
	cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
	cmd.resp_type = MMC_RSP_R1;

	debug("card caps: 0x%x\n", mmc->card_caps);
	if (mmc->card_caps & MMC_MODE_4BIT) {
		/* Set SD bus width to 4 */
		cmd.cmdarg = 2;

		err = mmc_send_acmd(mmc, &cmd, NULL, 0);
		if (err) {
			printf("%s: Error setting bus width\n", __func__);
			return err;
		}
		mmc_set_bus_width(mmc, 4);
	} else {
		/* Set SD bus width to 1 */
		cmd.cmdarg = 0;

		err = mmc_send_acmd(mmc, &cmd, NULL, 0);
		if (err) {
			printf("%s: Error setting bus width\n", __func__);
			return err;
		}
		mmc_set_bus_width(mmc, 1);
	}
	if (mmc->card_caps & MMC_MODE_HS)
		mmc_set_clock(mmc, 50000000);
	else
		mmc_set_clock(mmc, 25000000);
	return sd_set_ios(mmc);
}
#endif

static int mmc_set_bus_width_speed(struct mmc *mmc)
{
	debug("%s: card caps: 0x%x\n", __func__, mmc->card_caps);
	if (mmc->card_caps & MMC_MODE_8BIT) {
		debug("%s: Set bus width to 8 bits\n", __func__);
		mmc_set_bus_width(mmc, 8);
	} else if (mmc->card_caps & MMC_MODE_4BIT) {
		debug("%s: Set bus width to 4 bits\n", __func__);
		/* Set the card to use 4 bit */
		mmc_set_bus_width(mmc, 4);
	}
	if (mmc->card_caps & MMC_MODE_HS) {
		debug("%s: Set high-speed mode\n", __func__);
		if (mmc->card_caps & MMC_MODE_HS_52MHz) {
			debug("%s: Set clock speed to 52MHz\n",
			      __func__);
			mmc_set_clock(mmc, 52000000);
		} else {
			mmc_set_clock(mmc, 26000000);
			debug("%s: Set clock speed to 26MHz\n",
			      __func__);
		}
	} else {
		debug("%s: Set clock speed to 20MHz\n", __func__);
		mmc_set_clock(mmc, 20000000);
	}
	mmc_set_ios(mmc);
	return 0;
}

#ifdef CONFIG_OCTEON_MMC_SD
int sd_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;
	uint32_t flags = MMC_CMD_FLAG_RTYPE_XOR(3) | MMC_CMD_FLAG_STRIP_CRC;

	debug("In %s\n", __func__);
	mmc->rca = 0;

	do {
		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		/*
		 * Most cards do not answer if some reserved bits
		 * in the ocr are set.  However, some controllers
		 * can set bit 7 (reserved low voltages), but
		 * how to manage low voltage SD cards is not yet
		 * specified.
		 */
		cmd.cmdarg = mmc->cfg->voltages & OCR_VOLTAGE_MASK;

		if ((mmc->version == SD_VERSION_1_0) ||
		    (mmc->version == SD_VERSION_1_10)) {
			cmd.cmdarg = 0x00ff8000;
			debug("%s: SD 1.X compliant card, voltages: 0x%x\n",
			      __func__, cmd.cmdarg);
		} else if (mmc->version == SD_VERSION_2) {
			cmd.cmdarg |= OCR_HCS;
			debug("%s: SD 2.0 compliant card, voltagess: 0x%x\n",
			      __func__, cmd.cmdarg);
		} else if (mmc->version == SD_VERSION_3) {
			cmd.cmdarg |= OCR_HCS | OCR_XPC;
			debug("%s: SD 3.0 compliant card, arg: 0x%x\n",
			      __func__, cmd.cmdarg);
		}

		err = mmc_send_acmd(mmc, &cmd, NULL, flags);
		if (err) {
			debug("%s: Error sending SD command, might be MMC\n",
			      __func__);
			return err;
		}

		debug("%s response: 0x%x\n", __func__, cmd.response[0]);
		mdelay(1);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0) {
		printf("%s: Timed out\n", __func__);
		return TIMEOUT;
	}

	if ((mmc->version != SD_VERSION_2) || (mmc->version != SD_VERSION_3))
		mmc->version = SD_VERSION_1_0;

	mmc->ocr = cmd.response[0];
	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);

	debug("%s: MMC high capacity mode %sdetected.\n",
	      __func__, mmc->high_capacity ? "" : "NOT ");
	return 0;
}
#endif

int mmc_send_op_cond(struct mmc *mmc)
{
	int timeout = WATCHDOG_COUNT;
	struct mmc_cmd cmd;
	int err;

	do {
		cmd.cmdidx = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = OCR_HCS | mmc->cfg->voltages;

		err = mmc_send_cmd_flags(mmc, &cmd, NULL, MMC_CMD_FLAG_STRIP_CRC);
		if (err) {
			if (!init_time)
				debug("%s: Returned %d\n", __func__, err);
			return err;
		}
		debug("%s: response 0x%x\n", __func__, cmd.response[0]);
		if (cmd.response[0] & OCR_BUSY)
			break;
		mdelay(1);
	} while (timeout--);

	if (timeout <= 0) {
		printf("%s: Timed out!", __func__);
		return TIMEOUT;
	}

	mmc->version = MMC_VERSION_UNKNOWN;
	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & 0x60000000) == OCR_HCS);

#ifdef DEBUG
	debug("%s: OCR: 0x%x\n", __func__, mmc->ocr);
	if (mmc->ocr & 0x80)
		debug("        1.70-1.95V\n");
	if (mmc->ocr & 0x3f00)
		debug("        2.0-2.6V\n");
	if (mmc->ocr & 0x007f8000)
		debug("        2.7-3.6V\n");
	debug("        Access Mode: %s\n",
	      (mmc->ocr & 0x40000000) == 0x40000000 ? "sector" : "byte");
	debug("        High Capacity: %s\n", mmc->ocr & OCR_HCS ? "yes" : "no");
#endif
	return 0;
}

/**
 * Get the extended CSD register
 *
 * @param mmc	pointer to mmc data structure
 *
 * @return 0 for success, error otherwise
 */
int mmc_send_ext_csd(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int err;
	int i __attribute__((unused));

	debug("%s: bus: %d\n", __func__, host->bus_id);
	mmc_switch_dev(mmc);
	mmc_set_blocklen(mmc, 512);

	cmd.cmdidx = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	data.dest = (char *)host->ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	err = octeon_mmc_send_cmd(mmc, &cmd, &data);

	if (err) {
		printf("%s: Error getting extended CSD\n", __func__);
	} else {
		debug("%s: Got good response\n", __func__);
		host->have_ext_csd = 1;
#ifdef DEBUG
		print_buffer(0, host->ext_csd, 1, 512, 0);
#endif
	}
	return err;
}

/* NOTE: We don't use this function since OCTEON handles this in hardware */
int mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		(index << 16) |
		(value << 8) | set;

	return octeon_mmc_send_cmd(mmc, &cmd, NULL);
}

int mmc_switch_part(int dev_num, unsigned int part_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return -1;

	debug("%s: Switching to partition %d\n", __func__, part_num);
	mmc_switch_dev(mmc);
	return mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONF,
			  (mmc->part_config & ~PART_ACCESS_MASK)
			  | (part_num & PART_ACCESS_MASK));
}
#ifdef CONFIG_OCTEON_MMC_SD
static int sd_set_ios(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int clock = mmc->clock;

	debug("%s: clock: %d (max %d), width %d\n",
	      __func__, clock, mmc->cfg->f_max, mmc->bus_width);
	if (mmc->bus_width > 4)
		mmc->bus_width = 4;
	host->clk_period = (host->sclock + clock - 1) / clock;
	host->power_class = 15;
	emm_switch.u64 = 0;
	emm_switch.s.hs_timing = mmc->clock > 20000000;
	debug("%s: hs timing: %d, caps: 0x%x\n", __func__,
	      emm_switch.s.hs_timing, mmc->card_caps);
	/* No DDR for now */
	host->bus_width = (mmc->bus_width == 4) ? 1 : 0;
	emm_switch.s.bus_width = host->bus_width;
	emm_switch.s.clk_hi = (host->clk_period + 1) / 2;
	emm_switch.s.clk_lo = (host->clk_period + 1) / 2;
	emm_switch.s.power_class = host->power_class;
	debug("%s: Writing emm_switch value 0x%llx\n",
	      __func__, emm_switch.u64);
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	emm_switch.s.bus_id = host->bus_id;
	udelay(100);
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	mdelay(20);
#ifdef DEBUG
	mmc_print_registers(mmc);
#endif
	return 0;
}
#endif

static void mmc_set_ios(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_rsp_sts_t emm_sts;
	cvmx_mio_emm_sample_t emm_sample;
	int switch_timeout_ms = 2550;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int timeout = 2000;
	char cardtype;
	int hs_timing = 0;
	int ddr = 0;
	int bus_width;
	int power_class;
	int clock = mmc->clock;
	uint32_t flags = 0;
	cvmx_mio_emm_rsp_lo_t emm_rsp_lo __attribute__((unused));
	int index;

	debug("In %s\n", __func__);
	debug("Starting clock is %uHz\n", clock);
	mmc->card_caps = 0;

	/* Only version 4 supports high speed */
	if (mmc->version < MMC_VERSION_4)
		return;

	if (clock == 0) {
		puts("mmc switch: Error, clock is zero!\n");
		return;
	}

	mmc_switch_dev(mmc);

	cardtype = host->ext_csd[EXT_CSD_CARD_TYPE] & 0x3f;
	if (cardtype == 7) {
		ddr = 1;
		mmc->card_caps |= MMC_MODE_HS | MMC_MODE_HS_52MHz |
				  MMC_MODE_DDR_52MHz;
		debug("%s: Dual voltage card type supports 52MHz DDR at 1.8 and 3V\n",
		      __func__);
	}
	debug("%s: card type flags (device_type): 0x%x\n", __func__, cardtype);
	hs_timing = false;
	if (cardtype & (1 << 2)) {
		hs_timing = true;
		ddr = true;
		mmc->card_caps |= MMC_MODE_DDR_52MHz;
		debug("        High-Speed eMMC %dMHz %sat 1.2v or 3v\n",
		      52, "DDR ");
	}
	if (cardtype & (1 << 3)) {
		/* Octeon can't take advantage of this mode */
		debug("        High-Speed eMMC %dMHz %sat 1.2v I/O\n",
		      52, "DDR ");
	}
	if (cardtype & EXT_CSD_CARD_TYPE_26) {
		hs_timing = true;
		debug("        High-Speed eMMC %dMHz %sat rated device voltage(s)\n",
		      26, "");
	}
	if (cardtype & EXT_CSD_CARD_TYPE_52) {
		debug("        High-Speed eMMC %dMHz %sat rated device voltage(s)\n",
		      52, "");
		mmc->card_caps |= MMC_MODE_HS_52MHz;
		hs_timing = true;
	}
	if (cardtype & EXT_CSD_CARD_TYPE_DDR_1_8V) {
		debug("        High-Speed DDR eMMC 52MHz at 1.8V or 3V I/O\n");
		hs_timing = true;
		if ((mmc->cfg->voltages & MMC_VDD_165_195)
		    || getenv("octeon_mmc_ddr"))
			ddr = true;
	}
	if (cardtype & EXT_CSD_CARD_TYPE_DDR_1_2V) {
		debug("        High-Speed DDR eMMC 52MHz at 1.2V I/O\n");
		/* hs_timing = true; */ /* We don't support 1.2V */
		/* DDR only works at 1.2V which OCTEON doesn't support */
	}
	if (cardtype & (1 << 4))
		debug("        HS200 Single Data Rate eMMC 200MHz at 1.8V I/O\n");
	if (cardtype & (1 << 5))
		debug("        HS200 Single Data Rate eMMC 200MHz at 1.2V I/O\n");
	if (cardtype & (1 << 6))
		debug("        HS400 DDR eMMC 200MHz at 1.8V I/O\n");
	if (cardtype & (1 << 7))
		debug("        HS400 DDR eMMC 200MHz at 1.2V I/O\n");
	if (!(cardtype & 0x7))
		hs_timing = false;

	mmc->bus_width = host->max_width;
	debug("        Max bus width: %d\n", host->max_width);
	/* Limit bus width to 4 for SD */
	if (IS_SD(mmc) && mmc->bus_width > 4)
		mmc->bus_width = 4;

	/* NOTE: We only use DDR mode for SD cards.  For MMC DDR will be enabled
	 * later after the bus width is detected since DDR mode is not allowed
	 * when detecting the bus width.
	 */
	switch (mmc->bus_width) {
	case 8:
		if (ddr)
			bus_width = EXT_CSD_DDR_BUS_WIDTH_8;
		else
			bus_width = EXT_CSD_BUS_WIDTH_8;
		break;
	case 4:
		if (ddr)
			bus_width = EXT_CSD_DDR_BUS_WIDTH_4;
		else
			bus_width = EXT_CSD_BUS_WIDTH_4;
		break;
	case 1:
		bus_width = EXT_CSD_BUS_WIDTH_1;
		break;
	default:
		printf("%s: Unknown bus width %d\n", __func__, mmc->bus_width);
		return;
	}

	if (hs_timing) {
		if ((cardtype & (MMC_HS_52MHZ | MMC_HS_DDR_52MHz_18_3V)) &&
		    (mmc->cfg->f_max >= 50000000)) {
			mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
			clock = min(52000000, mmc->cfg->f_max);
			debug("High-speed 52MHz timing mode detected\n");
		} else {
			mmc->card_caps |= MMC_MODE_HS;
			clock = min(26000000, mmc->cfg->f_max);
			debug("High-speed 26MHz timing mode detected\n");
		}
		if (ddr && (mmc->card_caps & MMC_MODE_HS_52MHz)) {
			mmc->card_caps |= MMC_MODE_DDR_52MHz;
			debug("DDR mode enabled\n");
		}
	} else {
		if (mmc->tran_speed)
			clock = min(clock, mmc->tran_speed);
		else
			clock = min(clock, 20000000);
		debug("High-speed clock mode NOT detected, setting to %dhz\n",
		      clock);
	}

#ifdef DEBUG
	debug("%s: extended CSD before switch_exe\n", __func__);
	mmc_send_ext_csd(mmc);
#endif
	host->bus_width = bus_width;
	debug("%s: Clock set to %u Hz\n", __func__, mmc->clock);
	switch_timeout_ms = host->ext_csd[EXT_CSD_GENERIC_CMD6_TIME] * 4;
	if (switch_timeout_ms == 0) {
		switch_timeout_ms = 2550 * 4;
		debug("extended CSD generic cmd6 timeout not specified\n");
	} else {
		debug("extended CSD generic cmd6 timeout %d ms\n",
		      switch_timeout_ms);
	}

	/* Adjust clock skew */
	emm_sample.u64 = 0;
	emm_sample.s.cmd_cnt = host->cmd_clk_skew;
	emm_sample.s.dat_cnt = host->dat_clk_skew;
	mmc_write_csr(mmc, CVMX_MIO_EMM_SAMPLE, emm_sample.u64);
	debug("%s: Setting command clock skew to %d, data to %d sclock cycles\n",
	      __func__, host->cmd_clk_skew, host->dat_clk_skew);

again:
	host->clk_period = (host->sclock + mmc->clock - 1) / mmc->clock;

	debug("%s: Setting clock period to %d for MMC clock: %d, hs: %d\n",
	      __func__, host->clk_period, mmc->clock, hs_timing);

	/* Set the watchdog since the switch operation can be long */
	mmc_set_watchdog(mmc, switch_timeout_ms * 1000);

	if (mmc->clock > 20000000) {

	}

	if (clock > 26000000) {
		if (bus_width & (EXT_CSD_DDR_BUS_WIDTH_4 | EXT_CSD_DDR_BUS_WIDTH_8))
			index = EXT_CSD_PWR_CL_DDR_52_360;
		else
			index = EXT_CSD_PWR_CL_52_360;
	} else {
		index = EXT_CSD_PWR_CL_26_360;
	}

	power_class = host->ext_csd[index];

	if (bus_width & (EXT_CSD_BUS_WIDTH_8 | EXT_CSD_DDR_BUS_WIDTH_8))
		power_class = (power_class >> 4) & 0xf;
	else
		power_class &= 0xf;

	debug("%s: Power class 0x%x selected\n", __func__, power_class);
	emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
	debug("%s: clock hi/low before emm_switch_exe operation: 0x%x/0x%x\n",
	      __func__, emm_switch.s.clk_hi, emm_switch.s.clk_lo);
	emm_switch.u64 = 0;
	emm_switch.s.bus_id = host->bus_id;
	emm_switch.s.switch_exe = 1;
	emm_switch.s.hs_timing = hs_timing;
	emm_switch.s.bus_width = bus_width;
	emm_switch.s.power_class = power_class;
	host->power_class = power_class;
	emm_switch.s.clk_hi = (host->clk_period + 1) / 2;
	emm_switch.s.clk_lo = (host->clk_period + 1) / 2;

	debug("%s: Writing 0x%llx to mio_emm_switch\n",
	      __func__, emm_switch.u64);
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	udelay(100);

	timeout = (switch_timeout_ms + 10) * 10;
	do {
		emm_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
		if (!emm_sts.s.switch_val)
			break;
		udelay(100);
	} while (timeout-- > 0);
	if (timeout <= 0) {
		printf("%s: switch command timed out, bus = %d, status=0x%llx\n",
		       __func__, host->bus_id, emm_sts.u64);
		return;
	}

	emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
	debug("Switch command response: 0x%llx, switch: 0x%llx, clk hi/lo: 0x%x, 0x%x\n",
	      emm_sts.u64, emm_switch.u64, emm_switch.s.clk_hi,
	      emm_switch.s.clk_lo);
#if defined(DEBUG)
	emm_rsp_lo.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_LO);
	debug("Switch response lo: 0x%llx\n", emm_rsp_lo.u64);
#endif

	if (emm_sts.s.rsp_crc_err && mmc->clock <= 20000000) {
		uint32_t next_speed;
		if (mmc->clock >= 4000000)
			next_speed = mmc->clock - 2000000;
		else if (mmc->clock > 1400000)
			next_speed = mmc->clock - 1000000;
		else if (mmc->clock > 400000)
			next_speed = 400000;
		else {
			printf("%s: CRC errors at 400KHz, MMC device unusable\n",
			       __func__);
			return;
		}
		printf("%s: CRC error communicating with MMC device at %uHz, trying %uHz.\n",
		       __func__, mmc->clock, next_speed);
		mmc->clock = next_speed;
		goto again;
	}

	if ((host->bus_id > 0) && !emm_switch.s.switch_err0 &&
	    !emm_switch.s.switch_err1 && !emm_switch.s.switch_err2) {
		/* If the bus id is non-zero then the changes might not
		 * actually occur.  In this case, if there are no errors we
		 * apply the changes to bus 0 as well as the current bus id.
		 *
		 * If there are errors below then the operation will be retried
		 * anyway.
		 */
		emm_switch.s.bus_id = 0;
		emm_switch.s.switch_exe = 0;
		emm_switch.s.hs_timing = hs_timing;
		emm_switch.s.bus_width = bus_width;
		emm_switch.s.power_class = power_class;
		emm_switch.s.clk_hi = emm_switch.s.clk_lo =
					(host->clk_period + 1) / 2;
		mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
		mdelay(1);
		emm_switch.s.bus_id = host->bus_id;
		mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
		mdelay(1);
	}
	if ((emm_switch.s.switch_err1 | emm_switch.s.switch_err2) &&
	    emm_sts.s.rsp_crc_err && mmc->clock > 20000000) {
		debug("%s: emm_switch error detected\n", __func__);
		mmc_print_registers(mmc);
		/* A CRC error occurred so try adjusting some things */
		/* If we're in DDR mode, turn off DDR. */
		switch (bus_width) {
		case EXT_CSD_DDR_BUS_WIDTH_8:
			bus_width = EXT_CSD_BUS_WIDTH_8;
			debug("%s: Switching from DDR 8 to non-DDR 8\n",
			      __func__);
			goto again;
		case EXT_CSD_DDR_BUS_WIDTH_4:
			bus_width = EXT_CSD_BUS_WIDTH_4;
			debug("%s: Switching from DDR 4 to non-DDR 4\n",
			      __func__);
			goto again;
		default:
			break;
		}
		/* A CRC error occurred during the switch operation, try slowing
		 * things down by 5MHz down to about 20MHz.
		 */
		mmc->clock = max(mmc->clock - 5000000, 20000000);
		if (mmc->clock <= 20000000)
			hs_timing = 0;
		host->clk_period = (host->sclock + mmc->clock - 1) / mmc->clock;
		debug("%s: bus_id %d detected CRC error, slowing clock down to %d and setting clock period to %d cycles\n",
		      __func__, host->bus_id, mmc->clock, host->clk_period);
		goto again;
	}
	if (emm_switch.s.switch_err0) {
		/* Error while performing POWER_CLASS switch */
		debug("%s: Error: Could not change power class to %d\n",
		      __func__, power_class);
	}
	if (emm_switch.s.switch_err1) {
		/* Error while performing HS_TIMING switch */
		if (ddr) {
			ddr = 0;
			debug("%s: Turning off DDR mode\n", __func__);
			mmc->card_caps &= ~MMC_MODE_DDR_52MHz;
			if (bus_width == EXT_CSD_DDR_BUS_WIDTH_8)
				bus_width = EXT_CSD_BUS_WIDTH_8;
			else if (bus_width == EXT_CSD_DDR_BUS_WIDTH_4)
				bus_width = EXT_CSD_BUS_WIDTH_4;
			goto again;
		}
		if (hs_timing) {
			if (clock >= 26000000) {
				clock = 26000000;
				debug("%s: Reducing clock to 26MHz\n", __func__);
			} else {
				hs_timing = 0;
				debug("%s: Turning off high-speed timing\n",
				      __func__);
			}
			mmc_set_clock(mmc, clock);
			goto again;
		}
		printf("%s: Error setting hs timing\n", __func__);
		return;
	}

	/* CMD19 and CMD14 are only supported for MMC devices and only in
	 * the single data rate mode.  In the dual data rate mode these
	 * commands are illegal.
	 */
	if (!IS_SD(mmc)) {	/* Only MMC supports bus testing */
		debug("Testing bus width %d\n", mmc->bus_width);
		/* Test bus width */
		if (!emm_switch.s.switch_err2 && !ddr &&
		    (bus_width != EXT_CSD_DDR_BUS_WIDTH_8) &&
		    (bus_width != EXT_CSD_DDR_BUS_WIDTH_4)) {
			/* Width succeeded, test the bus */
			struct mmc_cmd mmc_cmd;
			struct mmc_data mmc_data;
			uint8_t buffer[11];

			debug("Testing bus width %d (%d)\n",
			      mmc->bus_width, bus_width);
			mmc_data.src = (char *)buffer;
			mmc_data.blocks = 1;
			mmc_data.flags = MMC_DATA_WRITE;

			switch (mmc->bus_width) {
			case 8:
				buffer[0] = 0x55;
				buffer[1] = 0xaa;
				buffer[2] = 0x00;
				buffer[3] = 0x00;
				buffer[4] = 0x00;
				buffer[5] = 0x00;
				buffer[6] = 0x00;
				buffer[7] = 0x00;
				buffer[8] = 0x05;
				buffer[9] = 0xd4;
				buffer[10] = 0xff;
				mmc_data.blocksize = 11;
				break;
			case 4:
				buffer[0] = 0x5a;
				buffer[1] = 0x00;
				buffer[2] = 0x00;
				buffer[3] = 0x00;
				buffer[4] = 0x99;
				buffer[5] = 0x50;
				buffer[6] = 0x0f;
				mmc_data.blocksize = 7;
				break;
			case 1:
				buffer[0] = 0x80;
				buffer[1] = 0x70;
				buffer[2] = 0x78;
				buffer[3] = 0x01;
				mmc_data.blocksize = 4;
				break;
			default:
				printf("Unknown bus width %d\n", mmc->bus_width);
				return;
			}

#ifdef DEBUG
			print_buffer(0, buffer, 1, mmc_data.blocksize, 0);
#endif
			mmc_cmd.cmdarg = 0;
			mmc_cmd.cmdidx = 19;	/* BUSTEST_W */
			mmc_cmd.resp_type = MMC_RSP_R1;
	        	if (octeon_mmc_send_cmd(mmc, &mmc_cmd, &mmc_data) != 0)
        			puts("Warning: problem sending BUSTEST_W command\n");

			debug("BUSTEST_W response is 0x%x 0x%x 0x%x 0x%x\n",
			      mmc_cmd.response[0], mmc_cmd.response[1],
			      mmc_cmd.response[2], mmc_cmd.response[3]);
			mdelay(1);

			mmc_data.blocksize -= 2;
			memset(buffer, 0, sizeof(buffer));
			mmc_cmd.cmdarg = 0;
			mmc_cmd.cmdidx = 14;	/* BUSTEST_R */
			mmc_cmd.resp_type = MMC_RSP_R1;
        		flags = MMC_CMD_FLAG_OFFSET(63);
			memset(buffer, 0, sizeof(buffer));
			mmc_data.dest = (char *)buffer;
			mmc_data.blocks = 1;
			mmc_data.flags = MMC_DATA_READ;
        		if (mmc_send_cmd_flags(mmc, &mmc_cmd, &mmc_data, flags) != 0)
        			puts("Warning: problem sending BUSTEST_R command\n");

			debug("BUSTEST_R response is 0x%x %x %x %x\n",
			      mmc_cmd.response[0], mmc_cmd.response[1],
			      mmc_cmd.response[2], mmc_cmd.response[3]);
#ifdef DEBUG
			mmc_send_ext_csd(mmc);
#endif
			switch (bus_width) {
			case EXT_CSD_DDR_BUS_WIDTH_8:
				if (buffer[0] != 0xaa || buffer[1] != 0x55) {
					debug("DDR Bus width 8 test failed, returned "
					      "0x%02x%02x, expected 0xAA55, trying "
					      "bus width 8\n",
					      buffer[0], buffer[1]);
					bus_width = EXT_CSD_DDR_BUS_WIDTH_4;
					mmc->bus_width = 4;
					goto again;
				}
				break;
			case EXT_CSD_DDR_BUS_WIDTH_4:
				if (buffer[0] != 0xa5) {
					debug("DDR Bus width 4 test failed, returned "
					      "0x%02x%02x, expected 0xA5, trying "
					      "bus width %d\n",
					      buffer[0], buffer[1], host->max_width);
					bus_width = (host->max_width == 8) ?
							EXT_CSD_BUS_WIDTH_8
							: EXT_CSD_BUS_WIDTH_4;
					mmc->bus_width = host->max_width;
					goto again;
				}
				break;
			case EXT_CSD_BUS_WIDTH_8:
				if (buffer[0] != 0xaa || buffer[1] != 0x55) {
					debug("Bus width 8 test failed, returned "
					      "0x%02x%02x, expected 0xAA55, trying bus width 4\n",
					      buffer[0], buffer[1]);
					bus_width = EXT_CSD_BUS_WIDTH_4;
					mmc->bus_width = 4;
					goto again;
				}
				break;
			case EXT_CSD_BUS_WIDTH_4:
				if (buffer[0] != 0xa5) {
					debug("DDR bus width 4 test failed, returned "
					      "0x%02x, expected 0xA5, trying bus width 1\n",
					      buffer[0]);
					bus_width = EXT_CSD_BUS_WIDTH_1;
					mmc->bus_width = 1;
					goto again;
				}
				break;
			case EXT_CSD_BUS_WIDTH_1:
				if ((buffer[0] & 0xc0) != 0x40) {
					debug("DDR bus width 1 test failed, returned "
					      "0x%02x, expected 0x4x, trying bus width 1\n",
					      buffer[0]);
					return;
				}
				break;
			default:
				break;
			}
		}

		if (emm_switch.s.switch_err2) {
			/* Error while performing BUS_WIDTH switch */
			switch (bus_width) {
			case EXT_CSD_DDR_BUS_WIDTH_8:
				debug("DDR bus width 8 failed, trying DDR bus width 4\n");
				bus_width = EXT_CSD_DDR_BUS_WIDTH_4;
				goto again;
			case EXT_CSD_DDR_BUS_WIDTH_4:
				debug("DDR bus width 4 failed, trying bus width %d\n",
				      host->max_width);
				bus_width = (host->max_width == 8) ?
						EXT_CSD_BUS_WIDTH_8
						: EXT_CSD_BUS_WIDTH_4;
				goto again;
			case EXT_CSD_BUS_WIDTH_8:
				debug("Bus width 8 failed, trying bus width 4\n");
				bus_width = EXT_CSD_BUS_WIDTH_4;
				goto again;
			case EXT_CSD_BUS_WIDTH_4:
				debug("Bus width 4 failed, trying bus width 1\n");
				bus_width = EXT_CSD_BUS_WIDTH_1;
				goto again;
			default:
				printf("%s: Could not set bus width\n", __func__);
				return;
			}
		}
	}

	if (ddr && hs_timing && clock >= 26000000) {
		if (clock >= 52000000)
			mmc->card_caps |= MMC_MODE_DDR_52MHz;

		switch (mmc->bus_width) {
		case 8:
			bus_width = EXT_CSD_DDR_BUS_WIDTH_8;
			break;
		case 4:
			bus_width = EXT_CSD_DDR_BUS_WIDTH_4;
			break;
		default:
			puts("Error: MMC DDR mode only supported with bus widths of 4 or 8!\n");
			return;
		}
		emm_switch.u64 = 0;
		emm_switch.s.bus_id = host->bus_id;
		emm_switch.s.switch_exe = 1;
		emm_switch.s.hs_timing = 1;
		emm_switch.s.bus_width = bus_width;
		emm_switch.s.power_class = host->power_class;
		emm_switch.s.clk_hi = (host->clk_period + 1) / 2;
		emm_switch.s.clk_lo = (host->clk_period + 1) / 2;
		mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
		udelay(100);

		timeout = switch_timeout_ms + 10;
		do {
			emm_sts.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_RSP_STS);
			if (!emm_sts.s.switch_val)
				break;
			mdelay(1);
		} while (timeout-- > 0);
		if (timeout < 0) {
			printf("Error: MMC timed out when converting to DDR mode\n");
			return;
		}
	}
	/* Store the bus width */
	host->bus_width = emm_switch.s.bus_width;
	host->ddr = ddr;
	/* Set watchdog for command timeout */
	mmc_set_watchdog(mmc, 1000000);

	debug("%s: Set hs: %d, ddr: %s, clock: %u, bus width: %d, power class: %d\n",
	      __func__, hs_timing, ddr ? "yes" : "no",
	      mmc->clock, bus_width, power_class);

	/* Re-read the extended CSD register since it has likely changed */
	mmc_send_ext_csd(mmc);

	return;
}

/* Set the clock speed.
 *
 * NOTE: The clock speed will be limited to the maximum supported clock speed.
 */
void mmc_set_clock(struct mmc *mmc, uint clock)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	cvmx_mio_emm_switch_t emm_switch;
	unsigned bus;

	debug("%s: min: %u, max: %u, trans: %u, hs: %u, set: %u\n",
	      __func__, mmc->cfg->f_min, mmc->cfg->f_max, mmc->tran_speed,
	      (mmc->card_caps & MMC_MODE_HS) ? 1 : 0, clock);
	if (clock == 0) {
		printf("%s: ERROR: Cannot set clock to zero!\n", __func__);
		return;
	}
	clock = min(clock, mmc->cfg->f_max);
	clock = max(clock, mmc->cfg->f_min);
	if (mmc->tran_speed && !(mmc->card_caps & MMC_MODE_HS)) {
		clock = min(clock, mmc->tran_speed);
		debug("%s: Limiting clock to trans speed %u\n",
		      __func__, mmc->tran_speed);
	}
	debug("%s: Setting clock to %uHz\n", __func__, clock);
	mmc->clock = clock;
	host->clk_period = (host->sclock + clock - 1) / clock;

	/* Write the change to the hardware */
	emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
	emm_switch.s.clk_hi = emm_switch.s.clk_lo = (host->clk_period + 1) / 2;
	bus = emm_switch.s.bus_id;

	emm_switch.s.bus_id = 0;
	emm_switch.s.hs_timing = (mmc->clock > 20000000);
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	udelay(1200);
	emm_switch.s.bus_id = bus;
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	udelay(1200);

	mmc_set_watchdog(mmc, 1000000);
}

#ifdef CONFIG_OCTEON_MMC_SD
int sd_switch(struct mmc *mmc, int mode, int group, u8 value, u32 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err;

	debug("%s(mmc, %d, %d, %u, 0x%p)\n",
	      __func__, mode, group, value, resp);
	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (mode << 31) | 0xffffff;
	cmd.cmdarg &= ~(0xf << (group * 4));
	cmd.cmdarg |= value << (group * 4);

	data.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd_flags(mmc, &cmd, &data,  MMC_CMD_FLAG_CTYPE_XOR(1));
	if (err) {
		printf("%s: failed, rc: %d\n", __func__, err);
		return err;
	}
	memcpy(resp, &cmd.response[0], sizeof(cmd.response));
	return 0;
}

int sd_change_freq(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;
	uint32_t scr[2];
	uint32_t switch_status[16];
	struct mmc_data data;
	int timeout;
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	uint32_t flags;
	int i __attribute__((unused));


	debug("In %s\n", __func__);

#ifdef DEBUG
	memset(scr, 0x55, sizeof(scr));
	memset(switch_status, 0xaa, sizeof(switch_status));
#endif
	mmc->card_caps = 0;

	cmd.cmdidx = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	flags = MMC_CMD_FLAG_RTYPE_XOR(1) | MMC_CMD_FLAG_CTYPE_XOR(1)
		    | MMC_CMD_FLAG_OFFSET(63);

	timeout = 3;

retry_scr:
	data.dest = (char *)&scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	err = mmc_send_acmd(mmc, &cmd, &data, flags);

	if (err) {
		debug("Retrying send SCR\n");
		if (timeout--)
			goto retry_scr;

		return err;
	}

	mmc->scr[0] = __be32_to_cpu(scr[0]);
	mmc->scr[1] = __be32_to_cpu(scr[1]);

	debug("%s: SCR=0x%08x 0x%08x\n", __func__, mmc->scr[0], mmc->scr[1]);
	switch ((mmc->scr[0] >> 24) & 0xf) {
		case 0:
			mmc->version = SD_VERSION_1_0;
			break;
		case 1:
			mmc->version = SD_VERSION_1_10;
			break;
		case 2:
			if ((mmc->scr[0] >> 15) & 1)
				/* NOTE: should check SD version 4 here but
				 * U-Boot doesn't yet define this.
				 */
				mmc->version = SD_VERSION_3;
			else
				mmc->version = SD_VERSION_2;
			break;
		default:
			mmc->version = SD_VERSION_1_0;
			break;
	}

	/* Version 1.0 doesn't support switching */
	if (mmc->version == SD_VERSION_1_0) {
		debug("%s: Returning for SD version 1.0\n", __func__);
		return 0;
	}

	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, 0, 1, switch_status);

		if (err) {
			debug("%s: Error calling sd_switch\n", __func__);
			return err;
		}

		/* The high-speed function is busy.  Try again */
		if (!(__be32_to_cpu(switch_status[7]) & SD_HIGHSPEED_BUSY)) {
			debug("%s: high speed function is !busy, done\n",
			      __func__);
			break;
		}
		mdelay(1);
	}

#ifdef DEBUG
	for (i = 0; i < 16; i++) {
		if (!(i & 3))
			debug("\n%02x: ", i * 4);
		debug("%08x ", switch_status[i]);
	}
	puts("\n");
#endif

	if (mmc->scr[0] & SD_DATA_4BIT && host->max_width >= 4) {
		mmc->card_caps |= MMC_MODE_4BIT;
		mmc->bus_width = 4;
		debug("%s: SD 4 bit mode detected\n", __func__);
	} else {
		debug("%s: SD 4-bit mode NOT detected\n", __func__);
	}
	if (mmc->scr[0] & 2) {
		host->flags |= OCTEON_MMC_FLAG_SD_CMD23;
		debug("%s: SD CMD23 support detected\n", __func__);
	}
#ifdef DEBUG
	debug("        max current: %u ma\n", switch_status[0] >> 16);
	if (switch_status[0] & 0xffff) {
		debug("        Group 6 functions supported: ");
		for (i = 0; i < 16; i++)
			if ((switch_status[0] >> i) & 1)
				debug("%i ", i);
		debug("\n");
	}
	if (switch_status[1] & 0xffff0000) {
		debug("        Group 5 functions supported: ");
		for (i = 0; i < 16; i++)
			if ((switch_status[1] >> (i + 16)) & 1)
				debug("%i ", i);
		debug("\n");
	}
	if (switch_status[1] & 0xffff) {
		debug("        Group 4 functions supported: ");
		for (i = 0; i < 16; i++)
			if ((switch_status[1] >> i) & 1)
				debug("%i ", i);
		debug("\n");
	}
	if (switch_status[2] & 0xffff0000) {
		debug("        Group 3 functions supported: ");
		for (i = 0; i < 16; i++)
			if ((switch_status[2] >> (i + 16)) & 1)
				debug("%i ", i);
		debug("\n");
	}
	if (switch_status[2] & 0x0000ffff) {
		debug("        Group 2 functions supported: ");
		for (i = 0; i < 16; i++)
			if ((switch_status[1] >> i) & 1)
				debug("%i ", i);
		debug("\n");
	}
	if (switch_status[3] & 0xffff0000) {
		debug("        Group 1 functions supported: ");
		for (i = 0; i < 16; i++)
			if ((switch_status[2] >> (i + 16)) & 1)
				debug("%i ", i);
		debug("\n");
	}
	if (switch_status[3] & 0x0000f000)
		debug("        Group 6 functions selected: 0x%x\n",
		      (switch_status[3] >> 12) & 0xF);
	if (switch_status[3] & 0x00000f00)
		debug("        Group 5 functions selected: 0x%x\n",
		      (switch_status[3] >> 8) & 0xF);

	if (switch_status[3] & 0x000000f0)
		debug("        Group 4 functions selected: 0x%x\n",
		      (switch_status[3] >> 4) & 0xF);
	if (switch_status[3] & 0x0000000f)
		debug("        Group 3 functions selected: 0x%x\n",
		      switch_status[3] & 0xF);
	if (switch_status[4] & 0xf0000000)
		debug("        Group 2 functions selected: 0x%x\n",
		      (switch_status[4] >> 28) & 0xF);
	if (switch_status[4] & 0x0f000000)
		debug("        Group 1 functions selected: 0x%x\n",
		      (switch_status[4] >> 24) & 0xF);
	debug("        Data structure version: %d\n",
	      (switch_status[4] >> 16) & 0xff);

	if (!(__be32_to_cpu(switch_status[4] & SD_HIGHSPEED_SUPPORTED)))
		debug("%s: high speed mode not supported\n", __func__);
	else
		debug("%s: high speed mode supported\n", __func__);

#endif	/* DEBUG */

	err = sd_switch(mmc, SD_SWITCH_SWITCH, 0, 1, switch_status);

	if (err) {
		debug("%s: switch failed\n", __func__);
		return err;
	}

#ifdef DEBUG
	for (i = 0; i < 16; i++) {
		if (!(i & 3))
			debug("\n%02x: ", i * 4);
		debug("%08x ", switch_status[i]);
	}
	puts("\n");
#endif
	if ((__be32_to_cpu(switch_status[4]) & 0x0f000000) == 0x01000000) {
		mmc->card_caps |= MMC_MODE_HS;
		debug("%s: High speed mode supported\n", __func__);
	}

	return 0;
}
#endif

#ifdef CONFIG_OCTEON_MMC_SD
static int sd_version_1_x(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;
	uint32_t flags;
	ulong start;

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	cmd.cmdarg = 0;
	flags = MMC_CMD_FLAG_CTYPE_XOR(1);

	err = mmc_send_cmd_timeout(mmc, &cmd, NULL, flags, 5);
	if (!err) {
		mmc->version = SD_VERSION_2;
		return 0;
	}

	err = mmc_go_idle(mmc);
	if (err) {
		debug("%s: mmc_go_idle() returned error\n", __func__);
	}
	start = get_timer(0);
	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1;
		err = mmc_send_cmd_flags(mmc, &cmd, NULL, 0);
		if (err) {
			debug("%sL ACMD failed\n", __func__);
			return err;
		}
		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.cmdarg = 0x00ff8000;
		cmd.resp_type = MMC_RSP_R3;
		flags = MMC_CMD_FLAG_RTYPE_XOR(3) | MMC_CMD_FLAG_STRIP_CRC;
		err = mmc_send_cmd_flags(mmc, &cmd, NULL, flags);
		if (err) {
			debug("%s: ACMD41 failed\n", __func__);
			return err;
		}
		debug("%s: ACMD41 response: 0x%x\n", __func__, cmd.response[0]);
	} while (!(cmd.response[0] & OCR_BUSY) && get_timer(start) < 100);
	if (!(cmd.response[0] & OCR_BUSY)) {
		debug("%s: ACMD41 timed out\n", __func__);
		return -1;
	}
	return 0;
}
#endif

int mmc_send_if_cond(struct mmc *mmc)
{
#ifdef CONFIG_OCTEON_MMC_SD
	struct mmc_cmd cmd;
	int err;
	uint32_t flags;

	/* We only need a very short timeout here, 5ms */
	mmc_set_watchdog(mmc, 5000);

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	/* We set the bit if the host supports voltages between 2.7 and 3.6 V */
	cmd.cmdarg = ((mmc->cfg->voltages & OCR_VOLTAGE_MASK) != 0) << 8 | 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	flags = MMC_CMD_FLAG_CTYPE_XOR(1) | MMC_CMD_FLAG_RTYPE_XOR(2);

	err = mmc_send_cmd_timeout(mmc, &cmd, NULL, flags, 5);

	if (err) {
		debug("%s failed\n", __func__);
		err = sd_version_1_x(mmc);
		if (err) {
			return err;
		} else {
			debug("%s: detected SD version 1.x\n", __func__);
			mmc->version = SD_VERSION_1_0;
			mmc_go_idle(mmc);
			return 0;
		}
	}

	if ((cmd.response[0] & 0xff) != 0xaa) {
		debug("%s: Unusable error, response is 0x%x\n",
		      __func__, cmd.response[0]);
		return UNUSABLE_ERR;
	} else {
		mmc->version = SD_VERSION_2;
		debug("%s: SD version 2 detected\n", __func__);
	}
#endif
	return 0;
}

void mmc_reset_bus(struct mmc *mmc, int preserve_switch)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	cvmx_mio_emm_cfg_t emm_cfg;
	cvmx_mio_emm_switch_t emm_switch;

	if (preserve_switch) {
		emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
		if (emm_switch.s.bus_id != host->bus_id) {
			emm_switch.s.bus_id = host->bus_id;
			mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
		}
	}

	/* Reset the bus */
	emm_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CFG);
	emm_cfg.u64 &= ~(1 << host->bus_id);
	mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);
	mdelay(20);	/* Wait 20ms */
	emm_cfg.u64 |= 1 << host->bus_id;
	mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);

	mdelay(20);

	/* Restore switch settings */
	if (preserve_switch) {
		emm_switch.s.switch_exe = 0;
		emm_switch.s.switch_err0 = 0;
		emm_switch.s.switch_err1 = 0;
		emm_switch.s.switch_err2 = 0;
		emm_switch.s.clk_hi = (host->clk_period + 1) / 2;
		emm_switch.s.clk_lo = (host->clk_period + 1) / 2;
		emm_switch.s.hs_timing = (mmc->clock > 20000000);
		emm_switch.s.bus_id = 0;
		mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
		udelay(100);
		emm_switch.s.bus_id = host->bus_id;
		mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	}
}

int mmc_startup(struct mmc *mmc)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	u64 cmult, csize, capacity;
	int err;
	uint mult, freq;
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_cfg_t emm_cfg;
	cvmx_mio_emm_sts_mask_t emm_sts_mask;
	cvmx_mio_emm_wdog_t emm_wdog;
	int i __attribute__((unused));
	int classes __attribute__((unused));
	debug("%s: bus_id: %d\n", __func__, host->bus_id);

	mmc->rca = 0;

	/* Clear interrupt status */
	mmc_write_csr(mmc, CVMX_MIO_EMM_INT, mmc_read_csr(mmc, CVMX_MIO_EMM_INT));

	/* Enable the bus */
	emm_cfg.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_CFG);
	emm_cfg.u64 |= (1 << host->bus_id);
	debug("%s: writing 0x%llx to mio_emm_cfg\n", __func__, emm_cfg.u64);
	mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);
	mdelay(2);

	/* Set clock period */
	host->clk_period = (host->sclock + mmc->clock - 1) / mmc->clock;

	/* Default to RCA of 1 */
	mmc_write_csr(mmc, CVMX_MIO_EMM_RCA, 1);

	/* Set the bus speed and width */
	emm_switch.u64 = 0;
	emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_1;
	emm_switch.s.power_class = 10;
	emm_switch.s.clk_hi = emm_switch.s.clk_lo = (host->clk_period + 1) / 2;
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	emm_switch.s.bus_id = host->bus_id;
	udelay(1200);
	mmc_write_csr(mmc, CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	udelay(1200);

	last_bus_id[host->node] = host->bus_id;

#ifdef DEBUG
	debug("%s: Set clock period to %d clocks, sclock: %u\n", __func__,
	      emm_switch.s.clk_hi + emm_switch.s.clk_lo, host->sclock);
#endif
	/* Set watchdog for command timeout */
	if (host->bus_id == 0)
		emm_wdog.u64 = 0;
	else
		emm_wdog.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_WDOG);
	emm_wdog.s.clk_cnt = mmc->clock;
	debug("Setting command timeout value to %u\n", emm_wdog.s.clk_cnt);
	mmc_write_csr(mmc, CVMX_MIO_EMM_WDOG, emm_wdog.u64);

	mdelay(10);	/* Wait 10ms */

	/* Set status mask */
	emm_sts_mask.u64 = 0;
	emm_sts_mask.s.sts_msk = 1 << 7 | 1 << 22 | 1 << 23 | 1 << 19;
	mmc_write_csr(mmc, CVMX_MIO_EMM_STS_MASK, emm_sts_mask.u64);

	/* Reset the card */
	debug("Resetting card\n");
	if ((err = mmc_pre_idle(mmc))) {
		mmc_print_registers(mmc);
		if (!init_time)
			printf("%s: Could not enter pre-idle state\n", __func__);
		return err;
	}
	if ((err = mmc_go_idle(mmc))) {
		mmc_print_registers(mmc);
		if (!init_time)
			printf("%s: Could not reset MMC card\n", __func__);
		return err;
	}

#ifdef CONFIG_OCTEON_MMC_SD
	/* Note that this doesn't work on the CN61XX pass 1.0.
	 * The CN61XX pass 1.0 has an errata where only 8-bit wide buses are
	 * supported due to checksum errors on narrower busses.
	 */
	debug("Testing for SD version 2, voltages: 0x%x\n", mmc->cfg->voltages);
	/* Test for SD version 2 */
	err = mmc_send_if_cond(mmc);
#endif
	/* Now try to get the SD card's operating condition */
	if (!err && IS_SD(mmc)) {
#ifdef CONFIG_OCTEON_MMC_SD
		debug("Getting SD card operating condition\n");
		err = sd_send_op_cond(mmc);
		if (err == TIMEOUT) {
			debug("Cannot get SD operating condition, trying MMC\n");
			err = mmc_send_op_cond(mmc);
			if (err) {
				printf("Card did not respond to voltage select!\n");
				return UNUSABLE_ERR;
			}
		}

		if (err) {
			err = mmc_send_op_cond(mmc);
			if (err) {
				puts("MMC Init: Error recovering after SD Version 2 test\n");
				return UNUSABLE_ERR;
			}
		}
#endif
	} else {
		mdelay(100);
		/* Clear interrupt status */
		mmc_write_csr(mmc, CVMX_MIO_EMM_INT, mmc_read_csr(mmc, CVMX_MIO_EMM_INT));
		debug("Resetting card for MMC\n");
		mmc_pre_idle(mmc);
		mdelay(2);
		mmc_go_idle(mmc);
		mdelay(2);

		debug("Getting MMC card operating condition\n");
		err = mmc_send_op_cond(mmc);
		if (err == TIMEOUT) {
			debug("Trying again...\n");
			/* Resetting MMC bus */
			mmc_reset_bus(mmc, TRUE);
			debug("%s: Going idle\n", __func__);
			mmc_pre_idle(mmc);
			mmc_go_idle(mmc);
			err = mmc_send_op_cond(mmc);
			if (err) {
				debug("MMC Init: Card did not respond to voltage select, might be 1.x SD card\n");
				mmc->version = SD_VERSION_1_0;
				mmc_reset_bus(mmc, TRUE);
				mmc_go_idle(mmc);
			}
		} else if (err) {
			debug("Resetting MMC bus\n");
			/* Resetting MMC bus */
			mmc_reset_bus(mmc, TRUE);
			debug("%s: Going idle\n", __func__);
			mmc_pre_idle(mmc);
			mmc_go_idle(mmc);
			mdelay(100);
			err = mmc_send_op_cond(mmc);
			if (err) {
				puts("MMC Init: Error recovering after SD Version 2 test\n");
				return UNUSABLE_ERR;
			}
		}
	}

	debug("%s: Getting CID\n", __func__);
	err = mmc_all_send_cid(mmc);
	if (err) {
		debug("%s: Error getting CID, card may be missing\n", __func__);
		return err;
	}

	/* For MMC cards, set the Relative Address.
	 * For SD cards, get the Relative address.
	 * This also puts the cards into Standby State.
	 */
	if (IS_SD(mmc)) {
#ifdef CONFIG_OCTEON_MMC_SD
		debug("%s: Getting SD relative address\n", __func__);
		err = sd_send_relative_addr(mmc);
		if (err) {
			printf("%s: Error getting RCA\n", __func__);
			return err;
		}
#endif
	} else {
		mmc->rca = host->bus_id + 0x10;	/* RCA must be > 1 */
		debug("%s: Setting MMC relative address to %d\n",
		      __func__, mmc->rca);
		err = mmc_set_relative_addr(mmc);
		if (err) {
			mmc_print_registers(mmc);
			printf("%s: Error setting MMC RCA to %d\n",
			       __func__, mmc->rca);
			return err;
		}
#if 0
		mmc_write_csr(mmc, CVMX_MIO_EMM_RCA, mmc->rca);
#endif
	}

	debug("Getting CSD\n");
	err = mmc_get_csd(mmc);
	if (err) {
		cvmx_mio_emm_switch_t emm_switch;
		printf("%s: Error getting CSD\n", __func__);
		emm_switch.u64 = mmc_read_csr(mmc, CVMX_MIO_EMM_SWITCH);
		printf("clk period: %d\n",
		       emm_switch.s.clk_hi + emm_switch.s.clk_lo);
		return err;
	}
	if (mmc->version == MMC_VERSION_UNKNOWN) {
		int version = get_csd_bits(mmc, 122, 125);

		switch (version) {
			case 0:
				mmc->version = MMC_VERSION_1_2;
				debug("MMC version 1.2 detected\n");
				break;
			case 1:
				mmc->version = MMC_VERSION_1_4;
				debug("MMC version 1.4 detected\n");
				break;
			case 2:
				mmc->version = MMC_VERSION_2_2;
				debug("MMC version 2.2 detected\n");
				break;
			case 3:
				mmc->version = MMC_VERSION_3;
				debug("MMC version 3 detected\n");
				break;
			case 4:
				mmc->version = MMC_VERSION_4;
				debug("MMC version 4 detected\n");
				break;
			default:
				mmc->version = MMC_VERSION_1_2;
				debug("MMC version 1.2 (unknown) detected\n");
				break;
		}
	}

#ifdef DEBUG
	i = 0;
	classes = get_csd_bits(mmc, 84, 95);
	debug("Classes supported: ");
	while (classes) {
		if (classes & 1)
			debug("%i ", i);
		classes >>= 1;
		i++;
	}
	debug("\n%s: Read block length: %u\n", __func__,
	      1 << ((mmc->csd[1] >> 16) & 0xF));
#endif

	if (IS_SD(mmc)) {
		switch (get_csd_bits(mmc, 96, 103)) {
		case 0x0b:	mmc->tran_speed = 50000000;	break;
		case 0x2b:	mmc->tran_speed = 200000000;	break;
		case 0x32:	mmc->tran_speed = 25000000;	break;
		case 0x5a:	mmc->tran_speed = 50000000;	break;
		default:
			printf("Unknown tran_speed value 0x%x in SD CSD register\n",
			       mmc->csd[0] & 0x7f);
			mmc->tran_speed = 25000000;
			break;
		}
	} else {
		const uint32_t tran_speed_freq[8] = {
			10000, 100000, 1000000, 10000000, 0, 0, 0, 0
		};
		const uint32_t tran_mult[16] = {
			 0, 10, 12, 13, 15, 20, 26, 30,
			35, 40, 45, 52, 55, 60, 70, 80
		};

		freq = tran_speed_freq[get_csd_bits(mmc, 96, 98)];
		mult = tran_mult[get_csd_bits(mmc, 99, 102)];

		mmc->tran_speed = freq * mult;
	}

	debug("%s CSD tran_speed: %u\n",
	      IS_SD(mmc) ? "SD" : "MMC", mmc->tran_speed);

	mmc->read_bl_len = 1 << get_csd_bits(mmc, 80, 83);

	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << get_csd_bits(mmc, 22, 25);

	if (mmc->high_capacity) {
		csize = (mmc->csd[1] & 0x3f) << 16
			| (mmc->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
	} else {
		csize = get_csd_bits(mmc, 62, 73);
		cmult = get_csd_bits(mmc, 47, 49);
		debug("  csize: 0x%llx, cmult: 0x%llx\n", csize, cmult);
	}

	debug("  read_bl_len: 0x%x\n", mmc->read_bl_len);
	mmc->capacity = (csize + 1) << (cmult + 2);
	mmc->capacity *= mmc->read_bl_len;
	debug("%s: capacity: %llu bytes (%llu blocks)\n", __func__,
	      mmc->capacity, mmc->capacity / mmc->read_bl_len);
	debug("%s: read to write program time factor: %d\n", __func__,
	      1 << ((mmc->csd[0] >> 26) & 7));

	mmc->erase_grp_size = 1;
	mmc->read_bl_len = min(mmc->read_bl_len, 512);
	mmc->write_bl_len = min(mmc->write_bl_len, 512);

	/* Select the card and put it into Transfer Mode */

	debug("%s: Selecting card to rca %d\n", __func__, mmc->rca);

	err = mmc_select_card(mmc);
	if (err) {
		printf("%s: Error selecting card\n", __func__);
		return err;
	}

	if (IS_SD(mmc)) {
		debug("SD version: ");
		switch(mmc->version) {
		case SD_VERSION_3:	debug("3\n");		break;
		case SD_VERSION_2:	debug("2\n");		break;
		case SD_VERSION_1_0:	debug("1.0\n");		break;
		case SD_VERSION_1_10:	debug("1.10\n");	break;
		default:		debug("Undefined\n");	break;
		}

		debug("sd version 0x%x\n", mmc->version);
	} else {
		debug("MMC version: ");
		switch (mmc->version) {
		case MMC_VERSION_UNKNOWN:	debug("UNKNOWN\n");	break;
		case MMC_VERSION_1_2:		debug("1.2\n");		break;
		case MMC_VERSION_1_4:		debug("1.4\n");		break;
		case MMC_VERSION_2_2:		debug("2.2\n");		break;
		case MMC_VERSION_3:		debug("3\n");		break;
		case MMC_VERSION_4:		debug("4\n");		break;
		default:			debug("Undefined\n");	break;
		}

		debug("mmc version 0x%x\n", mmc->version);
		if (mmc->version >= MMC_VERSION_4) {
		/* Check ext_csd version and capacity */
			err = mmc_send_ext_csd(mmc);
			if (err) {
				printf("%s: Error: cannot read extended CSD\n",
				       __func__);
				return -1;
			}
			if (host->ext_csd[EXT_CSD_REV] >= 2) {
				capacity = host->ext_csd[EXT_CSD_SEC_CNT] << 0
					| host->ext_csd[EXT_CSD_SEC_CNT + 1] << 8
					| host->ext_csd[EXT_CSD_SEC_CNT + 2] << 16
					| host->ext_csd[EXT_CSD_SEC_CNT + 3] << 24;
				debug("MMC EXT CSD reports capacity of %llu sectors (0x%llx bytes)\n",
				      capacity, capacity * 512);
				capacity *= 512;
				if (((capacity >> 20) > 2 * 1024)
				    && mmc->high_capacity)
					mmc->capacity = capacity;
			}
			if (host->ext_csd[EXT_CSD_ERASE_GROUP_DEF]) {
				mmc->erase_grp_size =
					host->ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]
						* 512 * 1024;
			} else {
				int erase_gsz, erase_gmul;
				erase_gsz = get_csd_bits(mmc, 42, 46);
				erase_gmul = get_csd_bits(mmc, 37, 41);
				mmc->erase_grp_size = (erase_gsz + 1)
							* (erase_gmul + 1);
			}
			debug("%s: erase group size %d\n",
			      __func__, mmc->erase_grp_size);
			if (host->ext_csd[EXT_CSD_PARTITIONING_SUPPORT] & PART_SUPPORT) {
				mmc->part_config = host->ext_csd[EXT_CSD_PART_CONF];
				debug("%s: partitioning config 0x%x\n", __func__,
				      mmc->part_config);
			}
			debug("%s: part config: 0x%x\n", __func__,
			      mmc->part_config);
			if (host->ext_csd[EXT_CSD_CARD_TYPE]
			    & EXT_CSD_CARD_TYPE_26) {
				mmc->card_caps |= MMC_MODE_HS;
				debug("%s: high-speed mode supported\n",
				      __func__);
			}
			if (host->ext_csd[EXT_CSD_CARD_TYPE]
			    & EXT_CSD_CARD_TYPE_52) {
				mmc->card_caps |=
					MMC_MODE_HS_52MHz | MMC_MODE_HS;
				debug("%s: high-speed 52MHz mode supported\n",
				      __func__);
			}
		}
	}

	debug("%s: Changing frequency\n", __func__);
#ifdef CONFIG_OCTEON_MMC_SD
	if (IS_SD(mmc)) {
		err = sd_change_freq(mmc);
	} else
#endif
	{
		mmc_set_ios(mmc);
		err = 0;
	}
	if (err) {
		printf("%s: Error changing frequency\n", __func__);
		return err;
	}

	/* Restrict card's capabilities by what the host can do. */
	debug("%s: MMC card caps: 0x%x, host caps: 0x%x\n",
	      __func__, mmc->card_caps, mmc->cfg->host_caps);
	mmc->card_caps &= mmc->cfg->host_caps;


	if (IS_SD(mmc)) {
#ifdef CONFIG_OCTEON_MMC_SD
		err = sd_set_bus_width_speed(mmc);
		if (err) {
			printf("%s: Error setting SD bus width and/or speed\n",
			       __func__);
			return err;
		}
#endif
	} else {
		err = mmc_set_bus_width_speed(mmc);
		if (err) {
			printf("%s: Error setting MMC bus width and/or speed\n",
			       __func__);
			return err;
		}
	}
	if (!IS_SD(mmc) || (IS_SD(mmc) && mmc->high_capacity)) {
		err = mmc_set_blocklen(mmc, mmc->read_bl_len);
		if (err) {
			printf("%s: Error setting block length to %d\n",
			      __func__, mmc->read_bl_len);
			return err;
		}
	}

	/* Set watchdog for command timeout again */
	mmc_set_watchdog(mmc, 10000);

	/* Fill in device description */
	debug("%s: Filling in block descriptor\n",  __func__);
	mmc->block_dev.lun = 0;
	mmc->block_dev.type = 0;
	mmc->block_dev.blksz = mmc->read_bl_len;
	mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
	if (IS_SD(mmc)) {
		sprintf(mmc->block_dev.vendor, "Man %02x Snr %08x",
			mmc->cid[0] >> 24,
		(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
		sprintf(mmc->block_dev.product, "%c%c%c%c%c",
			mmc->cid[0] & 0xff,
		mmc->cid[1] >> 24, (mmc->cid[1] >> 16) & 0xff,
		(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
	} else {
		sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x",
			mmc->cid[0] >> 8,
			(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
		sprintf(mmc->block_dev.product, "%c%c%c%c%c%c",
			mmc->cid[0] & 0xff,
		mmc->cid[1] >> 24, (mmc->cid[1] >> 16) & 0xff,
		(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff,
		(mmc->cid[2] >> 24) & 0xff);
	}
	debug("%s: %s\n", __func__, mmc->block_dev.vendor);
	debug("%s: %s\n", __func__, mmc->block_dev.product);
	if (IS_SD(mmc))
		sprintf(mmc->block_dev.revision,"%d.%d",
			(mmc->cid[2] >> 28) & 0xf,
		(mmc->cid[2] >> 24) & 0xf);
	else
		sprintf(mmc->block_dev.revision,"%d.%d",
			(mmc->cid[2] >> 20) & 0xf,
		(mmc->cid[2] >> 16) & 0xf);
	debug("%s: %s\n", __func__, mmc->block_dev.revision);

	return 0;
}

/**
 * This is the external mmc_send_cmd function.  It was required that
 * the internal version support flags so this version is required.
 */
static int octeon_mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
			       struct mmc_data *data)
{
	uint32_t flags = 0;
	int ret;
	static bool acmd = false;
	/* Some SD commands require some flags to be changed */
	if (IS_SD(mmc)) {
		switch (cmd->cmdidx) {
		case SD_CMD_SEND_RELATIVE_ADDR:
			flags = MMC_CMD_FLAG_RTYPE_XOR(2);
			break;
		case SD_CMD_SEND_IF_COND:
			flags = MMC_CMD_FLAG_CTYPE_XOR(1) |
				MMC_CMD_FLAG_RTYPE_XOR(2);
			break;
		case 11:
			flags = MMC_CMD_FLAG_CTYPE_XOR(1);
			break;
		case 19:
			flags = MMC_CMD_FLAG_OFFSET(63);
			break;
		case 20:
			flags = MMC_CMD_FLAG_CTYPE_XOR(2);
			break;
		case SD_CMD_ERASE_WR_BLK_START:
		case SD_CMD_ERASE_WR_BLK_END:
			flags = MMC_CMD_FLAG_RTYPE_XOR(1);
			break;
		case SD_CMD_APP_SEND_OP_COND:
			if (acmd)
				flags = MMC_CMD_FLAG_RTYPE_XOR(1);
			break;
		case MMC_CMD_APP_CMD:
			acmd = true;
			break;
		case MMC_CMD_SPI_READ_OCR:
			flags = MMC_CMD_FLAG_RTYPE_XOR(3);
			break;
		case MMC_CMD_SPI_CRC_ON_OFF:
			flags = MMC_CMD_FLAG_RTYPE_XOR(1);
			break;
		default:
			break;
		}
	}
	ret = mmc_send_cmd_flags(mmc, cmd, data, flags);
	if (cmd->cmdidx != MMC_CMD_APP_CMD)
		acmd = false;
	return ret;
}

/**
 * Returns the card write protect status
 *
 * @param mmc	pointer to mmc data structure
 * @return true if card is write protected, false otherwise
 */
int __mmc_getwp(struct mmc *mmc)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int bus = host->bus_id;
	int val = 0;

	if (host->wp_gpio >= 0) {
		val = gpio_get_value(host->wp_gpio);
		debug("%s(%d): gpio %d returned %d\n", __func__, bus,
		      host->wp_gpio, val);
		if (host->wp_active_low)
			val = !val;
	}
	return val;
}
int mmc_getwp(struct mmc *mmc) __attribute__((weak, alias("__mmc_getwp")));

/**
 * Returns the card detect of a MMC device
 *
 * @param mmc	pointer to mmc data structure
 * @return true if card is present, false otherwise
 */
int __mmc_getcd(struct mmc *mmc)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int bus = host->bus_id;
	int val = 1;

	if (host->cd_gpio >= 0) {
		val = gpio_get_value(host->cd_gpio);
		debug("%s(%d): gpio %d returned %d\n", __func__, bus,
		      host->cd_gpio, val);
		if (host->cd_active_low)
			val = !val;
	}
	debug("%s(%d): card %sdetected\n", __func__, bus, val ? "" : "not ");
	return val;
}
int mmc_getcd(struct mmc *mmc) __attribute__((weak, alias("__mmc_getcd")));

/**
 * Controls the power to a MMC device
 *
 * @param mmc	pointer to mmc data structure
 * @param on	true to turn on power, false to turn off power
 */
void __mmc_set_power(struct mmc *mmc, int on)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int bus = host->bus_id;
	int val;

	debug("%s(%p, %d)\n", __func__, mmc, on);
	if (host->power_gpio >= 0) {
		if (host->power_active_low)
			val = !on;
		else
			val = !!on;
		gpio_direction_output(host->power_gpio, val);
		debug("%s(%d, %s) set GPIO %d to %d\n", __func__, bus,
		      on ? "on" : "off", host->power_gpio, val);
		/* Give time for power to settle */
		if (on)
			mdelay(POWER_ON_TIME);
	}
}
void mmc_set_power(struct mmc *mmc, int on)
	__attribute__((weak, alias("__mmc_set_power")));

/**
 * Initialize all MMC devices on a board
 *
 * @param bis	pointer to board information structure
 *
 * @return 0 for success, error otherwise
 *
 * TODO: Modify this to support multiple nodes
 */
int mmc_initialize(bd_t *bis)
{
	static int not_first = 0;
	cvmx_mio_emm_cfg_t emm_cfg;
	struct octeon_mmc_info mmc_info[CVMX_MAX_NODES];
	struct octeon_mmc_info *mi;
	struct list_head *entry;
	struct mmc *mmc = NULL;
	struct mmc *tmp_mmc;
	struct mmc_config *mconf = NULL;
	struct mmc_host *host = NULL;
	int bus_id = 0;
	int node = 0;
	int rc = -1;
	int repeat;
	uint64_t clock_period;
	int found = 0;
	char *name = NULL;
#ifdef CONFIG_OCTEON_MMC_MAX_FREQUENCY
	ulong f_max = CONFIG_OCTEON_MMC_MAX_FREQUENCY;
#else
	ulong f_max = 52000000;
#endif
#ifdef CONFIG_OCTEON_OCX
	const int max_node = CVMX_MAX_NODES;
#else
	const int max_node = 1;
#endif

	debug("%s(%p) ENTER\n", __func__, bis);
	/* The first time through clear out all of the last bus ids per node
	 * for switching between buses.
	 */
	if (!not_first) {
		not_first = 1;
		for (repeat = 0; repeat < CVMX_MAX_NODES; repeat++)
			last_bus_id[repeat] = -1;
	}
	cur_dev_num = 0;
	for (node = 0; node < max_node; node++) {
		if (!(gd->arch.node_mask & (1 << node))) {
			debug("%s: Skipping node %d\n", __func__, node);
			continue;
		}
		debug("%s: initializing node %d\n", __func__, node);
		mi = &mmc_info[node];
		memset(mi, 0, sizeof(*mi));

		/* Get MMC information from the FDT */
		if ((rc = octeon_boot_bus_mmc_get_info(node, mi)) != 0) {
			debug("%s: Could not get MMC info\n", __func__);
			goto out;
		}

		init_time = 1;	/* Suppress error messages */
		/* Disable all MMC slots */
		emm_cfg.u64 = 0;
		mmc_write_csr(mmc, CVMX_MIO_EMM_CFG, emm_cfg.u64);
		mdelay(100);

		rc = -1;

		/* Power down all MMC devices */
		for (bus_id = 0; bus_id < CONFIG_OCTEON_MAX_MMC_SLOT; bus_id++) {
			if (mi->slot[bus_id].chip_sel < 0) {
				debug("%s: Skipping MMC bus %d\n", __func__, bus_id);
				continue;
			}
			debug("%s: Initializing node: %d, bus %d\n",
			      __func__, node, bus_id);
			mconf = calloc(1, sizeof(*mconf));
			if (!mconf) {
				puts("Out of memory\n");
				return -1;
			}

			debug("Initializing MMC bus %d data structures\n", bus_id);
			name = calloc(64, 1);
			if (!name)
				goto nomem;
			sprintf(name, "Octeon MMC/SD%d", cur_dev_num);
			mconf->name = name;
			mconf->ops = &octeon_mmc_ops;
			mconf->f_min = CONFIG_OCTEON_MMC_MIN_BUS_SPEED_HZ;
			mconf->f_max = mi->slot[bus_id].max_frequency;
			f_max = min(mconf->f_max, f_max);
			f_max = min(f_max, getenv_ulong("mmc_max_freq", 10,
							52000000));
			mconf->f_max = f_max;
			mconf->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;
			mconf->part_type = PART_TYPE_DOS;
			if (mi->slot[bus_id].bus_max_width >= 4)
				mconf->host_caps |= MMC_MODE_4BIT;
			if (mi->slot[bus_id].bus_max_width == 8)
				mconf->host_caps |= MMC_MODE_8BIT;
			if (f_max > 20000000)
				mconf->host_caps |= MMC_MODE_HS;
			if (f_max >= 52000000)
				mconf->host_caps |= MMC_MODE_HS_52MHz | MMC_MODE_DDR_52MHz;

			mconf->voltages = getenv_ulong("mmc_voltages", 16, 0);
			if (!mconf->voltages)
				mconf->voltages =  MMC_VDD_27_28 |
						   MMC_VDD_28_29 |
						   MMC_VDD_29_30 |
						   MMC_VDD_30_31 |
						   MMC_VDD_31_32 |
						   MMC_VDD_32_33 |
						   MMC_VDD_33_34 |
						   MMC_VDD_34_35 |
						   MMC_VDD_35_36;
			mconf->voltages &= OCR_VOLTAGE_MASK;

			host = calloc(1, sizeof(*host));
			if (!host)
				goto nomem;

			mmc = mmc_create(mconf, host);
			if (!mmc) {
				debug("%s: Could not create MMC device\n", __func__);
				goto nomem;
			}

			mmc->priv = (void *)host;
			mmc->version = MMC_VERSION_UNKNOWN;
			mmc->rca = bus_id + 0x10;
			mmc->clock = CONFIG_OCTEON_MMC_MIN_BUS_SPEED_HZ;
			host->node = node;
			host->bus_id = bus_id;
			host->mmc = mmc;
			host->dev_index = bus_id;	/* TODO: change this */
			host->sector_mode = 0;
			host->sclock = cvmx_clock_get_rate(CVMX_CLOCK_SCLK);
			if (OCTEON_IS_OCTEON2() || OCTEON_IS_MODEL(OCTEON_CN70XX))
				host->use_ndf = 1;
			else
				host->use_ndf = 0;
			clock_period = 1000000000000ULL / host->sclock;
			debug("%s: sclock period: %llu ps\n",
			      __func__, clock_period);
			host->cmd_clk_skew = (mi->slot[bus_id].cmd_clk_skew
					      + clock_period / 2)
							/ clock_period;
			debug("%s: command clock skew: %u sclock cycles\n",
			      __func__, host->cmd_clk_skew);

			host->dat_clk_skew = (mi->slot[bus_id].dat_clk_skew
					      + clock_period / 2)
							/ clock_period;
			debug("%s: data clock skew: %u sclock cycles\n",
			      __func__, host->dat_clk_skew);
			host->bus_width = EXT_CSD_BUS_WIDTH_1;
			host->max_width = mi->slot[bus_id].bus_max_width;
			host->cd_gpio = mi->slot[bus_id].cd_gpio;
			host->cd_active_low = mi->slot[bus_id].cd_active_low;
			if (host->cd_gpio != -1)
				gpio_direction_input(host->cd_gpio);

			host->wp_gpio = mi->slot[bus_id].wp_gpio;
			host->wp_active_low = mi->slot[bus_id].wp_active_low;
			if (host->wp_gpio != -1)
				gpio_direction_input(host->wp_gpio);

			host->power_gpio = mi->slot[bus_id].power_gpio;
			host->power_active_low = mi->slot[bus_id].power_active_low;
			host->non_removable = mi->slot[bus_id].non_removable;
			debug("Powering down MMC slot %d\n", bus_id);
			mmc_set_power(mmc, 0);
		}
	}
	mdelay(100);
	/* Power them all up */
	debug("Powering up all devices");
	list_for_each(entry, &mmc_devices) {
		mmc = list_entry(entry, struct mmc, link);
		host = mmc->priv;
		debug("Powering up MMC slot %d:%d\n", host->node, host->bus_id);
		mmc_set_power(mmc, 1);
	}
	mdelay(POWER_ON_TIME);
	debug("%s: mmc_devices.next: %p, mmc_devices.prev: %p\n",
	      __func__, mmc_devices.next, mmc_devices.prev);
	list_for_each_entry_safe(mmc, tmp_mmc, &mmc_devices, link) {
		host = mmc->priv;
		bus_id = host->bus_id;
		debug("%s: mmc: %p, host: %p, bus_id: %d:%d\n",
		      __func__, mmc, host, host->node, bus_id);

		if (!mmc_getcd(mmc)) {
			debug("%s: Skipping empty slot %d\n", __func__, bus_id);
			continue;
		}

		for (repeat = 0; repeat < 2; repeat++) {
			host->bus_width = EXT_CSD_BUS_WIDTH_1;

			debug("%s: Calling mmc_init for %s, try %d\n",
			      __func__, mmc->cfg->name, repeat);
			rc = mmc_init(mmc);
			if (!rc) {
				uint8_t val;

				if (!IS_SD(mmc)) {
					val = host->ext_csd[162];
					if ((val & 3) == 0) {
						debug("Enabling RST_n_FUNCTION\n");
						val |= 1;
						mmc_switch(mmc,
							   EXT_CSD_CMD_SET_NORMAL,
							   EXT_CSD_RST_N_FUNCTION,
							   val);
					}
				}
				found++;
				break;
			} else {
				debug("MMC device %d initialization failed, try %d\n",
				      bus_id, repeat);
				mmc_pre_idle(mmc);
				mmc_go_idle(mmc);
			}
		}
	}
	init_time = 0;
	debug("%s: done initializing, found: %s, rc: %d\n", __func__,
	      found ? "true" : "false", rc);
out:
	if (found) {
		debug("%s: Printing devices\n", __func__);
		print_mmc_devices(',');
	} else {
		printf("not available\n");
	}
	debug("%s: exit(%d), return address: %p\n", __func__, rc, __builtin_return_address(0));
	return rc;

nomem:
	printf("%s: Out of memory\n", __func__);
	return -1;
}

int mmc_init(struct mmc *mmc)
{
	struct mmc_host *host = (struct mmc_host *)(mmc->priv);
	int rc;

	if (!octeon_has_feature(OCTEON_FEATURE_MMC))
		return 0;

	debug("%s: Entry\n", __func__);

	mmc_enable(mmc);

	mmc->clock = mmc->cfg->f_min;
	host->sclock = cvmx_clock_get_rate(CVMX_CLOCK_SCLK);
	debug("%s: sclock: %u\n", __func__, host->sclock);

	mmc->bus_width = host->max_width;
	debug("%s: Max bus width: %d\n", __func__, host->max_width);

	mmc_set_bus_width(mmc, 1);
	if (mmc->cfg->f_min == 0) {
		printf("%s: Error: min clock speed is zero!\n", __func__);
		return -1;
	}
	mmc_set_clock(mmc, mmc->cfg->f_min);

	if (last_bus_id[host->node] >= 0 &&
	    last_bus_id[host->node] != host->bus_id)
		mmc_switch_dev(mmc);

	rc = mmc_startup(mmc);
	if (rc == 0 && !IS_SD(mmc) && !(host->ext_csd[162] & 3) &&
	    (host->power_gpio >= 0) && host->non_removable) {
		uint8_t val;
		debug("Enabling RST_n_FUNCTION\n");
		val = host->ext_csd[162] | 1;
		rc = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_RST_N_FUNCTION, val);
		if (!rc)
			debug("RST_n_FUNCTION set correctly\n");
		else
			printf("RST_n_FUNCTION failed, rc: %d\n", rc);
	} else {
		debug("%s: rst_n not needed.\n", __func__);
		debug("  rc: %d, sd: %s, ext_csd[162]: 0x%x, power gpio: %d, active low: %s, non-removable: %s\n",
		      rc, IS_SD(mmc) ? "true" : "false", host->ext_csd[162],
		      host->power_gpio, host->power_active_low ? "true" : "false",
		      host->non_removable ? "true" : "false");
	}

	mmc_disable(mmc);
	if (rc == 0) {
		host->initialized = 1;
		debug("Initializing partitions\n");
		init_part(&mmc->block_dev);
	} else if (!list_empty(&mmc_devices)) {
		struct list_head *entry, *tmp_entry;

		debug("mmc dev %d initialization failed\n", mmc->block_dev.dev);

		list_for_each_safe(entry, tmp_entry, &mmc_devices) {
			if (list_entry(entry, struct mmc, link) == mmc) {
				debug("%s: Deleting from list\n", __func__);
				list_del(entry);
				break;
			}
		}
		/* Change current device to first good device if we fail */
		mmc = list_first_entry(&mmc_devices, struct mmc, link);
#if 0
		mmc_set_dev(mmc ? mmc->block_dev.dev : -1);
#endif
	}

	return rc;
}

int get_mmc_num(void)
{
	return cur_dev_num;
}

/**
 * Create a new MMC device and links it in
 *
 * @param[in] cfg	pointer to configuration data structure
 * @param[in] priv	pointer to private data
 *
 * @return	pointer to new mmc data structure or NULL if error
 */
struct mmc *mmc_create(const struct mmc_config *cfg, void *priv)
{
	struct mmc *mmc;

	debug("%s(%p, %p)\n", __func__, cfg, priv);
	if (cfg == NULL || cfg->ops == NULL || cfg->ops->send_cmd == NULL ||
	    cfg->f_min == 0 || cfg->f_max == 0 || cfg->b_max == 0) {
		printf("%s: config error:\n"
		       "  cfg: %p\n"
		       "  cfg->ops: %p\n"
		       "  cfg->ops->send_cmd: %p\n"
		       "  cfg->f_min: %d\n"
		       "  cfg->f_max: %d\n"
		       "  cfg->b_max: %d\n",
		       __func__, cfg, cfg->ops, cfg->ops->send_cmd,
		       cfg->f_min, cfg->f_max, cfg->b_max);
		return NULL;
	}

	mmc = calloc(1, sizeof(*mmc));
	if (mmc == NULL)
		return NULL;
	mmc->cfg = cfg;
	mmc->priv = priv;

	/* the following chunk was mmc_register */

	/* Setup dsr related values */
	mmc->dsr_imp = 0;
	mmc->dsr = 0xffffffff;
	/* Setup the universal parts of the block interface just once */
	mmc->block_dev.if_type = IF_TYPE_MMC;
	mmc->block_dev.dev = cur_dev_num++;
	mmc->block_dev.block_read = mmc_bread;
	mmc->block_dev.block_write = mmc_bwrite;
	mmc->block_dev.block_erase = mmc_berase;

	/* setup initial part type */
	mmc->block_dev.part_type = mmc->cfg->part_type;

	INIT_LIST_HEAD(&mmc->link);
	debug("%s: Adding mmc device\n", __func__);
	list_add_tail(&mmc->link, &mmc_devices);

	return mmc;
}

void mmc_destroy(struct mmc *mmc)
{
	/* only freeing memory for now */
	debug("%s(%p) ENTRY\n", __func__, mmc);
	free(mmc);
}

#ifdef DEBUG
int do_oct_mmc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	mmc_initialize(gd->bd);
	debug("%s: returning\n", __func__);
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(octmmc, 2, 1, do_oct_mmc, "Octeon MMC initialization", NULL);
#endif
