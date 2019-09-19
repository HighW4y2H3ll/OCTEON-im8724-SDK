/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2012 Cavium, Inc. <support@cavium.com>
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

/*
 * Memory Functions
 *
 * Copied from FADS ROM, Dan Malek (dmalek@jlc.net)
 */

#include <common.h>
#include <command.h>
#include <compiler.h>
#ifdef CONFIG_HAS_DATAFLASH
#include <dataflash.h>
#endif
#include <watchdog.h>
#include <linux/ctype.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access.h>

/* Display values from last command.
 * Memory modify remembered values are different from display memory.
 */
static uint64_t	dp_last_addr, dp_last_size;
static uint64_t	dp_last_length = 0x40;
static uint64_t	mm_last_addr, mm_last_size;
static int dp_last_swap = 0;

uint64_t	base_address64 = (1ull << 63);

static int mod_mem64(cmd_tbl_t *, int, int, int, char * const []);

#define DEFAULT_LINE_LENGTH_BYTES (16)
#define MAX_LINE_LENGTH_BYTES (64)

int print_buffer64(uint64_t addr, uint64_t data, uint width, uint count,
		   int swap, uint linelen)
{
	/* linebuf as a union causes proper alignment */
	union linebuf {
		uint64_t ud[MAX_LINE_LENGTH_BYTES/sizeof(uint64_t) + 1];
		uint32_t ui[MAX_LINE_LENGTH_BYTES/sizeof(uint32_t) + 1];
		uint16_t us[MAX_LINE_LENGTH_BYTES/sizeof(uint16_t) + 1];
		uint8_t  uc[MAX_LINE_LENGTH_BYTES/sizeof(uint8_t) + 1];
	} lb;
	int i;

	if (linelen * width > MAX_LINE_LENGTH_BYTES)
		linelen = MAX_LINE_LENGTH_BYTES / width;
	if (linelen < 1)
		linelen = DEFAULT_LINE_LENGTH_BYTES / width;

	while (count) {
		printf("%016llx:", addr);

		/* check for overflow condition */
		if (count < linelen)
			linelen = count;

		/* Copy from memory into linebuf and print hex values */
		for (i = 0; i < linelen; i++) {
			uint64_t x;
			if (width == 8) {
				x = lb.ud[i] = cvmx_read_csr(data);
				if (swap)
					x = swab64(x);
			} else if (width == 4) {
				x = lb.ui[i] = cvmx_read64_uint32(data);
				if (swap)
					x = swab32(x);
			} else if (width == 2) {
				x = lb.us[i] = cvmx_read64_uint16(data);
				if (swap)
					x = swab16(x);
			} else {
				x = lb.uc[i] = cvmx_read64_int8(data);
			}
			printf(" %0*llx", width * 2, x);
			data += width;
		}

		/* Print data in ASCII characters */
		for (i = 0; i < linelen * width; i++) {
			if (!isprint(lb.uc[i]) || lb.uc[i] >= 0x80)
				lb.uc[i] = '.';
		}
		lb.uc[i] = '\0';
		printf("    %s\n", lb.uc);

		/* update references */
		addr += linelen * width;
		count -= linelen;

		if (ctrlc())
			return -1;
	}

	return 0;
}

int cmd_get_data_swap64(char* arg)
{
	int len = strlen(arg);
	return (len > 3 && arg[len-1] == 's');
}

int cmd_get_data_size_swap(const char *arg, int *size, bool *swap,
			   int default_size, bool default_swap)
{
	const char *dot = strchr(arg, '.');

	*size = default_size;
	*swap = default_swap;

	if (!dot)
		return 0;

	switch (dot[1]) {
	case 'b':
		*size = 1;
		break;
	case 'w':
		*size = 2;
		break;
	case 'l':
		*size = 4;
		break;
	case 'q':
	case 'd':
		*size = 8;
		break;
	case 's':
		*swap = true;
		break;
	default:
		return -1;
	}
	if (dot[1] != 's')
		*swap = (dot[2] == 's');

	return 0;
}

/* Memory Display
 *
 * Syntax:
 *	md{.b, .w, .l} {addr} {len}
 */
