---
tags: kernel
title: Synchronization
date: 2015-02-27 15:46:12
category: kernel
---

#Reference
Perfbook

# Why we need synchronization

a critical section is a piece of code that accesses a shared resource 
(data structure or device) that must not be concurrently accessed by more than one thread of execution.

This overlap, where the result depends on the relative timing of multiple tasks, is called a race condition. 

# How to use synchronization mechanism
A must read book[Unreliable Guide To Locking -- Rusty Russell](https://www.kernel.org/pub/linux/kernel/people/rusty/kernel-locking/index.html)
## Common usage
Protect from interruption by hardware interrupts:
        local_irq_disable(int irq) & local_irq_enable(int irq)
Protection from software interrupts:
        local_bh_disable(void) & local_bh_enable(void)
Protection from other CPUs:
        spin_lock(spinlock_t *) & spin_unlock(spinlock_t *)
Preemption by other user contexts:
        preempt_disable(void) & preempt_enable(void) 


# What is synchronization in computer science 
[Synchronize](http://www.etymonline.com/index.php?term=synchronize)
synchronization means be of the same time.
It means "make it synchronous", something like coexistence.

Process synchronization refers to the idea that multiple processes are 
to join up or handshake at a certain point, in order to reach an 
agreement or commit to a certain sequence of action.
* Mutual exclusion -- only one excution routine in  critical section
Readers–writers problem
The Producer-Consumer Problem 
* Cooperative
TCP Handshake 
* Deadlock
Dining philosophers problem
* Starvation
* Prioirty inversion
* Busy waiting

# Mutual exclusion
## spinlock - unsleepable mutex
[Linux 内核的排队自旋锁FIFO Ticket Spinlock](https://www.ibm.com/developerworks/cn/linux/l-cn-spinlock/)
spinlock保护的critical section执行时间短且不睡眠, 副产品就是避免了
context switch, 相对semaphore效率高.
显然对于little critical section 使用semaphore而不是spinlock,将带来性能
上的巨大损耗.
* rw_lock

## Mutex
Monitor lock before sleep.

## RCU -- lockless
Check The Journey to RCU for more details
## Per-cpu

# Cooperative
## semaphore 
it was implement based on spinlock.
it can have more than one holder at any time (the number decided at initialization time), 
although it is most commonly used as a single-holder lock (a mutex).
if you can not get a semaphore, your task will put itself on the wait queue, and be woken 
up the semaphore is released.
sleepable
preemptable
## TCP handshake

# Memory barrier
## Refernce
http://en.wikipedia.org/wiki/Memory_barrier
Documentation/memory-barriers.txt
## Common
* When a program runs on a single-CPU machine, the hardware performs the necessary bookkeeping 
to ensure that the program executes as if all memory operations were performed in the order 
specified by the programmer (program order), so memory barriers are not necessary. 
However, when the memory is shared with multiple devices, such as other CPUs in a multiprocessor 
system, or memory mapped peripherals, out-of-order access may affect program behavior. 
For example, a second CPU may see memory changes made by the first CPU in a sequence which differs from program order.
* Compiler and cpu do the same optimization: reorder of instructions
## The Linux kernel has a variety of different barriers that act at different
levels:
  (*) Compiler barrier.
  (*) CPU memory barriers.
  (*) MMIO write barrier.
## ACCESS_ONCE
* Does it work cast a variable to volatile?
No, there is no efects on cast a variable to volatile. 
Because, access variable is before volatile cast! That means you
still get a register value. What you do is just conversion a temporary 
variable
Rationale for International Standard--Programming Languages--C
6.7.3 
## For volatile
http://www.geeksforgeeks.org/understanding-volatile-qualifier-in-c/
