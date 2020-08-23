#!/usr/bin/env python

#from __future__ import print_function
import sys
import subprocess
import os,re

# if (__name__ == '__main__'):
#	print("%s" % exec_crash_command("p filp_cachep"))

def exec_crash_command(s):
	print(s)

if (__name__ == '__main__'):
	comm = "%d %d"%(1, 2)
	exec_crash_command(comm)
	dict = {1 : "firo"}
	print(dict[1])
	if 2 in dict:
		print(dict[2])
		
	#result = os.system("cat t.log")
	result = subprocess.check_output(["cat", "t.log"])
	result = 
   	
	
	print(result.splitlines()[1].split())