#define DISP_LINE_LEN	16
int do_mem_md64(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t	addr, length;
#if defined(CONFIG_HAS_DATAFLASH)
	uint64_t	nbytes, linebytes;
#endif
	int		size;
	int		rc = CMD_RET_SUCCESS;
	bool		swap = false;

	/* We use the last specified parameters, unless new ones are
	 * entered.
	 */
 	addr = dp_last_addr;
	size = dp_last_size;
	length = dp_last_length;
	swap = dp_last_swap;

	if (argc < 2)
		return CMD_RET_USAGE;

	if ((flag & CMD_FLAG_REPEAT) == 0) {
		/* New command specified.  Check for a size specification.
		 * Defaults to long if no or incorrect specification.
		 */
		if (cmd_get_data_size_swap(argv[0], &size, &swap, size, swap))
			return CMD_RET_USAGE;

		/* Address is specified since argc > 1 */
		addr = simple_strtoull(argv[1], NULL, 16);
		addr |= base_address64;

		/* If another parameter, it is the length to display.
		 * Length is the number of objects, not number of bytes.
		 */
		if (argc > 2)
			length = simple_strtoul(argv[2], NULL, 16);
	}

#if defined(CONFIG_HAS_DATAFLASH)
	/* Print the lines.
	 *
	 * We buffer all read data, so we can make sure data is read only
	 * once, and all accesses are with the specified bus width.
	 */
	nbytes = length * size;
	do {
		char	linebuf[DISP_LINE_LEN];
		void* p;
		linebytes = (nbytes>DISP_LINE_LEN)?DISP_LINE_LEN:nbytes;

		rc = read_dataflash(addr, (linebytes/size)*size, linebuf);
		p = (rc == DATAFLASH_OK) ? linebuf : (void*)addr;
		print_buffer(addr, p, size, linebytes/size, DISP_LINE_LEN/size);

		nbytes -= linebytes;
		addr += linebytes;
		if (ctrlc()) {
			rc = 1;
			break;
		}
	} while (nbytes > 0);
#else

	{
		/* Print the lines. */
		print_buffer64(addr, addr, size, length, swap,
			       DISP_LINE_LEN/size);
		addr += size * length;
	}
#endif

	dp_last_addr = addr;
	dp_last_length = length;
	dp_last_size = size;
	dp_last_swap = swap;
	return rc;
}

int do_mem_mm64 ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return mod_mem64 (cmdtp, 1, flag, argc, argv);
}
int do_mem_nm64 ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return mod_mem64 (cmdtp, 0, flag, argc, argv);
}

int do_mem_mw64 ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t	addr, writeval, count;
	int		size;
	bool		swap = false;

	if ((argc < 3) || (argc > 4))
		return CMD_RET_USAGE;

	/* Check for size specification.
	 */
	if (cmd_get_data_size_swap(argv[0], &size, &swap, 8, swap))
		return CMD_RET_USAGE;

	/* Address is specified since argc > 1 */
	addr = simple_strtoull(argv[1], NULL, 16);
	addr |= base_address64;

	/* Get the value to write. */
	writeval = simple_strtoull(argv[2], NULL, 16);

	/* Count ? */
	if (argc == 4) {
		count = simple_strtoul(argv[3], NULL, 16);
	} else {
		count = 1;
	}

	while (count-- > 0) {
		if (size == 8) {
			if (swap)
				writeval = swab64(writeval);
			cvmx_write_csr(addr, writeval);
		} else if (size == 4) {
			if (swap)
				writeval = swab32(writeval);
			cvmx_write64_uint32(addr, writeval);
		} else if (size == 2) {
			if (swap)
				writeval = swab16(writeval);
			cvmx_write64_uint16(addr, writeval);
		} else {
			cvmx_write64_uint8(addr, writeval);
		}
		addr += size;
	}
	return 0;
}

#ifdef CONFIG_MX_CYCLIC
int do_mem_mdc64 ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i;
	ulong count;

	if (argc < 4)
		return CMD_RET_USAGE;

	count = simple_strtoul(argv[3], NULL, 10);

	for (;;) {
		do_mem_md64 (NULL, 0, 3, argv);

		/* delay for <count> ms... */
		for (i=0; i<count; i++)
			udelay (1000);

		/* check for ctrl-c to abort... */
		if (ctrlc()) {
			puts("Abort\n");
			return 0;
		}
	}

	return 0;
}

int do_mem_mwc64 ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i;
	ulong count;

	if (argc < 4)
		return CMD_RET_USAGE;

	count = simple_strtoul(argv[3], NULL, 10);

	for (;;) {
		do_mem_mw64 (NULL, 0, 3, argv);

		/* delay for <count> ms... */
		for (i=0; i<count; i++)
			udelay (1000);

		/* check for ctrl-c to abort... */
		if (ctrlc()) {
			puts("Abort\n");
			return 0;
		}
	}

	return 0;
}
#endif /* CONFIG_MX_CYCLIC */

int do_mem_cmp64 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t	addr1, addr2, count, ngood;
	int		size;
	int		rcode = 0;
	const char *type;

	if (argc != 4)
		return CMD_RET_USAGE;

	/* Check for size specification.
	*/
	if ((size = cmd_get_data_size(argv[0], 8)) < 0)
		return 1;
	type = size == 8 ? "dword" : size == 4 ? "word" : size == 2 ?
				"halfword" : "byte";

	addr1 = simple_strtoull(argv[1], NULL, 16);
	addr1 |= base_address64;

	addr2 = simple_strtoull(argv[2], NULL, 16);
	addr2 |= base_address64;

	count = simple_strtoull(argv[3], NULL, 16);

