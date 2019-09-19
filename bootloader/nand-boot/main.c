/***********************license start***************
 * Copyright (c) 2003-2007 Cavium Inc. (support@cavium.com). All rights
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
 ***********************license end**************************************/

/* We're running C but don't have a standard library or even a heap. Be very
    careful about what is included */
#define CVMX_ADD_IO_SEG(x) ((1ull<<63) | (x))
#include "cvmx.h"
#include "cvmx-csr.h"
#include "cvmx-nand.h"
#include "cvmx-bootloader.h"

/* The stage2 DRAM address is fixed, as u-boot is now TLB mapped, and the
** virtual address is in the header.  The physical address is not fixed, but
** must meet some alignment requirements. */
#define STAGE2_DRAM_ADDR                0xFFFFFFFF80400000ull


/* It is nice to have the symbols for the functions when we're debugging
    in the simulator. Don't make stuff static if DEBUG is on */
#ifdef DEBUG
#define STATIC
#else
#define STATIC static
#endif

/* Output routines defiend in start.S */
extern void uart_write_char(char c);
extern void uart_write_string(const char *str);
extern void uart_write_hex(uint64_t number);
extern void jump_to_bootloader(void *ptr) __attribute__ ((noreturn));
extern void __start;
extern void _end;

/* Temporary DMA buffer for when we are searching for the header. It is in the
    text section so that our link without libraries works */
static char dma_buffer[512] __attribute__ ((section (".text")));

/**
 * Covert a physical address into a C pointer
 *
 * @param address Address to convert
 *
 * @return Pointer value
 */
static inline void *phys_to_ptr(uint64_t address)
{
    return (void*)(address | (1ull<<63));
}

/**
 * Covert a C pointer into a physical address
 *
 * @param ptr    Pointer to convert
 *
 * @return Physical address
 */
static inline uint64_t ptr_to_phys(void *ptr)
{
    return 0x7fffffffull & (unsigned long)ptr;
}


/**
 * Replacement for the CVMX version that doesn't require including
 * a bunch of stuff.
 *
 * @param address
 *
 * @return
 */
static inline uint64_t __cvmx_read_csr(uint64_t address)
{
    return *(volatile uint64_t*)address;
}


/**
 * Replacement for the CVMX version that doesn't require including
 * a bunch of stuff.
 *
 * @param address
 *
 * @return
 */
static inline void __cvmx_write_csr(uint64_t address, uint64_t value)
{
    *(volatile uint64_t*)address = value;
}

/**
 * Internal CRC32 calculation taking advantage of the OCTEON CRC32 engine
 * 
 * @param crc - initial CRC value
 * @param buf - pointer to buffer to calculate CRC on
 * @param len - length of buffer in bytes
 * 
 * @return CRC32 value of buffer
 */
static uint32_t crc32_no_comp(uint32_t crc, const uint8_t *buf, uint32_t len)
{
        volatile uint64_t *ptr64;
        volatile uint8_t *ptr = (void *)buf;
        /* Bit reversed standard CRC32 polynomial - 0xedb88320 */
        uint32_t poly = 0x04c11db7;
        CVMX_MT_CRC_POLYNOMIAL(poly);
        /* Do endian byte swap on CRC, then use reflect mode of
         * MT IV to reverse the bits within the bytes.  This results
         * in a bit-reversed IV. */
        CVMX_ES32(crc, crc);
        CVMX_MT_CRC_IV_REFLECT(crc);
        while ((((intptr_t)ptr) & 0x7) && len) {
                CVMX_MT_CRC_BYTE_REFLECT(*ptr++);
                len--;
        }
        ptr64 = (void *)ptr;
        while (len > 8) {
                CVMX_MT_CRC_DWORD_REFLECT(*ptr64++);
                len -= 8;
        }
        ptr = (void *)ptr64;
        while (len--)
                CVMX_MT_CRC_BYTE_REFLECT(*ptr++);

        CVMX_MF_CRC_IV_REFLECT(crc);

        return crc;
}

/**
 * Perform a CRC32 calculation on a buffer
 * 
 * @param crc - initial crc value
 * @param p - pointer to buffer
 * @param len - length of buffer
 * 
 * @return CRC value
 */
