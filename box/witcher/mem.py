import crash_mem

class machine_state
	registers_space = { : }
	memory_space = { : }
	def load_mem(addr):
		return memory_space[addr]
	def store_mem(addr, value)
		memory_space[addr] = value

class registers(machine_state)

class memory(machine_state)
	def read_mem_8(addr, size):
		if addr in self.memory_space:
			print(self.load_mem(addr))
		else:
			value = read_crash_mem(addr, size)
			self.store_mem(addr, value)
			print(value)

	def write_mem_8(addr, value)
		self.store_mem(addr, value)


	
