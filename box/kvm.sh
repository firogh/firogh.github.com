#!/bin/bash

timeout --foreground 10s /usr/bin/qemu-system-x86_64 -nographic -enable-kvm -kernel /boot/vmlinuz-3.12.74-60.64.124-default -append "console=ttyS0"