static uint32_t crc32(uint32_t crc, const uint8_t *p, uint32_t len)
{
        return crc32_no_comp(crc ^ 0xffffffffL, p, len) ^ 0xffffffffL;
}


/**
 * Setup NAND flash so we can start reading from it
 */
STATIC inline void setup_nand_dma(void)
{
    /* Disable access to region 0 to stop stray core accesses */
    cvmx_mio_boot_reg_cfgx_t mio_boot_reg_cfg;
    mio_boot_reg_cfg.u64 = __cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
    mio_boot_reg_cfg.s.en = 0;
    __cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(0), mio_boot_reg_cfg.u64);

    /* Disable boot mode and reset the command fifo */
    cvmx_ndf_misc_t ndf_misc;
    ndf_misc.u64 = __cvmx_read_csr(CVMX_NDF_MISC);
    ndf_misc.s.bt_dma = 0;
    ndf_misc.s.bt_dis = 1;
    ndf_misc.s.rst_ff = 1;
    __cvmx_write_csr(CVMX_NDF_MISC, ndf_misc.u64);
    __cvmx_read_csr(CVMX_NDF_MISC);
    /* Bring the fifo out of reset */
    ndf_misc.s.rst_ff = 0;
    __cvmx_write_csr(CVMX_NDF_MISC, ndf_misc.u64);
    __cvmx_read_csr(CVMX_NDF_MISC);

    /* Setup the timing parameters to tm_mult=1, all params 32 * 2^tm_mult */
    __cvmx_write_csr(CVMX_NDF_CMD, 0x2020202020202011ull);
}


/**
 * Read a GPIO, or something else, and determine if we should
 * load the failsafe or normal stage 2 bootloader.
 *
 * @return 1 for failsafe, 0 for normal
 */
STATIC int get_failsafe_gpio(void)
{
    /* Assume GPIO 0 being set signals a failsafe boot */
    return 1 & __cvmx_read_csr(CVMX_GPIO_RX_DAT);
}


/**
 * Calculate a bootlaoder CRC over a region
 *
 * @param base   Starting address
 * @param length Number of bytes to CRC
 *
 * @return CRC result
 */
STATIC uint32_t calculate_crc(const void *base, int length)
{
    return crc32(0, base, length);
}


/**
 * Read an Octeon ECC block from NAND
 *
 * @param ecc_block_num
 *                   Linearly incrementing ECC block number
 * @param l2_address Address in L2 to write it to
 */
