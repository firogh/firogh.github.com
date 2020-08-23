// SPDX-License-Identifier: GPL-2.0-only
/*
 * NOTE: This example is works on x86 and powerpc.
 * Here's a sample kernel module showing the use of kprobes to dump a
 * stack trace and selected registers when _do_fork() is called.
 *
 * For more information on theory of operation of kprobes, see
 * Documentation/kprobes.txt
 *
 * You will see the trace data in /var/log/messages and on the console
 * whenever _do_fork() is invoked to create a new process.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/smp.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <asm/traps.h>

#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "__do_page_fault";
module_param_string(symbol, symbol, sizeof(symbol), 0644);
#define STACK_TOP_MARGIN        128


/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= symbol,
};

struct pf_stack_backups {
       unsigned char stack[PAGE_SIZE * 2]; /* Backup at most 2 pages */
       unsigned long sp;
};

static DEFINE_PER_CPU(struct pf_stack_backups, pf_stacks);

DECLARE_PER_CPU_FIRST(union irq_stack_union, irq_stack_union) __visible;
DECLARE_PER_CPU(char *, irq_stack_ptr);

static void pf_backup_stack(struct pt_regs *regs)
{
        unsigned long sp = regs->sp;
        unsigned long irq_stack_bottom, irq_stack_top;

        irq_stack_top = (unsigned long) this_cpu_ptr(irq_stack_union.irq_stack) +
                        STACK_TOP_MARGIN;
        irq_stack_bottom = (unsigned long) __this_cpu_read(irq_stack_ptr);

        if (sp >= irq_stack_top && sp <= irq_stack_bottom) {
                memcpy(this_cpu_ptr(pf_stacks.stack), (unsigned char *)
                        (irq_stack_bottom - PAGE_SIZE * 2), PAGE_SIZE * 2); 
                printk("SUSE debug back up data on IRQ stack: SP %lu\n", sp); 
        }   
}

enum x86_pf_error_code {
        X86_PF_PROT     =               1 << 0,
        X86_PF_WRITE    =               1 << 1,
        X86_PF_USER     =               1 << 2,
        X86_PF_RSVD     =               1 << 3,
        X86_PF_INSTR    =               1 << 4,
        X86_PF_PK       =               1 << 5,
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	struct pt_regs *registers = (struct pt_regs *)regs->di;
	unsigned long error_code = regs->si;

	pr_info("<%s> pre_handler: p->addr = 0x%p, ip = %lx, flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->ip, regs->flags);

	/* A dump_stack() here will give a stack backtrace */
	if (unlikely(error_code & X86_PF_INSTR && !(error_code & X86_PF_USER)))
		pf_backup_stack(registers);

	return 0;
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}

static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = NULL;
	kp.fault_handler = handler_fault;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
