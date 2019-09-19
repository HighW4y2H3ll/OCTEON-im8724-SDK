/***********************license start************************************
 * Copyright (c) 2011 Cavium, Inc. (support@cavium.com). All rights reserved.
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
 *     * Neither the name of Cavium, Inc. nor the names of
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
#ifndef __CONFIG_OCTEON_NAND_H__
#define __CONFIG_OCTEON_NAND_H__

/**
 * Enable the octnand command.  This is needed for writing the stage1 and
 * stage 2 bootloaders into NAND flash.
 */
#define CONFIG_CMD_OCTEON_NAND

/**
 * Enable OCTEON NAND driver
 */
#define CONFIG_NAND_OCTEON

/** Enable the NAND command */
#define CONFIG_CMD_NAND

#ifndef CONFIG_OCTEON_DISABLE_UBI

/** Enable MTD support for UBI volumes */
# define CONFIG_MTD_UBI

/** Enable the UBI command */
# define CONFIG_CMD_UBI

/** Enable support for UBIFS */
# define CONFIG_CMD_UBIFS

/** Enable red-black tree support, needed for UBIFS */
# define CONFIG_RBTREE

/** Enable LZO compression, needed for UBIFS */
# define CONFIG_LZO

#endif /* CONFIG_OCTEON_DISABLE_UBI */

/* NOTE: JFFS2 is not recommended.  Use UBIFS instead. */
#if !defined(CONFIG_OCTEON_DISABLE_JFFS2)

/** Enable NAND support for JFFS2 */
# define CONFIG_JFFS2_NAND

/** Enable ECC support for JFFS2 NAND */
# define CONFIG_MTD_NAND_ECC_JFFS2
#endif

/** Enable ONFI detection */
#define CONFIG_SYS_NAND_ONFI_DETECTION

#endif /* __CONFIG_OCTEON_NAND_H__ */
