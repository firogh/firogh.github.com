---
tags: kernel
title: Console and TTY
date: 2015-12-05 14:06:29
category: kernel
---
# About the design
Why dose we use /dev/xxx to represent the "tty" device?
> The whole point with "everything is a file" is not that you have some
> random filename (indeed, sockets and pipes show that "file" and "filename"
> have nothing to do with each other), *but the fact that you can use common*
> *tools to operate on different things*. -- Linus
So we got the key point!
In order to use the common tools, file ops and vfs layer, the tty device is "abstructed" to
files by us. Addnationaly, we must assurance that is the files is *different*. What does
the word "dirrerent" means is not that you have some random different filename, but the
fact that you can access the real device through the different.
> From wikipedia:
> In mathematics, injections, surjections and bijections are classes of functions distinguished 
> by the manner in which arguments (input expressions from the domain) and images 
> (output expressions from the codomain) are related or mapped to each other.
I got an insight that abstruction is a non-injective, right?
But non-injective may not be a anstruction.
An asbstruction should come from manipulating different objects.
Non-injective, 多对一; Multiplex, 一对多.
So we can use mathematical language to describe the linux subsystem.
From real life device to a filesystem file.
* Abstruction: Non-injective, Multiplex(not partial function).
* Jection num: injective, or non-jective, or multiplex; Jection level: domain set and codoain set!
Abstruction -> Control abstruction and data abstruction -> Abstruction layer

软件设计的两个主要目的.
1. 简化, 易用,易操作, 如computer architecture, tty subsystem. 抽象.
2. Separation of concerns, modular(abstruciton in layer), 笛卡尔划分的思想
# tty_struct->disc_data
tty_init_dev->
{
	initialize_tty_struct->tty_ldisc_init
	tty_ldisc_setup->tty_ldisc_open->n_tty_open-> tty->disc_data = ldata; 
}
sys_vhangup->tty_vhangup_self->__tty_hangup->tty_ldisc_hangup->tty_ldisc_reinit
vfs_write->redirected_tty_write->tty_write->n_tty_write->process_output
# early_con
EARLYCON_DECLARE(uart8250, early_serial8250_setup); EARLYCON_DECLARE(uart, early_serial8250_setup);
setup_earlycon->
{
	parse_options-> 
	{
		parse earlycon_device->port->uartclk and 
		earlycon_device->baud
	}
	setup = early_serial8250_setup-> init_port(device);
	register_console(early_console_dev.con)
}
# earlyprintk
early_param("earlyprintk", setup_early_printk)->
{
	early_serial_init->
	{


	}
	early_console_register(&early_serial_console, keep);
}
# cpu hotplug
* onset
static struct smp_hotplug_thread softirq_threads {
.thread_fn              = run_ksoftirqd
};
early_initcall(spawn_ksoftirqd)->mpboot_register_percpu_thread(&softirq_threads)
* nucles onset
suspend_enter->enable_nonboot_cpus->_cpu_up->smpboot_create_threads->__smpboot_create_thread(&hotplug_threads)->
kthread_create_on_cpu(smpboot_thread_fn-> ht->thread_fn(td->cpu)= run_ksoftirqd->__do_softirq->h->action(h) = run_timer_softirq->
__run_timers->call_timer_fn)

vt 就是个tty也有tty_driver 叫console_driver.
# Reference

