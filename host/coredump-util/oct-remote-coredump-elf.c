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
 * Coredump, ELF header populating
 *
 * <hr>$Revision$<hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>

#include "cvmx.h"
#include "cvmx-version.h"
#include "cvmx-swap.h"

#include "cvmx-coredump-bootmem.h"
#include "cvmx-coredump.h"
#include "oct-remote-coredump-elf.h"

#ifdef CVMX_COREDEUMP_DEBUG
	#define CVMX_COREDUMP_DEBUG_PRINT printf
#else
	#define CVMX_COREDUMP_DEBUG_PRINT(...)
#endif

#define ROUNDUP(x, y) (((x) + (y - 1)) & ~(y - 1))

#define ELFROUND_SIZE 4
#define ELF_ROUNDUP(x) (ROUNDUP((x), ELFROUND_SIZE))


/*
   This file is largely based on Linux's fs/binfmt_elf.c
 
	    +-----------------------+
	    |     ELF Header        |
	    |                       |
  offset ==>+-----------------------+
	    |     Program headers   |
	    |                       |
 foffset ==>+-----------------------+
	    |        Other info     |
	    |                       |
 dataoff ==>+-----------------------+
	    |     Segment 1(Text)   |
	    |                       |
	    +-----------------------+
	    |     Segment 2()       |
	    |                       |
	    +-----------------------+
	    |     Segment 3(Data)   |
	    |                       |
 e_shoff ==>+-----------------------+
	    |     Other Info        |
	    | (optional Section hdr)|
	    +-----------------------+

 Example Note segment
  ======================
	    +0    +1    +2    +3
	 +------+-----+-----+-----+
  namesz |            7           |
	 +------+-----+-----+-----+
  descsz |            0           |
	 +------+-----+-----+-----+
  type   |            1           |   => Entry 1
	 +------+-----+-----+-----+
  name   |  X   |  Y  |  Z  |     |
	 +------+-----+-----+-----+
	 |  C   |  o  | \0  | pad |
	 +------+-----+-----+-----+
	 +------+-----+-----+-----+
  namesz |            7           |
	 +------+-----+-----+-----+
  descsz |            8           |
	 +------+-----+-----+-----+
  type   |            1           |
	 +------+-----+-----+-----+
  name   |  X   |  Y  |  Z  |     |
	 +------+-----+-----+-----+
	 |  C   |  o  | \0  | pad |
	 +------+-----+-----+-----+
  desc   |         word0          |
	 +------+-----+-----+-----+
	 |         word1          |
	 +------+-----+-----+-----+
 */

static struct elf_prpsinfo psinfo_g[CVMX_MAX_CORES];
static struct elf_thread_status threads[CVMX_MAX_CORES];
static struct memelfnote notes[6];
static struct elf_prstatus prstatus;
uint64_t current_offset;

static inline int
get_nbits(uint64_t coremask)
{
	int i = 0;
	while(coremask) { i++; coremask &= (coremask - 1); }
	return i;
}

static size_t
TLB_SIZE(uint64_t pmask)
{
	uint64_t newmask = (pmask | 0x1fff) >> 1;
	int nbits = get_nbits(newmask) - 10;
	CVMX_COREDUMP_DEBUG_PRINT("pmask: %llx, newmask: %llx nbits:%d\n", (long long)pmask, (long long)newmask, nbits);
	return 1 << nbits;
}

static int
cvmx_coredump_dump(int fd, void *base, int size)
{
	CVMX_COREDUMP_DEBUG_PRINT("writing at offset:%llx size:%d\n", (long long)lseek(fd, 0, SEEK_CUR), size);

	size = write(fd, base, size);

	return size;
}


/* converting between big/little endian */
#define MEMCPY_CONVERT(d, s) {				\
		int sz = sizeof(s);			\
		switch(sz) {				\
		case 2: d = cvmx_cpu_to_be16((s)); break;	\
		case 4: d = cvmx_cpu_to_be32((s)); break;	\
		case 8: d = cvmx_cpu_to_be64((s)); break;	\
		default: assert("Unknown convert");	\
		}					\
	}

