---
tags: kernel
title: Power management
date: 2015-02-27 15:46:12
category: kernel
---

# Reference
[wowotech](http://www.wowotech.net/pm_subsystem/generic_pm_architecture.html)
[The cpuidle subsystem](https://lwn.net/Articles/384146/)cpuidle—Do nothing, efficiently
[Hardware Specifications 9.4](http://www.marvell.com/embedded-processors/armada-xp/assets/HW_MV78460_OS.PDF)
[Functional Specifications 34] (http://www.marvell.com/embedded-processors/armada-xp/assets/ARMADA-XP-Functional-SpecDatasheet.pdf)
Documentation/power
Controlling Processor C-State Usage in Linux
[Chapter 11. Power Management](http://doc.opensuse.org/documentation/html/openSUSE_114/opensuse-tuning/cha.tuning.power.html)

C-states: idle core power state

# The structure of PM source codes
* Interface  
syscall /sys/power
* PM core
kernel/power
main.c suspend.c suspend_test.c console.c process.c
* Device PM
driver/base/power -- Power management interface, firo
power management 是针对device, bus, driver这部分的. 在suspend_devices_and_enter用到dpm_suspend_start.
driver/各种设备的驱动
driver/cpuidle --firo
* Platform PM
include/linux/suspend.h----定义platform dependent PM有关的操作函数集
就是他platform_suspend_ops
arch/xxx/mach-xxx/xxx.c or arch/xxx/plat-xxx/xxx.c----平台相关的电源管理操作
* CPU control

# Important data structure
* Platform PM
platform_suspend_ops, platform plat-* is equivalent to mach-* and microarchitecture.
plat-* is abustruct from mach-*1 and mach-*2 and so on.
mach-* is more closer to Board!
所以这个platform_suspend_ops, 是非常重要的, 他包含了所有BSP底层的内容.
竟然是用suspend_ops这个全局变量, 来承载所有platform相关的内容.
* Device PM
dev_pm_ops这个和上面是完全不同的两条调用路线,在suspend_devices_and_enter用到dpm_suspend_start.
# Steps of suspend
* Function steps
state_store->pm_suspend->enter_state->
{
	valid_state & suspend_prepare
	suspend_devices_and_enter-> {
		suspend_console & ftrace_stop & dpm_suspend_start
		suspend_enter->
		{
			disable_nonboot_cpus & arch_suspend_disable_irqs
		}
	}
}
* PM core
freeze userspace 
console
* Device PM 
suspend device
before and first part in suspend_enter

* Mass stuffs
CPU & IRQ disable
syscore
Device PM check wakeup pendings
middle part in suspend_enter

* Platform PM
last part in suspend_enter 
# Steps of resume


# Knowledge
console switch、process freeze、CPU hotplug、wakeup


# Introdution
电源管理都涉及到那些内容?
PMU MP and DEV, CPU cores, L2 cache, Coherency Fabric, Devices
Core Power Modes: run, idle/WFI/WFE/stadnby, deep idle/power down
* WFI: core power down except snoop and interrupt cache working
disable clocks of the CPU 
snoop other Cores and io agent.
only snoop block are temporarily woken up and the block back to WFI mode after complete snoop.
can recognize interrupt.
* Power down: core power down, l1 flush, cache not mantained/snoop stoped, but l2 & fabric 
can not recognize interrupt, recovery is fully depended on MP_PMU


In hardware layer, we need [PMU](https://en.wikipedia.org/wiki/Power_Management_Unit) to complete power management.
What is the relation of PMU and CPU?
armadaxp的PMU与CPU物理上与逻辑上是独立的.
The Power Management functions are provided by two power manager units:
The Device Power Management Unit (DEV_PMU)
The Multiprocessor Power Management Service Unit (MP_PMU).
PMU省电模式:




# kernel cpuidle subsystem
## Steps of cpuilde
* interface
kernel sched and sysfs

* cpuidle core
cpuidle.c、driver.c、governor.c、sysfs.c。
抽象出cpuidle device、cpuidle driver、cpuidle governor三个实体
一个core对应一个cpuidle device drivers/cpuidle/cpuidle.c.
device 和driver隔离, 通过全局变量联系 ifndef CONFIG_CPU_IDLE_MULTIPLE_DRIVERS
管理cpuidle driver 和governor
上层sched模块 和sysfs 提供接口 

* cpuidle governors

* cpuidle drivers
drivers/cpudile/cpuidle-xxx.c or 
arch/arm/plat-armada/cpuidle.c
如何进入idle状态
什么条件下会退出


## Source code layout
* drivers/cpuidle
include/linux/cpuidle.h
cpuidle core、cpuidle governors和cpuidle drivers三个模块
* kernel\sched\idle.c
kernel sched中的cpuidle entry

## Important data structre
struct cpuidle_driver
struct cpuidle_state
struct cpuidle_device used by ladder or menu



device_initcall -> armadaxp_init_cpuidle -> cpuidle_register_driver  cpuidle_register_device
cpu_idle -> cpuidle_idle_call