STATIC void read_nand_block(uint64_t ecc_block_num, uint64_t l2_address)
{
    uint64_t cmd;

    /* Get the NAND config setup in start.S */
    cvmx_ndf_bt_pg_info_t ndf_bt_pg_info;
    ndf_bt_pg_info.u64 = __cvmx_read_csr(CVMX_NDF_BT_PG_INFO);

    /* Use the NAND config to map our linear ecc block number into a
        page number and page index */
    uint64_t column_bits = ndf_bt_pg_info.s.size+9;
    uint64_t column_shift = (column_bits+7)>>3<<3;
    uint64_t blocks_per_page = 1<<(column_bits-8);
    uint64_t page_index = 264 * (ecc_block_num & (blocks_per_page-1));
    uint64_t page_number = ecc_block_num >> (column_bits-8);
    uint64_t nand_address = page_index + (page_number << column_shift);

    /* Get the bootbus region 0 config so we know the width of the NAND
        interface */
    cvmx_mio_boot_reg_cfgx_t mio_boot_reg_cfg;
    mio_boot_reg_cfg.u64 = __cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));

    /* Build NAND command by hand to save space in flash. Every byte increases
        the odds we get killed by a double bit error in early boot */
    cmd = 0x001f;                   /* Bus select */
    cmd |= 0x0083 << 16;            /* Chip select */
    if (mio_boot_reg_cfg.s.width)
        cmd |= 0x0200 << 16;        /* 16bit chip select */
    else
        cmd |= 0x0100 << 16;        /* 8bit chip select */
    cmd |= 0x00c80004ull << 32;     /* CLE Command for NAND read with clen=[0,1,3] */
    __cvmx_write_csr(CVMX_NDF_CMD, cmd);

    cmd = 0x0a8c0005;               /* ALE with alen=[4,1,2,5] */
    cmd |= ndf_bt_pg_info.s.adr_cyc << 8;
    cmd |= nand_address << 32;      /* low 32 bits of ALE */
    __cvmx_write_csr(CVMX_NDF_CMD, cmd);
    if (ndf_bt_pg_info.s.adr_cyc > 4)
    {
        cmd = nand_address >> 32;   /* High 32 bits of ALE */
        __cvmx_write_csr(CVMX_NDF_CMD, cmd);
    }

    cmd = 0x008c3004ull;            /* CLE Command for NAND read2 with clen=[0,1,3] */
    cmd |= 0x00000112ull << 32;     /* Wait with r_b=1, n=1 */
    __cvmx_write_csr(CVMX_NDF_CMD, cmd);

    cmd = 0x3f101089ull << 32;     /* Read of 264 bytes with rdn=[1,6,7,1] */
    __cvmx_write_csr(CVMX_NDF_CMD, cmd);

    /* Setup the DMA */
    cvmx_mio_ndf_dma_cfg_t ndf_dma_cfg;
    ndf_dma_cfg.u64 = 0;
    ndf_dma_cfg.s.en = 1;
    ndf_dma_cfg.s.rw = 0;
    ndf_dma_cfg.s.size = 264/8-1;
    ndf_dma_cfg.s.adr = l2_address;
    asm volatile ("sync" ::: "memory");
    __cvmx_write_csr(CVMX_MIO_NDF_DMA_CFG, ndf_dma_cfg.u64);

    cmd = 0x0003;                   /* Chip deselect */
    cmd |= 0x000f << 16;            /* Bus release */
    __cvmx_write_csr(CVMX_NDF_CMD, cmd);

    /* Ring the doorbell */
    __cvmx_write_csr(CVMX_NDF_DRBELL, 1);

    /* Wait for the DMA to complete. You gotta love that you can DMA into L2
        before DRAM is intialized */
    do
    {
        ndf_dma_cfg.u64 = __cvmx_read_csr(CVMX_MIO_NDF_DMA_CFG);
    } while (ndf_dma_cfg.s.en);
}


/**
 * Check an Octeon ECC block, fixing errors if possible
 *
 * @param block  Pointer to block to check
 *
 * @return Zero if block has no errors, one if errors were corrected, two
 *         if the errors could not be corrected.
 */