int
cvmx_coredump_count_valid_tlbs(cvmx_coredump_registers_t *registers,
			       cvmx_coredump_mem_info_t *mem_info,
			       uint64_t tlb_entries)
{
	uint64_t i, count = 0;

	if (!registers)
		return 0;

	 for (i = 0; i < tlb_entries; i++) {
	     if (TLB_VALID0(registers->tlbs[i]) || TLB_VALID1(registers->tlbs[i]))
                     count++;
	 }

	 count += mem_info->num_mem_entry;

	 CVMX_COREDUMP_DEBUG_PRINT("no of TLB: %d\n", (int)count);
	 return count;
}


/* Creates PHDR entry per allocated entry via cvmx_bootmem_alloc* */
static void
coredump_fill_extra_dump_info(cvmx_coredump_mem_info_t *mi,
			      coredump_tlb_info_t *tlb_info,
			      int pi, off_t *secstart)
{
	Elf_Phdr *phdr;
	cvmx_coredump_memdesc_t *mem;
	uint32_t i;

	for (i = 0; i < CVMX_COREDUMP_MAX_MEM_DESC; i++) {
		if (mi->meminfo[i].valid) {
			mem = &mi->meminfo[i];
			tlb_info[pi].paddr1 = 0;
			phdr = &tlb_info[pi].phdr;

			phdr->p_type = PT_LOAD;

			phdr->p_offset = *secstart;
			phdr->p_vaddr = mem->base;
			phdr->p_paddr = mem->base;
			phdr->p_filesz = mem->size;
			phdr->p_memsz = 0;

			phdr->p_flags = PF_W | PF_R;

			CVMX_COREDUMP_DEBUG_PRINT("offset: 0x%llx vaddr: 0x%llx p_size:%llx\n",
				    (long long)*secstart, (long long)phdr->p_vaddr, (long long)phdr->p_memsz);

			/* The alignment of this section is 4kb (min alignment).  */
			phdr->p_align = 4 * KB;

			*secstart += mem->size;
			pi++;
		}
	}
}

/* 
 * Create one PHDR entry for each valid TLB entry, also one per allocated mem via
 * cvxm_bootmeme_alloc*
 */
int
cvmx_coredump_fill_phdr(cvmx_coredump_registers_t *registers,
			coredump_tlb_info_t *tlb_info,
			cvmx_coredump_mem_info_t *mem_info, off_t *secstart,
			uint64_t tlb_entries)
{
	Elf_Phdr *phdr;
	cvmx_coredump_registers_t *regs = registers;
	size_t size;
	int pi = 0;
	uint64_t i;

	for (i = 0; i < tlb_entries; i++) {
		cvmx_coredump_tlb_t tlb = regs->tlbs[i];
		
		if (TLB_VALID0(tlb) || TLB_VALID1(tlb)) {

                        size = TLB_PAGESIZE(tlb) * KB;

			tlb_info[pi].paddr1 = TLB_VALID1(tlb)?TLB_PAGE1(tlb): 0;

			phdr = &tlb_info[pi].phdr;
			tlb_info[pi].paddr1 = TLB_VALID1(tlb)?TLB_PAGE1(tlb): 0;

			phdr->p_type = PT_LOAD;

			phdr->p_offset = *secstart;
			phdr->p_vaddr = TLB_VA(tlb);
			phdr->p_paddr = TLB_PAGE0(tlb);

			phdr->p_filesz = size;
			phdr->p_memsz = 0;

			*secstart += size;

			phdr->p_flags = PF_W | PF_R | PF_X;
			if (TLB_XI0(tlb) || TLB_XI1(tlb))
				phdr->p_flags &= ~PF_X;
			if (TLB_RI0(tlb) || TLB_RI1(tlb))
				phdr->p_flags &= ~PF_R;

			CVMX_COREDUMP_DEBUG_PRINT("vaddr: 0x%llx paddr:0x%llx, p_filesz:%llx p_offset:%llx\n",
				    (long long)phdr->p_vaddr, (long long)phdr->p_paddr,
				    (long long)phdr->p_filesz, (long long)phdr->p_offset);

			phdr->p_align = size;

			if (tlb_info[pi].paddr1) {
                                *secstart += size; /* Section will have two pages copied */
			}

			pi++;

		}

	}

	coredump_fill_extra_dump_info(mem_info, tlb_info, pi, secstart);

	return *secstart;
}

