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
 * Coredump ELF interface and definitions
 *
 * <hr>$Revision$<hr>
 */

#ifndef __CVMX_COREDUMP_ELF_H__
#define __CVMX_COREDUMP_ELF_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Including elf.h is fine as they don't change between different
   targets/hosts. */
#include <elf.h>

/* FIXME: This is wrong for n32. */
#define CONFIG_64BIT

#ifdef CONFIG_64BIT
#define Elf_Ehdr    Elf64_Ehdr
#define Elf_Phdr    Elf64_Phdr
#define Elf_Shdr    Elf64_Shdr
#define Elf_Nhdr    Elf64_Nhdr

#define Elf_Addr    Elf64_Addr
#define Elf_Half    Elf64_Half
#define Elf_Off     Elf64_Off
#define Elf_Word    Elf64_Word
#define Elf_Xword    Elf64_Xword
#define Elf_Sxword    Elf64_Sxword

#define elf_note    elf64_note

#else

#define Elf_Ehdr    Elf32_Ehdr
#define Elf_Phdr    Elf32_Phdr
#define Elf_Shdr    Elf32_Shdr
#define Elf_Nhdr    Elf32_Nhdr

#define Elf_Addr    Elf32_Addr
#define Elf_Half    Elf32_Half
#define Elf_Off	    Elf32_Off
#define Elf_Word    Elf32_Word
#define Elf_Xword    Elf32_Xword
#define Elf_Sxword    Elf32_Sxword

#define elf_note    elf32_note
#endif

typedef uint64_t aint64_t  __attribute__((aligned(sizeof(uint64_t))));

/* e_ident */
#define IS_ELF(ehdr) ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                      (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                      (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                      (ehdr).e_ident[EI_MAG3] == ELFMAG3)


/* ELF register definitions */
#define ELF_NGREG       45
#define ELF_NFPREG      33


#define EF_LO			32
#define EF_HI			33

#define EF_CP0_EPC		34
#define EF_CP0_BADVADDR		35
#define EF_CP0_STATUS		36
#define EF_CP0_CAUSE		37


/* coredump for each core is at this offset */
#define ELF_HDRS_SIZE       (16 * KB)

/* On an average we need to store for each core,
 * ELF header information
 * Program header information
 * PINFO
 * PRPSINFO
 *
 * every core has designated address to store elf info at ROUND_UP(core_num * ELF_CORE_OFFSET)
 */
#define ELF_CORE_OFFSET	    (4*1024)

typedef aint64_t elf_greg_t;

typedef elf_greg_t elf_gregset_t[ELF_NGREG];

/* Use a 64bit integer to store the floating point. */
typedef aint64_t elf_fpreg_t;
typedef elf_fpreg_t elf_fpregset_t[ELF_NFPREG];

/* This is an internal note, it won't be written this way. */
struct memelfnote
{
	const char *name;
	Elf_Word type;
	Elf_Word datasz;
	void *data;
};

struct elf_siginfo
{
	int	si_signo;			/* signal number */
	int	si_code;			/* extra code */
	int	si_errno;			/* errno */
};

/* 
Definitions to generate Intel SVR4-like core files.  These mostly
have the same names as the SVR4 types with "elf_" tacked on the
front to prevent clashes with linux definitions, and the typedef
forms have been avoided.  This is mostly like the SVR4 structure,
but more Linuxy, with things that Linux does not support and which
gdb doesn't really use excluded.  Fields present but not used are
marked with "XXX".  
*/
struct cvmx_timeval
{
    aint64_t tv_sec;            /* Seconds.  */
    aint64_t tv_usec;      /* Microseconds.  */
};

struct elf_prstatus
{
	struct elf_siginfo pr_info;         /* Info associated with signal.  */
	short int pr_cursig;                /* Current signal.  */
	aint64_t pr_sigpend;       /* Set of pending signals.  */
	aint64_t pr_sighold;       /* Set of held signals.  */
	int pr_pid;
	int pr_ppid;
	int pr_pgrp;
	int pr_sid;
	struct cvmx_timeval pr_utime;            /* User time.  */
	struct cvmx_timeval pr_stime;            /* System time.  */
	struct cvmx_timeval pr_cutime;           /* Cumulative user time.  */
	struct cvmx_timeval pr_cstime;           /* Cumulative system time.  */
	elf_gregset_t pr_reg;               /* GP registers.  */
	int pr_fpvalid;                     /* True if math copro being used.  */
};


