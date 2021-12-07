# -*- coding: utf-8 -*-
# vim:set shiftwidth=4 softtabstop=4 expandtab textwidth=79:
# Find holder of a contended qspinlock.
# Run this script under drgn: exec(open('/suse/fyang/c/qspinlock.py').read())

nr_cpu_ids = prog['nr_cpu_ids'].value_()
print("nr cpu ids %d"%nr_cpu_ids)

def find_prev_mcs_node(curr_mcs_node):
    for i in range(nr_cpu_ids):
        offset = prog['__per_cpu_offset'][i].value_()
        address = (prog['mcs_nodes'][0].address_ & 0xffffff) + offset
        nodes = Object(prog, prog['mcs_nodes'].type_, address =  address)
        for n in range(nodes[0].count):
            # if nodes[n].locked == 0:
            if nodes[n].next.value_() == curr_mcs_node.address_:
                    print("CPU: %d %x"%(i, address))
                    return nodes[n]

    print("Not found.")
    return None

def find_lock_holder(curr_mcs_node):
    next = curr_mcs_node
    while next.locked != 1:
        next = find_prev_mcs_node(next)     
        print(next)

    return next

# @l3mule:/space/firo/dumps/ext4_mcs_1178087/vmcore
# mcs node at 0xffff9435c6423dc0 for CPU0 is spinning for the lock to be
# released.
# curr = Object(prog, prog['mcs_nodes'][0].type_, address = 0xffff9435c6423dc0)
# curr = Object(prog, prog['mcs_nodes'][0].type_, address = 0xffff9f5d7f8a3d80)
# /suse/fyang/s/1184207/SFSC00279333/var/crash/2021-03-26-22:25/vmcore cpu2 mcs
# node
curr = Object(prog, prog['mcs_nodes'][0].type_, address = 0xffff8804874a3e40)
find_lock_holder(curr)
