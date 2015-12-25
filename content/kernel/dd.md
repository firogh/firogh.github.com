---
tags: kernel
title: Linux device driver
date: 2015-02-27 15:46:12
category: kernel
---
#Bibilography
Driver porting: Device model overview: http://lwn.net/Articles/31185/
/Documentation/driver-model
The Linux Kernel Driver Model

# Contents
Why abstruction? 

# Device tree
* What infomation was included in dtb
HW platform识别，runtime parameter传递以及硬件设备描述
for platform device init see arch/powerpc/platforms/52xx/media5200.c ppc_init
mpc52xx_declare_of_platform_devices
* How does the device tree subsystem read and parse the dts file?


#Why DDM?
The device model was originally intended to make power management tasks easier 

* I/o Architecture
1 Expansion hardware. 
Bus includes internal bus/system bus(PCI,ISA,SBus,I2C) and external bus(ieee1394/firewire,USB) and both(SCSI).
2 Communication with peripherals
I/O ports
I/O memory mapping
Interrupts
hrough the maintenance of a representation of the host system's hardware structure.

# linux 设备驱动模型
分为三层：
从结构上看，每一层都有对应的数据结构。
1 设备驱动层，驱动开发工程师，也就是码农们，设计的akashadevice  akashadriver 结构体。
2 驱动程序核心层 ，是一个通用层，内核中的标准结构，device device_driver bus_type.
3 Kobject核心层，也是一个通用层， kobject kobj_type kset 组成。
其实下面还有一个sys文件系统层。

从操作上看，也是分为三类，初始化 注册 注销
1 初始化，是局限于每一层的。
2 注册和注销就是贯穿三层的。

，我们知道其实总线就是铁轨，而各种设备就是北京东站，大连站。
我们人呢，就是铁轨上跑的数据。driver还用说就是火车了呗。
Bus总线这类设备比较特殊,需要内核对总线支持，即总线的核心层；
另一部分在外设的驱动程序中，负责把驱动挂接到总线，如pci_driver, usb_driver, i2c_driver。记住，这是说的总线的驱动。和挂接在总线上的设备驱动不同。
就好像open系统调用和驱动程序中file_operations中xxx_open的关系一样。
作为中间传输用途的总线设备的驱动是在内核当中，已经写好了，不用像各种总线上的设备一样，再写什么专门服务pci，i2c，spi等
总线设备的驱动，这里感谢下内核，又轻松了不少。pci之类的总线的，驱动就是match probe remove的函数，
不用专门封装成一个结构体，像我们遇到的block char设备，他们才是真中的外设。总之总线要单独看。
作为一个外设的驱动是很不容易除了要完成外设本身功能的部分还要处理好和总线的关系。对于pci总线上设备驱动就是由pci_driver来调和的。

kobject核心层的注册有点小技巧，他是先在作为参数kobj结构体中指定了，这个结构体属于哪里kset，之后在注册函数里，在kset里把kobj链入。
PS：设备注册和驱动注册是分开的。当设备注册后通过udev开始寻找驱动的过程。驱动是通过pci_device_id和相应设备建立关联的，不是major 和minor设备号。
可别高乱了。



# Sys 文件系统
bus，总线树跟踪每个总线上的每个设备。实质是保存驱动，设备在device中，用链接指明。
class, 类树按功能分类设备。
device，干货，其他的目录都指向这里。

# Kobject
清0，init，set name，get，put
在sys中是dir，attr 是文件

# kobj_type
release(),

# Kset
添加kobject到kset（指针，add）

# Bus, device, driver.


#devres
* devm_kfree
[devm_kfree does not need for fail case of probe function and for remove function.](http://lists.freedesktop.org/archives/dri-devel/2013-May/038943.html)
__device_release_driver
really_probe

* a must use devm_kfree Julia Lawall
 The function at91_dt_node_to_map is ultimately called by the function
> pinctrl_get, which is an exported function.  Since it is possible that this
> function is not called from within a probe function, for safety, the kfree
> is converted to a devm_kfree, to both free the data and remove it from the
> device in a failure situation.
*for noop devm_kmalloc_release 
devm_kmalloc()->dr = alloc_dr(devm_kmalloc_release, size, gfp);
see release_nodes() ->kfree(dr);
