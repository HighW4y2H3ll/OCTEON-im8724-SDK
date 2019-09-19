/***********************license start***************
 * Copyright (c) 2003-2012  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Coredump remote dump utility
 *
 * <hr>$Revision$<hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/ptrace.h>

#include "cvmx.h"
#include "cvmx-version.h"
#include "cvmx-swap.h"

#include "octeon-remote.h"

#include "cvmx-coredump-bootmem.h"
#include "cvmx-coredump.h"
#include "oct-remote-coredump-elf.h"

#ifdef CVMX_COREDEUMP_DEBUG
	#define CVMX_COREDUMP_DEBUG_PRINT printf
#else
	#define CVMX_COREDUMP_DEBUG_PRINT(...)
#endif

#define CVMX_DUMP_FILE_NAME	"CVMX_DUMP.CORE"

#define ROUNDUP(x, y) (((x) + (y - 1)) & ~(y - 1))


static inline int
get_first_bit(uint64_t coremask)
{
	int i = 0;
	while (!(coremask & 1)) { i++; coremask >>= 1; }
	return i;
}

static inline int
get_next_bit(uint64_t coremask, uint32_t last)
{
	last++;
	coremask >>= last;
	if (!coremask)
		return 0;

	while (coremask && !(coremask & 1)) { last++; coremask >>= 1;}
	return last;
}

static void
write_remote_mem(int fd, uint64_t addr, uint64_t size)
{
	/* A 4 MB buffer is used as that is what octeon_pci_read_mem uses. */
	static uint8_t buf[4*MB];
	uint64_t start = addr, end = addr + size;
	ssize_t sz;

	/* FIXME: This loop would be better if we used mmap instead of a buf as we have double or more copying happening. */
	while (start < end) {
		ssize_t written;
		sz = end - start;
		if ( sz > (ssize_t)sizeof(buf))
			sz = sizeof(buf);

		octeon_remote_read_mem(buf, addr, sz);
		addr += sz;
		start += sz;
		CVMX_COREDUMP_DEBUG_PRINT ("start: %llx, end: %llx Writing %llx bytes to file @offset:%llx\n",
			(long long)start, (long long)end, (long long )sz,	(long long)lseek(fd, 0, SEEK_CUR));
		written = write(fd, buf, sz);
		if (written != sz)
		{
			fprintf (stderr, "Write did not fully finished.\n");
			abort();
		}
	}
}

static void
coredump_dump_remote_mem(int fd, coredump_tlb_info_t *tlb_info)
{
	Elf_Phdr *phdr = &tlb_info->phdr;
	uint32_t sz = phdr->p_filesz;
	off_t offset;

	offset = lseek(fd, phdr->p_offset, SEEK_SET);
	if (offset != (off_t)phdr->p_offset) {
		printf ("Error, Could go to the correct location.\n");
		exit (1);
	}

	CVMX_COREDUMP_DEBUG_PRINT("start: %lx, Writing %x bytes to file @offset:%lx\n",
		    phdr->p_paddr, sz, lseek(fd, 0, SEEK_CUR));

	write_remote_mem(fd, phdr->p_paddr, sz);

	if (tlb_info->paddr1) {
	    	CVMX_COREDUMP_DEBUG_PRINT("start: %lx, Writing %x bytes to file @offset:%lx\n",
			    tlb_info->paddr1, sz, lseek(fd, 0, SEEK_CUR));

		write_remote_mem(fd, tlb_info->paddr1, sz);
	}
}

static int
coredump_SE(int fd, cvmx_coredump_registers_t *registers,
	    cvmx_coredump_mem_info_t *mem_info, uint64_t tlb_entries)
{
	Elf_Ehdr ehdr;
	Elf_Phdr phdr;

	cvmx_coredump_info_t info;
	off_t offset;
	int i;

	offset = 0;
	info.Ehdr = &ehdr;
	/* 1 Extra for Notes */
	info.nPhdr = cvmx_coredump_count_valid_tlbs(registers, mem_info, tlb_entries) + 1;
	info.fd = fd;

	cvmx_coredump_fill_elf_header(info.Ehdr, info.nPhdr, 0);

	offset += sizeof(ehdr);

	/* Fill Notes so that we can encode note_phdr later */
	if (!cvmx_coredump_fill_notes(&info, registers)) {
		return -1;
	}
	offset += sizeof(phdr) * info.nPhdr;

	size_t sz = cvmx_coredump_get_note_info_sz(&info);

	/* Prepare the Note Phdr, with previously deduced size etc. */
	info.tlb_info = calloc(info.nPhdr, sizeof(*info.tlb_info));
	cvmx_coredump_fill_note_phdr(&info.tlb_info[0].phdr, offset, sz);
	offset += sz;

	offset = ROUNDUP(offset, 4*KB);

	CVMX_COREDUMP_DEBUG_PRINT("offset is :%lx\n", offset);

	/* Fill rest of phdr, we need to encode in phdrs where the actual memory starts,
	 * so the rounding off
	 */
	cvmx_coredump_fill_phdr(registers, &info.tlb_info[1], mem_info, &offset, tlb_entries);

	CVMX_COREDUMP_DEBUG_PRINT("Dumping ELF Header... \n");

	cvmx_coredump_write_ehdr_fields(info.fd, info.Ehdr);

	CVMX_COREDUMP_DEBUG_PRINT("Dumping Program Headers... \n");
	for (i = 0; i < info.nPhdr; i++) {
		cvmx_coredump_write_phdr_fields(info.fd, &info.tlb_info[i].phdr);
	}

	CVMX_COREDUMP_DEBUG_PRINT("Dumping Notes... @%llx \n", lseek(fd, 0, SEEK_CUR));
	cvmx_coredump_write_notes(&info);

	for (i = 1; i < info.nPhdr; i++) {
		coredump_dump_remote_mem(fd, &info.tlb_info[i]);
	}

	printf("=== Done ===\n");
	return 0;
}

