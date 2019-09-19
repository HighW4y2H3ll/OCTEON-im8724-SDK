/***********************license start************************************
 * Copyright (c) 2005-2011 Cavium Inc. (support@cavium.com). All rights
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
 ***********************license end**************************************/
#undef DEBUG
#undef DEBUG_PCI_MEM_IO

#define OCTEON
#include <common.h>

#include <pci.h>
#include <asm/addrspace.h>

#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_hal.h>
#include <asm/arch/octeon_pci.h>

extern void _start(void);
extern int octeon_pci_port_num(uint32_t addr);

void dumpPCIstate(void);

DECLARE_GLOBAL_DATA_PTR;

/**
 * This is the bit decoding used for the Octeon PCI controller addresses for
 * config space
 */
typedef union {
	uint64_t u64;
	uint64_t *u64_ptr;
	uint32_t *u32_ptr;
	uint16_t *u16_ptr;
	uint8_t *u8_ptr;
	struct {
		uint64_t upper:2;
		uint64_t reserved:13;
		uint64_t io:1;
		uint64_t did:5;
		uint64_t subdid:3;
		uint64_t reserved2:4;
		uint64_t endian_swap:2;
		uint64_t reserved3:10;
		uint64_t bus:8;
		uint64_t dev:5;
		uint64_t func:3;
		uint64_t reg:8;
	} s;
} octeon_pci_config_space_address_t;

typedef union {
	uint64_t u64;
	uint32_t *u32_ptr;
	uint16_t *u16_ptr;
	uint8_t *u8_ptr;
	struct {
		uint64_t upper:2;
		uint64_t reserved:13;
		uint64_t io:1;
		uint64_t did:5;
		uint64_t subdid:3;
		uint64_t reserved2:4;
		uint64_t endian_swap:2;
		uint64_t res1:1;
		uint64_t port:1;
		uint64_t addr:32;
	} s;
} octeon_pci_io_space_address_t;

#define CVMX_OCT_SUBDID_PCI_CFG     1
#define CVMX_OCT_SUBDID_PCI_IO      2
#define CVMX_OCT_SUBDID_PCI_MEM1    3
#define CVMX_OCT_SUBDID_PCI_MEM2    4
#define CVMX_OCT_SUBDID_PCI_MEM3    5
#define CVMX_OCT_SUBDID_PCI_MEM4    6

volatile uint64_t pci_cfg_base =
CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 1));
volatile uint64_t pci_io_base =
CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 2));
volatile uint64_t pci_mem1_base =
CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 3));
volatile uint64_t pci_mem2_base =
CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 4));
volatile uint64_t pci_mem3_base =
CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 5));
volatile uint64_t pci_mem4_base =
CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 6));

int octeon_pci_big_bar = 0;

#define PCI_ACCESS_READ  0
#define PCI_ACCESS_WRITE 1

/**
 * Read a value from configuration space
 *
 * @param bus
 * @param devfn
 * @param reg
 * @param size
 * @param val
 * @return
 */
static int octeon_read_config(pci_dev_t dev, int reg, int size, uint32_t * val)
{
	octeon_pci_config_space_address_t pci_addr;

	pci_addr.u64 = 0;
	pci_addr.s.upper = 2;
	pci_addr.s.io = 1;
	pci_addr.s.did = 3;
	pci_addr.s.subdid = 1;
	pci_addr.s.endian_swap = 1;
	pci_addr.s.bus = PCI_BUS(dev);
	pci_addr.s.dev = PCI_DEV(dev);
	pci_addr.s.func = PCI_FUNC(dev);
	pci_addr.s.reg = reg;

#if 0
	printf("About to read from config space with addr: 0x%llx, width: %d\n",
	       pci_addr.u64, size);
#endif
	switch (size) {
	case 4:
		*val = le32_to_cpu(cvmx_read64_uint32(pci_addr.u64));
		return 0;
	case 2:
		*val = le16_to_cpu(cvmx_read64_uint16(pci_addr.u64));
		return 0;
	case 1:
		*val = (cvmx_read64_uint8(pci_addr.u64));
		return 0;
	}
	return -1;
}

/**
 * Write a value to PCI configuration space
 *
 * @param bus
 * @param devfn
 * @param reg
 * @param size
 * @param val
 * @return
 */