uint64_t
cvmx_coredump_write_phdr_fields(int fd, Elf_Phdr *phdr)
{
	Elf_Phdr newphdr;
	MEMCPY_CONVERT(newphdr.p_type, phdr->p_type);
	MEMCPY_CONVERT(newphdr.p_flags, phdr->p_flags);
	MEMCPY_CONVERT(newphdr.p_offset, phdr->p_offset);
	MEMCPY_CONVERT(newphdr.p_vaddr, phdr->p_vaddr);
	MEMCPY_CONVERT(newphdr.p_paddr, phdr->p_paddr);
	MEMCPY_CONVERT(newphdr.p_filesz, phdr->p_filesz);
	MEMCPY_CONVERT(newphdr.p_memsz, phdr->p_memsz);
	MEMCPY_CONVERT(newphdr.p_align, phdr->p_align);

	return cvmx_coredump_dump(fd, &newphdr, sizeof(newphdr));
}

void
cvmx_coredump_fill_note_phdr(Elf_Phdr *phdr, loff_t offset, int sz)
{
	phdr->p_type = PT_NOTE;
	phdr->p_offset = offset;
	phdr->p_vaddr = 0;
	phdr->p_paddr = 0;
	phdr->p_filesz = sz;
	phdr->p_memsz = 0;
	phdr->p_flags = PF_R;
	phdr->p_align = ELFROUND_SIZE;

	return;
}

static void
fill_note(struct memelfnote *note, const char *name,
	  int type, size_t descsz, void *data)
{
	note->name = name;
	note->type = type;
	note->datasz = descsz;
	note->data = data;
}

static int
write_note_fields(int fd, Elf_Nhdr *nhdr)
{
	Elf_Nhdr newnhdr;
	memset(&newnhdr, 0, sizeof(newnhdr));
	MEMCPY_CONVERT(newnhdr.n_namesz, nhdr->n_namesz);
	MEMCPY_CONVERT(newnhdr.n_descsz, nhdr->n_descsz);
	MEMCPY_CONVERT(newnhdr.n_type, nhdr->n_type);

        return cvmx_coredump_dump(fd, &newnhdr, sizeof(newnhdr));
}

static void
elf_core_copy_task_regs(elf_greg_t *pr_reg, cvmx_coredump_registers_t *r)
{
	int i;
	CVMX_COREDUMP_DEBUG_PRINT("Sizeof pr_reg %lld regs: %p\n", (long long)sizeof(*pr_reg), r);
	for (i = 0; i < 32; i++) {
		pr_reg[i] = cvmx_cpu_to_be64(r->regs[i]);
		CVMX_COREDUMP_DEBUG_PRINT("Reg %d : %llx\n",i, (long long)pr_reg[i]);
	}

	/* PC - we'll make PC == EPC, as we know the exception started there*/
	CVMX_COREDUMP_DEBUG_PRINT("EPC: %llx \n", (long long)r->cop0.depc);
	/* PC - Program Counter */
	pr_reg[EF_CP0_EPC] = cvmx_cpu_to_be64(r->cop0.depc);

	/* SR - Status Register */
	CVMX_COREDUMP_DEBUG_PRINT("Status: %llx\n", (long long)r->cop0.status);
	pr_reg[EF_CP0_STATUS] = cvmx_cpu_to_be64(r->cop0.status);

	/* lo (multiply low) */
	CVMX_COREDUMP_DEBUG_PRINT("Lo: %llx\n", (long long)r->lo);
	pr_reg[EF_LO] = cvmx_cpu_to_be64(r->lo);

	/* high (multiply high) */
	CVMX_COREDUMP_DEBUG_PRINT("Hi: %llx\n", (long long)r->hi);
	pr_reg[EF_HI] = cvmx_cpu_to_be64(r->hi);

	/* BAD_VA */
	CVMX_COREDUMP_DEBUG_PRINT("BadVA: %llx\n", (long long)r->cop0.badvaddr);
	pr_reg[EF_CP0_BADVADDR] = cvmx_cpu_to_be64(r->cop0.badvaddr);

	/* CAUSE */
	CVMX_COREDUMP_DEBUG_PRINT("CAUSE: %llx\n", (long long)r->cop0.cause);
	pr_reg[EF_CP0_CAUSE] = cvmx_cpu_to_be64(r->cop0.cause);

}