#ifdef CONFIG_HAS_DATAFLASH
	if (addr_dataflash(addr1) | addr_dataflash(addr2)) {
		puts ("Comparison with DataFlash space not supported.\n\r");
		return 0;
	}
#endif

	for (ngood = 0; ngood < count; ++ngood) {
		uint64_t word1, word2;
		if (size == 8) {
			word1 = cvmx_read_csr(addr1);
			word2 = cvmx_read_csr(addr2);
		} else if (size == 4) {
			word1 = cvmx_read64_uint32(addr1);
			word2 = cvmx_read64_uint32(addr2);
		} else if (size == 2) {
			word1 = cvmx_read64_uint16(addr1);
			word2 = cvmx_read64_uint16(addr2);
		} else {
			word1 = cvmx_read64_uint8(addr1);
			word2 = cvmx_read64_uint8(addr2);
		}
		if (word1 != word2) {
			printf("%s at 0x%016llx (%#0*llx) != %s at 0x%16llx (%#0*llx)\n",
				type, addr1, size, word1,
				type, addr2, size, word2);
			rcode = 1;
			break;
		}

		addr1 += size;
		addr2 += size;

		/* reset watchdog from time to time */
		if ((ngood % (64 << 10)) == 0)
			WATCHDOG_RESET();
	}

	printf("Total of %llu %s(s) were the same\n", ngood, type);
	return rcode;
}

int do_mem_cp64 ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t	addr, dest, count;
	int		size;

	if (argc != 4)
		return CMD_RET_USAGE;

	/* Check for size specification.
	*/
	if ((size = cmd_get_data_size(argv[0], 8)) < 0)
		return 1;

	addr = simple_strtoull(argv[1], NULL, 16);
	addr |= base_address64;

	dest = simple_strtoull(argv[2], NULL, 16);
	dest |= base_address64;

	count = simple_strtoull(argv[3], NULL, 16);

	if (count == 0) {
		puts ("Zero length ???\n");
		return 1;
	}

#ifndef CONFIG_SYS_NO_FLASH
	/* check if we are copying to Flash */
	if ( (dest < 0xc0000000 && addr2info(dest) != NULL)
#ifdef CONFIG_HAS_DATAFLASH
	   && (!addr_dataflash(dest))
#endif
	   ) {
		int rc;

		if (addr + count >= 0x100000000ull) {
			puts("Source address too high to copy to flash\n");
			return 1;
		}
		puts ("Copy to Flash... ");

		rc = flash_write ((char *)((uint32_t)addr), (uint32_t)dest,
				  count * size);
		if (rc != 0) {
			flash_perror (rc);
			return (1);
		}
		puts ("done\n");
		return 0;
	}
#endif

#ifdef CONFIG_HAS_DATAFLASH
	/* Check if we are copying from RAM or Flash to DataFlash */
	if ((dest < 0xc0000000) &&
	    addr_dataflash((uint32_t)dest) && !addr_dataflash((uint32_t)addr)) {
		int rc;

		if (addr + count >= 0x100000000ull) {
			puts("Source address is too high to copy to flash\n");
			return 1;
		}
		puts ("Copy to DataFlash... ");

		rc = write_dataflash (dest, addr, count*size);

		if (rc != 1) {
			dataflash_perror (rc);
			return (1);
		}
		puts ("done\n");
		return 0;
	}

	/* Check if we are copying from DataFlash to RAM */
	if ((addr < 0xc0000000) && addr_dataflash((uint32_t)addr) &&
	    (dest + count < 0x100000000ull) && !addr_dataflash((uint32_t)dest)
#ifndef CONFIG_SYS_NO_FLASH
				 && (addr2info((uint32_t)dest) == NULL)
#endif
	   ){
		int rc;
		rc = read_dataflash((uint32_t)addr, count * size,
				    (char *)((uint32_t)dest));
		if (rc != 1) {
			dataflash_perror (rc);
			return (1);
		}
		return 0;
	}

	if ((addr | dest) < 0x10000000ull &&
	    addr_dataflash(addr) && addr_dataflash(dest)){
		puts ("Unsupported combination of source/destination.\n\r");
		return 1;
	}
#endif

	while (count-- > 0) {
		if (size == 8)
			cvmx_write_csr(dest, cvmx_read_csr(addr));
		else if (size == 4)
			cvmx_write64_uint32(dest, cvmx_read64_uint32(addr));
		else if (size == 2)
			cvmx_write64_uint16(dest, cvmx_read64_uint16(addr));
		else
			cvmx_write64_uint8(dest, cvmx_read64_uint8(addr));
		addr += size;
		dest += size;

		/* reset watchdog from time to time */
		if ((count % (64 << 10)) == 0)
			WATCHDOG_RESET();
	}
	return 0;
}

