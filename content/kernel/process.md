---
tags: kernel
title: Linux process management
date: 2015-02-27 15:46:12
category: kernel
---

# Protection ring
## Kernel mode:
## User mode
限制访问地址空间, 内核数据结构, I/O 指令, 中断代码等.

## Mode switch
esp既表示进程的用户栈，也表进程的内核栈，要看进程处于用户态还是内核态。
读TSS段中进程的内核态指针付给ss和esp。
把用户态SS和esp指针保存在进程的内核栈上，
从内核态返回时，弹出进程内核栈上用户态ss和esp值。
那么TSS段中进程的内核态指针是从哪里来的呢？

cpu -> tr -> tss -> esp0 => 内核栈->保存用户态cs eip esp等.
Hardware context switch.
pt_regs

## kernel stack
从system_call中, 我们可以发现:
GET_THREAD_INFO(%ebp)

master idle进程的kernel stack在init/init_task.c:init_thread_union
其他进程的kernel stack是fork产生.
this_cpu_write(kernel_stack,(unsigned long)task_stack_page(next_p) +THREAD_SIZE);
this_cpu_write(cpu_current_top_of_stack,(unsigned long)task_stack_page(next_p) +THREAD_SIZE);

# Schedule cfs
sum_runtime = sysctl_sched_min_granularity * nr_running（if 进程数 > 5）
sum_runtime = sysctl_sched_latency = 20 ms （if 进程数 <= 5） 
sysctl_sched_min_granularity = 4ms
ideal_time = sum_runtime * se.weight/cfs_rq.weight
vruntime +=  delta * NICE_0_LOAD/ se.weight;(if curr.nice!=NICE_0_LOAD)

# Idle
主处理器上的idle由原始进程(pid=0)演变而来。从处理器上的idle由init进程fork得到，但是它们的pid都为0
init_idle.

# Porcess context
## Userspace context
.txt, .data, .bss, userspace stack, heap, library.
## Kernespace context
task_struct, mm_struct, pgd, kernelspace stack.
## Hardware context
Registers

## Context switch
A context switch (also sometimes referred to as a process switch or a task switch) 
is the switching of the CPU (central processing unit) from one process or thread to another.

## 进程切换process switch
也叫上下文切换context switch只发生在内核态，必须理解而不是记住。
一个进程会用到许多cpu的寄存器比如esp，eip等等，他们被称作Hardware context硬件上下文，
如果进程是培养皿中的一条幼虫，那么硬件上下问就是培养液了。不同的虫子需要不同的培养液，
新的培养液进来，旧的培养液也需要保存，以备稍后使用。
当进程切换时，我们要保存当前被切换出去进程的硬件上下文，以备将来切换回来使用。
应该明白为什么进程切换也叫上下文切换了。怎么保存上下文呢？
在进程的task_struct结构的成员变量thread_struct结构保存被切换出去进程的硬件上下文。
我们所关心的换出的进程的esp指针也保存在thread_struct当中了。
当然，对于被切换进来的进程我们用这个进程的thread_struct成员中的esp 值更新硬件的esp寄存器，
也需要更新GDT中的TSS里的esp0,以备将来用来从用户态切换到内核态时，找到进程内核栈的栈顶指针。

## switch_to
__schedule()->swtich_to
### load TSS
进程切换只发生在内核态
/*将next进程的内核栈指针(next->thread->sp0)值更新到当前CPU的TSS中*/
load_sp0(tss, next);
task_struct.thread.sp0
thread_struct

## type of Context switch
* context switch by hardware
Old linux kernel
* context switch by software
kernel 2.6

# Common concepts
Multiprocessing for SMP

# Process flag
PF_MEMALLOC [Kill PF_MEMALLOC abuse](http://thread.gmane.org/gmane.linux.kernel/914878)

# Process user ID
## Effective user/group ID
euid:4777
egid:2777
chmod u+s cynthia
root进程执行cynthia后euid变为firo, saved user id变为root

## Example
u+s 就可以
firo@firo tmp$ iptables -L
iptables v1.4.21: can't initialize iptables table `filter': Permission denied (you must be root)
Perhaps iptables or your kernel needs to be upgraded.

chmod +s /usr/sbin/iptables

firo@firo tmp$ iptables -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination         

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination         

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination 

chmod -s /usr/sbin/iptables

iptables -L
iptables v1.4.21: can't initialize iptables table `filter': Permission denied (you must be root)
Perhaps iptables or your kernel needs to be upgraded.


# Scheduling strategies
multiprogramming
time-sharing(multi tasking)
real-time

# Multitasking 
## Reference
codes: comments of __schedule()
Book: Linux kernel development 

##Common concepts
* The process scheduler decides which process runs, when, and for how long.
* Multitasking: cooperative and preemptive.
* Preemption:Involuntarily suspending a running process.
* Timeslice: a slice of cpu's time.
* Yielding: The act of a process voluntarily suspending itself.
* Cooperative multitasking: a process does not stop running until it voluntary decides to do so.
* Time sharing: is cooperative mutitasking, 
* the codes that yeild the cpu when timeslice is zero while ret from syscall is also belong to user process!
so if !CONFIG_PREEMPT* is cooperative!
* CFS: Pick the task with the smallest vruntime. That's it!

##TIF_NEED_RESCHED
* The situation of set task need resched
scheduler_tick()
try_to_wake_up() when a process that has a higher priority than *current* is awakened.
Other?

## SAVE_ALL
no include cs ss ip sp eflag why?

#Deamonize
## fork, oraphan
similar &
not a goup leader inherit with parent pgid for setsid
## setsid
* session leader
* process group leader
A signal directed to a process group is delivered individually to 
all of the processes that are members of the group. 
* deatch controlling terminals(confusing)
## chdir
## umask
## deal with fd
below both output
守护进程继承了shell的屏幕和键盘输入.
pid_t pid= fork();
if (pid != 0) {
	system("sleep 3");
	printf("parent exit\n");
} else {
	printf("child start\n");
	setsid(); 
	fprintf(stdout, "out child\n");
	fprintf(stderr, "err child\n");

	system("sleep 7");

	fprintf(stdout, "after out child\n");
	fprintf(stderr, "after err child\n");
	printf("child exit\n");
}
# Zombie process
## <defunct>
forked child not reaped by parent will hooked in process list.
if parent was killed and exit <defunct> will repaped.
<defunct>表明父活着, 但不收尸.
## Double fork for parent long live 
mirgate subchild to init, parent just reap child.

# FAQ
* thread_info
可以快速访问task_struct.

* swtch_stack
线程切换

