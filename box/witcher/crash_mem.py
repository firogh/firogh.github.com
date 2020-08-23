#!/usr/bin/env python

#from __future__ import print_function
from pykdump.API import *
from LinuxDump.fs import *
from LinuxDump import Dev

import sys

# if (__name__ == '__main__'):
#	print("%s" % exec_crash_command("p filp_cachep"))

def ecc(comm):
	exec_crash_command(comm)

if (__name__ == '__main__'):
def crash_read_mem(addr, size)
	crash_comm = "x/%dg %lx"%(size, addr)
	result = ecc(crash_comm)
	print("%s" % result)

def crash_fetch_instruction(addr)
	crash_comm = "x/2i %lx"%(addr)
	result = ecc(crash_comm)
	second_addr = int(result.splitlines()[1].split()[0], 16) 
	instr_len = second_addr - int(addr, 16)
	instr_str = ""

	return instr_str
def decode_instruction(instr)
	parse_prefix(instr)
	

	
