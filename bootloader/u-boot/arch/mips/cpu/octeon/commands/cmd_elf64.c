/*
 * Copyright (c) 2001 William L. Pitts
 * Copyright (c) 2010 Cavium Inc., Inc. <marketing@cavium.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are freely
 * permitted provided that the above copyright notice and this
 * paragraph and the following disclaimer are duplicated in all
 * such forms.
 *
 * This software is provided "AS IS" and without any express or
 * implied warranties, including, without limitation, the implied
 * warranties of merchantability and fitness for a particular
 * purpose.
 */

#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <net.h>
#include <elf.h>
#include <vxworks.h>
#include <asm/arch/octeon_boot.h>

#include "octeon_biendian.h"

#if defined(CONFIG_WALNUT) || defined(CONFIG_SYS_VXWORKS_MAC_PTR)
DECLARE_GLOBAL_DATA_PTR;
#endif

int valid_elf_image (unsigned long addr);
unsigned long load_elf_image (unsigned long addr);
u64 load_elf64_image (unsigned long addr, uint64_t load_override);

/* Allow ports to override the default behavior */
__attribute__ ((weak))
unsigned long do_bootelf_exec (ulong (*entry) (int, char *const[]),
			       int argc, char *const argv[])
{
	unsigned long ret;

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	ret = entry (argc, argv);

	return ret;
}

/* ======================================================================
 * Interpreter command to boot an arbitrary ELF image from memory.
 * ====================================================================== */