STATIC int ecc_correct_block(uint8_t *block)
{
    unsigned char pd0, pd1, pd2;
    int i, j;
    unsigned char xorpd0, xorpd1, xorpd2;
    int xor_num;
    unsigned int check;

    asm volatile ("pref 0,0(%0);pref 0,128(%0);pref 0,256(%0)\n" :: "r" (block));

    pd0 = pd1 = pd2 = 0;

    for (i = 0; i < 256; i++)   /* PD0<0> */
        pd0 ^= (block[i] ^ (block[i] >> 2) ^ (block[i] >> 4) ^ (block[i] >> 6)) & 1;
    for (i = 0; i < 256; i++)   /* PD0<1> */
        pd0 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 4) ^ (block[i] >> 5)) & 1) << 1;
    for (i = 0; i < 256; i++)   /* PD0<2> */
        pd0 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 2) ^ (block[i] >> 3)) & 1) << 2;
    for (i = 0; i < 128; i++)   /* PD0<3> */
        pd0 ^= ((block[2*i] ^ (block[2*i] >> 1) ^ (block[2*i] >> 2) ^
                 (block[2*i] >> 3) ^ (block[2*i] >> 4) ^ (block[2*i] >> 5) ^
                 (block[2*i] >> 6) ^ (block[2*i] >> 7)) & 1) << 3;
    for (i = 0; i < 64; i++)    /* PD0<4> */
        for (j = 0; j < 2; j++)
            pd0 ^= ((block[4*i+j] ^ (block[4*i+j] >> 1) ^ (block[4*i+j] >> 2) ^
                     (block[4*i+j] >> 3) ^ (block[4*i+j] >> 4) ^ (block[4*i+j] >> 5) ^
                     (block[4*i+j] >> 6) ^ (block[4*i+j] >> 7)) & 1) << 4;
    for (i = 0; i < 32; i++)    /* PD0<5> */
        for (j = 0; j < 4; j++)
            pd0 ^= ((block[8*i+j] ^ (block[8*i+j] >> 1) ^ (block[8*i+j] >> 2) ^
                     (block[8*i+j] >> 3) ^ (block[8*i+j] >> 4) ^ (block[8*i+j] >> 5) ^
                     (block[8*i+j] >> 6) ^ (block[8*i+j] >> 7)) & 1) << 5;
    for (i = 0; i < 16; i++)    /* PD0<6> */
        for (j = 0; j < 8; j++)
            pd0 ^= ((block[16*i+j] ^ (block[16*i+j] >> 1) ^ (block[16*i+j] >> 2) ^
                     (block[16*i+j] >> 3) ^ (block[16*i+j] >> 4) ^ (block[16*i+j] >> 5) ^
                     (block[16*i+j] >> 6) ^ (block[16*i+j] >> 7)) & 1) << 6;
    for (i = 0; i < 8; i++)     /* PD0<7> */
        for (j = 0; j < 16; j++)
            pd0 ^= ((block[32*i+j] ^ (block[32*i+j] >> 1) ^ (block[32*i+j] >> 2) ^
                     (block[32*i+j] >> 3) ^ (block[32*i+j] >> 4) ^ (block[32*i+j] >> 5) ^
                     (block[32*i+j] >> 6) ^ (block[32*i+j] >> 7)) & 1) << 7;
    for (i = 0; i < 4; i++)     /* PD1<0> */
        for (j = 0; j < 32; j++)
            pd1 ^= ((block[64*i+j] ^ (block[64*i+j] >> 1) ^ (block[64*i+j] >> 2) ^
                     (block[64*i+j] >> 3) ^ (block[64*i+j] >> 4) ^ (block[64*i+j] >> 5) ^
                     (block[64*i+j] >> 6) ^ (block[64*i+j] >> 7)) & 1) << 0;
    for (i = 0; i < 2; i++)     /* PD1<1> */
        for (j = 0; j < 64; j++)
            pd1 ^= ((block[128*i+j] ^ (block[128*i+j] >> 1) ^ (block[128*i+j] >> 2) ^
                     (block[128*i+j] >> 3) ^ (block[128*i+j] >> 4) ^ (block[128*i+j] >> 5) ^
                     (block[128*i+j] >> 6) ^ (block[128*i+j] >> 7)) & 1) << 1;
    for (i = 0; i < 128; i++)   /* PD1<2> */
        pd1 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 2) ^
                 (block[i] >> 3) ^ (block[i] >> 4) ^ (block[i] >> 5) ^
                 (block[i] >> 6) ^ (block[i] >> 7)) & 1) << 2;
    /* PD1<3> */
    /* PD1<4> */
    for (i = 0; i < 256; i++)   /* PD1<5> */
        pd1 ^= (((block[i] >> 1) ^ (block[i] >> 3) ^ (block[i] >> 5) ^ (block[i] >> 7)) & 1) << 5;
    for (i = 0; i < 256; i++)   /* PD1<6> */
        pd1 ^= (((block[i] >> 2) ^ (block[i] >> 3) ^ (block[i] >> 6) ^ (block[i] >> 7)) & 1) << 6;
    for (i = 0; i < 256; i++)   /* PD1<7> */
        pd1 ^= (((block[i] >> 4) ^ (block[i] >> 5) ^ (block[i] >> 6) ^ (block[i] >> 7)) & 1) << 7;
    for (i = 0; i < 128; i++)   /* PD2<0> */
        pd2 ^= ((block[2*i+1] ^ (block[2*i+1] >> 1) ^ (block[2*i+1] >> 2) ^
                 (block[2*i+1] >> 3) ^ (block[2*i+1] >> 4) ^ (block[2*i+1] >> 5) ^
                 (block[2*i+1] >> 6) ^ (block[2*i+1] >> 7)) & 1) << 0;
    for (i = 0; i < 64; i++)    /* PD2<1> */
        for (j = 2; j < 4; j++)
            pd2 ^= ((block[4*i+j] ^ (block[4*i+j] >> 1) ^ (block[4*i+j] >> 2) ^
                     (block[4*i+j] >> 3) ^ (block[4*i+j] >> 4) ^ (block[4*i+j] >> 5) ^
                     (block[4*i+j] >> 6) ^ (block[4*i+j] >> 7)) & 1) << 1;
    for (i = 0; i < 32; i++)    /* PD2<2> */
        for (j = 4; j < 8; j++)
            pd2 ^= ((block[8*i+j] ^ (block[8*i+j] >> 1) ^ (block[8*i+j] >> 2) ^
                     (block[8*i+j] >> 3) ^ (block[8*i+j] >> 4) ^ (block[8*i+j] >> 5) ^
                     (block[8*i+j] >> 6) ^ (block[8*i+j] >> 7)) & 1) << 2;
    for (i = 0; i < 16; i++)    /* PD2<3> */
        for (j = 8; j < 16; j++)
            pd2 ^= ((block[16*i+j] ^ (block[16*i+j] >> 1) ^ (block[16*i+j] >> 2) ^
                     (block[16*i+j] >> 3) ^ (block[16*i+j] >> 4) ^ (block[16*i+j] >> 5) ^
                     (block[16*i+j] >> 6) ^ (block[16*i+j] >> 7)) & 1) << 3;
    for (i = 0; i < 8; i++)     /* PD2<4> */
        for (j = 16; j < 32; j++)
            pd2 ^= ((block[32*i+j] ^ (block[32*i+j] >> 1) ^ (block[32*i+j] >> 2) ^
                     (block[32*i+j] >> 3) ^ (block[32*i+j] >> 4) ^ (block[32*i+j] >> 5) ^
                     (block[32*i+j] >> 6) ^ (block[32*i+j] >> 7)) & 1) << 4;
    for (i = 0; i < 4; i++)     /* PD2<5> */
        for (j = 32; j < 64; j++)
            pd2 ^= ((block[64*i+j] ^ (block[64*i+j] >> 1) ^ (block[64*i+j] >> 2) ^
                     (block[64*i+j] >> 3) ^ (block[64*i+j] >> 4) ^ (block[64*i+j] >> 5) ^
                     (block[64*i+j] >> 6) ^ (block[64*i+j] >> 7)) & 1) << 5;
    for (i = 0; i < 2; i++)     /* PD2<6> */
        for (j = 64; j < 128; j++)
            pd2 ^= ((block[128*i+j] ^ (block[128*i+j] >> 1) ^ (block[128*i+j] >> 2) ^
                     (block[128*i+j] >> 3) ^ (block[128*i+j] >> 4) ^ (block[128*i+j] >> 5) ^
                     (block[128*i+j] >> 6) ^ (block[128*i+j] >> 7)) & 1) << 6;
    for (i = 128; i < 256; i++) /* PD2<7> */
        pd2 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 2) ^
                 (block[i] >> 3) ^ (block[i] >> 4) ^ (block[i] >> 5) ^
                 (block[i] >> 6) ^ (block[i] >> 7)) & 1) << 7;

    xorpd0 = pd0 ^ block[256];
    xorpd1 = pd1 ^ block[257];
    xorpd2 = pd2 ^ block[258];

    xor_num = __builtin_popcount((xorpd0 << 16) | (xorpd1 << 8) | xorpd2);
    check = (((xorpd1 & 7) << 8) | xorpd0) ^ ((xorpd2 << 3) | (xorpd1 >> 5));

    if (xor_num == 0)
        return 0;
    else if ((xor_num > 1) && (check != 0x7FF))
        return 2;

    if (check == 0x7FF)
    {
        /* Correct the error */
        block[xorpd2] ^= 1 << (xorpd1 >> 5);
    }

    return 1;
}