#define ELF_PRARGSZ	(80)	/* Number of chars for args */

struct elf_prpsinfo
{
	char	pr_state;	/* numeric process state */
	char	pr_sname;	/* char for pr_state */
	char	pr_zomb;	/* zombie */
	char	pr_nice;	/* nice val */
	aint64_t pr_flag;	/* flags */
	int	pr_pid, pr_ppid, pr_pgrp, pr_sid;
	char	pr_fname[16];	/* filename of executable */
	char	pr_psargs[ELF_PRARGSZ];	/* initial part of arg list */
};

struct elf_thread_status
{
	struct elf_prstatus prstatus;	/* NT_PRSTATUS */
	cvmx_coredump_registers_t *regs;
	struct memelfnote notes[3];     /* Per thread notes */
	int numnotes;
};


typedef struct {
	Elf_Phdr phdr;
	uint64_t paddr1;
} coredump_tlb_info_t ;


typedef struct {
	Elf_Ehdr    *Ehdr;
	int         nPhdr;
	Elf_Nhdr    *Nhdr;
	int         nNhdr;
	int         nthreads;
	int         fd;           /* Driver specific data */
	int         num_notes;
	int         thread_status_size;
	struct memelfnote *notes;
	struct elf_prstatus *prstatus;	/* NT_PRSTATUS */
	struct elf_prpsinfo *psinfo;	/* NT_PRPSINFO */
	coredump_tlb_info_t *tlb_info;
	struct elf_thread_status *threads;
} cvmx_coredump_info_t;

int cvmx_coredump_count_valid_tlbs(cvmx_coredump_registers_t *, cvmx_coredump_mem_info_t *,
				   uint64_t tlb_entries);
void cvmx_coredump_fill_elf_header(Elf_Ehdr *elf, int segs, uint32_t flags);
void cvmx_coredump_fill_note_phdr(Elf_Phdr *phdr, loff_t offset, int sz);
size_t cvmx_coredump_get_note_info_sz(cvmx_coredump_info_t *info);
void cvmx_coredump_fill_note_phdr(Elf_Phdr *phdr, loff_t offset, int sz);
int cvmx_coredump_fill_phdr(cvmx_coredump_registers_t *registers,
			    coredump_tlb_info_t *tlb_info, cvmx_coredump_mem_info_t *,
			    off_t *secstart, uint64_t tlb_entries);
uint64_t cvmx_coredump_write_ehdr_fields(int fd, Elf_Ehdr *elf);
uint64_t cvmx_coredump_write_phdr_fields(int fd, Elf_Phdr *phdr);
int cvmx_coredump_fill_notes(cvmx_coredump_info_t *info,
			     cvmx_coredump_registers_t *regs);
uint64_t cvmx_coredump_write_notes(cvmx_coredump_info_t *info);

#define KB  (1024)
#define MB  (KB * KB)
#define GB  (MB * KB)

#define TLB_VA(x)	((x).entryhi>>12<<12)
#define TLB_ASID(x)	((x).entryhi & 0xff)
#define TLB_PAGESIZE(x)	 (TLB_SIZE((x).pagemask))
#define TLB_PAGE0(x)	((x).entrylo[0]>>6<<12)
#define TLB_CACHE0(x)	(((x).entrylo[0]>>3) & 7)
#define TLB_VALID0(x)	(((x).entrylo[0]>>1) & 1)
#define TLB_GLOBAL0(x)	((x).entrylo[0] & 1)
#define TLB_RI0(x)	(((x).entrylo[0]>>63) & 1)
#define TLB_XI0(x)	(((x).entrylo[0]>>62) & 1)
#define TLB_PAGE1(x)	((x).entrylo[1]>>6<<12)
#define TLB_CACHE1(x)	(((x).entrylo[1]>>3) & 7)
#define TLB_VALID1(x)	(((x).entrylo[1]>>1) & 1)
#define TLB_GLOBAL1(x)	((x).entrylo[1] & 1)
#define TLB_RI1(x)	(((x).entrylo[1]>>63) & 1)
#define TLB_XI1(x)	(((x).entrylo[1]>>62) & 1)

#endif  /* __CVMX_COREDUMP_ELF_H__ */
