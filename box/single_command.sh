#!/bin/bash

return
mkdir /tmp/kpanic && cd /tmp/kpanic && printf '#include <linux/kernel.h>\n#include <linux/module.h>\nMODULE_LICENSE("GPL");static int8_t* message = "buffer overrun at 0x4ba4c73e73acce54";int init_module(void){char *killer = NULL;wmb();*killer = 1;return 0;}' > kpanic.c && printf 'obj-m += kpanic.o\nall:\n\tmake -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules' > Makefile && make && insmod kpanic.ko