int do_mem_base64(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc > 1) {
		/* Set new base address.
		*/
		base_address64 = simple_strtoull(argv[1], NULL, 16);
	}
	/* Print the current base address.
	*/
	printf("Base Address: 0x%16llx\n", base_address64);
	return 0;
}

int do_mem_loop64(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t	addr, length, i, dummy;
	int		size;

	if (argc < 3)
		return CMD_RET_USAGE;

	/* Check for a size spefication.
	 * Defaults to long if no or incorrect specification.
	 */
	if ((size = cmd_get_data_size(argv[0], 8)) < 0)
		return 1;

	/* Address is always specified.
	*/
	addr = simple_strtoull(argv[1], NULL, 16);

	/* Length is the number of objects, not number of bytes.
	*/
	length = simple_strtoull(argv[2], NULL, 16);

	/* We want to optimize the loops to run as fast as possible.
	 * If we have only one object, just run infinite loops.
	 */
	if (length == 1) {
		if (size == 8) {
			for (;;)
				i = cvmx_read_csr(addr);
		} else if (size == 4) {
			for (;;)
				i = cvmx_read64_uint32(addr);
		}
		if (size == 2) {
			for (;;)
				i = cvmx_read64_uint16(addr);
		}
		for (;;)
			i = cvmx_read64_uint8(addr);
	}

	if (size == 8) {
		for (;;) {
			dummy = addr;
			i = length;
			while (i-- > 0)
				cvmx_write_csr(dummy, cvmx_read_csr(dummy) + 1);
		}
	} else if (size == 4) {
		for (;;) {
			dummy = addr;
			i = length;
			while (i-- > 0)
				cvmx_write64_uint32(dummy,
						    cvmx_read64_uint32(dummy) + 1);
		}
	}
	if (size == 2) {
		for (;;) {
			dummy = addr;
			i = length;
			while (i-- > 0)
				cvmx_write64_uint16(dummy,
						    cvmx_read64_uint16(dummy) + 1);
		}
	}
	for (;;) {
		dummy = addr;
		i = length;
		while (i-- > 0)
			cvmx_write64_uint8(dummy, cvmx_read64_uint8(dummy) + 1);
	}
	return -1;
}

#ifdef CONFIG_LOOPW
int do_mem_loopw64 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t	addr, length, i, data, tmp_addr;
	int		size;

	if (argc < 4)
		return CMD_RET_USAGE;

	/* Check for a size spefication.
	 * Defaults to long if no or incorrect specification.
	 */
	if ((size = cmd_get_data_size(argv[0], 8)) < 0)
		return 1;

	/* Address is always specified.
	*/
	addr = simple_strtoull(argv[1], NULL, 16);

	/* Length is the number of objects, not number of bytes.
	*/
	length = simple_strtoull(argv[2], NULL, 16);

	/* data to write */
	data = simple_strtoull(argv[3], NULL, 16);

	/* We want to optimize the loops to run as fast as possible.
	 * If we have only one object, just run infinite loops.
	 */
	if (length == 1) {
		if (size == 8) {
			for (;;)
				cvmx_write_csr(addr, data);
		}
		if (size == 4) {
			for (;;)
				cvmx_write64_uint32(addr, data);
					}
		if (size == 2) {
			for (;;)
				cvmx_write64_uint16(addr, data);
		}
		cp = (u_char *)addr;
		for (;;)
			for (;;)
				cvmx_write64_uint8(addr, data);
	}

	if (size == 8) {
			tmp_addr = addr;
			i = length;
			while (i-- > 0) {
				cvmx_write_csr(tmp_addr, data);
				tmp_addr += size;
			}
		}
	}
	if (size == 4) {
		for (;;) {
			tmp_addr = addr;
			i = length;
			while (i-- > 0) {
				cvmx_write64_uint32(tmp_addr, data);
				tmp_addr += size;
			}
		}
	}
	if (size == 2) {
		for (;;) {
			tmp_addr = addr;
			i = length;
			while (i-- > 0) {
				cvmx_write64_uint16(tmp_addr, data);
				tmp_addr += size;
			}
		}
	}
	for (;;) {
		tmp_addr = addr;
		i = length;
		while (i-- > 0) {
			cvmx_write64_uint8(tmp_addr, data);
			tmp_addr += size;
		}
	}
	return 0;
}
#endif /* CONFIG_LOOPW */

#if 0
/*
 * Perform a memory test. A more complete alternative test can be
 * configured using CONFIG_SYS_ALT_MEMTEST. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 */