/**
 * Lock a single line into L2. The line is zeroed before locking
 * to make sure no dram accesses are made.
 *
 * @param addr   Physical address to lock
 *
 * @return Zero on success
 */
STATIC int l2c_lock_line(uint64_t addr)
{
#if OCTEON_MODEL == OCTEON_CN52XX
    const int l2_set_bits =  9; /* 512 sets */
    char *addr_ptr = phys_to_ptr(addr);
    int retval = 0;
    cvmx_l2c_lckbase_t lckbase;
    cvmx_l2t_err_t l2t_err;
    lckbase.u64 = 0;

    /* Clear l2t error bits if set */
    l2t_err.u64 = __cvmx_read_csr(CVMX_L2T_ERR);
    l2t_err.s.lckerr = 1;
    l2t_err.s.lckerr2 = 1;
    __cvmx_write_csr(CVMX_L2T_ERR, l2t_err.u64);

    addr &= ~127;

    /* Set this core as debug core */
    __cvmx_write_csr(CVMX_L2C_DBG, 0);
    __cvmx_write_csr(CVMX_L2C_LCKOFF, 0);

    if (((cvmx_l2c_cfg_t)(__cvmx_read_csr(CVMX_L2C_CFG))).s.idxalias)
    {
        int alias_shift = 7 + 2 * l2_set_bits - 1;
        uint64_t addr_tmp = addr ^ (addr & ((1 << alias_shift) - 1)) >> l2_set_bits;
        lckbase.s.lck_base = addr_tmp >> 7;
    }
    else
    {
        lckbase.s.lck_base = addr >> 7;
    }

    lckbase.s.lck_ena = 1;

    asm volatile (
        "sd %[lck], 0(%[lckbase])   \r\n"   /* Write CVMX_L2C_LCKBASE */
        "ld $0, 0(%[lckbase])       \r\n"   /* Read it back to make sure write completes */
        "pref 30, %[line]           \r\n"   /* Prepare for store to the line */
        "ld $0, %[line]             \r\n"   /* Load the line */
        "sd $0, 0(%[lckbase])       \r\n"   /* Disable CVMX_L2C_LCKBASE */
        "ld $0, 0(%[lckbase])       \r\n"   /* Read it back to make sure write completes */
         :: [lck] "r" (lckbase.u64),
            [lckbase] "r" (CVMX_L2C_LCKBASE),
            [line] "m" (*addr_ptr));

    l2t_err.u64 = __cvmx_read_csr(CVMX_L2T_ERR);
    if (l2t_err.s.lckerr || l2t_err.s.lckerr2)
        retval = 1;  /* We were unable to lock the line */
    return(retval);
#else
    char *addr_ptr = phys_to_ptr(addr);
    asm volatile (
        "pref 30, %[line]           \r\n"   /* Prepare for store to the line */
        "cache 31, %[line]          \r\n"   /* Lock the line */
         :: [line] "m" (*addr_ptr));
    return 0;
#endif
}


