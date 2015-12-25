---
tags: cs
title: Computer architecture
date: 2015-10-13 14:40:13
category: cs
---
##Reference
Legacy 常用寄存器，常见指令 实模式和保护模式 分段和分页机制 TSS和任务管理 中断机制 时钟机制 高速缓存
《模拟电子技术基础 童诗白 第四版》第一章前半部分
Structured Computer Organization 6th Edition
Digital Design and Computer Architecture 2nd Edition
Computer Organization and Design 5th Edition
Write Great Code: Volume 1: Understanding the Machine
See MIPS run
Intel 64 and IA-32 architectures software developers manual combined volumes 3A, 3B, and 3C: System programming guide	
Microelectronics

# Computer architecture
cpu
memory
I/O

# Contents
I/O IC
Assembly language
Interrupt
ISA
Machine language
Interpreting Machine Language Code
Microarchitecture
Digital Logic circuit(Logic gates)
ALU
Computer memory
Register/Flip-flop
Cache/SRAM
DRAM/capacitor
Analog Circuit
BJT FET Didoe

## ISA
model: 1570s, "likeness made to scale; architect's set of designs," from Middle French modelle 
Semantics in computer science: In programming language theory, semantics is the field concerned 
with the rigorous mathematical study of the meaning of programming languages.
Language primitive
ISA: memory model, registers, data types, instructions, word size(?).
Memory model: unit of address resolution, word, aligment, address space, addressing mode, memory barrier/memory order primitive's semantics.
The interface between hardware and software.
Arm v7
word = instruction
vocabulary = instruction set
## Microarchitecture
datapath: memories, registers, ALUs, multiplexers.
control unit/decoder
Microcode
How to gurante that every control signal arrive the destnations before the data that will also pass through the destination?
What is the pipeline?
architectural state (the program counter and registers
# Logic circuit
Sequential logic
Combinational logic
## Computer memory
flip-flops & latches, register files(?), memorys
What is NOR gates?
What is the implement of gate?

data type
instructions: 算术, 控制, 数据传送
register file
addressing modes(include memory mode)
interrupt and exception handling, 
external I/O

#x86 Interrupt
If interrupt occured in user mode, then cpu will context swith for potential reschedule.
The Interrupt Descriptor Table (IDT) is a data structure used by the x86 architecture to implement an interrupt vector table. 
##Hardware interrupts
are used by devices to communicate that they require attention from the operating system.
asynchronus
more details in init_IRQ() or set_irq() in driver.
##software interrupt 
synchronus
more details in trap_init().
* exception or trap
is caused either by an exceptional condition in the processor itself, 
divide zero painc?
* special instruction, for example INT 0x80
or a special instruction in the instruction set which causes an interrupt when it is executed.
## Microarchitecture.
* pipeline, 为什么流水工作效率高?复用? 一整条流水线对应一种指令:运算指令(算术 控制) or 数据传输指令(load sotre)对于RISC
IF阶段也可以访问存储器,要和load sotre分开. IF是对指令的读取. load store是对数据, 这是icache和dcache产生的原因, FIXME.
结构冒险说的就是i/dcache没分开, IF 和MEM冲突了.
数据冒险, 指令件参数依赖造成的问题.
控制冒险, 流水越深,危害越大.软件排空流水的方法是加nop, x86硬件排空,分支预测减少危害. 
* branch prediction.
dsp用条件执行替代跳转.
* out-of-order
乱序执行相对于顺序执行.
无关指令可以任意执行.
* superscalar
instruction cycle, 什么是cycle.
* data level parallelism
* thread level parallelism
超线程, 同步问题.
* cache
cache miss 
住要看cache, 乱序, 还有lock对执行的影响.
temporal locality: for icache
spatial locality: array
第一次访问内存里arry[0]很慢, 可被load cacheline 后其他数组成员访问延时就飞了.
##Cache
a cache is a component that stores data so future requests for that data can be served faster; 
the data stored in a cache might be the results of an earlier computation, 
or the duplicates of data stored elsewhere. 
###CPU cache
A CPU cache is a cache used by the central processing unit (CPU) of a computer 
to reduce the average time to access data from the main memory.
#### Associativity - CPU Cache algorithms
* Two-way set associative cache
#### Cache coherence
Cache coherence is the consistency of shared resource data that ends up stored in multiple local caches.
Cache conerence is mantained by mostly archtecure like cpu
* MSI
####Cache type
* Instructin cache
* Data cache
* TLB - Translation lookaside buffer
###GPU cache
###Disk cache
###Web cache
## FIXME concepts
code: 手电筒 ->　莫尔斯码
Numeral system: 手指头　-> 二进制
logic: 三段论 -> bool -> 电路开关 亚里士多德 布尔 香农
A Symbolic Analysis of Relay and Switching Circuits
The Mathematical Theory of Communication
触发器 英国射电物理学家 William Henry Eccles
晶体管
# PHY -- The physical layer
##  Introductions
Fromwikipedia:
> In the seven-layer OSI model of computer networking, the physical layer or layer 1 is the first (lowest) layer.[1] 
> The implementation of this layer is often termed PHY.
> The physical layer consists of the basic networking hardware transmission technologies of a network.
https://www.kernel.org/doc/Documentation/networking/phy.txt
PCS
PMD

# I/O IC 
serial communication: UART(16550) + RS-232 
parallel communication: SCSI, ISA, ATA, PCI, FSB