static int octeon_write_config(pci_dev_t dev, int reg, int size, uint32_t val)
{
	octeon_pci_config_space_address_t pci_addr;

	pci_addr.u64 = 0;
	pci_addr.s.upper = 2;
	pci_addr.s.io = 1;
	pci_addr.s.did = 3;
	pci_addr.s.subdid = 1;
	pci_addr.s.endian_swap = 1;
	pci_addr.s.bus = PCI_BUS(dev);
	pci_addr.s.dev = PCI_DEV(dev);
	pci_addr.s.func = PCI_FUNC(dev);
	pci_addr.s.reg = reg;

	switch (size) {
	case 4:
		cvmx_write64_uint32(pci_addr.u64, cpu_to_le32(val));
		return 0;
	case 2:
		cvmx_write64_uint16(pci_addr.u64, cpu_to_le16(val));
		return 0;
	case 1:
		cvmx_write64_uint8(pci_addr.u64, (val));
		return 0;
	}
	return -1;
}

/*
 *	Access Octeon PCI Config / IACK / Special Space
 */

static int octeon_pci_write_config_byte(struct pci_controller *hose,
					pci_dev_t dev, int reg, u8 val)
{
	u32 data;
	data = val;
	octeon_write_config(dev, reg, 1, data);
	return 0;

}

static int octeon_pci_write_config_word(struct pci_controller *hose,
					pci_dev_t dev, int reg, u16 val)
{
	u32 data;
	data = val;
	octeon_write_config(dev, reg, 2, data);
	return 0;

}

static int octeon_pci_write_config_dword(struct pci_controller *hose,
					 pci_dev_t dev, int reg, u32 val)
{
	u32 data;
	data = val;
	octeon_write_config(dev, reg, 4, data);
	return 0;

}

static int octeon_pci_read_config_byte(struct pci_controller *hose,
				       pci_dev_t dev, int reg, u8 * val)
{
	u32 data;

	octeon_read_config(dev, reg, 1, &data);
	*val = (u8) data;
	return 0;
}

static int octeon_pci_read_config_word(struct pci_controller *hose,
				       pci_dev_t dev, int reg, u16 * val)
{
	u32 data;

	octeon_read_config(dev, reg, 2, &data);
	*val = (u16) data;
	return 0;
}

static int octeon_pci_read_config_dword(struct pci_controller *hose,
					pci_dev_t dev, int reg, u32 * val)
{
	u32 data;

	octeon_read_config(dev, reg, 4, &data);
	*val = (u32) data;
	return 0;
}

/*
 *	Access Octeon PCI I/O Space
 */
uint32_t octeon_pci_read_io(uint32_t addr, uint8_t size)
{

	octeon_pci_io_space_address_t io_addr;
	io_addr.u64 = 0;
	io_addr.s.upper = 2;
	io_addr.s.io = 1;
	io_addr.s.did = 3;
	io_addr.s.subdid = CVMX_OCT_SUBDID_PCI_IO;
	io_addr.s.endian_swap = 1;
	io_addr.s.port = octeon_pci_port_num(addr);
	io_addr.s.addr = addr;

	CVMX_SYNCW;

	switch (size) {
	case 4:
		return le32_to_cpu(cvmx_read64_uint32(io_addr.u64));
	case 2:
		return le16_to_cpu(cvmx_read64_uint16(io_addr.u64));
	case 1:
		return (cvmx_read64_uint8(io_addr.u64));
	}
	return -1;
}

int octeon_pci_write_io(uint32_t addr, uint8_t size, uint32_t val)
{

	octeon_pci_io_space_address_t io_addr;
	io_addr.u64 = 0;
	io_addr.s.upper = 2;
	io_addr.s.io = 1;
	io_addr.s.did = 3;
	io_addr.s.subdid = CVMX_OCT_SUBDID_PCI_IO;
	io_addr.s.endian_swap = 1;
	io_addr.s.port = octeon_pci_port_num(addr);
	io_addr.s.addr = addr;

	CVMX_SYNCW;
	switch (size) {
	case 4:
		cvmx_write64_uint32(io_addr.u64, cpu_to_le32(val));
		return 0;
	case 2:
		cvmx_write64_uint16(io_addr.u64, cpu_to_le16(val));
		return 0;
	case 1:
		cvmx_write64_uint8(io_addr.u64, (val));
		return 0;
	}
	return -1;
}

