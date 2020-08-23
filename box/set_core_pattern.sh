#!/bin/bash

sudo su -c "echo '/tmp/coredump %P %u %g %s %t %c %h' >  /proc/sys/kernel/core_pattern"