int do_mem_mtest64 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	vu_long	*addr, *start, *end;
	ulong	val;
	ulong	readback;
	ulong	errs = 0;
	int iterations = 1;
	int iteration_limit;

#if defined(CONFIG_SYS_ALT_MEMTEST)
	vu_long	len;
	vu_long	offset;
	vu_long	test_offset;
	vu_long	pattern;
	vu_long	temp;
	vu_long	anti_pattern;
	vu_long	num_words;
#if defined(CONFIG_SYS_MEMTEST_SCRATCH)
	vu_long *dummy = (vu_long*)CONFIG_SYS_MEMTEST_SCRATCH;
#else
	vu_long *dummy = 0;	/* yes, this is address 0x0, not NULL */
#endif
	int	j;

	static const ulong bitpattern[] = {
		0x00000001,	/* single bit */
		0x00000003,	/* two adjacent bits */
		0x00000007,	/* three adjacent bits */
		0x0000000F,	/* four adjacent bits */
		0x00000005,	/* two non-adjacent bits */
		0x00000015,	/* three non-adjacent bits */
		0x00000055,	/* four non-adjacent bits */
		0xaaaaaaaa,	/* alternating 1/0 */
	};
#else
	ulong	incr;
	ulong	pattern;
#endif

	if (argc > 1)
		start = (ulong *)simple_strtoul(argv[1], NULL, 16);
	else
		start = (ulong *)CONFIG_SYS_MEMTEST_START;

	if (argc > 2)
		end = (ulong *)simple_strtoul(argv[2], NULL, 16);
	else
		end = (ulong *)(CONFIG_SYS_MEMTEST_END);

	if (argc > 3)
		pattern = (ulong)simple_strtoul(argv[3], NULL, 16);
	else
		pattern = 0;

	if (argc > 4)
		iteration_limit = (ulong)simple_strtoul(argv[4], NULL, 16);
	else
		iteration_limit = 0;