int octeon_pci_io_readl(unsigned int reg)
{
	return octeon_pci_read_io(reg, 4);
}

void octeon_pci_io_writel(int value, unsigned int reg)
{
	octeon_pci_write_io(reg, 4, value);
}

int octeon_pci_io_readw(unsigned int reg)
{
	return octeon_pci_read_io(reg, 2);
}

void octeon_pci_io_writew(int value, unsigned int reg)
{
	octeon_pci_write_io(reg, 2, value);
}

int octeon_pci_io_readb(unsigned int reg)
{
	return octeon_pci_read_io(reg, 1);
}

void octeon_pci_io_writeb(int value, unsigned int reg)
{
	octeon_pci_write_io(reg, 1, value);
}

/*
 *	Access Octeon PCI Memory Space
 */

static inline uint64_t octeon_pci_mem_address(unsigned int reg)
{
	if (reg >= OCTEON_PCI_SLOT1_BAR_ADDR &&
	    reg < OCTEON_PCI_SLOT2_BAR_ADDR)
		return pci_mem2_base + reg;
	else if (reg >= OCTEON_PCI_SLOT2_BAR_ADDR &&
		 reg < OCTEON_PCI_SLOT3_BAR_ADDR)
		return pci_mem3_base + reg;
	else if (reg >= OCTEON_PCI_SLOT3_BAR_ADDR)
		return pci_mem4_base + reg;
	else
		return pci_mem1_base + reg;
}

int octeon_pci_mem1_readl(unsigned int reg)
{
	u64 addr = octeon_pci_mem_address(reg);
	u32 value;

	CVMX_SYNCW;
	value = le32_to_cpu(cvmx_read64_uint32(addr));
#ifdef DEBUG_PCI_MEM_IO
	printf("%s: address: 0x%x (0x%llx), value: 0x%x\n",
	       __func__, reg, addr, value);
#endif
	return value;
}

void octeon_pci_mem1_writel(int value, unsigned int reg)
{
	u64 addr = octeon_pci_mem_address(reg);
#ifdef DEBUG_PCI_MEM_IO
	printf("%s: address: 0x%x (0x%llx), value: 0x%x\n",
	       __func__, reg, addr, value);
#endif
	CVMX_SYNCW;
	cvmx_write64_uint32(addr, cpu_to_le32(value));
	CVMX_SYNCW;
}

int octeon_pci_mem1_readw(unsigned int reg)
{
	CVMX_SYNCW;
	return le16_to_cpu(cvmx_read64_uint16(octeon_pci_mem_address(reg)));
}

void octeon_pci_mem1_writew(int value, unsigned int reg)
{
	CVMX_SYNCW;
	cvmx_write64_uint16(octeon_pci_mem_address(reg), cpu_to_le16(value));
	CVMX_SYNCW;
}

int octeon_pci_mem1_readb(unsigned int reg)
{
	CVMX_SYNCW;
	return cvmx_read64_uint8(octeon_pci_mem_address(reg));
}

void octeon_pci_mem1_writeb(int value, unsigned int reg)
{
	CVMX_SYNCW;
	cvmx_write64_uint8(octeon_pci_mem_address(reg), (value));
	CVMX_SYNCW;
}

/*
 *	Initialize OCTEON PCI
 */

/* Assign bus address ranges used to configure devices found on the PCI buss.
 * We will choose to exclude the low 128 Megs (0x0 - 0x10000000) from these
 * so that we can use this low memory for BAR 1 on Octeon to allow easy access
 * to the low 128 Megs of Octeon memory.
 */

/* Skip low IO space so that we don't interfere with legacy ISA ports if in
 * use
 */
#define PCI_PNP_IO_BASE     0x00001000
#define PCI_PNP_IO_SIZE     0x08000000

/* Use range above 256MB to map devices to allow use of big bar 0 support on
 * some chips
 */
