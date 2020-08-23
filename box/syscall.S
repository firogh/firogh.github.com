# as -o syscall.o syscall.s #assemble
# ld -o syscall syscall.o #link 
.data
	.quad msg
msg:
	.ascii "Hello, world!\n"
	len = . - msg

.text
	.global _start

_start:
	movl	$1, %eax  # write
	movl	$0, %edi
	movl	$msg, %esi 
	movl	$len, %edx 
	syscall

	movl	%eax, %edi
	movl	$60, %eax # exit
	syscall
