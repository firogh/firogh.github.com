---
tags: cs
title: Light camera action - compile link load 
date: 2015-10-25 01:46:28
category: cs
---
# Reference
[compiler, assembler, linker and loader: a brief story](http://www.tenouk.com/ModuleW.html)
# Contents
Why complier can manipulate the address of vriables? 
What is the address in lds(??) file?

# Build automation
* Make
make cynthia
## kbuild
* if no .config, every config tools make a .config from scrach!
* oldconfig just for new moduels patch merged in trunk. This no any relations to .config.old and /boot/config.x.y.z
* make localmodconfig will reduce many unused kernel config.
* make bzImage  #kmods will not build that configured with M! 
* make modules_install INSTALL_MOD_PATH=/home/firo/kmods
## yocto
make ARCH=arm menuconfig
##build signle kernel module
make menuconfig
make oldconfig && make prepare
make -C $(pwd) M=/home/firo/linux/fs/ext3 modules V=1
make ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnu- drivers/pcmcia/sa11xx_base.o
# Compile construction
Understand pl deeply 
how to translate it into machine code for specific CPU
* gcc
-E -S -c 
-I -L -l
-ansi
-fsyntax-only 
-Werror
-save-temps
-H: show all header used
-v
-Wall
-fPIC: for shared library
-Q: print function and statisc?
-Wextra: 用处不大.
-U: undefine a preprocessor macro.
-pipe: make compilation faster, replace xx.S with |
-Wpadded: we can rearrange the fields of structure to make the structure smaller
-time
-x c: c language
-: stdin
asmlinkage
However, for C functions invoked from assembly code, 
we should explicitly declare the function's calling convention, 
because the parameter passing code in assembly side has been fixed. 
##[Generating optimized code](http://www.stlinux.com/devel/debug/jtag/build?q=node/82)
##Symbol table
* System.map less 
When you compile the kernel
nm vmlinux
readelf -s 
* /proc/kallsyms
# Link
* GNU ld
* gold