#if 0
#define PCI_PNP_MEM_BASE	0x80000000
#define PCI_PNP_MEM_SIZE	0x40000000
#else
#define PCI_PNP_MEM_BASE	OCTEON_PCI_SLOT0_BAR_ADDR
#define PCI_PNP_MEM_SIZE	OCTEON_PCI_SLOT0_BAR_SIZE
#endif
/* If no big bar support put bar 0 at 128 meg, bar 1 at 0 */
#define PCI_BAR0_BASE       0x00001000
#define PCI_BAR1_BASE       0x00000000
/* If big bar support, put bar 0 at 0, disable bar 1 */
#define PCI_BAR0_BASE_BIG   0x00000000

#define PCI_BAR2_BASE       0x8000000000ull

/* Only needed for NPI register access */
#define SWAP_ADDR32_LE_TO_BE(x)         ((x)^ 0x4)
void octeon_pci_initialize(void)
{
	cvmx_pci_cfg01_t cfg01;
	cvmx_pci_ctl_status_2_t ctl_status_2;
	cvmx_pci_cfg19_t cfg19;
	cvmx_pci_cfg16_t cfg16;
	cvmx_pci_cfg22_t cfg22;
	cvmx_pci_cfg56_t cfg56;

	int big_bar;
#if CONFIG_OCTEON_PCI_BIG_BAR
	big_bar = CONFIG_OCTEON_PCI_BIG_BAR;
#else
	big_bar = 0;
#endif

	if (OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2)
	    || OCTEON_IS_MODEL(OCTEON_CN31XX))
		big_bar = 0;

	octeon_pci_big_bar = big_bar;

	/* Reset the PCI Bus */
	cvmx_write_csr(CVMX_CIU_SOFT_PRST, 0x1);
	cvmx_read_csr(CVMX_CIU_SOFT_PRST);

	mdelay(2);		/* Hold  PCI reset for 2 ms */

	if (OCTEON_IS_MODEL(OCTEON_CN31XX) || OCTEON_IS_MODEL(OCTEON_CN30XX)
	    || OCTEON_IS_MODEL(OCTEON_CN50XX)) {
		/*
		 * Deassert PCI reset and advertize PCX Host Mode Device
		 * Capability (32b)
		 */
		cvmx_write_csr(CVMX_CIU_SOFT_PRST, 0x0);
	} else {
		/*
		 * Deassert PCI reset and advertize PCX Host Mode Device
		 * Capability (64b)
		 */
		cvmx_write_csr(CVMX_CIU_SOFT_PRST, 0x4);
	}
	cvmx_read_csr(CVMX_CIU_SOFT_PRST);

	mdelay(2);		/* Wait 2 ms after deasserting PCI reset */

	ctl_status_2.u32 = 0;
	/* Initializes to 0.  Must be set before any PCI reads. */
	ctl_status_2.s.tsr_hwm = 1;
	if (big_bar) {
		debug("Big BAR enabled\n");
		ctl_status_2.s.bb_ca = 1;	/* Don't Use L2 with big bars */
		ctl_status_2.s.bb_es = 1;	/* Big bar in byte swap mode */
		ctl_status_2.s.bb0 = 1;		/* BAR0 is big */

		ctl_status_2.s.bb1 = 1;		/* BAR1 is big */
		ctl_status_2.s.bb1_hole = 6;	/* Hole is 512MB */
		ctl_status_2.s.bb1_siz = 1;	/* BAR1 is 2GB */
	}

	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CTL_STATUS_2),
			    ctl_status_2.u32);
	mdelay(2);		/* Wait 2 ms before doing PCI reads */

	ctl_status_2.u32 =
	    cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CTL_STATUS_2));
	printf("PCI Status: %s %s-bit\n",
	       ctl_status_2.s.ap_pcix ? "PCI-X" : "PCI",
	       ctl_status_2.s.ap_64ad ? "64" : "32");

	if (OCTEON_IS_MODEL(OCTEON_CN58XX) || OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		cvmx_pci_cnt_reg_t cnt_reg;
		cvmx_npi_pci_int_arb_cfg_t arb_cfg;
		const int speeds[] = { 33, 66, 100, 133, 133 };

		cnt_reg.u64 = cvmx_read_csr(CVMX_NPI_PCI_CNT_REG);
		printf("PCI Status: %d MHz (measured %d-%d MHz)\n",
		       speeds[cnt_reg.s.hm_speed],
		       speeds[cnt_reg.s.ap_speed],
		       speeds[1 + cnt_reg.s.ap_speed]);

		arb_cfg.u64 = cvmx_read_csr(CVMX_NPI_PCI_INT_ARB_CFG);
		if (arb_cfg.s.pci_ovr & 0x8) {
			printf("PCI Status: SOFTWARE OVERRIDE to %s %d MHz\n",
			       arb_cfg.s.pci_ovr & 0x4 ? "PCI-X" : "PCI",
			       speeds[arb_cfg.s.pci_ovr & 0x3]);
		}
	}

	/*
	 * TDOMC must be set to one in PCI mode. TDOMC should be set to 4
	 * in PCI-X mode to allow four oustanding splits. Otherwise,
	 * should not change from its reset value. Don't write PCI_CFG19
	 * in PCI mode (0x82000001 reset value), write it to 0x82000004
	 * after PCI-X mode is known. MRBCI,MDWE,MDRE -> must be zero.
	 * MRBCM -> must be one.
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

		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG19),
				    cfg19.u32);
	}

	cfg01.u32 = 0;
	cfg01.s.msae = 1;	/* Memory Space Access Enable */
	cfg01.s.me = 1;		/* Master Enable */
	cfg01.s.pee = 1;	/* PERR# Enable */
	cfg01.s.see = 1;	/* System Error Enable */
	cfg01.s.fbbe = 1;	/* Fast Back to Back Transaction Enable */

	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG01),
			    cfg01.u32);
	cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG01));