/**
 * Lock a physical memory range into L2. The entire range will be
 * zeroed.
 *
 * @param start  Starting physical address
 * @param len    Length in bytes
 *
 * @return Zero on success
 */
STATIC int l2c_lock_mem_region(uint64_t start, uint64_t len)
{
    int retval = 0;

    /* Round start/end to cache line boundaries */
    len += start & 127;
    start &= ~127;
    len = (len + 127) & ~127;

    while (len)
    {
        retval += l2c_lock_line(start);
        start += 128;
        len -= 128;
    }

    return(retval);
}


/**
 * Use the bootbus moveable regions and L2 to give us some
 * exception vectors in case we die.
 */
STATIC void install_exception_vectors(void)
{
    extern uint64_t handle_exception_stub;
    /* Install moveable region 0 at the reset vector */
    __cvmx_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0);
    __cvmx_write_csr(CVMX_MIO_BOOT_LOC_DAT, handle_exception_stub);
    __cvmx_write_csr(CVMX_MIO_BOOT_LOC_CFGX(0), (1ull<<31) | (0x1fc00000>>7<<3));

    /* Install moveable region 1 at the ejtag exception vector */
    __cvmx_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0x80);
    __cvmx_write_csr(CVMX_MIO_BOOT_LOC_DAT, handle_exception_stub);
    __cvmx_write_csr(CVMX_MIO_BOOT_LOC_CFGX(1), (1ull<<31) | (0x1fc00480>>7<<3));

    /* The exception and interrupt handler locations are 0x0, 0x80, 0x100,
        0x180, and 0x200 when BEV is not set. BEV was cleared earlier in the
        assembly since NAND doesn't have the exception vectors */
    l2c_lock_line(0x000);
    *(uint64_t*)phys_to_ptr(0x000) = handle_exception_stub;
    l2c_lock_line(0x080);
    *(uint64_t*)phys_to_ptr(0x080) = handle_exception_stub;
    l2c_lock_line(0x100);
    *(uint64_t*)phys_to_ptr(0x100) = handle_exception_stub;
    l2c_lock_line(0x180);
    *(uint64_t*)phys_to_ptr(0x180) = handle_exception_stub;
    l2c_lock_line(0x200);
    *(uint64_t*)phys_to_ptr(0x200) = handle_exception_stub;

    /* Make sure all writes are done and the icache is updated */
    asm volatile ("sync; synci 0($0)" ::: "memory");
}