#if defined(CONFIG_SYS_ALT_MEMTEST)
	printf ("Testing %08x ... %08x:\n", (uint)start, (uint)end);
	debug("%s:%d: start 0x%p end 0x%p\n",
		__FUNCTION__, __LINE__, start, end);

	for (;;) {
		if (ctrlc()) {
			putc ('\n');
			return 1;
		}


		if (iteration_limit && iterations > iteration_limit) {
			printf("Tested %d iteration(s) with %lu errors.\n",
				iterations-1, errs);
			return errs != 0;
		}

		printf("Iteration: %6d\r", iterations);
		debug("\n");
		iterations++;

		/*
		 * Data line test: write a pattern to the first
		 * location, write the 1's complement to a 'parking'
		 * address (changes the state of the data bus so a
		 * floating bus doen't give a false OK), and then
		 * read the value back. Note that we read it back
		 * into a variable because the next time we read it,
		 * it might be right (been there, tough to explain to
		 * the quality guys why it prints a failure when the
		 * "is" and "should be" are obviously the same in the
		 * error message).
		 *
		 * Rather than exhaustively testing, we test some
		 * patterns by shifting '1' bits through a field of
		 * '0's and '0' bits through a field of '1's (i.e.
		 * pattern and ~pattern).
		 */
		addr = start;
		for (j = 0; j < sizeof(bitpattern)/sizeof(bitpattern[0]); j++) {
		    val = bitpattern[j];
		    for(; val != 0; val <<= 1) {
			*addr  = val;
			*dummy  = ~val; /* clear the test data off of the bus */
			readback = *addr;
			if(readback != val) {
			    printf ("FAILURE (data line): "
				"expected %08lx, actual %08lx\n",
					  val, readback);
			    errs++;
			    if (ctrlc()) {
				putc ('\n');
				return 1;
			    }
			}
			*addr  = ~val;
			*dummy  = val;
			readback = *addr;
			if(readback != ~val) {
			    printf ("FAILURE (data line): "
				"Is %08lx, should be %08lx\n",
					readback, ~val);
			    errs++;
			    if (ctrlc()) {
				putc ('\n');
				return 1;
			    }
			}
		    }
		}

		/*
		 * Based on code whose Original Author and Copyright
		 * information follows: Copyright (c) 1998 by Michael
		 * Barr. This software is placed into the public
		 * domain and may be used for any purpose. However,
		 * this notice must not be changed or removed and no
		 * warranty is either expressed or implied by its
		 * publication or distribution.
		 */

		/*
		 * Address line test
		 *
		 * Description: Test the address bus wiring in a
		 *              memory region by performing a walking
		 *              1's test on the relevant bits of the
		 *              address and checking for aliasing.
		 *              This test will find single-bit
		 *              address failures such as stuck -high,
		 *              stuck-low, and shorted pins. The base
		 *              address and size of the region are
		 *              selected by the caller.
		 *
		 * Notes:	For best results, the selected base
		 *              address should have enough LSB 0's to
		 *              guarantee single address bit changes.
		 *              For example, to test a 64-Kbyte
		 *              region, select a base address on a
		 *              64-Kbyte boundary. Also, select the
		 *              region size as a power-of-two if at
		 *              all possible.
		 *
		 * Returns:     0 if the test succeeds, 1 if the test fails.
		 */
		len = ((ulong)end - (ulong)start)/sizeof(vu_long);
		pattern = (vu_long) 0xaaaaaaaa;
		anti_pattern = (vu_long) 0x55555555;

		debug("%s:%d: length = 0x%.8lx\n",
			__FUNCTION__, __LINE__,
			len);
		/*
		 * Write the default pattern at each of the
		 * power-of-two offsets.
		 */
		for (offset = 1; offset < len; offset <<= 1) {
			start[offset] = pattern;
		}

		/*
		 * Check for address bits stuck high.
		 */
		test_offset = 0;
		start[test_offset] = anti_pattern;

		for (offset = 1; offset < len; offset <<= 1) {
		    temp = start[offset];
		    if (temp != pattern) {
			printf ("\nFAILURE: Address bit stuck high @ 0x%.8lx:"
				" expected 0x%.8lx, actual 0x%.8lx\n",
				(ulong)&start[offset], pattern, temp);
			errs++;
			if (ctrlc()) {
			    putc ('\n');
			    return 1;
			}
		    }
		}
		start[test_offset] = pattern;
		WATCHDOG_RESET();

		/*
		 * Check for addr bits stuck low or shorted.
		 */
		for (test_offset = 1; test_offset < len; test_offset <<= 1) {
		    start[test_offset] = anti_pattern;

		    for (offset = 1; offset < len; offset <<= 1) {
			temp = start[offset];
			if ((temp != pattern) && (offset != test_offset)) {
			    printf ("\nFAILURE: Address bit stuck low or shorted @"
				" 0x%.8lx: expected 0x%.8lx, actual 0x%.8lx\n",
				(ulong)&start[offset], pattern, temp);
			    errs++;
			    if (ctrlc()) {
				putc ('\n');
				return 1;
			    }
			}
		    }
		    start[test_offset] = pattern;
		}

		/*
		 * Description: Test the integrity of a physical
		 *		memory device by performing an
		 *		increment/decrement test over the
		 *		entire region. In the process every
		 *		storage bit in the device is tested
		 *		as a zero and a one. The base address
		 *		and the size of the region are
		 *		selected by the caller.
		 *
		 * Returns:     0 if the test succeeds, 1 if the test fails.
		 */
		num_words = ((ulong)end - (ulong)start)/sizeof(vu_long) + 1;

		/*
		 * Fill memory with a known pattern.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
			WATCHDOG_RESET();
			start[offset] = pattern;
		}

		/*
		 * Check each location and invert it for the second pass.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
		    WATCHDOG_RESET();
		    temp = start[offset];
		    if (temp != pattern) {
			printf ("\nFAILURE (read/write) @ 0x%.8lx:"
				" expected 0x%.8lx, actual 0x%.8lx)\n",
				(ulong)&start[offset], pattern, temp);
			errs++;
			if (ctrlc()) {
			    putc ('\n');
			    return 1;
			}
		    }

		    anti_pattern = ~pattern;
		    start[offset] = anti_pattern;
		}

		/*
		 * Check each location for the inverted pattern and zero it.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
		    WATCHDOG_RESET();
		    anti_pattern = ~pattern;
		    temp = start[offset];
		    if (temp != anti_pattern) {
			printf ("\nFAILURE (read/write): @ 0x%.8lx:"
				" expected 0x%.8lx, actual 0x%.8lx)\n",
				(ulong)&start[offset], anti_pattern, temp);
			errs++;
			if (ctrlc()) {
			    putc ('\n');
			    return 1;
			}
		    }
		    start[offset] = 0;
		}
	}

#else /* The original, quickie test */
	incr = 1;
	for (;;) {
		if (ctrlc()) {
			putc ('\n');
			return 1;
		}

		if (iteration_limit && iterations > iteration_limit) {
			printf("Tested %d iteration(s) with %lu errors.\n",
				iterations-1, errs);
			return errs != 0;
		}
		++iterations;

		printf ("\rPattern %08lX  Writing..."
			"%12s"
			"\b\b\b\b\b\b\b\b\b\b",
			pattern, "");

		for (addr=start,val=pattern; addr<end; addr++) {
			WATCHDOG_RESET();
			*addr = val;
			val  += incr;
		}

		puts ("Reading...");

		for (addr=start,val=pattern; addr<end; addr++) {
			WATCHDOG_RESET();
			readback = *addr;
			if (readback != val) {
				printf ("\nMem error @ 0x%08X: "
					"found %08lX, expected %08lX\n",
					(uint)(uintptr_t)addr, readback, val);
				errs++;
				if (ctrlc()) {
					putc ('\n');
					return 1;
				}
			}
			val += incr;
		}

		/*
		 * Flip the pattern each time to make lots of zeros and
		 * then, the next time, lots of ones.  We decrement
		 * the "negative" patterns and increment the "positive"
		 * patterns to preserve this feature.
		 */
		if(pattern & 0x80000000) {
			pattern = -pattern;	/* complement & increment */
		}
		else {
			pattern = ~pattern;
		}
		incr = -incr;
	}
#endif
	return 0;	/* not reached */
}
#endif

