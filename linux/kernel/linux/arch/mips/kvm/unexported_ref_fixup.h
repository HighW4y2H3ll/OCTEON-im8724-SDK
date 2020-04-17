extern struct mm_struct *init_mm__kvmref__;
extern void (*tlbmiss_handler_setup_pgd__kvmref__)(unsigned long);
extern void (*pmd_init__kvmref__)(unsigned long page, unsigned long pagetable);
extern void (*pgd_init__kvmref__)(unsigned long page);
extern void (*__flush_cache_all__kvmref__)(void);
extern int *pgd_reg__kvmref__;
struct uasm_label;
struct uasm_reloc;
enum tlb_write_entry;
extern void (*build_tlb_write_entry__kvmref__)(u32 **p, struct uasm_label **l,
				  struct uasm_reloc **r,
				  enum tlb_write_entry wmode);
extern void (*build_get_pmde64__kvmref__)(u32 **p, struct uasm_label **l, struct uasm_reloc **r,
		 unsigned int tmp, unsigned int ptr);
extern void (*build_get_ptep__kvmref__)(u32 **p, unsigned int tmp, unsigned int ptr);
extern void (*build_update_entries__kvmref__)(u32 **p, unsigned int tmp, unsigned int ptep);

#ifndef __PAGETABLE_PMD_FOLDED
extern pmd_t *invalid_pmd_table__kvmref__;

static inline int pud_none__kvmref__(pud_t pud)
{
	return pud_val(pud) == (unsigned long) invalid_pmd_table__kvmref__;
}
static inline int pud_present__kvmref__(pud_t pud)
{
	return pud_val(pud) != (unsigned long) invalid_pmd_table__kvmref__;
}

static inline void pud_clear__kvmref__(pud_t *pudp)
{
	pud_val(*pudp) = ((unsigned long) invalid_pmd_table__kvmref__);
}
#endif