#ifdef USE_OCTEON_INTERNAL_ARBITER
	/*
	 * When OCTEON is a PCI host, most systems will use OCTEON's
	 * internal arbiter, so must enable it before any PCI/PCI-X
	 * traffic can occur.
	 */
	{
		cvmx_npi_pci_int_arb_cfg_t pci_int_arb_cfg;

		pci_int_arb_cfg.u64 = 0;
		pci_int_arb_cfg.s.en = 1;	/* Internal arbiter enable */
		cvmx_write_csr(CVMX_NPI_PCI_INT_ARB_CFG, pci_int_arb_cfg.u64);
	}
#endif /* USE_OCTEON_INTERNAL_ARBITER */

	/*
	 * Preferrably written to 1 to set MLTD. [RDSATI,TRTAE,
	 * TWTAE,TMAE,DPPMR -> must be zero. TILT -> must not be set to
	 * 1..7.
	 */
	cfg16.u32 = 0;
	cfg16.s.mltd = 1;	/* Master Latency Timer Disable */
	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG16),
			    cfg16.u32);

	/*
	 * Should be written to 0x4ff00. MTTV -> must be zero.
	 * FLUSH -> must be 1. MRV -> should be 0xFF.
	 */
	cfg22.u32 = 0;
	cfg22.s.mrv = 0xff;	/* Master Retry Value [1..255] and 0=infinite */
	cfg22.s.flush = 1;	/* AM_DO_FLUSH_I control NOTE: This
				   bit MUST BE ONE for proper N3K
				   operation */
	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG22),
			    cfg22.u32);

	/*
	 * MOST Indicates the maximum number of outstanding splits (in -1
	 * notation) when OCTEON is in PCI-X mode.  PCI-X performance is
	 * affected by the MOST selection.  Should generally be written
	 * with one of 0x3be807, 0x2be807, 0x1be807, or 0x0be807,
	 * depending on the desired MOST of 3, 2, 1, or 0, respectively.
	 */
	cfg56.u32 = 0;
	cfg56.s.pxcid = 7;	/* RO - PCI-X Capability ID */
	cfg56.s.ncp = 0xe8;	/* RO - Next Capability Pointer */
	cfg56.s.dpere = 1;	/* Data Parity Error Recovery Enable */
	cfg56.s.roe = 1;	/* Relaxed Ordering Enable */
	cfg56.s.mmbc = 1;	/* Maximum Memory Byte Count [0=512B,1=1024B,2=2048B,3=4096B] */
	cfg56.s.most = 3;	/* Maximum outstanding Split transactions [0=1 .. 7=32] */

	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG56),
			    cfg56.u32);

	/*
	 * Affects PCI performance when OCTEON services reads to its
	 * BAR1/BAR2. Refer to Section 10.6.1.  The recommended values are
	 * 0x22, 0x33, and 0x33 for PCI_READ_CMD_6, PCI_READ_CMD_C, and
	 * PCI_READ_CMD_E, respectively. Note that these values differ
	 * from their reset values.
	 */
	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_READ_CMD_6),
			    0x21);
	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_READ_CMD_C),
			    0x31);
	cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_READ_CMD_E),
			    0x31);

