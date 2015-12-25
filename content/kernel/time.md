---
layout: post
date: 2015-09-22
title: Linux time subsystem
category: kernel
---
我写作的主要目的是理清基本概念与原理, 而非实现,细节这一类.

时间是人类发明出来的概念, Kant也认为时间是心智概念. 
一百年前的世界和现在有什么不同呢?这个提问揭示生活中时间最重要的用途, 
*标记*某中状态下的世界. 所谓的过去世界不过是世界的一个个连续状态
那么时空穿越, 就变成了保住项少龙这个肉身不变, 世界的其他部分完全
转变到过去的某一个状态. 而所谓现在世界就被抹杀了. 

	The future is already here – it's just not evenly distributed.
							— William Gibson
人想回到"过去"多半源于情感因素, 而想去"未来"则是因为"物质"的缘故.
想象下中国的未来50年, 很可能是现在美国or西欧现在的样子.虽然中西虽然处在
时间概念上相同的现在, 而物质世界确实"过去"和"未来"之差.

比如我们下周而要开会, 那么此时下周二就是有意识的规划未来, 时间依然是标记.
UTC/GMT
Based on caesium microwave atomic clock
Leap second


## Genus-differentia definition
interrupt, clock event,  clock source, tick, timer, timekeeping, gettimeofday.

## 功用定义
获取时间
timer
sleep ?
update sched info

## 发生定义/设计   etymology, history Operational definition
timekeeping/计时:时刻．
clock source: 时间之源, 表针之力.
clock event: 闹铃之力, 经过tick展现能力.
tick: timer的中断事件叫tick, tick device产生tick, 可以说tick决定了clock_event event_handler进而决定了, clock_event行为.
tickless: 
for timer: use HW timer one shot, set next.
for update time: in above HW timer, not good 
for sched: for priority distributed in time slice, use timer.
dynamic tick/no HZ:
No HZ in idle 
No HZ while only 1 process running for HPC.
tick devies 就是clock event包了层虎皮.
tick broadcast framework:  based on tick device
clock: 可记录时间, 表盘.利用timekeeping
timer: use clock. what about timer_list?
timer_list: do in softirq
* timekeeping aspect
onset: start_kernel -> timekeeping_init & time_init &(rest_init-> kernel_init-> \
kernel_init_freeable->do_basic_setup->do_initcalls--device_initcall(init_clocksource_sysfs))
nucleus:used by clock see init_posix_timers and update_wall_time and sys_time
coda:

* clock source aspect

* tick device layer aspect
per_cpu(tick_cpu_device, cpu)
onset: time_init->mach->init_time->...clockevents_register_device->tick_check_new_device
nucleus: a38x_timer_interrupt->a38x_clkevt.event_handler

* tick broadcast framework
onset:

* clock aspect
k_clock
onset:init_posix_timers & init_posix_cpu_timers
nucleus: 用户-> posix_clock->timepkeeping->clock_socurce

* timer aspect
onset: open_softirq(HRTIMER_SOFTIRQ, run_hrtimer_softirq); 
nucleus: common_timer_set
hrtimer_interrupt | (tick_handle_periodic-> tick_periodic-> update_process_times->run_local_timers->hrtimer_run_queues)->__run_hrtimer->timer.function = posix_timer_fn;(set in common_timer_set)



计算机概念
system clock CLOCK_REALTIME CLOCK_MONOTONIC
time.h time_t=tm=timeval=timespec calendar time  clock_t

clock event -> tick device-> handle function
clock event 相较于timer更为抽象, timer 是一种用途,timer 通过clock event来实现功能, 
clock event 通过timer来体现自己, timer面向使用者的onset, 
而clock event 则是偏向于nucleus and coda.
timer: 标记不能立即执行的变化.
clock source: 用一些整型抽象一个过程, 对于时间来说, 简直是完美的抽象.
* kernel requirement
linux的时间子系统要求硬件timer提供下面两种能力：
一是free running的counter，此外需要能够在指定的counter值上产生中断的能力。


