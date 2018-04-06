#!/usr/bin/env python3
#file: set-byte

import sys
import binascii

fileName = sys.argv[1]
offset = int(sys.argv[2], 0)
byte = int(sys.argv[3], 0)
new_data = sys.argv[4]

with open(fileName, "r+b") as f:
	f.seek(offset)
	out= f.read(byte)
	print("Before write, old data:")
	print(binascii.hexlify(out))
	f.seek(offset)
	#obuf = '4A00000008000000e1840000'
	print("Writing data: \n"+ new_data)
	obuf = new_data
	s = binascii.unhexlify(obuf)
	f.write(s)
	f.seek(offset)
	out = f.read(byte)
	print("After write, new data:")
	print(binascii.hexlify(out))
