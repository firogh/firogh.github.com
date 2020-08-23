#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/kprobes.h>
#include <linux/kallsyms.h>

static unsigned int counter = 0;
static unsigned int counter1 = 0;
static unsigned int counter2 = 0;
static unsigned int counter3 = 0;
static unsigned int counter4 = 0;
static unsigned int counterkrp = 0;
static unsigned int counterkrp1 = 0;
int Pre_Handler(struct kprobe *p, struct pt_regs *regs){
    //printk("Pre_Handler: counter=%u\n",counter++);
    return 0;
}
void kp_func(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
    printk("vmdq: counter=%u\n",counter++);
}
void kp_func1(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
    printk("==================================vxlan: counter=%u\n",counter1++);
}
void kp_func2(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
    printk("rar: counter=%u\n",counter2++);
}
void kp_func3(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
    printk("configure: counter=%u\n",counter3++);
}
void kp_func4(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
    printk("=====================================up: counter=%u\n",counter4++);
}

static int krp_func(struct kretprobe_instance *kreti,struct pt_regs *regs)
{

    printk("----------------------------krp vxlan: counter=%u\n",counterkrp++);
        return 0;
}

static int krp_func1(struct kretprobe_instance *kreti,struct pt_regs *regs)
{

    printk("----------------------------krp up: counter=%u\n",counterkrp1++);
        return 0;
}
static struct kprobe kp,kp1,kp2,kp3,kp4;
static struct kretprobe krp, krp1;
int myinit(void)
{
    kp.pre_handler = Pre_Handler;
    kp.post_handler = kp_func;
    kp.addr = (kprobe_opcode_t *)0xffffffffa05b1ba0;
    register_kprobe(&kp);


    kp1.post_handler = kp_func1;
    kp1.addr =(kprobe_opcode_t *)kallsyms_lookup_name("ixgbe_clear_vxlan_port.part.97");
    register_kprobe(&kp1);

    kp2.post_handler = kp_func2;
    kp2.addr=(kprobe_opcode_t *)kallsyms_lookup_name("ixgbe_clear_rar_generic");
    register_kprobe(&kp2);

    kp3.post_handler = kp_func3;
    kp3.addr = (kprobe_opcode_t *)kallsyms_lookup_name("ixgbe_configure");
    register_kprobe(&kp3);
    kp4.post_handler = kp_func4;
    kp4.addr = (kprobe_opcode_t *)kallsyms_lookup_name("ixgbe_up_complete");
    register_kprobe(&kp4);

    krp.handler = (kretprobe_handler_t) krp_func;
    krp1.handler = (kretprobe_handler_t) krp_func1;

    krp1.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("ixgbe_up_complete");
        register_kretprobe(&krp);
        register_kretprobe(&krp1);
    printk("module inserted\n ");
    return 0;
}
void myexit(void)
{
    unregister_kprobe(&kp);
    unregister_kprobe(&kp1);
    unregister_kprobe(&kp2);
    unregister_kprobe(&kp3);
    unregister_kprobe(&kp4);
    unregister_kretprobe(&krp);
    unregister_kretprobe(&krp1);
    printk("module removed\n ");
}
module_init(myinit);
module_exit(myexit);
MODULE_AUTHOR("Manoj");
MODULE_DESCRIPTION("KPROBE MODULE");
MODULE_LICENSE("GPL");
