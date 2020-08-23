extern phys_addr_t firo_bpa_start;
extern unsigned long firo_bpa_size;

bool firo_set_ro(struct page *page, struct vm_area_struct *vma, unsigned 
			long addr, void *arg)
{
	/*
	 * We should turn off swap, transparent huge page.
	 */
	pgd_t *pgd = pgd_offset(vma->vm_mm, addr);
	if (!pgd_present(*pgd)
		return false;
	p4d_t *p4d = p4d_offset(pgd, addr);
	if (!p4d_present(*p4d))
		return false;
	pud_t *pud = pud_offset(p4d, addr);
	if (!pud_present(*pud))
		return false;
	pmd_t *pmd = pmd_offset(pud, addr);
	if (!pmd_present(*pmd))
		return false;	
	spinlock_t *ptl;
	pte_t *ptep;

	/* 
	* This lock shoule be useless; Just for some scenarios that 
	* I cannot consider.
	*/
	ptep = pte_offset_map_lock(vma->vm_mm, pmd, addr, &ptl);
	clear_bit(_PAGE_BIT_RW, (unsigned long *)&ptep->pte);
	pte_unmap_unlock(ptep, ptl);

	return true;	
}

/*
bool firo_set_ro_v2(struct page *page, struct vm_area_struct *vma, unsigned 
			long addr, void *arg)
{
	        struct mm_struct *mm = vma->vm_mm;
        struct page_vma_mapped_walk pvmw = {
                .page = page,
                .vma = vma,
                .address = addr,
        };
        pte_t pteval;

	if (page_vma_mapped_walk(&pvmw)) {
		clear_bit(_PAGE_BIT_RW, (unsigned long *)&(pvmw.pte->pte));
		page_vma_mapped_walk_done(&pvmw);
		return true;
	}
	
	return false;
} */

bool firo_pte_is_ro(struct page *page)
{
	return true;
}
int firo_try_to_set_pte_to_ro(struct page *page, phys_addr_t pa)
{
	struct rmap_walk_control rwc = {
		.rmap_one = firo_set_ro;
		.arg = (void *)pa;
		/*.done = firo_pte_is_ro,*/
		.anon_lock = page_lock_anon_vma_read,
	};

	rmap_walk(page, &rwc);

	return 0;
}

int firo_set_physical_range_ro(void)
{
	int err = 0;

	struct page *page;
	phys_addr_t start,addr;
	long size = 0,ro_size = 30 * 1024 * 1024;
	start = firo_bpa_start + firo_bpa_size;

	for (addr = start; addr < (unsigned char*)start + ro_size; addr + PAGE_SIZE)
	{
		page = pfn_to_page(addr >> PAGE_SHIFT);
		if (page_mapped(page)) {
			/*
			 * try to set the pte to (pte & ~_PAGE_RW), read only.
			 */
			err = firo_try_to_set_pte_to_ro(page, addr);
			if (err)
				printk("Failed to set pte to ro\n");
		}
	}

	return err;
}