/* Modify memory.
 *
 * Syntax:
 *	mm{.b, .w, .l} {addr}
 *	nm{.b, .w, .l} {addr}
 */
static int
mod_mem64(cmd_tbl_t *cmdtp, int incrflag, int flag, int argc, char * const argv[])
{
	uint64_t	addr, i;
	int64_t		nbytes, size;

	if (argc != 2)
		return CMD_RET_USAGE;

#ifdef CONFIG_BOOT_RETRY_TIME
	reset_cmd_timeout();	/* got a good command to get here */
#endif
	/* We use the last specified parameters, unless new ones are
	 * entered.
	 */
	addr = mm_last_addr;
	size = mm_last_size;

	if ((flag & CMD_FLAG_REPEAT) == 0) {
		/* New command specified.  Check for a size specification.
		 * Defaults to long if no or incorrect specification.
		 */
		if ((size = cmd_get_data_size(argv[0], 8)) < 0)
			return 1;

		/* Address is specified since argc > 1
		*/
		addr = simple_strtoull(argv[1], NULL, 16);
		addr |= base_address64;
	}

#ifdef CONFIG_HAS_DATAFLASH
	if (addr_dataflash(addr)){
		puts ("Can't modify DataFlash in place. Use cp instead.\n\r");
		return 0;
	}
#endif


	/* Print the address, followed by value.  Then accept input for
	 * the next value.  A non-converted value exits.
	 */
	do {
		printf("%16llx:", addr);
		if (size == 8)
			printf(" %016llx", cvmx_read_csr(addr));
		else if (size == 4)
			printf(" %08x", cvmx_read64_uint32(addr));
		else if (size == 2)
			printf(" %04x", cvmx_read64_uint16(addr));
		else
			printf(" %02x", cvmx_read64_uint8(addr));

		nbytes = readline (" ? ");
		if (nbytes == 0 || (nbytes == 1 && console_buffer[0] == '-')) {
			/* <CR> pressed as only input, don't modify current
			 * location and move to next. "-" pressed will go back.
			 */
			if (incrflag)
				addr += nbytes ? -size : size;
			nbytes = 1;
#ifdef CONFIG_BOOT_RETRY_TIME
			reset_cmd_timeout(); /* good enough to not time out */
#endif
		}
#ifdef CONFIG_BOOT_RETRY_TIME
		else if (nbytes == -2) {
			break;	/* timed out, exit the command	*/
		}
#endif
		else {
			char *endp;
			i = simple_strtoull(console_buffer, &endp, 16);
			nbytes = endp - console_buffer;
			if (nbytes) {
#ifdef CONFIG_BOOT_RETRY_TIME
				/* good enough to not time out
				 */
				reset_cmd_timeout();
#endif
				if (size == 8)
					cvmx_write_csr(addr, i);
				else if (size == 4)
					cvmx_write64_uint32(addr, i);
				else if (size == 2)
					cvmx_write64_uint16(addr, i);
				else
					cvmx_write64_uint8(addr, i);
				if (incrflag)
					addr += size;
			}
		}
	} while (nbytes);

	mm_last_addr = addr;
	mm_last_size = size;
	return 0;
}
#if 0
#ifdef CONFIG_CMD_CRC32

#ifndef CONFIG_CRC32_VERIFY

int do_mem_crc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong addr, length;
	ulong crc;
	ulong *ptr;

	if (argc < 3)
		return CMD_RET_USAGE;

	addr = simple_strtoul (argv[1], NULL, 16);
	addr += base_address;

	length = simple_strtoul (argv[2], NULL, 16);

	crc = crc32_wd (0, (const uchar *) addr, length, CHUNKSZ_CRC32);

	printf ("CRC32 for %08lx ... %08lx ==> %08lx\n",
			addr, addr + length - 1, crc);

	if (argc > 3) {
		ptr = (ulong *) simple_strtoul (argv[3], NULL, 16);
		*ptr = crc;
	}

	return 0;
}

