/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <lzma/LzmaTypes.h>
#include <lzma/LzmaDec.h>
#include <lzma/LzmaTools.h>

#ifdef CONFIG_SYS_BOOTM_LEN
# define MAX_UNCOMPRESS_SIZE	CONFIG_SYS_BOOTM_LEN
#else
# define MAX_UNCOMPRESS_SIZE	((unsigned long)(-1))
#endif

static int do_unlzma(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long unc_len = MAX_UNCOMPRESS_SIZE;
	unsigned long src, dst;
	uint32_t src_len = ~0UL, dst_len = unc_len;
	int ret;
	char buf[32];

	switch (argc) {
		case 4:
			dst_len = simple_strtoul(argv[3], NULL, 16);
			/* fall through */
		case 3:
			src = simple_strtoul(argv[1], NULL, 16);
			dst = simple_strtoul(argv[2], NULL, 16);
			break;
		default:
			return cmd_usage(cmdtp);
	}

	ret = lzmaBuffToBuffDecompress((unsigned char *)dst, &dst_len,
				       (unsigned char *)src, src_len);

	if (ret) {
		printf("LZMA decompression returned %d\n", ret);
		return ret;
	}
	printf("Uncompressed size: %u = 0x%X\n", dst_len, dst_len);
	sprintf(buf, "%X", dst_len);
	setenv("filesize", buf);
	sprintf(buf, "%lX", dst);
	setenv("fileaddr", buf);

	return 0;
}

U_BOOT_CMD(
	unlzma,	4,	1,	do_unlzma,
	"uncompress a lzma compressed memory region",
	"srcaddr dstaddr [dstsize]"
);
