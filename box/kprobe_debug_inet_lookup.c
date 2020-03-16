#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <net/sock.h>

#define MAX_SYMBOL_LEN  64
static char symbol[MAX_SYMBOL_LEN] = "__inet_lookup_established";
module_param_string(symbol, symbol, sizeof(symbol), 0644);
DEFINE_PER_CPU(struct sock *, sock_first);
DEFINE_PER_CPU(struct sock *, sock_prev);
EXPORT_SYMBOL(sock_first);
EXPORT_SYMBOL(sock_prev);

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp_4f = { 
        .symbol_name    = symbol,
};
static struct kprobe kp_59 = { 
        .symbol_name    = symbol,
};

/*
0xffffffffbc609dea <__inet_lookup_established+0x3a>:    callq  0xffffffffbc609960 <inet_ehashfn>
0xffffffffbc609def <__inet_lookup_established+0x3f>:    mov    %eax,%r15d
0xffffffffbc609df2 <__inet_lookup_established+0x42>:    and    0x10(%r14),%r15d
0xffffffffbc609df6 <__inet_lookup_established+0x46>:    mov    %eax,%ebx
0xffffffffbc609df8 <__inet_lookup_established+0x48>:    mov    (%r14),%rax
0xffffffffbc609dfb <__inet_lookup_established+0x4b>:    lea    (%rax,%r15,8),%r14
0xffffffffbc609dff <__inet_lookup_established+0x4f>:    mov    (%r14),%rdx
0xffffffffbc609e02 <__inet_lookup_established+0x52>:    test   $0x1,%dl
0xffffffffbc609e05 <__inet_lookup_established+0x55>:    je     0xffffffffbc609e11 <__inet_lookup_established+0x61>
0xffffffffbc609e07 <__inet_lookup_established+0x57>:    jmp    0xffffffffbc609e71 <__inet_lookup_established+0xc1>
0xffffffffbc609e09 <__inet_lookup_established+0x59>:    mov    (%rdx),%rdx
0xffffffffbc609e0c <__inet_lookup_established+0x5c>:    test   $0x1,%dl
0xffffffffbc609e0f <__inet_lookup_established+0x5f>:    jne    0xffffffffbc609e71 <__inet_lookup_established+0xc1>
0xffffffffbc609e11 <__inet_lookup_established+0x61>:    cmp    %ebx,-0x60(%rdx)
0xffffffffbc609e14 <__inet_lookup_established+0x64>:    jne    0xffffffffbc609e09 <__inet_lookup_established+0x59>
*/

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	struct sock *sk = (struct sock *)(regs->dx - 0x68);

	/*
	if (regs->dx & 0x1) {
		pr_info("<%s> prev: p->addr = 0x%p, ip = %lx, sock prev %lx, nulls, hash %lx, flags = 0x%lx\n", p->symbol_name, p->addr, regs->ip, regs->dx,  regs->bx, regs->flags);
	} else {	
		pr_info("<%s> prev: p->addr = 0x%p, ip = %lx, sock prev %lx, sock hash %x, hash %lx, flags = 0x%lx\n", p->symbol_name, p->addr, regs->ip, regs->dx, sk->sk_hash, regs->bx, regs->flags);

	}
	*/

	/* No need to disable preempt, since we are in softirq */
        per_cpu(sock_prev, smp_processor_id()) = sk;

        return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void handler_post(struct kprobe *p, struct pt_regs *regs,
                                unsigned long flags)
{
	struct sock *sk = (struct sock *)(regs->dx - 0x68);

	/*
	if (regs->dx & 0x1) {
		pr_info("<%s> post: p->addr = 0x%p, ip = %lx, sock first %lx, nulls, hash %lx, flags = 0x%lx\n", p->symbol_name, p->addr, regs->ip, regs->dx,  regs->bx, regs->flags);
	} else {	
		pr_info("<%s> post: p->addr = 0x%p, ip = %lx, sock first %lx, sock hash %x, hash %lx, flags = 0x%lx\n", p->symbol_name, p->addr, regs->ip, regs->dx, sk->sk_hash, regs->bx, regs->flags);

	}
	*/
	per_cpu(sock_first, smp_processor_id()) = sk;
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
        pr_info("firo: SUSE: fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
        /* Return 0 because we don't handle the fault. */
        return 0;
}

static int __init kprobe_init(void)
{
        int ret;

        kp_4f.post_handler = handler_post;
        kp_4f.fault_handler = handler_fault;
        kp_4f.offset = 0x4f;

        kp_59.pre_handler = handler_pre;
        kp_59.fault_handler = handler_fault;
        kp_59.offset = 0x59;

        ret = register_kprobe(&kp_4f);
        if (ret < 0) {
                pr_err("register_kprobe 4f failed, returned %d\n", ret);
                return ret;
        }
	ret = register_kprobe(&kp_59);
	if (ret < 0) {
		unregister_kprobe(&kp_4f);
                pr_err("register_kprobe 59 failed, returned %d\n", ret);
                return ret;
	}

        pr_info("firo: SUSE: Planted kprobe at %p %p\n", kp_4f.addr, kp_59.addr);
        return 0;
}

static void __exit kprobe_exit(void)
{
        unregister_kprobe(&kp_4f);
        unregister_kprobe(&kp_59);
        pr_info("firo: SUSE: kprobe at %p %p unregistered and sock %p\n", kp_4f.addr, kp_59.addr, 
					per_cpu(sock_prev, smp_processor_id()));
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