int do_bootelf (cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long addr;	/* Address of the ELF image     */
	unsigned long rc;	/* Return value from user code  */

	/* -------------------------------------------------- */
	int rcode = 0;

	if (argc < 2)
		addr = load_addr;
	else
		addr = simple_strtoul (argv[1], NULL, 16);

	if (!valid_elf_image (addr))
		return 1;

	if (((Elf32_Ehdr *) addr)->e_ident[EI_CLASS] == ELFCLASS32)
		addr = load_elf_image (addr);
	else
	  addr = load_elf64_image (addr, 0);

	printf ("## Starting application at 0x%08lx ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	rc = do_bootelf_exec ((void *)addr, argc - 1, argv + 1);
	if (rc != 0)
		rcode = 1;

	printf ("## Application terminated, rc = 0x%lx\n", rc);
	return rcode;
}

/* ======================================================================
 * Interpreter command to boot VxWorks from a memory image.  The image can
 * be either an ELF image or a raw binary.  Will attempt to setup the
 * bootline and other parameters correctly.
 * ====================================================================== */
int do_bootvx (cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long addr;	/* Address of image            */
	unsigned long bootaddr;	/* Address to put the bootline */
	char *bootline;		/* Text of the bootline        */
	char *tmp;		/* Temporary char pointer      */
	char build_buf[128];	/* Buffer for building the bootline */

	/* ---------------------------------------------------
	 *
	 * Check the loadaddr variable.
	 * If we don't know where the image is then we're done.
	 */

	if (argc < 2)
		addr = load_addr;
	else
		addr = simple_strtoul (argv[1], NULL, 16);

#if defined(CONFIG_CMD_NET)
	/* Check to see if we need to tftp the image ourselves before starting */

	if ((argc == 2) && (strcmp (argv[1], "tftp") == 0)) {
		if (NetLoop (TFTPGET) <= 0)
			return 1;
		printf
		    ("Automatic boot of VxWorks image at address 0x%08lx ... \n",
		     addr);
	}
#endif

	/* This should equate
	 * to NV_RAM_ADRS + NV_BOOT_OFFSET + NV_ENET_OFFSET
	 * from the VxWorks BSP header files.
	 * This will vary from board to board
	 */

#if defined(CONFIG_WALNUT)
	tmp = (char *)CONFIG_SYS_NVRAM_BASE_ADDR + 0x500;
	eth_getenv_enetaddr ("ethaddr", (uchar *) build_buf);
	memcpy (tmp, &build_buf[3], 3);
#elif defined(CONFIG_SYS_VXWORKS_MAC_PTR)
	tmp = (char *)CONFIG_SYS_VXWORKS_MAC_PTR;
	eth_getenv_enetaddr ("ethaddr", (uchar *) build_buf);
	memcpy (tmp, build_buf, 6);
#else
	puts ("## Ethernet MAC address not copied to NV RAM\n");
#endif

	/*
	 * Use bootaddr to find the location in memory that VxWorks
	 * will look for the bootline string. The default value for
	 * PowerPC is LOCAL_MEM_LOCAL_ADRS + BOOT_LINE_OFFSET which
	 * defaults to 0x4200
	 */

	if ((tmp = getenv ("bootaddr")) == NULL)
		bootaddr = CONFIG_SYS_VXWORKS_BOOT_ADDR;
	else
		bootaddr = simple_strtoul (tmp, NULL, 16);

	/*
	 * Check to see if the bootline is defined in the 'bootargs'
	 * parameter. If it is not defined, we may be able to
	 * construct the info
	 */

	if ((bootline = getenv ("bootargs")) != NULL) {
		memcpy ((void *)bootaddr, bootline,
			max (strlen (bootline), 255));
		flush_cache (bootaddr, max (strlen (bootline), 255));
	} else {

		sprintf (build_buf, CONFIG_SYS_VXWORKS_BOOT_DEVICE);
		if ((tmp = getenv ("bootfile")) != NULL) {
			sprintf (&build_buf[strlen (build_buf)],
				 "%s:%s ", CONFIG_SYS_VXWORKS_SERVERNAME, tmp);
		} else {
			sprintf (&build_buf[strlen (build_buf)],
				 "%s:file ", CONFIG_SYS_VXWORKS_SERVERNAME);
		}

		if ((tmp = getenv ("ipaddr")) != NULL) {
			sprintf (&build_buf[strlen (build_buf)], "e=%s ", tmp);
		}

		if ((tmp = getenv ("serverip")) != NULL) {
			sprintf (&build_buf[strlen (build_buf)], "h=%s ", tmp);
		}

		if ((tmp = getenv ("hostname")) != NULL) {
			sprintf (&build_buf[strlen (build_buf)], "tn=%s ", tmp);
		}
#ifdef CONFIG_SYS_VXWORKS_ADD_PARAMS
		sprintf (&build_buf[strlen (build_buf)],
			 CONFIG_SYS_VXWORKS_ADD_PARAMS);
#endif

		memcpy ((void *)bootaddr, build_buf,
			max (strlen (build_buf), 255));
		flush_cache (bootaddr, max (strlen (build_buf), 255));
	}

	/*
	 * If the data at the load address is an elf image, then
	 * treat it like an elf image. Otherwise, assume that it is a
	 * binary image
	 */

	if (valid_elf_image (addr)) {
		addr = load_elf_image (addr);
	} else {
		puts ("## Not an ELF image, assuming binary\n");
		/* leave addr as load_addr */
	}

	printf ("## Using bootline (@ 0x%lx): %s\n", bootaddr,
		(char *)bootaddr);
	printf ("## Starting vxWorks at 0x%08lx ...\n", addr);

	((void (*)(void))addr) ();

	puts ("## vxWorks terminated\n");
	return 1;
}

static uint16_t elf_accessors_le16(uint16_t v)
{
	return le16_to_cpu(v);
}
static uint32_t elf_accessors_le32(uint32_t v)
{
	return le32_to_cpu(v);
}
static uint64_t elf_accessors_le64(uint64_t v)
{
	return le64_to_cpu(v);
}
static uint16_t elf_accessors_be16(uint16_t v)
{
	return be16_to_cpu(v);
}
static uint32_t elf_accessors_be32(uint32_t v)
{
	return be32_to_cpu(v);
}
static uint64_t elf_accessors_be64(uint64_t v)
{
	return be64_to_cpu(v);
}

static struct elf_accessors le_elf_accessors = {
	.w16 = elf_accessors_le16,
	.w32 = elf_accessors_le32,
	.w64 = elf_accessors_le64
};
static struct elf_accessors be_elf_accessors = {
	.w16 = elf_accessors_be16,
	.w32 = elf_accessors_be32,
	.w64 = elf_accessors_be64
};

struct elf_accessors *get_elf_accessors(unsigned long addr)
{
	return is_little_endian_elf(addr) ? &le_elf_accessors : &be_elf_accessors;
}

/* ======================================================================
 * Determine if a valid ELF image exists at the given memory location.
 * First looks at the ELF header magic field, the makes sure that it is
 * executable and makes sure that it is for a PowerPC.
 * ====================================================================== */
int valid_elf_image (unsigned long addr)
{
	Elf32_Ehdr *ehdr;	/* Elf header structure pointer */
	struct elf_accessors *a;
	/* -------------------------------------------------- */

	ehdr = (Elf32_Ehdr *) cvmx_phys_to_ptr (addr);

	if (!IS_ELF (*ehdr)) {
		printf ("## No elf image at address 0x%08lx\n", addr);
		return 0;
	}

	if (!(ehdr->e_ident[EI_CLASS] == ELFCLASS32 ||
	      ehdr->e_ident[EI_CLASS] == ELFCLASS64)) {
		printf("## Not a 32-bit or 64-bit elf image at address 0x%08lx\n", addr);
		return 0;
	}

	a = get_elf_accessors(addr);

	if (a->w16(ehdr->e_type) != ET_EXEC) {
		printf("## Not an EXEC elf image at address 0x%08lx\n", addr);
		return 0;
	}

	if (a->w16(ehdr->e_machine) != EM_MIPS) {
		printf ("## Not a MIPS elf image at address 0x%08lx\n",
			addr);
		return 0;
	}

	return 1;
}

/* ======================================================================
 * A very simple elf loader, assumes the image is valid, returns the
 * entry point address.
 * ====================================================================== */
unsigned long load_elf_image (unsigned long addr)
{
	Elf32_Ehdr *ehdr;	/* Elf header structure pointer     */
	Elf32_Shdr *shdr;	/* Section header structure pointer */
	unsigned char *strtab = 0;	/* String table pointer             */
	unsigned char *image;	/* Binary image pointer             */
	int i;			/* Loop counter                     */

	/* -------------------------------------------------- */

	ehdr = (Elf32_Ehdr *) cvmx_phys_to_ptr (addr);

	/* Find the section header string table for output info */
	shdr = (Elf32_Shdr *) cvmx_phys_to_ptr (addr + ehdr->e_shoff +
						(ehdr->e_shstrndx *
						 sizeof (Elf32_Shdr)));

	if (shdr->sh_type == SHT_STRTAB)
		strtab =
		    (unsigned char *)cvmx_phys_to_ptr (addr + shdr->sh_offset);

	/* Load each appropriate section */
	for (i = 0; i < ehdr->e_shnum; ++i) {
		shdr = (Elf32_Shdr *) cvmx_phys_to_ptr (addr + ehdr->e_shoff +
							(i *
							 sizeof (Elf32_Shdr)));

		if (!(shdr->sh_flags & SHF_ALLOC)
		    || shdr->sh_addr == 0 || shdr->sh_size == 0) {
			continue;
		}

		if (strtab) {
			debug ("%sing %s @ 0x%08lx (%ld bytes)\n",
			       (shdr->sh_type == SHT_NOBITS) ?
			       "Clear" : "Load",
			       &strtab[shdr->sh_name],
			       (unsigned long)shdr->sh_addr,
			       (long)shdr->sh_size);
		}

		if (shdr->sh_type == SHT_NOBITS) {
			memset ((void *)shdr->sh_addr, 0, shdr->sh_size);
		} else {
			image =
			    (unsigned char *)cvmx_phys_to_ptr (addr +
							       shdr->sh_offset);
			memcpy ((void *)shdr->sh_addr, (const void *)image,
				shdr->sh_size);
		}
		flush_cache (shdr->sh_addr, shdr->sh_size);
	}

	return ehdr->e_entry;
}

/* ======================================================================
 * A very simple elf loader, assumes the image is valid, returns the
 * entry point address.
 * ====================================================================== */
u64 load_elf64_image (unsigned long addr, uint64_t load_override)
{
	Elf64_Ehdr *ehdr;	/* Elf header structure pointer     */
	Elf64_Shdr *shdr;	/* Section header structure pointer */
	unsigned char *strtab = 0;	/* String table pointer     */
	unsigned char *image;	/* Binary image pointer             */
	int i;			/* Loop counter                     */
	uint64_t phoff;
	uint64_t daddr;		/* 64-bit virtual address */
	unsigned phnum;
	unsigned phentsize;
	struct elf_accessors *a = get_elf_accessors(addr);

	/* -------------------------------------------------- */

	ehdr = (Elf64_Ehdr *) addr;

	/*
	 * Take care to strip of the cache coherency bits from the XKPHYS
	 * addresses using octeon_fixup_xkphys().
	 * Use 64 bit memset/memcpy that take 64 bit addresses (xkphys) to
	 * clear/copy memory
	 */
	phoff = a->w64(ehdr->e_phoff);
	phnum = a->w16(ehdr->e_phnum);
	phentsize = a->w16(ehdr->e_phentsize);
	if (a->w16(ehdr->e_type) == ET_EXEC && phoff && phnum) {
		/* Load program headers.  */
		for (i = 0; i < phnum; i++) {
			Elf64_Phdr *phdr =
			    (Elf64_Phdr *) (uint32_t)(addr + phoff + (i * phentsize));
			if (a->w32(phdr->p_type) == PT_LOAD) {
				uint64_t offset = a->w64(phdr->p_offset);
				uint64_t filesz = a->w64(phdr->p_filesz);
				uint64_t memsz = a->w64(phdr->p_memsz);
				uint64_t paddr = load_override ? load_override : a->w64(phdr->p_paddr);

				/* Load it. */
				debug ("Processing PHDR %d\n", i);
				image = (unsigned char *)(uint32_t)(offset + addr);
#ifdef CONFIG_OCTEON
				daddr = octeon_fixup_xkphys(paddr);
#else
				daddr = paddr;
#endif

				if (filesz) {
					debug ("  Loading 0x%llx bytes at %llx\n",
					       filesz, daddr);
					memcpy64(daddr, (int32_t) image,
						 filesz);
				}
				if (memsz > filesz) {
					debug ("  Clearing 0x%llx bytes at %llx\n",
					       memsz - filesz,
					       daddr + filesz);
					memset64(daddr + filesz, 0,
						 memsz - filesz);
				}
				if (is_little_endian_elf(addr)) {
					/* swap the bytes */
					uint64_t base = daddr;
					uint64_t pos = 0;
					uint64_t ptr;
					uint64_t v;

					while (pos < memsz) {
						ptr = base + pos;
						asm volatile ("ld %0,0(%1)\n"
							      "	dsbh %0,%0\n"
							      "	dshd %0,%0\n"
							      "	sd %0,0(%1)"
							      : "=&r" (v) : "r" (ptr) : "memory");
						pos += sizeof(uint64_t);
					}

				}
				flush_cache (daddr, memsz);
			}
		}
#ifdef CONFIG_OCTEON
		return octeon_fixup_xkphys (a->w64(ehdr->e_entry));
#else
		return a->w64(ehdr->e_entry);
#endif
	}
	/* Otherwise do it the old way with section headers.  */

	/* Find the section header string table for output info */
#ifdef CONFIG_OCTEON
	shdr = (Elf64_Shdr *) cvmx_phys_to_ptr (addr + ehdr->e_shoff +
						(ehdr->e_shstrndx *
						 sizeof (Elf64_Shdr)));
#else
	shdr = (Elf64_Shdr *) (addr + ehdr->e_shoff +
			       (ehdr->e_shstrndx * sizeof (Elf64_Shdr)));
#endif

	if (shdr->sh_type == SHT_STRTAB)
		strtab = (unsigned char *)(u32) (addr + shdr->sh_offset);

	/* Load each appropriate section */
	for (i = 0; i < ehdr->e_shnum; ++i) {
		shdr = (Elf64_Shdr *) (u32) (addr + ehdr->e_shoff +
					     (i * sizeof (Elf32_Shdr)));

		if (!(shdr->sh_flags & SHF_ALLOC)
		    || shdr->sh_addr == 0 || shdr->sh_size == 0) {
			continue;
		}

		if (strtab) {
			debug ("%sing %s @ 0x%08lx (%ld bytes)\n",
			       (shdr->sh_type == SHT_NOBITS) ?
			       "Clear" : "Load",
			       &strtab[shdr->sh_name],
			       (unsigned long)shdr->sh_addr,
			       (long)shdr->sh_size);
		}
#ifdef CONFIG_OCTEON
		daddr = octeon_fixup_xkphys(shdr->sh_addr);
#else
		daddr = shdr->sh_addr;
#endif
		if (shdr->sh_type == SHT_NOBITS) {
			memset64 (daddr, 0, shdr->sh_size);
		} else {
			image = (unsigned char *)addr + shdr->sh_offset;
			memcpy64 (daddr, (u32) image, shdr->sh_size);
		}
		flush_cache (daddr, shdr->sh_size);
	}

	return ehdr->e_entry;
}

/* ====================================================================== */
U_BOOT_CMD (bootelf, 2, 0, do_bootelf,
	    "Boot from an ELF image in memory",
	    " [address] - load address of ELF image.");

U_BOOT_CMD (bootvx, 2, 0, do_bootvx,
	    "Boot vxWorks from an ELF image",
	    " [address] - load address of vxWorks ELF image.");