#else	/* CONFIG_CRC32_VERIFY */

int do_mem_crc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong addr, length;
	ulong crc;
	ulong *ptr;
	ulong vcrc;
	int verify;
	int ac;
	char * const *av;

	if (argc < 3) {
usage:
		return CMD_RET_USAGE;
	}

	av = argv + 1;
	ac = argc - 1;
	if (strcmp(*av, "-v") == 0) {
		verify = 1;
		av++;
		ac--;
		if (ac < 3)
			goto usage;
	} else
		verify = 0;

	addr = simple_strtoul(*av++, NULL, 16);
	addr += base_address;
	length = simple_strtoul(*av++, NULL, 16);

	crc = crc32_wd (0, (const uchar *) addr, length, CHUNKSZ_CRC32);

	if (!verify) {
		printf ("CRC32 for %08lx ... %08lx ==> %08lx\n",
				addr, addr + length - 1, crc);
		if (ac > 2) {
			ptr = (ulong *) simple_strtoul (*av++, NULL, 16);
			*ptr = crc;
		}
	} else {
		vcrc = simple_strtoul(*av++, NULL, 16);
		if (vcrc != crc) {
			printf ("CRC32 for %08lx ... %08lx ==> %08lx != %08lx ** ERROR **\n",
					addr, addr + length - 1, crc, vcrc);
			return 1;
		}
	}

	return 0;

}
#endif	/* CONFIG_CRC32_VERIFY */

#endif
#endif /* 0 */
/**************************************************/
U_BOOT_CMD(
	md64,	3,	1,	do_mem_md64,
	"memory display",
	"[.b, .w, .l, .q][s] address [# of objects]\n"
	"where b = byte, w = short, l = long, q = double word and s = byte swap\n"
	"example:\n"
	"  md64.ls 0x10f0000000"
);


U_BOOT_CMD(
	mm64,	2,	1,	do_mem_mm64,
	"memory modify (auto-incrementing address)",
	"[.b, .w, .l, .q] address"
);


U_BOOT_CMD(
	nm64,	2,	1,	do_mem_nm64,
	"memory modify (constant address)",
	"[.b, .w, .l, .q] address"
);

U_BOOT_CMD(
	mw64,	4,	1,	do_mem_mw64,
	"memory write (fill)",
	"[.b, .w, .l, .q][s] address value [count]"
);

U_BOOT_CMD(
	cp64,	4,	1,	do_mem_cp64,
	"memory copy",
	"[.b, .w, .l, .q] source target count"
);

U_BOOT_CMD(
	cmp64,	4,	1,	do_mem_cmp64,
	"memory compare",
	"[.b, .w, .l, .q] addr1 addr2 count"
);
#if 0
#ifdef CONFIG_CMD_CRC32

#ifndef CONFIG_CRC32_VERIFY

U_BOOT_CMD(
	crc32,	4,	1,	do_mem_crc,
	"checksum calculation",
	"address count [addr]\n    - compute CRC32 checksum [save at addr]"
);

#else	/* CONFIG_CRC32_VERIFY */

U_BOOT_CMD(
	crc32,	5,	1,	do_mem_crc,
	"checksum calculation",
	"address count [addr]\n    - compute CRC32 checksum [save at addr]\n"
	"-v address count crc\n    - verify crc of memory area"
);

#endif	/* CONFIG_CRC32_VERIFY */

#endif
#endif
U_BOOT_CMD(
	base64,	2,	1,	do_mem_base64,
	"print or set address offset",
	"\n    - print address offset for memory commands\n"
	"base off\n    - set address offset for memory commands to 'off'"
);

U_BOOT_CMD(
	loop64,	3,	1,	do_mem_loop64,
	"infinite loop on address range",
	"[.b, .w, .l, .q] address number_of_objects"
);

#ifdef CONFIG_LOOPW
U_BOOT_CMD(
	loopw64,	4,	1,	do_mem_loopw64,
	"infinite write loop on address range",
	"[.b, .w, .l, .q] address number_of_objects data_to_write"
);
#endif /* CONFIG_LOOPW */
#if 0
U_BOOT_CMD(
	mtest64,	5,	1,	do_mem_mtest64,
	"simple RAM read/write test",
	"[start [end [pattern [iterations]]]]"
);
#endif
#ifdef CONFIG_MX_CYCLIC
U_BOOT_CMD(
	mdc64,	4,	1,	do_mem_mdc64,
	"memory display cyclic",
	"[.b, .w, .l, .q] address count delay(ms)"
);

U_BOOT_CMD(
	mwc64,	4,	1,	do_mem_mwc64,
	"memory write cyclic",
	"[.b, .w, .l, .q] address value delay(ms)"
);
#endif /* CONFIG_MX_CYCLIC */
