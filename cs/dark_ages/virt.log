---
layout: post
date: 2015-06-17
title: Linux virtualization
category: cs
---

# Reference
[KVM源代码分析3:CPU虚拟化](http://www.oenhan.com/kvm-src-3-cpu)
[VIRTUALIZATION TUNING AND OPTIMIZATION GUIDE][1]
[1]: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/7/html-single/Virtualization_Tuning_and_Optimization_Guide/

本文着重探讨下kvm的实现, 11年在snia的实习的时候, 接触过一点.
已经过去将近4年了,  当初看到qemu的代码, 被吓到了, 我靠, 这代码量赶上内核了!
现在kvm核心就1069行当前kernel 4.1 rc7, qemu虽然赶不上kernel但是也不少1144756.
看过指环王的, 都知道神奇中土大陆是托尔金虚构的魔幻世界,那里活着阿拉贡和
他的朋友们, 以及他们精彩的历程.人的想象力, 如此神奇, 他能使人超越时间与
空间去体验到不同生命历程.
今天, 我们来探索一下[虚拟化](https://en.wikipedia.org/wiki/Virtualization)
如wikipedia所说, 虚拟化始于上个世纪60年代, 这是指的计算机领域内的.
显然, 我们要把眼光放得更远, 因为我们想洞见一些深层次的内容.
法国哲学家福柯一生都在从事"知识考古学", 而我也喜欢在研究问题时, 从语言学特别是
词源角度开始. virtual这个词, 最早来自于中世纪拉丁文virtualis, 涵义是
influencing by physical virtues or capabilities, 
effective with respect to inherent natural qualities
受某种实在的长处能力影响, 源自本质的效用.
这里从两种角度理解virtual, 受影响者, 影响者本身.
所以这里virtual的真实涵义应该是延续某种内在的本质并施加于外物.
这个英文单词的汉语翻译是很有趣的, 摘自金山词霸:

	virtual 音节划分：vir▪tual
	英 [ˈvɜ:tʃuəl] 美 [ˈvɜ:rtʃuəl]
	adj.实质上的，事实上的；（计算机）虚拟的；<物>有效的，虚像的；（粒子）实际存在的
	虚；虚拟艺术；虚拟的；模拟帐户
这个词汇简直是英语翻译界的笑话, 一会实一会虚, 非常让人困惑!
话说回来, 这个词的翻译确实有难度.我翻译的话会叫质延.
很多事情, 在我们出生前就决定了, 我是89年9月出生的.
我们搞懂了所谓"虚拟化"的实质, 之后就可以继续探索了.
如wikipedia, 所说计算机领域的虚拟化更多的是指hardware virtualization.
没错就是延伸硬件的特质, 准确说是在软件层继续延伸硬件的特质.
好, 首先, 我们要搞懂硬件都有那些特质? 硬件有很多, 在计算机领域指什么呢?
没错, CPU, 硬盘, 电路板, 内存, 鼠标键盘等等. 那么他们有什么特质呢?
所谓的特质就是特别的性质, 一种属性, 哲学上, 称之为事物对外物的作用的一种表现.
那么CPU的最大特质就是指令处理, 我想这也是虚拟化的核心吧!
我们硬件虚拟化延伸的终点是让另一个操作系统运行于当前操作系统提供的硬件特质延伸之上. 
终于进入正文了. 现在我们自己来设计一套虚拟化基础架构, 让一个简单的kernel运行在当前的fedora
之上.

# Intel VT-x
VMCS manages transitons into and out of VMX non-root operation(VM entries and VM exits) as well as processor behavior
in VMX non-root operation.
ORGANIZATION OF VMCS DATA
The VMCS data are organized into six logical groups:
Guest-state area. Processor state is saved into the guest-state area on VM exits and loaded from there on VM entries.
Host-state area. Processor state is loaded from the host-state area on VM exits.
VM-exit control fields. These fields control VM exits.
VM-execution control fields. These fields control processor behavior in VMX non-root operation. They determine in part the causes of VM exits.
VM-entry control fields. These fields control VM entries.
VM-exit information fields. These fields receive information on VM exits and describe the cause and the nature of VM exits. On some processors, these fields are read-only. 3

VMX NON-ROOT OPERATION
INSTRUCTIONS THAT CAUSE VM EXITS
Relative Priority of Faults and VM Exits
Instructions That Cause VM Exits Unconditionally
Instructions That Cause VM Exits Conditionally
OTHER CAUSES OF VM EXITS

CHANGES TO INSTRUCTION BEHAVIOR IN VMX NON-ROOT OPERATION

VM ENTRIES

VM EXITS

VMX SUPPORT FOR ADDRESS TRANSLATION

APIC VIRTUALIZATION AND VIRTUAL INTERRUPTS
# 代码实现
* KVM architecture
* Kernel running flow
  svm.ko -> svm_init(svm.c) -> kvm_init(kvm_main.c) ->
* File
  svm.c vmx.c kvm_main.c kvm_svm.h
* Function
* Struct
  kvm_x86_ops x86.h
  vcpu_svm    kvm_svm.h

# qemu-system-ppc
-mem-path:
