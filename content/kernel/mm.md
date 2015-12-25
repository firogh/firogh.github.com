---
layout: post
date: 2014-12-28
title: Linux memory management
category: kernel
---

# Reference
Computer memory#Management_of_memory
Memory management (operating systems)

# Contents
Logic gates: SRAM, DRAM
What is data/contrl/addr bus?
What problems we will meet in memory management?
Memory allocation and release. Bootmem and Buddy system is pretty good. Can we eliminate Bootmem?
How to distribute these memory to processes? Virtual memory.
How to translate linear address to physical address? Page table.
Exchange data between primary memory and second memory. Paging.


What are the pitfall of manuplate directly physcial memory

# page
[An introduction to compound pages](https://lwn.net/Articles/619514/)


# Memroy mangement
## GFP flags
__GFP_IO: allow disk IO
__GFP_FS: allow fs operations, depend on io.
more details in lwn, lkd

# Virtual memory

##Page table
mk_pte(page, pgprot)    pfn_pte(page_to_pfn(page), (pgprot))
原来低12位里面存了flag啊!
__pte(((phys_addr_t)page_nr << PAGE_SHIFT) | massage_pgprot(pgprot));
For vmalloc(), chechk here vmap_page_range_noflush()
For kmap(), check kmap_init()
[How to emulate the process of translate va to pa?](http://edsionte.com/techblog/archives/1966)

## copy_to_user
not in kernel?
read only?

## flush tlb after page table modification.

## Direct mapping area
high_memory (-128UL << 20)
__get_free_pages()
kmalloc()
kmem_cache_alloc()/slab

## Vmalloc
may sleep.
### Hwo Vmalloc works? 
Work in HIGHMEM and NORMALMEM
The skeleton is rbtree, root is global variable vmap_area_root.rb_node.
struct vm_struct likes struct address_space, functionlly;
struct vmap_area likes struct vm_area_struct.
map_vm_area 页表映射
the page in ZONE_NORMAL will not use directly mapping pfn address! It use VMALLOC address! 

### Vmalloc coherence with vfree() after vmalloc_sync_one
在进程的内核页目录中补上的是只是页目录项，而页表对所有进程来说是共用的，不管vfree()多大的内存块，在vmalloc()时新分配的页表不会被释放，当重新vmalloc()时，仍旧使用原来的页表。 page_fault使得进程的内核页目录项与swapper_pg_dir保持同步，swapper_pg_dir的内核页目录项一旦建立就不再被改变，需要改变的只是共享的页表而已。
deatils in vmalloc_sync_one() and vunmap_pte_range()
只释放pte

## Persistent Kernel Mappings
kmap(struct page *)
How kmap works? 
	Check kmap_init(), later!

## Temporay Mappings(Fixmaps)
kmap_atomic()

##Process virtual memory
* struct vm_area_struct: The intervals of legal address are called *memory areas* is permitted to access.
* struct address_space: To establish an association between the regions of the vm and the places where the related data are located.
i_mmap: how many processes opened this file.
https://lkml.org/lkml/2012/8/7/46
* sturct mm_struct: how many files(vm_area_struct) does this process opened.

* Memory mappings
syscall remap_file_pages Nolinear mappings is deprecated, since Linux 3.16

* link 
a virtual address and physical address. --page tale
a memory region of a process and its virtual page addresses. --vm_area_struct
a region of file(one physical) and all virtual address spaces(many virtual) into which the region is mapped. address_space->i_mmap.
a physical page and the processes that share the page(used in swap case) 


#Page faults
Page fault: If a process accesses a part of virtual address space not yet associated with a page in memory

* Segemnt fault
Segment fault: address is not in VMA, namely invalid address! Check bad_area() in x86.
https://www.kernel.org/doc/Documentation/x86/exception-tables.txt
Null pointer: probably, mostly catched in below 0x08048000.

* Demanding page 
vmalloc_fault(), 进程从用户态进入内核态不会引起CR3的改变.页表同步!
expand_stack()/handle_mm_fault()

* vmalloc fault

* Oops
kernel access invalid address, in __print_signal_info() task->sighand and  task->nsproxy is null.

#Physical memory
* NUMA/UMA pg_data_t: My PC is UMA, numatop, numastat, numactl
* ZONE(DMA/NORMAL/HIGHMEM) struct zone:
* struct page is the basic unit of kernel mm knowns as page frame.
	The goal of strcut page is to describe physical memory, not the data contained therein.
* The buddy system is per-zone struct free_area
* Physical address is connected to Virtual address by pfn = page - mem_map;
## page allocator
alloc_pages()
##Page/buffer cache
struct address_space->page_tree
##Page writeback
data synchronization, the flush threads, pdflush
##Page swap
The available RAM memory in a computer is never enough to meet user needs or to always satisfy memory-intensive applications.


#FAQ
* Where is Per-CPU variable?
static Per-CPU in .data(?) below high_memory!
runtime Per-CPU, it's GFP_KERNEL in pcpu_create_chunk()

* Memory mode
flat mem -> uma
discontig -> NUMA
sparse -> Hotplug + NUMA

* When does kernel alloc these struct pages in x86_64?
http://lwn.net/Articles/229670/
vmemmap silimar to memmap

* When kmap_atomic() BUG_ON effect?

* How cpu resolve address below high_memory?
Cpu-spicific!
x86 used page table to all address!
Mips cpu can be aware of this address!

* How to deal with useless page? : > /home/firo/bigdata

*pfmemalloc -- skb 表示申请了紧急内存!
page free

* compound pages
18fa11efc279c20af5eefff2bbe814ca067
