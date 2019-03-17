#!/usr/bin/python

from __future__ import print_function
from unicorn import *
from unicorn.x86_const import *

# code to be emulated
X86_CODE32 = b"\x41\x4a" # INC ecx; DEC edx

raw_machine_code = "bf 03 00 00 00 48 83 f8 ff 48 85 c0" # mov    $0x3,%edi; 48 cmp    $0xffffffffffffffff,%rax; 48 test rax rax

rmc_mov_mem_reg = "48 8b 45 08 48 85 c0" # mov    0x8(%rbp),%rax ; set rbp to 0xffffffff81787626; e9 3c fc ff ff  jmpq   0xffffffff81787626 <__schedule+118>; 48 test rax rax
rmc = rmc_mov_mem_reg

# FIXME:  bytes.fromhex(hex_string)  vs bytearray.fromhex(hex_string)
# Note that bytes is an immutable version of bytearray. https://stackoverflow.com/questions/5649407/hexadecimal-string-to-byte-array-in-python
X86_64CODE = rmc.replace(" ", "").decode("hex")
# memory address where emulation starts
ADDRESS = 0x1000000

print("Emulate x86 64code")
def map(uc, address, size):
    alignment = address % 4096
    base_address = address - alignment

    page_size = (int(size / 4096) * 4096) + 4096
    return uc.mem_map(base_address, page_size)
    # return uc.mem_map(address, size)

def crash_mem_invalid(uc, access, address, size, value, user_data):
	a_str = "unkown"
	if access == UC_MEM_WRITE_UNMAPPED:
		a_str = "write" 
	elif access == UC_MEM_READ_UNMAPPED:
		a_str = "read" 

	print("invalid %s %x %x %x \n"%(a_str, address, size, value))
	m = map(uc, address, 8)
	return True

def crash_mem_access(uc, access, address, size, value, current_run):
	a_str = "unkown"
	if access == UC_MEM_WRITE:
		a_str = "write" 
	elif access == UC_MEM_READ:
		a_str = "read" 

	print("access: %s %x %x %x \n"%(a_str, address, size, value))
	return True

try:
	# Initialize emulator in X86-32bit mode
	mu = Uc(UC_ARCH_X86, UC_MODE_64)

	# map 2MB memory for this emulation
	mu.mem_map(ADDRESS, 2 * 1024 * 1024)

	# write machine code to be emulated to memory
	mu.mem_write(ADDRESS, X86_64CODE)

	# initialize machine registers
	mu.reg_write(UC_X86_REG_ECX, 0x1234)
	mu.reg_write(UC_X86_REG_RAX, 0x0)
	mu.reg_write(UC_X86_REG_RBP, 0xffffffff81787626)

	eflags = mu.reg_read(UC_X86_REG_EFLAGS)
	print(">>> eflags = 0x%x" %eflags)

	# Register hook functions
	mu.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED, crash_mem_invalid)
	mu.hook_add(UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, crash_mem_access)
	# emulate code in infinite time & unlimited instructions
	mu.emu_start(ADDRESS, ADDRESS + len(X86_64CODE))

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
	print(">>> eflags = 0x%x" %eflags)

except UcError as e:
	print("ERROR: %s" % e)