/**
 * Search NAND and load the stage 2 bootloader
 *
 * @param failsafe Non zero if we should look for a failsafe image instead
 *                 of a normal image.
 *
 * @return Zero on failure, otherwise the address in L2 where the image
 *         starts.
 */
STATIC void *load_stage2_bootloader(int failsafe)
{
    uint64_t    ecc_block_num = 0;
    int         bytes_to_copy = 0;
    uint64_t    l2_address = ptr_to_phys(dma_buffer);
    void *      image_load_ptr = NULL;
    enum
    {
        STAGE_LOOKING_FOR_HEADER,
        STAGE_READING_DATA
    } stage;
    stage = STAGE_LOOKING_FOR_HEADER;

    /* Let the world know what is going on */
    uart_write_string("Searching for ");
    uart_write_string((failsafe) ? "failsafe...\r\n" : "stage2...\r\n");

    while (1)
    {
        /* Bail if we search the low NAND range. We should
            have found a bootlaoder by now */
        if (ecc_block_num >= MAX_NAND_SEARCH_ADDR/CVMX_NAND_BOOT_ECC_BLOCK_SIZE) {
            uart_write_string((failsafe) ? "failsafe...\r\n" : "stage2...\r\n");
            uart_write_string("not found in low NAND memory.\r\n");
            return 0;
        }
        /* Read the next ECC block from NAND */
        read_nand_block(ecc_block_num, l2_address);
        ecc_block_num++;
        /* Check ECC and see if the block is usable */
        uint8_t *l2_ptr = phys_to_ptr(l2_address);
        int errors = ecc_correct_block(l2_ptr);
        if (errors > 1)
        {
            if (stage == STAGE_READING_DATA)
            {
            /* Block has too many errors, throw it away and start over looking
                for a header */
                uart_write_string("ERROR: Block ");
                uart_write_hex(ecc_block_num-1);
                uart_write_string(" uncorrectable\r\n");
            }
            l2_address = ptr_to_phys(dma_buffer);
            stage = STAGE_LOOKING_FOR_HEADER;
        }
        else if (stage == STAGE_LOOKING_FOR_HEADER)
        {
            const bootloader_header_t *header = (const bootloader_header_t *)l2_ptr;
            /* Block is good, see if it contains a bootloader image header */
            if ((header->magic == BOOTLOADER_HEADER_MAGIC) &&
                (header->maj_rev == 1) &&
                (header->image_type == BL_HEADER_IMAGE_STAGE2) /*&&
                ((!!(header->flags & BL_HEADER_FLAG_FAILSAFE)) == (uint32_t)failsafe)*/)
            {

                /* Check the CRC of the header */
                uint32_t crc;
                crc = crc32(0, (void *)header, 12);
                crc = crc32(crc,  (void *)header + 16, header->hlen - 16);

                if (crc != header->hcrc)
                {
                    uart_write_string("ERROR: Header CRC mismatch, expected: ");
                    uart_write_hex(header->hcrc);
                    uart_write_string(" computed: ");
                    uart_write_hex(crc);
                    uart_write_string("\r\n");
                }
                else
                {
                    image_load_ptr = (void*)STAGE2_DRAM_ADDR;
                    uart_write_string("Header at block ");
                    uart_write_hex(ecc_block_num-1);
                    uart_write_string(", image size ");
                    uart_write_hex(header->dlen);
                    uart_write_string(", addr ");
                    uart_write_hex((uint64_t)image_load_ptr);
                    uart_write_string(", crc ");
                    uart_write_hex(header->hcrc);
                    uart_write_string("\r\n");
                    bytes_to_copy = header->hlen + header->dlen;
                    l2_address = ptr_to_phys(image_load_ptr);
                    if ((l2_address < ptr_to_phys(&_end)) && (l2_address + bytes_to_copy > ptr_to_phys(&__start)))
                    {
                        uart_write_string("ERROR: Image overlaps with stage1, skipping\r\n");
                        l2_address = ptr_to_phys(dma_buffer);
                        stage = STAGE_LOOKING_FOR_HEADER;
                    }
                    else
                    {
                        uart_write_string("Starting L2 locking.\r\n");
                        if (l2c_lock_mem_region(l2_address, bytes_to_copy+264))
                            uart_write_string("ERROR: L2 locking failed\r\n");
                        else
                            uart_write_string("L2 locking complete\r\n");
                        /* We've already copied the data after the header */
                        __builtin_memcpy(image_load_ptr, dma_buffer, 256);
                        bytes_to_copy -= 256;
                        l2_address += 256;
                        stage = STAGE_READING_DATA;
                    }
                }
            }
        }
        else
        {
            /* Data block for the bootloader. Print a message to let
                people know on blocks with corrected errors */
            if (errors)
            {
                uart_write_string("Corrected block ");
                uart_write_hex(ecc_block_num);
                uart_write_string("\r\n");
            }
            /* Move forward in L2 and reduce the bytes to go */
            l2_address += 256;
            bytes_to_copy -= 256;
            if (bytes_to_copy <= 0)
            {
                /* We loaded the entire image. Check its CRC */
                const bootloader_header_t *header = image_load_ptr;
                uint32_t crc = calculate_crc((void*)header + header->hlen, header->dlen);
                if (crc == header->dcrc)
                {
                    uart_write_string("Finished copying to ");
		    uart_write_hex(image_load_ptr);
		    uart_write_string("\r\nCRC OK\r\n");
                    return image_load_ptr;
                }
                else
                {
                    uart_write_string("ERROR: Image fails CRC\r\n");
                    uart_write_string("Expected CRC ");
                    uart_write_hex(header->dcrc);
                    uart_write_string("\r\nGot CRC ");
                    uart_write_hex(crc);
                    uart_write_string("\r\n");
                    l2_address = ptr_to_phys(dma_buffer);
                    stage = STAGE_LOOKING_FOR_HEADER;
                }
            }
        }
    }
}


