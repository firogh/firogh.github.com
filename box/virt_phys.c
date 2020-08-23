#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
//#include <linux/hugetlb.h>

static int pmd_huge(pmd_t pmd)
{
        return !pmd_none(pmd) &&
                (pmd_val(pmd) & (_PAGE_PRESENT|_PAGE_PSE)) != _PAGE_PRESENT;
}

static int pud_huge(pud_t pud)
{
        return !!(pud_val(pud) & _PAGE_PSE);
}

void follow_pte(struct mm_struct * mm, unsigned long address, pte_t * entry)
{
    pgd_t * pgd = pgd_offset(mm, address);

    printk("follow_pte() for %lx\n", address);

    entry->pte = 0;
    if (!pgd_none(*pgd) && !pgd_bad(*pgd)) {
        pud_t * pud = pud_offset(pgd, address);
        struct vm_area_struct * vma = find_vma(mm, address);

        printk(" pgd = %lx\n", pgd_val(*pgd));

        if (pud_none(*pud)) {
            printk("  pud = empty\n");
            return;
        }
        if (pud_huge(*pud) && vma->vm_flags & VM_HUGETLB) {
            entry->pte = pud_val(*pud);
            printk("  pud = huge\n");
            return;
        }

        if (!pud_bad(*pud)) {
            pmd_t * pmd = pmd_offset(pud, address);

            printk("  pud = %lx\n", pud_val(*pud));

            if (pmd_none(*pmd)) {
                printk("   pmd = empty\n");
                return;
            }
            if (pmd_huge(*pmd) && vma->vm_flags & VM_HUGETLB) {
                entry->pte = pmd_val(*pmd);
                printk("   pmd = huge\n");
                return;
            }
            if (pmd_trans_huge(*pmd)) {
                entry->pte = pmd_val(*pmd);
                printk("   pmd = trans_huge\n");
                return;
            }
            if (!pmd_bad(*pmd)) {
                pte_t * pte = pte_offset_map(pmd, address);

                printk("   pmd = %lx\n", pmd_val(*pmd));

                if (!pte_none(*pte)) {
                    entry->pte = pte_val(*pte);
                    printk("    pte = %lx\n", pte_val(*pte));
                } else {
                    printk("    pte = empty\n");
                }
                pte_unmap(pte);
            }
        }
    }
}
int tmp;
static int __init pte_init(void)
{
	printk("Firo virt addr %p\n", (unsigned long)&tmp);
	pte_t p;
	
	follow_pte(current->mm, (unsigned long)&tmp, &p);
	printk("Firo phy addr %p\n", p.pte); 
	return 0;
}

static void __exit pte_exit(void)
{

}
module_init(pte_init)
module_exit(pte_exit)
MODULE_LICENSE("GPL");