static int
get_notesize(struct memelfnote *en)
{
	int sz;

	sz = sizeof(Elf_Nhdr);
	sz += ELF_ROUNDUP(strlen(en->name) + 1);
	sz += ELF_ROUNDUP(en->datasz);

	return sz;
}

#if 0
/* Needed once multicore ELF interface is integrated to GDB */
static void
fill_prstatus(struct elf_prstatus *prstatus, int corenum)
{
	memset(prstatus, 0, sizeof(*prstatus));
	prstatus->pr_pid = 1;
}

static int
coredump_fill_elf_thread_status(struct elf_thread_status *t, int corenum,
				cvmx_coredump_registers_t *r, size_t *size)
{
	struct elf_prpsinfo *psi = &psinfo;

	t->numnotes = 0;

	fill_prstatus(&t->prstatus, corenum);	/* Not necessary for SE */
	elf_core_copy_task_regs(&t->prstatus.pr_reg[0], r);

	fill_note(&t->notes[1], "CORE", NT_PSINFO, sizeof(*psi),
		  psi);
	*size += get_notesize(&t->notes[1]);
	t->numnotes++;

	t->prstatus.pr_pid = corenum;
	fill_note(&t->notes[0], "CORE", NT_PRSTATUS, sizeof(t->prstatus),
		  &(t->prstatus));
	*size += get_notesize(&t->notes[0]);
	t->numnotes++;

	return 1;
}
#endif

static uint64_t
dump_psinfo(int fd, struct elf_prpsinfo *ps)
{
	struct elf_prpsinfo newps;

	/* Also update any other fields on need basis */
	MEMCPY_CONVERT(newps.pr_pid, ps->pr_pid);
	memcpy(&newps.pr_fname, ps->pr_fname, sizeof(ps->pr_fname));
	memcpy(&newps.pr_psargs, ps->pr_psargs, sizeof(ps->pr_psargs));

	return cvmx_coredump_dump(fd, &newps, sizeof(newps));
}

static uint64_t
dump_prstatus(int fd, struct elf_prstatus *pr)
{
	struct elf_prstatus newpr;

	/* Also update any other fields on need basis */
	MEMCPY_CONVERT(newpr.pr_pid, pr->pr_pid);

	memcpy(&newpr.pr_reg, &pr->pr_reg, sizeof(newpr.pr_reg));

	return cvmx_coredump_dump(fd, &newpr, sizeof(newpr));
}


static int
coredump_write_single_note(int fd, struct memelfnote *note)
{
	Elf_Nhdr nhdr;
	char buf[ELFROUND_SIZE + 1];
        uint64_t ret = 0;
	memset(buf, 0, sizeof(buf));

	nhdr.n_namesz = strlen(note->name) + 1;
	nhdr.n_descsz = note->datasz;
	nhdr.n_type = note->type;
	CVMX_COREDUMP_DEBUG_PRINT("type:%x namesz: %x, descsz: %x\n",
		    nhdr.n_type, nhdr.n_namesz, nhdr.n_descsz);

	ret += write_note_fields(fd, &nhdr);

	ret += cvmx_coredump_dump(fd, (void*)note->name, nhdr.n_namesz);
        CVMX_COREDUMP_DEBUG_PRINT("namesz:%x Padded size %x\n", nhdr.n_namesz, ELF_ROUNDUP(nhdr.n_namesz));
	/* Pad the name */
	ret += cvmx_coredump_dump(fd, &buf, ELF_ROUNDUP(nhdr.n_namesz) - nhdr.n_namesz);

        CVMX_COREDUMP_DEBUG_PRINT("descsz:%x Padded size %x\n", nhdr.n_descsz, ELF_ROUNDUP(nhdr.n_descsz));
	switch (note->type) {
	case NT_PRSTATUS:
		ret += dump_prstatus(fd, (void*)note->data);
		break;
	case NT_PSINFO:
		ret += dump_psinfo(fd, (void*)note->data);
		break;
	default:
		ret += cvmx_coredump_dump(fd, (void*)note->data, nhdr.n_descsz);
		break;
	}

	/* Pad the data */
	ret += cvmx_coredump_dump(fd, &buf, ELF_ROUNDUP(nhdr.n_descsz) - nhdr.n_descsz);

	return ret;
}


/* 
 * There will be one info entry for entire process thread, one per thread
 */