/**
 * Our main entry point into C code. This is called by start.S
 * as soon as we're copied into L2 and have a stack.
 *
 * @return We should never return
 */
int main(void)
{
#ifdef DEBUG
    uart_write_string("Running in C code\r\n");
    uart_write_string("Stage1 is located at ");
    uart_write_hex(ptr_to_phys(&__start));
    uart_write_string(" - ");
    uart_write_hex(ptr_to_phys(&_end));
    uart_write_string("\r\n");
#endif
#if OCTEON_MODEL == OCTEON_CN52XX
    cvmx_l2c_cfg_t l2c_cfg;
    l2c_cfg.u64 = __cvmx_read_csr(CVMX_L2C_CFG);
    if (l2c_cfg.s.lbist)
    {
        uint64_t bist = __cvmx_read_csr(CVMX_L2C_BST0);
        if (bist)
        {
            uart_write_string("L2C BIST0 Falure: ");
            uart_write_hex(bist);
            uart_write_string("\r\n");
        }
        bist = __cvmx_read_csr(CVMX_L2C_BST1);
        if (bist)
        {
            uart_write_string("L2C BIST1 Falure: ");
            uart_write_hex(bist);
            uart_write_string("\r\n");
        }
        bist = __cvmx_read_csr(CVMX_L2C_BST2);
        if (bist)
        {
            uart_write_string("L2C BIST2 Falure: ");
            uart_write_hex(bist);
            uart_write_string("\r\n");
        }
    }
#else
    // FIXME: Check L2 BIST
#endif
    install_exception_vectors();
    setup_nand_dma();
    int need_failsafe = get_failsafe_gpio();

failsafe_retry:
    ;
    void *address = load_stage2_bootloader(need_failsafe);
    if (address)
    {
        uart_write_string("Jumping to ");
        uart_write_hex((uint64_t)address);
        uart_write_string("...\r\n");
        jump_to_bootloader(address);
    }

    if (!need_failsafe)
    {
        uart_write_string("ERROR: Unable to find stage 2 bootloader, trying failsafe\r\n");
        need_failsafe = 1;
        goto failsafe_retry;
    }
    uart_write_string("ERROR: Unable to find stage 2 failsafe bootloader. Halting!\r\n");
    /* We have nowhere to go. So now what? */
    while (1) {}
}