# Contents
[What is the difference between shell, console, and terminal?](http://superuser.com/questions/144666/what-is-the-difference-between-shell-console-and-terminal)
What does console do in kernel or u-boot?
Linux console?
Computer terminal: keyboard + dispaly
Terminal emulator:

# get a glance on u-boot
start.S:board_init_r
init_sequence_f: ->init_baud_rate; serial_init; console_init_f
serial_init ->&eserial1_device->start=eserial##port##_init->NS16550_init: UART divisor init.
Firstly, the struct of serial define some input and output funtion.
It's basic function of serial, put or get!
console_init_f: just gd->have_console = 1;
init_sequence_r: stdio_init_tables,initr_serial, stdio_add_devices, console_init_r,
initr_serial: just register &eserial1_device to serial_devices
stdio_add_devices: drv_system_init, serial_stdio_init 
drv_system_init: register default serial dev to devs.list.
serial_stdio_init: register &eserial1_device to devs.list. Duplicate, but serial dev "eserial0"  and system "serial".
console_init_r: console_doenv ->console_setfile:stdio_devices[file(0/1/2)] = dev; actually, dev is "serial", but they may be KBD!
看来console的真正作用就是在serial和kbd中选择一个, 可能多选iomux? 
main_loop:cli_loop: getc!

# what is platform device or driver?
# A reallife serial8250
drivers/tty/serial/8250/8250_boca.c:plat_serial8250_port
module_init-> 
{
	serial8250_init->serial8250_isa_init_ports->serial8250_ports[i].port.ops = &serial8250_pops; //insidious
	boca_init->platform_device_register(&boca_device); //register platform device and data.
}
* uart_port->tty_port

serial8250_probe(plat_serial8250_port)->serial8250_register_8250_port(uart_8250_port)->
uart_add_one_port(&serial8250_reg, &uart->port=uart_port)->
{
	
	uart_configure_port->
	{
		port->ops->config_port(port, flags)=serial8250_config_port
		if post is console and not registered; register!
		we know that uport->cons = drv->cons; what is the relation to registering about up->cons.
		why we register it? where does drv->cons come from?
		//这个uart_driver drv就是serial8250_reg, 我们也就知道了
		// console是设备的一种天生能力. 能否使用, 只关乎你是否想用, 就是配置相关的config SERIAL8250_CONSOLE
		// con_driver is the backends, vga, dummy/serial?, fb. 
	}
	tty_port_register_device_attr->
	{
		//tty_driver  tty_port
		tty_port_link_device
		tty_register_device_attr
	}
}
# Simple conceptions
[You must read this -> The TTY demystified](http://www.linusakesson.net/programming/tty/)
System consoles are generalized to computer terminals, which are abstracted respectively by virtual consoles and terminal emulators.
		UART
		Line displine + TTY(pts, dummy/serial, kbd+vga/fb)
* System console
Virtual termial, Terminal emulator/telnet/ssh -> pts ,Physical terminal 
You need at least one virtual terminal device in order to make use of your keyboard and monitor.
VT combine keyboard and display see con_init
con_init init a virtual terminal like gnome-terminal but in kernel.
con_init mainly init display.
vty_init mainly init kbd
They all can be system console.(Exception pts??), if you enable it.
Console is the entry of linux system.
* Console driver -- banckends of  console
struct console 指定了console的结构.
* Console config
If I disable CONFIG_SERIAL8250_CONSOLE(enable vt console), then no booting log and I can not login system.
If I disable CONFIG_VT_CONSOLE(enable serail console, /dev/console point to ttyS0 see show_cons_active), no booting log but I can lgin system.
How to explain this phenomena?
From show_cons_active, we know /dev/console should come from console_drivers.
/dev/console is really the pointer.
Now, let's inspect open /dev/console.
* Open /dev/console
Fisrt, it's the very last place of booting kernel.
start_kernel->rest_init->kernel_init->kernel_init_freeable->sys_open((const char __user *) "/dev/console", O_RDWR, 0)->...->
console_fops->tty_open->
{

	// Sitiuations: disbale VT CONSOLE byt enable SERIAL8250_CONSOLE.
	// This function work only for /dev/tty
	// ls -l /dev/tty
	// crw-rw-rw- 1 root tty 5, 0 Dec 10 10:24 /dev/tty
	tty_open_current_tty->
	{
		/dev/console is 5:1, just return NULL
	}
	//This index should be Ctrl + Alt + Fn??
	// tty_struct is corresponding virtual console, or just console??
	// lookup tty_driver. It looks like lookup a inode, right?
	tty_lookup_driver->
	{
		// Find a tty_driver by device() in console_drivers.
		// So we know, got a console, then got a tty_driver, right?
		// Where do the components of console_drivers come form?
		// At present, we should have only serial8250_console because 
		// console vt_console_driver is disabled by us! But tty_driver console_driver
		// still do exist!So console and tty is really separated!
		// When we init vt, we get the tty_driver console_driver with con_ops type of tty_operations.
		// tty_drivers have the same major with /dev/tty0!
		// char_dev -> tty_driver int vty_init, not disable.
		// vt console---^
		console_device->c->device(c, index)
		// What about serial8250_console?
		// Where is the tty_driver of serial8250_console?
		// console->data = uart_driver->tty_driver.
		// We got another scene : uart->tty
		// serial8250 console->data-^
		// serial's tty_driver alloced in serial8250_init with uart_ops.
		// vt's tty_driver alloced in vty_init.
		// We summarize these:
		// uart_driver serial8250_reg <->  vc dev or /dev/tty*
		// serial tty driver <-> vt tty driver 
		// fs:vty_init <-> module:serial8250_init
		// tty driver ops con_ops <-> uart_ops
		// vt use major to connect tty and  vc dev
		// serial use major and -> to connect tty and uart_driver
		// It seems that uart and tty has a strong relationship, yet vt.
		// Ok... we got tty driver.
		// If we disable vt console, then here is the serial8250 tty_driver.
	}
	// Lookup for tty_struct
	tty_driver_lookup_tty ->
	{
		//tty_struct is alloced in init function alloc_tty_driver.
		// ttys, termios, ports, cdevs.
		// ttys was used by tty_standard_install then tty_driver_install_tty 
		// then tty_init_dev then ok we return to tty open. So this is the start place.

		So we know tty_driver->ttys[*] must be NULL.
	}
	tty_init_dev->
	{
		// tty_driver likes a process, ttys like the files, tty_struct like a file!
		// So we know a tty_struct is a tty file.
		// tty_driver much like a inode
		//So tty_struct->ops = tty_driver->ops = & uart_ops
		alloc_tty_struct->tty->ops = driver->ops;
		tty_driver_install_tty(driver, tty_struct)-> tty_standard_install->driver->ttys[tty->index] = tty;
	}
	// Ok... We got a tty_struct.
	// Add /dev/console to tty_struct->tty_files
	tty_add_file
	// At present, what have we done?
	// open(/dev/console)->console_drivers->console->tty_driver->tty_struct, right?

	tty->ops->open(tty, filp)->//ops = &uart_ops
	{
		uart_ops->open = uart_open->
		{
			uart-> tty struct
			struct uart_driver *drv = (struct uart_driver *)tty->driver->driver_state;
			struct uart_state *state = drv->state + line; //uart_state
			tty->driver_data = state;
		}
	}
}
// At present, we understand the flow of open /dev/console to serial console
# What about opening /dev/console to vt console
sys_open(/dev/console)-> ... tty_open ->
{
	tty_lookup_driver-> get tty_driver=console_driver,
	Through the name console_driver, we know, vt tty_driver is the defaut driver of console!
	// 同时, 我们也应该知道所谓的kernel的system console 是你只要enable了先关的config CONFIG_VT_CONSOLE and CONFIG_SERIAL_8250_CONSOLE.
	// system console就有了, printk也就有了归处. In other words, you registered, you got printed
	// In theory, you should be able to input something, like sysrq, I maybe test tomorrow.
	// 那么在kernel_init中sys_open又是什么鬼呢?
	// dev/tty 是专门真对进程的, 就是进程之前打开了一个tty就存在singnal里, 这个dev/tty就是取出来的.

}

# 总结下, 打开/dev/console, 会从console_drivers, 最终到达tty_driver.
# 这和/dev/tty* /dev/ttyS* 从tty_drivers, 差不多.
// 这么输出为什么不会打窜了?
// How ctrl alt Fn work?
// echo xxx /dev/tty in serial tty_lookup_driver
// 另一个问题, serial 的terminal?
# The perspective
/dev/*
vfs
chrdev
tty_fops--------------------------->tty core
	ld_ops --------------------> tty line discipline(for read, write)
tty_driver con_ops/uart_ops--------> tty driver and tty_operations
HW
There are three different types of tty drivers: console, serial port, and pty.
serial8250_default_handle_irq
UART console 
			  |---- Virtual terminal ----------------- VT console
			  |					| -- VT console
			  |---- 
		Terminal--|
			  |---- 

# What about console?
* early_con
start_kernel or setup_arch(arm)->parse_early_param->do_early_param->p->setup_func()= setup_early_printk->register_console

* vga_con
start_kerenl-> 
{
	// All about vga console
	set_arch-> conswitchp = &vga_con; or conswitchp = &dummy_con; 
	console_init->
	{
		con_init->
		{
			// vc->vc_sw->con_putcs is DUMMY
			//内存映射64KB or 32KB的VGA区域. 启动VGA
			conswitchp->con_startup = vgacon_startup ->vga_vram_base = VGA_MAP_MEM(vga_vram_base, vga_vram_size);
			con_driver_map[0~MAX_NR_CONSOLES] = conswitchp; //空间换时间
			// 核心内容!
			for (currcons = 0; currcons < MIN_NR_CONSOLES; currcons++) {
				// 给vc_cons[currcons].d分配内存
				vc_cons[currcons].d = vc = kzalloc(sizeof(struct vc_data), GFP_NOWAIT);
				INIT_WORK(&vc_cons[currcons].SAK_work, vc_SAK);
				//初始化vc_cons[currcons].d
				tty_port_init(&vc->port);
				// 继续初始化, 主要是确定screenbuf size
				visual_init(vc, currcons, 1);
				// 给vc_screenbuf分配内存
				vc->vc_screenbuf = kzalloc(vc->vc_screenbuf_size, GFP_NOWAIT);
				vc_init(vc, vc->vc_rows, vc->vc_cols,
					currcons || !vc->vc_sw->con_save_screen);
			}

			[    0.000000] Console: colour VGA+ 80x25
			register_console(&vt_console_driver);//vt_console can use vgacon writing.
		}
		serial8250_console_init->register_console(&serial8250_console) to console_drivers; exclusive_console.
		[    0.000000] console [tty0] enabled
	}
	module_init(serial8250_init);??
}
* fbcon
register_framebuffer-> do_take_over_console ->
{
	do_register_con_driver->csw->con_startup();registered_con_driver
	do_bind_con_driver ->
	{
		[    3.882220] Console: switching to colour dummy device 80x25
		[    4.720732] Console: switching to colour frame buffer device 170x48
	}
}
## VGA text console printk & write
* kernel space
printk-> ...->log_buf
* userspace for ttyN
tty_fops->write=tty_write-> tty_ldisc_N_TTY->write=n_tty_write-> tty_driver->ops=con_ops->write=con_write->do_con_write
* agent
console_drivers->vt_console_driver->serial8250_console->NULL
console_unlock->..->__call_console_drivers-> console_drivers->write = vt_console_print 
{
	//保存到screen buf, vga_con也什么不做啊.
	scr_writew((vc->vc_attr << 8) + c, (unsigned short *)vc->vc_pos);

}
# What about tty
* onset
console_init->tty_ldisc_begin->tty_register_ldisc(N_TTY, &tty_ldisc_N_TTY);
N_TTY:[Serial Drivers by Alessandro Rubini](http://www.linux.it/~rubini/docs/serial/serial.html)
fs_initcall:chr_dev_init->drivers/tty/tty_io.c: tty_init->
{
	
	/* /dev/tty0 = /dev/console console_ops */
	/* /dev/tty = the /dev/ttyN whererun echo /dev/tty tty_ops */

	//"/dev/tty",
	cdev_init(&tty_cdev, &tty_fops);
	"/dev/console"
	cdev_init(&console_cdev, &console_fops);
	vty_init->
	{
		//"dev/tty0"	
		cdev_init(&vc0_cdev, console_fops);	
		//"/dev/ttyN"
		tty_register_driver->
		{
			// What does tty_register_driver do ?
			// Alloc and register chr dev region.
			// Add cdev with tty_ops and above region.
			// Register tty device
			// Why do we register tty devices?
			// These devices must be used in some place.
			// After registering itself, the driver registers the devices it controls through the tty_register_device function. 
			// 原来是把major 和minor做成dev_t放到driver->cdevs[index].dev里面了.
			// 也就是说driver->cdevs[index]就是tty driver控制的device啊, 怪不得cdev_init(&driver->cdevs[index], &tty_fops);
			// 那么看来用到的时候就是open了, 竟然没有不过有个tty_get_device用了tty_class
			// tty_register_device_attr->device_register->device_add->klist_add_tail(&dev->knode_class,&dev->class->p->klist_devices)
			// 果然是在open tty->dev = tty_get_device(tty);在alloc_tty_struct
			// 不知道这个tty->dev在哪里用, 不管他了.
			tty_register_device(_attr) ->tty_cdev_add-> cdev_init(&driver->cdevs[index], &tty_fops);
		}
		kbd_init
	}
}
device_init:serial8250_init-> 
{
	// In this function we decide "/dev/ttyS*"
	// dmesg |grep Serial
	// [    0.696341] Serial: 8250/16550 driver, 32 ports, IRQ sharing enabled
	// serial8250.c -> tty_io.c
	serial8250_reg.nr = UART_NR;
	ret = uart_register_driver(&serial8250_reg);
	tty_driver set to uart_driver by uart_register_driver ->
	{
		drv->state = kzalloc		//uart_state
		normal->driver_state    = drv; //args struct uart_driver *drv = &serial8250_reg
		tty_set_operations(normal, &uart_ops); 
		struct tty_port *port = &state->port
		tty_port_init(port);
		port->ops = &uart_port_ops; //tty_port
		// We register "/dev/ttyS*" files here.
		static struct uart_driver serial8250_reg = {
			.owner                  = THIS_MODULE,
			.driver_name            = "serial",
			.dev_name               = "ttyS",
			.major                  = TTY_MAJOR,
			.minor                  = 64,
			.cons                   = SERIAL8250_CONSOLE,
		};
		retval = tty_register_driver(normal); -> register_chrdev_region(dev, driver->num, driver->name) //32, ttyS?*? should be tty_ops

	}
	serial8250_register_ports(&serial8250_reg, &serial8250_isa_devs->dev);
}
* nucleus
tty_write->ld->ops->write=n_tty_write->(tty_struct tty->ops->write)=uart_write->
{
	struct uart_state *state = tty->driver_data;
	port = state->uart_port;
	circ = &state->xmit;
	memcpy(circ->buf + circ->head, buf, c);
	uart_start->__uart_start->(uart_port->ops->start_tx(port)); //&uart_port_ops ?? uart_ops??
}

# What about Pseudoterminal
/dev/ptmx is the "pseudo-terminal master multiplexer". from wikipedia
static struct tty_driver *ptm_driver;
static struct tty_driver *pts_driver;
module_init(pty_init)->unix98_pty_init-> 
{
	tty_set_operations(ptm_driver, &ptm_unix98_ops);
	tty_register_driver(ptm_driver)
	tty_set_operations(pts_driver, &pty_unix98_ops);
	tty_register_driver(pts_driver)
	ptmx_fops = tty_fops;
	ptmx_fops.open = ptmx_open;
	cdev_init(&ptmx_cdev, &ptmx_fops);
}

* How to use ptmx?

# tty drivers
* cat /proc/tty/drivers 
/dev/tty             /dev/tty        5       0 system:/dev/tty
/dev/console         /dev/console    5       1 system:console
/dev/ptmx            /dev/ptmx       5       2 system
/dev/vc/0            /dev/vc/0       4       0 system:vtmaster
usbserial            /dev/ttyUSB   188 0-511 serial
serial               /dev/ttyS       4 64-95 serial
pty_slave            /dev/pts      136 0-1048575 pty:slave
pty_master           /dev/ptm      128 0-1048575 pty:master
unknown              /dev/tty        4 1-63 console

# Question?
what is /dev/vcs?

# Backup
./drivers//tty/vt/vt.c:3042:	    register_chrdev_region(MKDEV(TTY_MAJOR, 0), 1, "/dev/vc/0") < 0)
./drivers//tty/vt/vc_screen.c:644:	if (register_chrdev(VCS_MAJOR, "vcs", &vcs_fops))
./drivers//tty/tty_io.c:3377:		error = register_chrdev_region(dev, driver->num, driver->name);
./drivers//tty/tty_io.c:3414:	unregister_chrdev_region(dev, driver->num);
./drivers//tty/tty_io.c:3430:	unregister_chrdev_region(MKDEV(driver->major, driver->minor_start),
./drivers//tty/tty_io.c:3607:	    register_chrdev_region(MKDEV(TTYAUX_MAJOR, 0), 1, "/dev/tty") < 0)
./drivers//tty/tty_io.c:3613:	    register_chrdev_region(MKDEV(TTYAUX_MAJOR, 1), 1, "/dev/console") < 0)
./drivers//tty/pty.c:841:	    register_chrdev_region(MKDEV(TTYAUX_MAJOR, 2), 1, "/dev/ptmx") < 0)

