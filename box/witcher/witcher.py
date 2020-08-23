#!/usr/bin/python

from __future__ import print_function

from pykdump.API import *
from LinuxDump.fs import *
from LinuxDump import Dev

import sys
l36_path = ['/usr/lib/python36.zip', '/usr/lib64/python3.6', '/usr/lib64/python3.6/lib-dynload', '/usr/lib64/python3.6/site-packages', '/usr/lib64/python3.6/_import_failed', '/usr/lib/python3.6/site-packages', '/usr/lib/python3.6/site-packages/crash-0.1-py3.6.egg', '/usr/lib/python3.6/site-packages/pyelftools-0.25-py3.6.egg', '/usr/lib/python3.6/site-packages/future-0.17.1-py3.6.egg']
for p in l36_path:
        if p not in sys.path:
                sys.path.append(p)
print(sys.path)

from unicorn import *
from unicorn.x86_const import *

# code to be emulated
raw_machine_code = "bf 03 00 00 00 48 83 f8 ff 48 85 c0" # mov    $0x3,%edi; 48 cmp    $0xffffffffffffffff,%rax; 48 test rax rax

rmc_mov_mem_reg = "48 8b 45 08 48 85 c0" # mov    0x8(%rbp),%rax ; set rbp to 0xffffffff81787626; e9 3c fc ff ff  jmpq   0xffffffff81787626 <__schedule+118>; 48 test rax rax
rmc = rmc_mov_mem_reg

def wait(uc):
	a = sys.stdin.readline()
	if a ==  "e\n":
		uc.emu_stop()

# FIXME:  bytes.fromhex(hex_string)  vs bytearray.fromhex(hex_string)
# Note that bytes is an immutable version of bytearray. https://stackoverflow.com/questions/5649407/hexadecimal-string-to-byte-array-in-python
print(rmc.replace(" ", ""))
#X86_64CODE = rmc.replace(" ", "").decode("hex")
X86_64CODE = bytes.fromhex(rmc.replace(" ", ""))
# memory address where emulation starts
ADDRESS = 0x1000000

print("Emulate x86 64code")
def crash_hook_code(uc, address, size, user_data):
	# wait(uc)
	eflags = uc.reg_read(UC_X86_REG_EFLAGS)
	# rip = uc.reg_read(UC_X86_REG_RIP)
	rsp = uc.reg_read(UC_X86_REG_RSP)
	rbp = uc.reg_read(UC_X86_REG_RBP)
	r15 = uc.reg_read(UC_X86_REG_R15)
	r14 = uc.reg_read(UC_X86_REG_R14)
	r13 = uc.reg_read(UC_X86_REG_R13)
	r12 = uc.reg_read(UC_X86_REG_R12)
	rbx = uc.reg_read(UC_X86_REG_RBX)
	rdi = uc.reg_read(UC_X86_REG_RDI)
	rsi = uc.reg_read(UC_X86_REG_RSI)
	rdx = uc.reg_read(UC_X86_REG_RDX)

	rcx = uc.reg_read(UC_X86_REG_RCX)
	r8 = uc.reg_read(UC_X86_REG_R8,)
	r9 = uc.reg_read(UC_X86_REG_R9,)
	rax = uc.reg_read(UC_X86_REG_RAX)
	if rsp == 0xffff8808069b7ba0:
		uc.emu_stop()
	inst = crash_disasm_addr(address)
	print("0x%x: 0x%x : 0x%x : %s  " %(address, size, eflags, inst.rstrip()))
	print("rsp 0x%x : rbp 0x%x : r15 0x%x : r14 0x%x : r13 0x%x : r12 0x%x : rbx 0x%x" %(rsp, rbp, r15, r14, r13, r12, rbx))
	print("rdi 0x%x : rsi 0x%x : rdx 0x%x : rcx 0x%x : r8 0x%x : r9 0x%x : rax 0x%x" %(rdi, rsi, rdx, rcx, r8, r9, rax))

def ecc(comm):
        return exec_crash_command(comm)

def crash_disasm_addr(addr):
	crash_comm = "x/i 0x%lx"%(addr)
	result = ecc(crash_comm)
	return result