#if (CONFIG_OCTEON_PCI_HOST)
	/*
	 * Map the Octeon BARs in case they are not mapped later by
	 * pciauto_config_device().  If the Octeon idsel signal is
	 * connected these BARs will be remapped by
	 * pciauto_config_device().  By mapping it here either hardware
	 * configuration will work.
	 */

	if (big_bar) {
		/* Enable big bar */

		/* Remap the OCTEON BAR 0 to 0-2GB */
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG04),
				    PCI_BAR0_BASE_BIG);
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG05),
				    0x00000000);	/* Assume 32-bit addressing */

		/* Remap OCTEON BAR 1 to map 2GB-4GB (minus the BAR 1 hole).
		 */
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG06),
				    2ul << 30);
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG07),
				    0);
	} else {
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG04),
				    PCI_BAR0_BASE);
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG05),
				    0x00000000);	/* Assume 32-bit addressing */

		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG06),
				    PCI_BAR1_BASE);
		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG07),
				    0x00000000);	/* Assume 32-bit addressing */
	}

#endif /* CONFIG_OCTEON_PCI_HOST */
}

static struct pci_config_table pci_board_config_table[] = {
	/* vendor, device, class */
	/* bus, dev, func */
	{}
};

struct pci_controller hose0;

/*
 *	Initialize Module
 */
void init_octeon_pci(void)
{
	cvmx_pci_bar1_indexx_t pci_bar1_indexx;
	struct pci_controller *hose;
	int idx;
	uint64_t bar1_mem_base;
	cvmx_npi_mem_access_subidx_t mem_access;
	uint64_t bar0addr, bar1addr;

	hose = &hose0;
	hose->config_table = pci_board_config_table;

	hose->first_busno = 0;
	hose->last_busno = 0xff;

	printf("Starting PCI\n");

	octeon_pci_initialize();	/* Initialize OCTEON PCI */

	debug("%s: BAR1 mem base: 0x%llx\n", __func__, bar1_mem_base);

	if (octeon_pci_big_bar)
		bar1_mem_base = 0x100000000ull;
	else
		bar1_mem_base = gd->bd->bi_uboot_ram_addr;

	if (bar1_mem_base >= 0x410000000ull && bar1_mem_base < 0x420000000ull)
		bar1_mem_base -= 0x400000000ull;

	/* Set endian swapping for MMIO MEM transactions (mem1 accesses) */
	mem_access.u64 = 0;
	mem_access.s.esr = 1;	/**< Endian-Swap on read. */
	mem_access.s.esw = 1;	/**< Endian-Swap on write. */
	mem_access.s.nsr = 0;	/**< No-Snoop on read. */
	mem_access.s.nsw = 0;	/**< No-Snoop on write. */
	mem_access.s.ror = 0;	/**< Relax Read on read. */
	mem_access.s.row = 0;	/**< Relax Order on write. */
	mem_access.s.ba = 0;	/**< PCI Address bits [63:36]. */
	cvmx_write_csr(CVMX_NPI_MEM_ACCESS_SUBID3, mem_access.u64);

	/* PCI I/O space (Sub-DID == 2) */
	pci_set_region(hose->regions + 0,
		       PCI_PNP_IO_BASE, PCI_PNP_IO_BASE,
		       PCI_PNP_IO_SIZE, PCI_REGION_IO);

	/* PCI memory space (Sub-DID == 3) */
	pci_set_region(hose->regions + 1,
		       PCI_PNP_MEM_BASE, PCI_PNP_MEM_BASE,
		       PCI_PNP_MEM_SIZE, PCI_REGION_MEM);

	hose->region_count = 3;

	pci_set_ops(hose,
		    octeon_pci_read_config_byte,
		    octeon_pci_read_config_word,
		    octeon_pci_read_config_dword,
		    octeon_pci_write_config_byte,
		    octeon_pci_write_config_word,
		    octeon_pci_write_config_dword);

	pci_register_hose(hose);

	hose->last_busno = pci_hose_scan(hose);

#define BAR1_INDEX_MASK (~0ull << 22)

	/* System memory space, using BAR1 */
	pci_set_region(hose->regions + 2, cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG06)) & ~0xf,	/* Octeon BAR1 */
		       (bar1_mem_base & BAR1_INDEX_MASK),
		       0x08000000,
		       PCI_REGION_IO | PCI_REGION_MEM | PCI_REGION_SYS_MEMORY);

	bar0addr =
	    (cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG04)) &
	     ~0xf) | (uint64_t)
	    cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG05)) <<
	    32ull;
	bar1addr =
	    (cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG06)) &
	     ~0xf) | (uint64_t)
	    cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_CFG07)) <<
	    32ull;
	printf("PCI BAR 0: 0x%08llx, PCI BAR 1: Memory 0x%08llx  PCI 0x%08llx\n",
	       bar0addr, (bar1_mem_base & BAR1_INDEX_MASK), bar1addr);

	/* Setup BAR1 Map Table - Map contiguous 128 MB */
	for (idx = 0; idx < 32; ++idx) {
		pci_bar1_indexx.u32 = 0;
		pci_bar1_indexx.s.addr_idx = (bar1_mem_base >> 22) + idx;
		pci_bar1_indexx.s.addr_v = 1;
		pci_bar1_indexx.s.ca = 1;	/* 1= allocate into L2 */
		pci_bar1_indexx.s.end_swp = 1;

		cvmx_write64_uint32(SWAP_ADDR32_LE_TO_BE
				    (CVMX_NPI_PCI_BAR1_INDEXX(idx)),
				    pci_bar1_indexx.u32);
	}

	/*
	 * Some flops used to generate the upper address bits are not
	 * being reset which can result in the wrong address being
	 * used for only the first PCI access.  A read to any BAR
	 * region will initialize these flops. Errata PCI-302.
	 */
	octeon_pci_mem1_readl(bar0addr + 0x80);	/* Read PCI_DBELL_0... Good as any */

