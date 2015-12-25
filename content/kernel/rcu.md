---
tags: kernel
title: The Journey to RCU
date: 2015-05-24 09:52:12
category: kernel
---

# Reference
<iframe frameborder="no" border="0" marginwidth="0" marginheight="0" width=330 height=86 src="http://music.163.com/outchain/player?type=2&id=444737&auto=1&height=66"></iframe>
[官网Read-Copy Update Mutual Exclusion](http://lse.sourceforge.net/locking/rcupdate.html)
[Read Copy Update HOWTO](http://lse.sourceforge.net/locking/rcu/HOWTO/index.html)
[Read-Copy Update Mutual-Exclusion in Linux](http://lse.sourceforge.net/locking/rcu/rcupdate_doc.html)
[Thanh Do's notes Read-copy update. In Ottawa Linux Symposium, July 2001](http://pages.cs.wisc.edu/~thanhdo/qual-notes/sync/sync2-rcu.txt)

# Why do need RCU
For [scalable](http://en.wikipedia.org/wiki/Scalability) mutual exclusion.
scale有两个词源涵义Proto-Germanic原始日耳曼, 酒杯, 盘子, 称重的盘子, 后来演化成标量的意思.
scalar在数学上是标量, 在c语言指int float, Scalar processor标量计算机也是来源于此.
另一个出处是Latin拉丁文scandere, 有攀爬之意, 进而衍生出扩展之意.
From wikipedia,  A system whose performance improves after adding hardware, 
proportionally to the capacity added, is said to be a scalable system.
[因为rwlock, brlock在多核性能下降.需要个高性能的锁](https://www.ibm.com/developerworks/cn/linux/l-rcu/)
言下之意, 就是其他的mutual exclusion 机制不能很好的扩展, 需要RCU.
Read Copy Update HOWTO介绍了当初RCU开发的动机:
* Increase in cost of conventional locks
第一个原因也是最重要的, 最根本原因.
传统的锁the contended lock如spinlock实现上不断访问内存轮询锁当前状态, cpu和内存速度的*越来越*巨大差异;
一种新的锁机制减少访问内存,就成了强烈的需求.
* Cache benefits of lock-free reads
传统锁/tried-and-true 如spinlock一类的实现在多核下, 在出现锁竞争时会导致[cache line bouncing](http://www.quora.com/What-is-cache-line-bouncing-How-spinlock-may-trigger-this-frequently). 
3个cpu, A占有spinlock, 另外两个轮询尝试获取在test and set版本的
spinlock, 如果Bcpu 修改lock那么C cpu的d cache line 就会强制无效,
之后c 也修改lock, B的d cache line就无效了.之后B又来了, 就这样.
在不同cpu之间同步数据, 会耗费很多cpu 指令周期.   
像[x86的spinlock的实现用lock指令前缀锁住总线](https://www.ibm.com/developerworks/cn/linux/l-cn-spinlock/), 
其他cpu or dma就不能访问内存, 降低系统的性能, ibm这篇文章说P6之后的处理器减少这种危害.
* Avoiding complicated races
No deadlock, 减少了开发维护.

# How to use RCU
list rcu
[What is RCU? Part 2: Usage](https://lwn.net/Articles/263130/)
[RCU Usage In the Linux Kernel: One Decade Later](http://www2.rdrop.com/users/paulmck/techreports/RCUUsage.2013.02.24a.pdf)

# What is RCU
[What is RCU, Really?](http://www.rdrop.com/~paulmck/RCU/whatisRCU.html)
[What is RCU, Fundamentally?](https://lwn.net/Articles/262464/)
[RCU part 3: the RCU API](http://lwn.net/Articles/264090/)
RCU supports concurrency between a single updater and multiple readers!
## Design pattern
* Publish-Subscribe 
For updater before synchnization_rcu(); similar to softirq rcu
* Observer 
For updater to wait For Pre-Existing RCU Readers to Complete
Maintain Multiple Versions of Recently Updated Objects

# Positivism Implementions
## Classic RCU
Classic RCU requires that read-side critical sections obey the same rules 
obeyed by the critical sections of pure spinlocks: 
blocking or sleeping of any sort is strictly prohibited.
## Preemtible RCU
[The design of preemptible read-copy-update](http://lwn.net/Articles/253651/)
stage 的切换是由rcu_read_unlock 推动的!

## Tree RCU 
For hundreds and thounds CPU cores

#FAQ
* Difference with primitive and atomic
* Grace period is synchroinze_rcu or a softirq justment
* __rcu sparse will warn you if you access that pointer without the services of one of the variants of rcu_dereference().
# Below is how to use RCU!
# Reference
[RCU Usage In the Linux Kernel: One Decade Later](http://www2.rdrop.com/users/paulmck/techreports/RCUUsage.2013.02.24a.pdf)
# Introduction
本文内容并非我原创, 实际上是翻译加上个人理解Paul E. McKenney的paper.Credit to Paul and others.
本文不关心RCU的实现. 
本文的核心目标就是了解如何使用RCU.
所有使用RCU代码, 都可以分为4部分:
RCU read critical section: 就是rcu_read_lock()和rcu_read_unlock()之间的代码.
update: 这部分代码在synchronize_rcu/call_rcu之前执行. 比如给pointer = new_data;
synchronize: 这部分代码是等待调用synchronize/call_rcu时的所有Rcu read critical section 执行完.
free: 这部分是在上面synchronize/call_rcu确认所有对就数据引用的RCU read critical section都执行完了, 安全释放内存.
Paul的paper总结了5种用法, 并且附上了代码, 便于理解.
理解的过程实质上是用RCU的外衣再次理解这个5种方法!

# Wait for Completion
内核中有[完成量completion](https://lwn.net/Articles/23993/)的基础设施
上面的RCU read critical section和synchronize可以构成completion的语义.
为什么要用RCU而不是内核自身提供的completion函数呢?
Paul的文中举的例子实在NMI中要作这样的事情, 显然基于waitqueue的completion是毫无用处的.
所以这里的RCU, 应该叫做Completion RCU-version.

# Reference Counting
传统的引用计数就是往结构体里面内嵌一个atomic,加加减减.
但RCU实现的引用计数把实际工作转移到softirq-rcu部分去了.
Paul给了一个例子, 我认为是及其不合适的.因为他忽略了softirq部分随着smp的核数增多
同样也会带来性能的损耗,而不是almost zero in his paper.
引用计数和完成量的区别, 除了语义上的差异, 看起来没有太大的区分.FIXME.

# Type Safe Memory
 
# Publish-Subscribe

# Read-Write Lock Alternative 

