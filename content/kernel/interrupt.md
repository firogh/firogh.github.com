---
tags: kernel
title: Interrupt
date: 2015-02-27 15:46:12
category: kernel
---

# The player
* Interrupt -- irq,softirq
* Process -- user process, kernel thread(workqueue)

# Common concpets
userspace: A process executing its own code outside the kernel.

##What is context?
###process context/user context: 
The kernel executing on behalf of a particular process 
(ie. a system call or trap) or kernel thread. 
You can tell which process with the current macro.) 
Not to be confused with userspace. 
Can be interrupted by software or hardware interrupts.

* register, kernel task_struct, stack. user text stack heap
###interrupt context: 
* Firo thinks the key point is interrupt context is irrelevant to process context!
* Can not schedule!
* register, maybe interrupt/process stack.
###softirq context
in the context of whichever process happens to be running at the wrong time; 
that is the "randomly chosen victim" aspect that Thomas was talking about.

##Preemption
* Linux kernel user mode is always User preemption.
When returning to user-space from a system call.
When returning to user-space from an interrupt hander.
* Linux kernel kernel mode is coppertive when CONFIG_PREEMPT is not set.
If a task in the kernel explicitly calls schedule()
If a task in the kernel blocked (which results in a all to schedule())
* Linux kernel kernel mode is coppertive + preemptive when CONFIG_PREEMPT is set.
schedule + blcoked
When an interrupt handler exits, before returning to kernel-space.
When kernel code becomes preemptible again.

###Entries of preempt schdule, FIXME
1 After Interrupt return,preempt_count_irq.
2 cond_resched
3 preempt_count

###What is the PREEMPT_ACTIVE
* This sets a flag bit in the preemption counter that has such a large value
	that it is never affected by the regular preemption counter increments

* It indicates to the schedule function that scheduling was not 
	invoked in the normal way but as a result of a kernel preemption.

* This ensures that the next task is selected as quickly as possible without 
	the hassle of deactivating the current one. If a high-priority task is waiting 
	to be scheduled, it will be picked by the scheduler class and will be allowed to run.

###preempt_count
* preempt 8
* softirq 8
* irq	4
* preempt active 1
* nmi 1


#Interrupt
## init
native_init_IRQ()->  set_intr_gate(i, irq_entries_start...
irq_entries_start define in arch/x86/kernel/entry_32.S

##Interrupt Context terminology
* in_irq
in __irq_enter I found preempt_count_add(HARDIRQ_OFFSET) only in here!
+irq disable 部设计preempt_cout

* in_softirq
should rename to in_softirq_or_bh_disable()
local_bh_disable use SOFTIRQ_DISABLE_OFFSET  (2 * SOFTIRQ_OFFSET)
__do_softirq use __local_bh_disable_ip(_RET_IP_, SOFTIRQ_OFFSET), damn it!

* in_interrupt
一种是irq路径上的do_softirq, 即in_serving_softirq
+local_bh_disbale()
//not in softirq context!!!
+local_bh_enable()
硬中断呢?
do_IRQ,肯定是.
local_irq_disable,不是, 但是no operation with preempt_count() 这种情况x86 in_interrupt是无感的.use irqs_disabled to check
[Question about in_interrupt() semantics with regard to softirqs]()

[Deal PF_MEMALLOC in softirq](http://thread.gmane.org/gmane.linux.kernel/1152658)

##softirq
同一个softirq可以在不同的CPU上同时运行，softirq必须是可重入的。
* not allow execute nest but can recusive lock:local_bh_disable 
current->preemt_count + SOFIRQ_OFFSET also disable preempt current process.
* hardirq on, can't sleep
* not percpu
## tasklet and kernel timer is based on softirq
新增softirq, 是要重新编译内核的, 试试tasklet也不错.
.不允许两个两个相同类型的tasklet同时执行，即使在不同的处理器上
* First of all, it's a conglomerate of mostly unrelated jobs, 
 which run in the context of a randomly chosen victim 
 w/o the ability to put any control on them. --Thomas Gleixner

tasklet different with other softirq is run  signal cpu core
spinlock_bh wider then spinlock 

###time of softirq
* follow hardirq, irq_exit()
* re-enables softirq, local_bh_enable/spin_unlock_bh(); explicity checks executes, netstack/blockIO.
* ksoftirqd

###tasklet
tasklet like a workqueue, sofirq like kthread. that is wonderful, does it?
tasklet 被__tasklet_schedule到某个cpu的percu 变量tasklet_vec.tail上保证了
只有一个cpu执行同一时刻.

#Process
##kthread and workqueue
kthread do specific func onshot
workqueue can dynamic set different specific work
workqueue is just a coat of kthread

## Differences between softirq and workqueue
Softirq(tasklet)
init static:	DECLARE_TASKLET(), DECLARE_TASKLET_DISABLED()
init dynamic:	tasklet_init()
schedule:	tasklet_schedule()
exit:		tasklet_kill()
pros and cons:	execute in interrupt context, serialization, tasklet blonged to the cpu which tasklet_schedule() it, oneshot

Workqueue
init static:	DECLARE_WORK()
init dynamic:	kzalloc() INIT_WORK(), kzalloc() PREPARE_WORK()
schedule1:	create_singlethread_workqueue(), create_workqueue()  
		queue_work()
exit1:		flush_workqueue(), destroy_workqueue()
schedule2:	schedule_work()
exit2:		(none)
pros and cons:	execute in process context, workqueue has two type, driver can create workqueue or use system workqueue, oneshot

#FAQ
##When to save irq rather than just disable irq
local_irq_disable() used in the code path that never disabled interrupts.
local_irq_save(flags) used in the code path that already disabled interrupts.

##what about irq nested?
http://lwn.net/Articles/380937/

##irq and lock
irq and lock are complete different things!
when need to disable irq just get rid of mess of shared data.