uint64_t
cvmx_coredump_write_notes(cvmx_coredump_info_t *info)
{
	int i;
        uint64_t offset = 0;
	for (i = 0; i < info->num_notes; i++)
		offset += coredump_write_single_note(info->fd, &info->notes[i]);

	for (i = 0; i < info->nthreads; i++) {
		int j;
		struct elf_thread_status *t = &info->threads[i];
		for (j = 0; j < t->numnotes; j++) {
			/* Then write prstatus */
			offset += coredump_write_single_note(info->fd, &t->notes[j]);
		}
	}
	return offset;
}

int
cvmx_coredump_fill_notes(cvmx_coredump_info_t *info,
			 cvmx_coredump_registers_t *regs)
{
	info->notes = &notes[0];

	info->nthreads = 1;

	info->threads = &threads[0];
	info->prstatus = &prstatus;
	memset(info->prstatus, 0, sizeof(*info->prstatus));

	info->psinfo = psinfo_g;
	info->thread_status_size = 0;

	fill_note(&info->notes[0], "CORE", NT_PRSTATUS,
		  sizeof(*info->prstatus), info->prstatus);
	{
		elf_core_copy_task_regs(&info->prstatus->pr_reg[0],
					&regs[0]);
	}

	fill_note(&info->notes[1], "CORE", NT_PRPSINFO, sizeof(*info->psinfo),
		  info->psinfo);

	info->num_notes = 2;

	return 1;
}

size_t
cvmx_coredump_get_note_info_sz(cvmx_coredump_info_t *info)
{
	size_t sz = 0;
	int i;
	for (i = 0; i < info->num_notes; i++)
		sz += get_notesize(&info->notes[i]);

	return sz + info->thread_status_size;
}

uint64_t
cvmx_coredump_write_ehdr_fields(int fd, Elf_Ehdr *elf)
{
	Elf_Ehdr newehdr;

	memcpy(&newehdr.e_ident, &elf->e_ident, sizeof(elf->e_ident));

	/* rest of it is interpretted as Bigendian */
	MEMCPY_CONVERT(newehdr.e_type, elf->e_type);
	MEMCPY_CONVERT(newehdr.e_machine, elf->e_machine);
	MEMCPY_CONVERT(newehdr.e_version, elf->e_version);
	MEMCPY_CONVERT(newehdr.e_entry, elf->e_entry);
	MEMCPY_CONVERT(newehdr.e_phoff, elf->e_phoff);
	MEMCPY_CONVERT(newehdr.e_shoff, elf->e_shoff);
	MEMCPY_CONVERT(newehdr.e_flags, elf->e_flags);
	MEMCPY_CONVERT(newehdr.e_ehsize, elf->e_ehsize);
	MEMCPY_CONVERT(newehdr.e_phentsize, elf->e_phentsize);
	MEMCPY_CONVERT(newehdr.e_phnum, elf->e_phnum);
	MEMCPY_CONVERT(newehdr.e_shentsize, elf->e_shentsize);
	MEMCPY_CONVERT(newehdr.e_shnum, elf->e_shnum);
	MEMCPY_CONVERT(newehdr.e_shstrndx, elf->e_shstrndx);

	return cvmx_coredump_dump(fd, &newehdr, sizeof(newehdr));
}

void
cvmx_coredump_fill_elf_header(Elf_Ehdr *elf, int segs, uint32_t flags)
{
	memset(elf, 0, sizeof(*elf));
	memcpy(elf->e_ident, ELFMAG, SELFMAG);
	elf->e_ident[EI_CLASS] = ELFCLASS64;
	elf->e_ident[EI_DATA] = ELFDATA2MSB;
	elf->e_ident[EI_VERSION] = EV_CURRENT;
	elf->e_ident[EI_OSABI] = ELFOSABI_NONE;

	/* rest of it is interpretted as Bigendian */
	elf->e_type = ET_CORE;
	elf->e_machine = EM_MIPS;
	elf->e_version = EV_CURRENT;
	elf->e_phoff = sizeof(Elf_Ehdr);
	elf->e_flags = 0x80000000|flags;
	elf->e_ehsize = sizeof(Elf_Ehdr);
	elf->e_phentsize = sizeof(Elf_Phdr);
	elf->e_phnum = segs;

	return;
}
