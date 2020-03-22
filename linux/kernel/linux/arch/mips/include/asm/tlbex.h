
#include <asm/uasm.h>

extern int pgd_reg;
enum tlb_write_entry { tlb_random, tlb_indexed };
extern void tlbmiss_handler_setup_pgd(unsigned long);
void build_get_pmde64(u32 **p, struct uasm_label **l, struct uasm_reloc **r,
		 unsigned int tmp, unsigned int ptr);
void build_get_ptep(u32 **p, unsigned int tmp, unsigned int ptr);
void build_update_entries(u32 **p, unsigned int tmp, unsigned int ptep);
void build_tlb_write_entry(u32 **p, struct uasm_label **l,
				  struct uasm_reloc **r,
				  enum tlb_write_entry wmode);
