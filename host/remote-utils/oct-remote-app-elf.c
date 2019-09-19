
/***********************license start************************************
 * Copyright (c) 2017 Cavium (support@cavium.com). All rights
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
 * For any questions regarding licensing please contact support@cavium.com
 *
 ***********************license end**************************************/

/**
 * @file
 *
 * $Id: oct-app-elf.c 104074 2014-09-08 23:50:44Z lrosenboim $
 *
 * DESCRIPTION
 *
 * This module impelements the parsing of ELF object file format headers
 * and results in a simplified data structure containing the size and
 * location (in virtual process memory), one for each segment type.
 * Three segment types are supported - text and read-only data,
 * shared data and core-private data.
 * Private data contains the .bss section too.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <libelf.h>
#include <gelf.h>

#include "octeon-remote.h"
#include "oct-remote-app.h"

static const char shared_sh_name[] = ".cvmx_shared";

static unsigned short const elf_machines[] = {
#ifdef	__mips__
	EM_MIPS, EM_MIPS_RS3_LE,
#else
	EM_ARMV8
#endif
};


int elf_file_prep(int fd, cvmx_app_boot_record_t *cabr_ptr,
	cvmx_app_boot_segment_t *segments, unsigned max_segments)
{
	Elf	*elf;
	Elf_Kind ek;
	unsigned i;
	unsigned cix;
	GElf_Ehdr ehdr;
	GElf_Phdr phdr;
	size_t phnum, shnum;
	size_t strndx;
	Elf_Scn *scn;
	GElf_Shdr shdr;
	uint64_t elf_shared_addr = 0;
	const char *seg_type;

	/* Initlaize libelf */
	if (elf_version(EV_CURRENT) == EV_NONE) {
		fprintf(stderr,  "ERROR: "
			" ELF library initialization failed: %s ",
			elf_errmsg(-1));
		return -1;
	}

	/* Parse top header */
	elf = elf_begin(fd, ELF_C_READ, NULL);
	if (elf == NULL) {
		fprintf(stderr,  "ERROR: "
			" ELF  onject-file initialization failed: %s ",
			elf_errmsg(-1));
		return -1;
	}

	/* Validate object type */
	ek = elf_kind(elf);

	if (ek != ELF_K_ELF) {
		fprintf(stderr,  "ERROR: "
			" ELF  onject-file is not an ELF: %s ",
			elf_errmsg(-1));
		elf_end(elf);
		return -1;
	}

	i = gelf_getclass(elf);
	if (i != ELFCLASS32 && i != ELFCLASS64) {
		fprintf(stderr, "ERROR: object-file type unknown\n");
		elf_end(elf);
		return -1;
	}

	/* XXX detect LITTLE_ENDIAN Elf format */

	if (gelf_getehdr(elf , &ehdr) == NULL) {
		fprintf(stderr, "ERROR: "
			"getehdr() failed: %s " , elf_errmsg(-1));
		elf_end(elf);
		return -1;
	}

	if (ehdr.e_type != ET_EXEC) {
		fprintf(stderr, "ERROR: "
			"obnject-file is not an executable.\n");
		elf_end(elf);
		return -1;
	}

	/* Verify target CPU architecture */
	for (i = 0; i < sizeof(elf_machines)/sizeof(elf_machines[0]); i++) {
		if (ehdr.e_machine == elf_machines[i])
			break;
	}

	if (ehdr.e_machine != elf_machines[i]) {
		fprintf(stderr, "ERROR: "
			"obnject-file machine mismatch %#x.\n", ehdr.e_machine);
		elf_end(elf);
		return -1;
	}

	/* Get size of program header table */
	elf_getphdrnum(elf, &phnum);

	if (phnum < 1) {
		fprintf(stderr, "ERROR: "
			"object-file contains no program headers.\n");
		elf_end(elf);
		return -1;
	}

	/* Scan program header segments and store their attributes */
	for (cix = i = 0; i < phnum; i++) {
		if (gelf_getphdr(elf , i,  &phdr) == NULL) {
			fprintf(stderr, "ERROR: "
				" getphdr() failed: %s " , elf_errmsg(-1));
			elf_end(elf);
			return -1;
		}

		/* Skip segments represending the header otself */
		if (phdr.p_type == PT_PHDR || phdr.p_type == PT_NOTE)
			continue;

		if (phdr.p_type == PT_GNU_STACK) {
			extern void oct_tlb_stack_exec(bool stack_exec);
			bool stack_exec = (phdr.p_flags & PF_X) != 0;
			if (verbose)
				fprintf(stderr, "DEBUG: "
					"stack exec = %d\n", stack_exec);
			oct_tlb_stack_exec(stack_exec);
			continue;
			}

		/* Skip segments with unexpected segment type */
		if (phdr.p_type != PT_LOAD) {
			if (verbose)
				fprintf(stderr, "DEBUG: "
					"Unexpected segment type %#x at phdr[%u], "
					"ignored\n", phdr.p_type, i);
			continue;
		}

		/* This is a loadable segment, record it */
		segments[cix].vaddr = phdr.p_vaddr;
		segments[cix].mem_size = phdr.p_memsz;
		segments[cix].file_offset = phdr.p_offset;
		segments[cix].file_size = phdr.p_filesz;

		/* Decide segment type */
		if (phdr.p_flags & PF_X) {
			segments[cix].cabs_type = CABS_TYPE_TEXT;
			seg_type = "text";
		} else if (phdr.p_flags & PF_W) {
			segments[cix].cabs_type = CABS_TYPE_RW_PRIVATE;
			seg_type = "rw-data";
		} else {
			segments[cix].cabs_type = CABS_TYPE_RO_DATA;
			seg_type = "ro-data";
		}

		if (verbose > 2)
			fprintf(stderr, "DEBUG: "
				"ELF Segment %u: vaddr=%#llx size=%#llx type=%s\n",
				cix,
				(unsigned long long) phdr.p_vaddr,
				(unsigned long long) phdr.p_memsz, seg_type);
		if (verbose > 2)
			fprintf(stderr, "DEBUG: "
				"ELF Segment %u: file-off %#llx file-size=%#x\n",
				cix,
				(unsigned long long) phdr.p_offset,
				(unsigned) phdr.p_filesz);

		cix++;

	} /* for i = 0 .. phnum */

	/* Store program entry address */
	cabr_ptr->cabr_entry_point = ehdr.e_entry;

	/* Need to find the share data using section name */
	elf_getshdrnum(elf, &shnum);
	elf_getshdrstrndx(elf, &strndx);

	for (scn = elf_nextscn(elf, NULL);
		scn != NULL;
		scn = elf_nextscn(elf , scn)) {
		char *name;

		if (gelf_getshdr(scn , &shdr) != &shdr) {
			fprintf(stderr, "ERROR: "
				"getshdr() at %u failed: %s." ,
				(unsigned int) elf_ndxscn(scn),
				elf_errmsg(-1));
			continue;
		}

		name = elf_strptr(elf , strndx , shdr . sh_name);
		if (name  == NULL) {
			fprintf(stderr, "ERROR: "
				"elf_strptr() at %u failed: %s." ,
				(unsigned int) elf_ndxscn(scn),
				elf_errmsg(-1));
			continue;
		}

	/* We are only looking for the shared section, by prefix */
	if (0 != strncmp(name, shared_sh_name, strlen(shared_sh_name)))
		continue;

	/* To keep up with tradition, only the first ocurrance is recorded */
	if (elf_shared_addr == 0ULL)
		elf_shared_addr = shdr.sh_addr;

	} /* for scn */

	/* Change the type of segment that contains the shared section */
	for (i = 0; i < cix; i++) {
		if (segments[i].cabs_type != CABS_TYPE_RW_PRIVATE)
			continue;
		if (elf_shared_addr >= segments[i].vaddr &&
		    elf_shared_addr <=
			(segments[i].vaddr + segments[i].mem_size)) {
			segments[i].cabs_type =
				CABS_TYPE_RW_SHARED;
			if (verbose > 2)
				fprintf(stderr, "DEBUG: "
					"ELF Segment %u is shared\n", i);
		}
	}

	elf_end(elf);
	return cix;
}


/* TBD: (for Thunder):
 * ARMCC includes a section 'heap_info'.
 * The 64-bit value at
 * heap_info+0x10 is the base address of the stack.
 */

/* End of File */