static void context_be_to_cpu(cvmx_debug_core_context_t *context)
{
#define F(X) context->X = cvmx_be64_to_cpu(context->X)
    CVMX_DEBUG_BASIC_CONTEXT;
    CVMX_DEBUG_COP0_CONTEXT;
    CVMX_DEBUG_HW_IBP_CONTEXT
    CVMX_DEBUG_HW_DBP_CONTEXT
    CVMX_DEBUG_TLB_CONTEXT;
#undef F
}

static void mem_info_be_to_cpu(cvmx_coredump_mem_info_t *mem_info)
{
	int i;
	mem_info->num_mem_entry = cvmx_be64_to_cpu(mem_info->num_mem_entry);
	for(i = 0;i < CVMX_COREDUMP_MAX_MEM_DESC;i++)
	{
		cvmx_coredump_memdesc_t *desc = &mem_info->meminfo[i];
		desc->valid = cvmx_be64_to_cpu(desc->valid);
		desc->base = cvmx_be64_to_cpu(desc->base);
		desc->size = cvmx_be64_to_cpu(desc->size);
	}
}

static void usage(const char *argv[])
{
    printf("\n"
           "Usage: %s <coremask>\n"
           "    Dump the core files.\n"
           "\n"
           "    coremask - Octeon coremask to dump.\n"
           "\n", argv[0]);
}

int
main(int argc, const char *argv[])
{
	uint64_t coremask;
	uint32_t corenum;
	uint64_t elf_addr, size, mem_info_base;
	uint64_t tlb_entries;
	cvmx_coredump_mem_info_t *mem_info_buf;
	cvmx_coredump_registers_t *reg_info_buf;

	char filename[128];
	int fd;

	if (argc != 2)
	{
		usage(argv);
		return -1;
	}

	coremask = strtoull(argv[1], NULL, 0);
        if (coremask == 0)
	{
		usage(argv);
		return -1;
	}

	if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
		return -1;

	if (!octeon_remote_mem_access_ok()) {
		octeon_remote_close();
		printf("Error: DRAM not setup, board needs to be booted\n");
		goto failure;
	}

	if (!octeon_remote_named_block_find(CVMX_COREDUMP_BLK_NAME, &elf_addr, &size)) {
		printf("Unable to find the named block: %s\n", CVMX_COREDUMP_BLK_NAME);
		goto failure;
	}

	if (!elf_addr) {
		printf("Unable to find Named block\n");
		goto failure;
	}

	corenum = get_first_bit(coremask);
	if (octeon_remote_read_mem64 (elf_addr + offsetof(cvmx_coredump_mem_block_t, magic)) != CVMX_COREDUMP_MAGIC) {
		printf("The coredump named block is not setup yet.\n");
		goto failure;
	}
	if (octeon_remote_read_mem64 (elf_addr + offsetof(cvmx_coredump_mem_block_t, version)) != CVMX_COREDUMP_CURRENT_VERSION) {
		printf("The coredump named block is the wrong version.\n");
		goto failure;
	}

	reg_info_buf = malloc(sizeof(*reg_info_buf));

	mem_info_base = elf_addr + offsetof(cvmx_coredump_mem_block_t, mem_dump);
	mem_info_buf = malloc(sizeof(cvmx_coredump_mem_info_t));
	octeon_remote_read_mem(mem_info_buf, mem_info_base, sizeof(cvmx_coredump_mem_info_t));
	mem_info_be_to_cpu (mem_info_buf);
	tlb_entries = octeon_remote_read_mem64 (elf_addr + offsetof(cvmx_coredump_mem_block_t, tlb_entries));

	do {

		uint64_t raddr = elf_addr + offsetof (cvmx_coredump_mem_block_t, registers[corenum]);

		octeon_remote_read_mem(reg_info_buf, raddr, sizeof(cvmx_coredump_registers_t));

		context_be_to_cpu(reg_info_buf);

		if (!reg_info_buf->remote_controlled) {
			printf("Skipping core: %d because the core has not crashed yet.\n", corenum);
			continue;
		}

		sprintf(filename, "%s_%d", CVMX_DUMP_FILE_NAME, corenum);
		printf("Writing coredump for core %d, file: %s\n", corenum, 
			filename);

		/* Don't use the uid rather than effective uid. */
		seteuid(getuid());
		fd = open(filename, O_CREAT|O_RDWR|O_TRUNC|O_LARGEFILE, 0666);
		seteuid(0);

		if (fd < 0) {
			perror("open:");
			goto failure;
		}

		CVMX_COREDUMP_DEBUG_PRINT("core%d: elf_addr:%llx raddr:%llx"
			    " mem_info_base:%llx\n",
			    corenum, elf_addr, raddr, mem_info_base);

		coredump_SE(fd, reg_info_buf, mem_info_buf, tlb_entries);
	
		close(fd);
	} while ((corenum = get_next_bit(coremask, corenum)) != 0);

	octeon_remote_close();
	return EXIT_SUCCESS;

failure:
	octeon_remote_close();
	return EXIT_FAILURE;
}
