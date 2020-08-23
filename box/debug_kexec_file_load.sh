#!/bin/bash

cd /sys/kernel/debug/tracing
echo 0 > tracing_on
: > trace
echo 0 > events/kprobes/enable
echo > /sys/kernel/debug/tracing/kprobe_events
# echo SyS_kexec_file_load > set_graph_function
echo 'r:firor SyS_kexec_file_load $retval' >> kprobe_events
echo 'r:firor1 kimage_load_segment $retval' >> kprobe_events
echo 'r:firor2 machine_kexec_prepare $retval' >> kprobe_events
echo 'r:firor3 sanity_check_segment_list $retval' >> kprobe_events
echo 'r:firor4 kernel_read_file_from_fd $retval' >> kprobe_events
echo 'r:firor5 arch_kexec_kernel_image_probe $retval' >> kprobe_events
echo 'r:firor6 arch_kexec_kernel_verify_sig $retval' >> kprobe_events
echo 'r:firor7 arch_kexec_kernel_image_load $retval' >> kprobe_events
echo 'r:firor8 bzImage64_load $retval' >> kprobe_events
echo 'r:firor9 crash_load_segments $retval' >> kprobe_events
echo 'r:firora walk_system_ram_res $retval' >> kprobe_events
echo 'r:firorb kexec_add_buffer $retval' >> kprobe_events
echo 'r:firorc kexec_locate_mem_hole $retval' >> kprobe_events
echo 'r:firord arch_kexec_walk_mem $retval' >> kprobe_events
echo 'r:firore walk_iomem_res_desc $retval' >> kprobe_events
echo 'r:firorf __walk_iomem_res_desc $retval' >> kprobe_events
echo 'r:firorg locate_mem_hole_callback $retval' >> kprobe_events
echo 'r:firorh find_next_iomem_res $retval' >> kprobe_events
#echo 'r:firori locate_mem_hole_top_down $retval' >> kprobe_events
#echo 'r:firorj locate_mem_hole_bottom_up $retval' >> kprobe_events

echo 'p:firo0 locate_mem_hole_callback memsz=+0x20(%si)' >> kprobe_events
echo 'p:firo1 __walk_iomem_res_desc start=+0(%di) end=+0x8(%di) kbuf=%cx memsz=+0x20(%cx)' >> kprobe_events
echo 'p:firo2 walk_iomem_res_desc desc=%di flags=%si start=%dx end=%cx kbuf=%r8 memsz=+0x20(%r8)' >> kprobe_events
#echo 'r:firor do_sys_open $retval' >> kprobe_events
echo 1 > events/kprobes/enable
echo 1 > tracing_on

strace -e kexec_file_load /sbin/kexec -s -p /boot/vmlinuz-4.12.14-197.15-default --append="video=1024x768 console=ttyS1,115200n vt.color=0x07 quiet mitigations=auto elevator=deadline sysrq=yes reset_devices acpi_no_memhotplug cgroup_disable=memory nokaslr numa=off irqpoll nr_cpus=1 root=kdump rootflags=bind rd.udev.children-max=8 disable_cpu_apicid=0   panic=1" --initrd=/boot/initrd-4.12.14-197.15-default-kdump

echo 0 > tracing_on
cat trace
cat kprobe_events