# ifdef DEBUG
	dumpPCIstate();
# endif
}

# ifdef DEBUG
void dumpPCIstate(void)
{
	int idx;

	printf("NPI_PCI_CFG[0..63]\n");
	for (idx = 0; idx < 64; idx += 4) {
		printf("\t0x%02x [%02d]: 0x%08x 0x%08x 0x%08x 0x%08x\n",
		       idx * 4, idx,
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_CFG00 + idx * 4 + 0x0)),
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_CFG00 + idx * 4 + 0x4)),
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_CFG00 + idx * 4 + 0x8)),
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_CFG00 + idx * 4 +
					   0xc)));
	}

	printf("NPI_PCI_READ_CMD_6  : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
				  (CVMX_NPI_PCI_READ_CMD_6)));
	printf("NPI_PCI_READ_CMD_C  : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
				  (CVMX_NPI_PCI_READ_CMD_C)));
	printf("NPI_PCI_READ_CMD_E  : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
				  (CVMX_NPI_PCI_READ_CMD_E)));
	printf("NPI_PCI_CTL_STATUS_2: %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
				  (CVMX_NPI_PCI_CTL_STATUS_2)));
	printf("NPI_PCI_INT_SUM2    : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_INT_SUM2)));
	printf("NPI_PCI_INT_ENB2    : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_INT_ENB2)));
	printf("NPI_PCI_SCM_REG     : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_SCM_REG)));
	printf("NPI_PCI_TSR_REG     : %#010x\n",
	       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE(CVMX_NPI_PCI_TSR_REG)));

	for (idx = 0; idx < 32; idx += 4) {
		printf("PCI_BAR1_INDEXX(%2d): %#07x %#07x %#07x %#07x\n", idx,
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_BAR1_INDEXX(idx + 0))),
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_BAR1_INDEXX(idx + 1))),
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_BAR1_INDEXX(idx + 2))),
		       cvmx_read64_uint32(SWAP_ADDR32_LE_TO_BE
					  (CVMX_NPI_PCI_BAR1_INDEXX(idx + 3)))
		    );
	}
}
# endif /* DEBUG */
