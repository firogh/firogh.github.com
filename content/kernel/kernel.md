---
tags: kernel
title: kernel
date: 2015-02-27 15:46:12
category: kernel
---

# Reference
[Kernel in Wikipedia](https://en.wikipedia.org/wiki/Kernel_(operating_system)
* Book
* Website
http://lwn.net/
http://elinux.org/Kernel_Debugging_Tips
https://www.kernel.org/doc/
http://kernelnewbies.org/KernelJanitors/Todo
http://eudyptula-challenge.org/
[走近Linux内核-- 王聪](http://wangcong.org/2007/03/09/-e8-b5-b0-e8-bf-91linux-e5-86-85-e6-a0-b8/)

# CPU freq
* onset
device_initcall: intel_pstate_init->cpufreq_register_driver
* nuclus
** onset
__cpufreq_add_dev -> cpufreq init= intel_pstate_cpu_init->intel_pstate_init_cpu
** coda
_cpu_down->
{
	cpu_notify_nofail(CPU_DEAD | mod, hcpu)->timer_cpu_notify
	cpu_notify_nofail(CPU_POST_DEAD | mod, hcpu)->cpufreq_cpu_callback->cpufreq exit = intel_pstate_cpu_exit,
}

* coda

# kernel panic 3.10.62
general protection fault and page fault
1498 errorentry general_protection do_general_protection
1499 errorentry page_fault do_page_fault
static const struct stacktrace_ops print_trace_ops = { 
        .stack                  = print_trace_stack,
        .address                = print_trace_address,
        .walk_stack             = print_context_stack,
};
no_context->
{
	show_fault_oops->
	__die->
	{
		print_modules
		show_regs
		{
			printk(KERN_DEFAULT "Stack:\n");
			show_stack_log_lvl->
			{
				show_trace_log_lvl->
				{
					printk("%sCall Trace:\n", log_lvl);
					// arch/x86/kernel/dumpstack_64.c
					dump_trace->&print_trace_ops
				}
			}
			printk(KERN_DEFAULT "Code: ");
		}
	}
}
# Contents
What's the difference between kernel and OS?
cpu: syscall, process, ipc, smp
memory:  mm
io: buffer cache, fs, io subsys

# Design pattern
[Linux kernel design patterns](http://lwn.net/Articles/336224/)
[Linux Kernel Programming Patterns](http://www.cs.fsu.edu/~baker/devices/notes/patterns.html#)

# Interface
Latin inter (prep., adv.) "among, between, betwixt, in the midst of," from PIE *enter "between, among"
[Interface](http://www.webopedia.com/TERM/I/interface.html):A boundary across which two independent systems meet and act on or communicate with each other.
In computing, an [interface](https://en.wikipedia.org/wiki/Interface_(computing\) ) is a shared boundary across which two separate components of a computer system exchange information.
##API 
system call
+ procfs are ultimately accessed via system calls
##ABI
elf, cpu specific
### x32 ABI
ILP32
LP64
## System call

# Find bug
* Considered fault types
Block “To avoid deadlock, do not call blocking functions with interrupts disabled or a spinlock held.”
Null “Check potentially NULL pointers returned from routines”
Var “Do not allocate large stack variables (>1K) on the fixed-size kernel stack.”
INull “Do not make inconsistent assumptions about whether a pointer is NULL.”
Range “Always check bounds of array indices and loop bounds derived from user data.”
Lock “Release acquired locks; do not double-acquire locks.”
Intr “Restore disabled interrupts.”
Free “Do not use freed memory.”
Float “Do not use floating point in the kernel.”
Size “Allocate enough memory to hold the type for which you are allocating.”

##KSM

#Signal
* struct signal_sturct: 
The "struct signal_struct" is the random *leftovers* from all the other stuff.
http://thread.gmane.org/gmane.linux.kernel/512831/focus=513990
* sigpending
Store blocked signal info
* Non-mask signal
SIGKILL, SIGSTOP
##Generate signal
__send_signal();
##Process siganl
* SIGKILL (may be some other)
process in _send_signal()-> complete_signal() tsk->state |= TASK_WAKEKILL 
http://lwn.net/Articles/288056/
http://www.ibm.com/developerworks/library/l-task-killable/
* others
each time a switch is made from kernel mode to user mode, 
arch-specific: entry.S -> do_siganl()
{ 
	get_signal_deliver()
	{
		if fatal -> do_greoup_exit()->...__cleanup_sighand()
	}

	handle_signal() -> k->u(hanle)-sigreturn->k
}

#Netlink
* Group
enum rtnetlink_groups
##What is netlink
Networking related kernel configuration and monitoring interfaces.
* IPC between kernel and user spacess process.
ioctl
* prarts
	libnl
	libnl-route
	libnl-genl
	libnl-nf

* How many parts does libnl-route has?
Address,  links, neighboring, routing, TC

## Need patch
skbedit action
cgroup classifier
tun/tap dev 
gre tunnel dev 
tc classifier/action

#Namespace

#Module
##pre-require
modules.alias
http://doc.opensuse.org/documentation/html/openSUSE_113/opensuse-reference/cha.udev.html

http://blog.chinaunix.net/uid-22954220-id-4380202.html
运行时, 插入u盘也是这样.

2. 内核自己加载比如缺少模块的时候 网络协议, fs
https://unix.stackexchange.com/questions/90027/what-is-the-sequence-loading-linux-kernel-module-on-startup-how-priority-is-set/90037#90037

3 rc 里面smartqos之类的.

##Load module into kernel
vmlinux.lds.h linker scipts include helper macros.

#Data structures
* u32 __u32
__u32 is used for user-space. declare a variabe used by icotl. qosmark.
u32 is used for kernel.
deatils in ldd3e chapter 10

#Panic
kernel/kernel/panic.c

#init
##initcall

##disk 
subsys_initcall 4 genhd_device_init with base_probe{ request_module()}
module_init 6 ->init_sd->sync_schedule_domain(sd_probe_async

##cmdline
root= name_to_dev_t, mount_root in prepare_namespace
如果/init不能 sys_access, 则prepare_namespace,切换到真正的root=指定的设备上设备在sd_probe上初始化了.
systemd负责挂在文件系统, 切换.

#src-tree
include/linux: share with userspace
include/net: kernel stuff

# Assembly in kernel
为了简单! 内核修改特权寄存器和指令, 用汇编实现简单.
尽可能加速. c到汇编, 编译器相对保守, 

# The principle of kernel & driver backport
将高本版kernel的feature移植到低版本的kernel的过程就是backport.
1. 尽量保持与mainline的代码一致
2. 做好取舍不要引入太多patch. 
## 开发流程
理清所引入feature的代码. 主要数据结构以及功能流程.

# ARM
## smp_ops
Machine: Marvell Armada XP Development Board
MACHINE_START(_type,_name)
__section__(".arch.info.init")
arch/arm/tools/mach-types
armada_xp_db            MACH_ARMADA_XP_DB       ARMADA_XP_DB            3036

## smp
An Implementation Of Multiprocessor Linux Alan Cox

### cpumask

## Interrupt
SPI and PPI or IPI, difference?


# Faq
MACHINE_START

## Kernel/system boot hang/freeze
### Sysrq
Whe sysrq work?

### bootargs
debug initcall_debug


### printk