def map(uc, address, size):
	alignment = address % 4096
	base_address = address - alignment

	page_size = (int(size / 4096) * 4096) + 4096

	uc.mem_map(base_address, page_size)
	page = readmem(base_address, page_size)
	uc.mem_write(base_address, page)

	return True

def crash_mem_invalid(uc, access, address, size, value, user_data):
	a_str = "unkown"
	if access == UC_MEM_WRITE_UNMAPPED:
		a_str = "write" 
	elif access == UC_MEM_READ_UNMAPPED:
		a_str = "read" 

	print("invalid %s %x %x %x \n"%(a_str, address, size, value))
	m = map(uc, address, size)
	return True

def crash_mem_access(uc, access, address, size, value, current_run):
	a_str = "unkown"
	if access == UC_MEM_WRITE:
		a_str = "write" 
	elif access == UC_MEM_READ:
		a_str = "read" 

	print("access: %s %x %x %x \n"%(a_str, address, size, value))
	return True

SCRATCH_ADDR = 0x80000
SCRATCH_SIZE = 0x1000

FSMSR = 0xC0000100
GSMSR = 0xC0000101

def get_msr(uc, msr, scratch=SCRATCH_ADDR):
	'''
	fetch the contents of the given model-specific register (MSR).
	this will clobber some memory at the given scratch address, as it emits some code.
	'''
	# save clobbered registers
	orax = uc.reg_read(UC_X86_REG_RAX)
	ordx = uc.reg_read(UC_X86_REG_RDX)
	orcx = uc.reg_read(UC_X86_REG_RCX)
	orip = uc.reg_read(UC_X86_REG_RIP)

	# x86: rdmsr
	buf = b'\x0f\x32'
	uc.mem_write(scratch, buf)
	uc.reg_write(UC_X86_REG_RCX, msr & 0xFFFFFFFF)
	uc.emu_start(scratch, scratch+len(buf), count=1)
	eax = uc.reg_read(UC_X86_REG_EAX)
	edx = uc.reg_read(UC_X86_REG_EDX)

	# restore clobbered registers
	uc.reg_write(UC_X86_REG_RAX, orax)
	uc.reg_write(UC_X86_REG_RDX, ordx)
	uc.reg_write(UC_X86_REG_RCX, orcx)
	uc.reg_write(UC_X86_REG_RIP, orip)

	return (edx << 32) | (eax & 0xFFFFFFFF)

def set_msr(uc, msr, value, scratch=SCRATCH_ADDR):
	'''
	set the given model-specific register (MSR) to the given value.
	this will clobber some memory at the given scratch address, as it emits some code.
	'''
	# save clobbered registers
	orax = uc.reg_read(UC_X86_REG_RAX)
	ordx = uc.reg_read(UC_X86_REG_RDX)
	orcx = uc.reg_read(UC_X86_REG_RCX)
	orip = uc.reg_read(UC_X86_REG_RIP)

	# x86: wrmsr
	buf = b'\x0f\x30'
	uc.mem_map(SCRATCH_ADDR, SCRATCH_SIZE)
	uc.mem_write(SCRATCH_ADDR, buf)
	uc.reg_write(UC_X86_REG_RAX, value & 0xFFFFFFFF)
	uc.reg_write(UC_X86_REG_RDX, (value >> 32) & 0xFFFFFFFF)
	uc.reg_write(UC_X86_REG_RCX, msr & 0xFFFFFFFF)
	uc.emu_start(SCRATCH_ADDR, scratch+len(buf), count=1)

	# restore clobbered registers
	uc.reg_write(UC_X86_REG_RAX, orax)
	uc.reg_write(UC_X86_REG_RDX, ordx)
	uc.reg_write(UC_X86_REG_RCX, orcx)
	uc.reg_write(UC_X86_REG_RIP, orip)

def set_gs(uc, addr):
	'''
	set the GS.base hidden descriptor-register field to the given address.
	this enables referencing the gs segment on x86-64.
	'''
	return set_msr(uc, GSMSR, addr)


def get_gs(uc):
	'''
	fetch the GS.base hidden descriptor-register field.
	'''
	return get_msr(uc, GSMSR)
def set_gs_to_addr(uc, addr):
	set_gs(uc, addr)

try:
	# Initialize emulator in X86-64bit mode
	mu = Uc(UC_ARCH_X86, UC_MODE_64)

	# map 2MB memory for this emulation
	# mu.mem_map(ADDRESS, 2 * 1024 * 1024)

	# write machine code to be emulated to memory
	# mu.mem_write(ADDRESS, X86_64CODE)

	# initialize machine registers
	mu.reg_write(UC_X86_REG_RSP, 0xffff8808069b7dd8)
	mu.reg_write(UC_X86_REG_RBP, 0xffff8808069b7e78)
	mu.reg_write(UC_X86_REG_R15, 0x1fb2aa0fa)
	mu.reg_write(UC_X86_REG_R14, 0x0)
	mu.reg_write(UC_X86_REG_R13, 3)
	mu.reg_write(UC_X86_REG_R12, 0xffff880046e72700)
	mu.reg_write(UC_X86_REG_RBX, 0xffff880046e6d9a0)
	#mu.reg_write(, )
	mu.reg_write(UC_X86_REG_RDI, 3)
	mu.reg_write(UC_X86_REG_RSI, 0xffff880046e72700)
	mu.reg_write(UC_X86_REG_RDX, 0xffff880046e6d9a0)

	mu.reg_write(UC_X86_REG_RCX, 0x0)
	mu.reg_write(UC_X86_REG_R8, 0x0)
	mu.reg_write(UC_X86_REG_R9, 0x0)
	mu.reg_write(UC_X86_REG_RAX, 0x0)
	set_gs_to_addr(mu, 0xffff880046e60000)

	eflags = mu.reg_read(UC_X86_REG_EFLAGS)
	gs = mu.reg_read(UC_X86_REG_GS)
	print(">>> eflags = 0x%x gs 0x%lx" %(eflags, gs))
	# mu.reg_write(UC_X86_REG_GS, 0xdeadbeef)
	mu.reg_write(UC_X86_REG_GS, 0x0)
	gs = mu.reg_read(UC_X86_REG_GS)
	print(">>> eflags = 0x%x gs 0x%lx" %(eflags, gs))


	# Register hook functions
	mu.hook_add(UC_HOOK_CODE, crash_hook_code)

	mu.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED, crash_mem_invalid)
	mu.hook_add(UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, crash_mem_access)
	# emulate code in infinite time & unlimited instructions
	# 0xffffffff8139d87d <thread_return+665>:	mov    %rbx,%rdx
	# 0xffffffff8139d880 <thread_return+668>:	mov    %r12,%rsi
	# 0xffffffff8139d883 <thread_return+671>:	mov    %r13d,%edi
	# 0xffffffff8139d886 <thread_return+674>:	callq  0xffffffff81047090 <load_balance_newidle>
	# 0xffffffff8139d88b <thread_return+679>:	mov    %eax,%r14d
	mu.emu_start(0xffffffff8139d87d, 0xffffffff8139d88b)

	# mu.emu_start(ADDRESS, ADDRESS + len(X86_64CODE))

	# now print out some registers
	print("Emulation done. Below is the CPU context")

	r_ecx = mu.reg_read(UC_X86_REG_ECX)
	r_edx = mu.reg_read(UC_X86_REG_EDX)
	rdi = mu.reg_read(UC_X86_REG_RDI)
	rbp = mu.reg_read(UC_X86_REG_RBP)
	eflags = mu.reg_read(UC_X86_REG_EFLAGS)
	print(">>> ECX = 0x%x" %r_ecx)
	print(">>> EDX = 0x%x" %r_edx)
	print(">>> rdi = 0x%x" %rdi)
	print(">>> rbp = 0x%x" %rbp)
	gs = mu.reg_read(UC_X86_REG_GS)
	print(">>> eflags = 0x%x gs 0x%lx" %(eflags, gs))

except UcError as e:
	print("ERROR: %s" % e)


print(readmem(0xffffffff81047090, 0x8).hex());
